#include "arduino.h"
#include <QRegularExpression>

ArduinoBridge::ArduinoBridge(QObject *parent)
    : QObject(parent)
{
    connect(&serial, &QSerialPort::readyRead, this, &ArduinoBridge::onReadyRead);
}

bool ArduinoBridge::connectPort(const QString &portName, qint32 baudRate)
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

    emit statusChanged(QString("Port %1 connecte").arg(portName));
    return true;
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

/**
 * @brief Transmet une commande texte via le port série vers l'Arduino.
 * 
 * Cette méthode gère l'aspect "Output" du côté PC. Elle est utilisée pour
 * envoyer les messages "OPEN" ou "DENY" qui seront interprétés par l'Arduino
 * pour actionner le servomoteur.
 * 
 * @param command La chaîne de caractères à envoyer (ex: "OPEN")
 * @return true si l'envoi a réussi, false sinon
 */
bool ArduinoBridge::sendCommand(const QString &command)
{
    // Vérification de la connexion avant l'envoi
    if (!serial.isOpen())
        return false;

    // Conversion de la commande en format UTF-8
    QByteArray payload = command.toUtf8();
    
    // Ajout d'un caractère de fin de ligne (\n) si absent,
    // car l'Arduino utilise \n pour détecter la fin d'une commande.
    if (!payload.endsWith('\n'))
        payload.append('\n');

    // Écriture effective sur le port série (Sortie vers le matériel)
    const qint64 written = serial.write(payload);
    
    // On retourne vrai si tous les octets ont été envoyés
    return written == payload.size();
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
