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
    SmartMarket w;

    if (!test) {
        QMessageBox::warning(&w, "Erreur BDD",
                             "Connexion a la base de donnees echouee.\n"
                             "L'application demarre quand meme.\n"
                             "Verifiez votre source ODBC (Source_Projet2A), l'utilisateur et le mot de passe.");
    }

    w.show();
    return a.exec();
}
