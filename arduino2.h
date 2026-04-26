#ifndef ARDUINO2_H
#define ARDUINO2_H

#include <QObject>
#include <QSerialPort>
#include <QDate>

class Arduino2 : public QObject
{
    Q_OBJECT
public:
    explicit Arduino2(QObject *parent = nullptr);
    ~Arduino2();

    static QStringList availablePorts();
    QString autoConnect(qint32 baudRate = 9600);
    
    bool connectPort(const QString &portName, qint32 baudRate = 9600);
    void disconnectPort();
    bool isConnected() const;

    // Envoie une deadline au LCD
    // Format: "D:NomReviewer|Date"
    bool displayDeadline(const QString &nom, const QDate &date);
    
    // Commandes simples
    bool clearDisplay();
    bool showWelcome();

    QString lastError() const;

signals:
    void statusChanged(const QString &status);
    void errorOccurred(const QString &error);

private:
    QSerialPort *serial;
};

#endif // ARDUINO2_H
