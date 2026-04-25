#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>

class ArduinoBridge : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoBridge(QObject *parent = nullptr);

    bool connectPort(const QString &portName, qint32 baudRate = 115200);
    void disconnectPort();
    bool isConnected() const;
    QString lastError() const;
    /**
     * @brief Envoie une commande brute à l'Arduino (ex: "OPEN", "DENY").
     * C'est le point de sortie principal pour piloter le servomoteur depuis l'application.
     */
    bool sendCommand(const QString &command);

signals:
    void uidScanned(const QString &uid);
    void rawLineReceived(const QString &line);
    void statusChanged(const QString &status);
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();

private:
    QString normalizeUid(const QString &uid) const;

    QSerialPort serial;
    QByteArray readBuffer;
};

#endif // ARDUINO_H
