#ifndef ARDUINOEQUIPEMENT_H
#define ARDUINOEQUIPEMENT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>

class ArduinoEquipement : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoEquipement(QObject *parent = nullptr);
    ~ArduinoEquipement();

    bool        connecter(const QString &portName, int baudRate = 9600);
    void        deconnecter();
    bool        estConnecte() const;
    QStringList portsDisponibles() const;

signals:
    void equipementUtilise(int idEquipement);
    void arduinoReady();
    void erreurConnexion(const QString &msg);

private slots:
    void onDataReceived();

private:
    QSerialPort *serialPort;
    QString      buffer;
};

#endif // ARDUINOEQUIPEMENT_H
