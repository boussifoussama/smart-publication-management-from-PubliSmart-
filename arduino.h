#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class ArduinoBridge : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoBridge(QObject *parent = nullptr);

    bool connectPort(const QString &portName, qint32 baudRate = 115200, bool requireHandshake = true);
    void disconnectPort();
    bool isConnected() const;
    QString lastError() const;
    bool sendCommand(const QString &command);
    QString lastSentCommand() const;

signals:
    void uidScanned(const QString &uid);
    void rawLineReceived(const QString &line);
    void statusChanged(const QString &status);
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();

private:
    bool performHandshake();
    QString normalizeUid(const QString &uid) const;

    QSerialPort serial;
    QByteArray readBuffer;
    QString lastCommand;
};

#endif // ARDUINO_H
