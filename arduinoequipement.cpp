#include "arduinoequipement.h"
#include <QDebug>

// ════════════════════════════════════════════════════════════
//  Constructor / Destructor
// ════════════════════════════════════════════════════════════

ArduinoEquipement::ArduinoEquipement(QObject *parent)
    : QObject(parent),
    serialPort(new QSerialPort(this))
{
    connect(serialPort, &QSerialPort::readyRead,
            this,       &ArduinoEquipement::onDataReceived);
}

ArduinoEquipement::~ArduinoEquipement()
{
    deconnecter();
}

// ════════════════════════════════════════════════════════════
//  Connection
// ════════════════════════════════════════════════════════════

bool ArduinoEquipement::connecter(const QString &portName, int baudRate)
{
    if (serialPort->isOpen())
        serialPort->close();

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadOnly)) {
        emit erreurConnexion(
            "Impossible d'ouvrir " + portName +
            " : " + serialPort->errorString()
            );
        return false;
    }

    qDebug() << "[Arduino] Connecté sur" << portName;
    return true;
}

void ArduinoEquipement::deconnecter()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        qDebug() << "[Arduino] Déconnecté.";
    }
}

bool ArduinoEquipement::estConnecte() const
{
    return serialPort && serialPort->isOpen();
}

QStringList ArduinoEquipement::portsDisponibles() const
{
    QStringList liste;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        liste << info.portName() + " — " + info.description();
    return liste;
}

// ════════════════════════════════════════════════════════════
//  Data Reception
// ════════════════════════════════════════════════════════════

void ArduinoEquipement::onDataReceived()
{
    buffer += QString::fromLatin1(serialPort->readAll());

    while (buffer.contains('\n')) {
        int     idx   = buffer.indexOf('\n');
        QString ligne = buffer.left(idx).trimmed();
        buffer        = buffer.mid(idx + 1);

        if (ligne.isEmpty()) continue;
        qDebug() << "[Arduino] <<" << ligne;

        if (ligne == "READY") {
            emit arduinoReady();
            continue;
        }

        if (ligne.startsWith("USE:")) {
            bool ok = false;
            int  id = ligne.mid(4).toInt(&ok);
            if (ok && id > 0)
                emit equipementUtilise(id);
            continue;
        }
    }
}
