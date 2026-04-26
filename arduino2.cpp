#include "arduino2.h"
#include <QDebug>
#include <QSerialPortInfo>

Arduino2::Arduino2(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
}

Arduino2::~Arduino2()
{
    if (serial->isOpen())
        serial->close();
}

bool Arduino2::connectPort(const QString &portName, qint32 baudRate)
{
    if (serial->isOpen())
        serial->close();

    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        emit statusChanged("Arduino LCD connecté sur " + portName);
        showWelcome();
        return true;
    } else {
        emit errorOccurred("Erreur connection Arduino LCD: " + serial->errorString());
        return false;
    }
}

void Arduino2::disconnectPort()
{
    if (serial->isOpen()) {
        serial->close();
        emit statusChanged("Arduino LCD déconnecté");
    }
}

bool Arduino2::isConnected() const
{
    return serial->isOpen();
}

bool Arduino2::displayDeadline(const QString &nom, const QDate &date)
{
    if (!isConnected()) return false;

    // On formate la commande pour l'Arduino
    // On limite le nom à 16 caractères pour le LCD 16x2
    QString shortNom = nom.left(16);
    QString dateStr = date.toString("dd/MM/yyyy");
    
    // Format: D:Nom|Date\n
    QString cmd = QString("D:%1|%2\n").arg(shortNom, dateStr);
    serial->write(cmd.toUtf8());
    return serial->waitForBytesWritten(500);
}

bool Arduino2::clearDisplay()
{
    if (!isConnected()) return false;
    serial->write("C\n");
    return serial->waitForBytesWritten(500);
}

bool Arduino2::showWelcome()
{
    if (!isConnected()) return false;
    serial->write("W\n");
    return serial->waitForBytesWritten(500);
}

QString Arduino2::lastError() const
{
    return serial->errorString();
}

QStringList Arduino2::availablePorts()
{
    QStringList ports;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ports.append(info.portName());
    }
    return ports;
}

QString Arduino2::autoConnect(qint32 baudRate)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "[AutoScan] Testing port:" << info.portName() << info.description() << info.manufacturer();
        
        if (connectPort(info.portName(), baudRate)) {
            emit statusChanged("Auto-detected Arduino on port " + info.portName());
            return info.portName();
        }
    }
    return QString();
}
