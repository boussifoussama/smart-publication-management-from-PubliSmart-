#include "smartmarket.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ✅ Connexion BDD EN PREMIER
    Connection c;
    bool test = c.createconnect();

    if (!test) {
        QMessageBox::critical(nullptr, "Erreur BDD",
                              "Connexion a la base de donnees echouee.\n"
                              "Verifiez votre source ODBC et relancez.");
        return -1;
    }

    SmartMarket w;
    w.show();
    return a.exec();
}
