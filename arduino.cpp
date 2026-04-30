#include "arduino.h"
#include <QRegularExpression>
#include <QElapsedTimer>

ArduinoBridge::ArduinoBridge(QObject *parent)
    : QObject(parent)
{
    connect(&serial, &QSerialPort::readyRead, this, &ArduinoBridge::onReadyRead);
}

bool ArduinoBridge::connectPort(const QString &portName, qint32 baudRate, bool requireHandshake)
{
    if (serial.isOpen())
        serial.close();

    serial.setPortName(portName);
    serial.setBaudRate(baudRate);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite)) {
        emit errorOccurred(serial.errorString());
        return false;
    }

    if (requireHandshake && !performHandshake()) {
        const QString err = QString("Echec handshake RFID sur %1 @ %2").arg(portName).arg(baudRate);
        serial.close();
        emit errorOccurred(err);
        return false;
    }

    emit statusChanged(QString("Port %1 connecte").arg(portName));
    return true;
}

bool ArduinoBridge::performHandshake()
{
    if (!serial.isOpen())
        return false;

    serial.clear(QSerialPort::AllDirections);

    // Some boards reset on open and may need extra time before replying.
    QByteArray probe;
    bool sawBoot = false;
    bool sawUid = false;
    QElapsedTimer bootTimer;
    bootTimer.start();
    while (bootTimer.elapsed() < 2500) {
        if (serial.waitForReadyRead(120)) {
            probe.append(serial.readAll());
            sawBoot = sawBoot || probe.contains("BOOT=RFID_READY");
            sawUid = sawUid || probe.contains("UID=");
            if (probe.contains("ACK=PONG"))
                return true;
        }
    }

    for (int attempt = 0; attempt < 3; ++attempt) {
        const QByteArray ping("PING\n");
        if (serial.write(ping) != ping.size())
            return false;
        if (!serial.waitForBytesWritten(400))
            return false;

        QElapsedTimer pongTimer;
        pongTimer.start();
        while (pongTimer.elapsed() < 1500) {
            if (serial.waitForReadyRead(120)) {
                probe.append(serial.readAll());
                sawBoot = sawBoot || probe.contains("BOOT=RFID_READY");
                sawUid = sawUid || probe.contains("UID=");
                if (probe.contains("ACK=PONG"))
                    return true;
            }
        }
    }

    // Backward-compatible fallback: accept firmware that emits BOOT/UID but has no PING handler.
    return sawBoot || sawUid;
}

void ArduinoBridge::disconnectPort()
{
    if (!serial.isOpen())
        return;

    const QString portName = serial.portName();
    serial.close();
    emit statusChanged(QString("Port %1 deconnecte").arg(portName));
}

bool ArduinoBridge::isConnected() const
{
    return serial.isOpen();
}

QString ArduinoBridge::lastError() const
{
    return serial.errorString();
}

bool ArduinoBridge::sendCommand(const QString &command)
{
    if (!serial.isOpen())
        return false;

    lastCommand = command.trimmed().toUpper();
    QByteArray payload = command.toUtf8();
    if (!payload.endsWith('\n'))
        payload.append('\n');

    const qint64 written = serial.write(payload);
    if (written != payload.size())
        return false;

    return serial.waitForBytesWritten(500);
}

QString ArduinoBridge::lastSentCommand() const
{
    return lastCommand;
}

QString ArduinoBridge::normalizeUid(const QString &uid) const
{
    QString normalized = uid;
    normalized.remove(' ');
    normalized.remove(':');
    normalized.remove('-');
    return normalized.trimmed().toUpper();
}

void ArduinoBridge::onReadyRead()
{
    readBuffer.append(serial.readAll());
    readBuffer.replace('\r', '\n');

    auto extractUid = [this](const QString &text) -> QString {
        QString uidCandidate;

        if (text.startsWith("UID=", Qt::CaseInsensitive)) {
            uidCandidate = text.mid(4);
        } else if (text.startsWith("UID:", Qt::CaseInsensitive)) {
            uidCandidate = text.mid(4);
        } else {
            static const QRegularExpression uidChunkRegex(
                "([0-9A-Fa-f]{2}(?:[\\s:-]?[0-9A-Fa-f]{2}){3,15})");
            const QRegularExpressionMatch match = uidChunkRegex.match(text);
            if (match.hasMatch())
                uidCandidate = match.captured(1);
        }

        const QString uid = normalizeUid(uidCandidate);
        static const QRegularExpression strictUidRegex("^[0-9A-F]{8,32}$");
        return strictUidRegex.match(uid).hasMatch() ? uid : QString();
    };

    int newLineIndex = readBuffer.indexOf('\n');
    while (newLineIndex >= 0) {
        QByteArray lineBytes = readBuffer.left(newLineIndex);
        readBuffer.remove(0, newLineIndex + 1);

        QString line = QString::fromUtf8(lineBytes).trimmed();
        if (!line.isEmpty()) {
            emit rawLineReceived(line);
            const QString uid = extractUid(line);
            if (!uid.isEmpty())
                emit uidScanned(uid);
        }

        newLineIndex = readBuffer.indexOf('\n');
    }

    if (!readBuffer.isEmpty()) {
        const QString pending = QString::fromUtf8(readBuffer).trimmed();
        const QString uid = extractUid(pending);
        if (!uid.isEmpty()) {
            emit rawLineReceived(pending);
            emit uidScanned(uid);
            readBuffer.clear();
        }
    }
}
