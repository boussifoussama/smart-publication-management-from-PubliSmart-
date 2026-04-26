#include <QApplication>
#include "smartmarket.h"
#include "connection.h"
#include <QMessageBox>

static int runApplication(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    bool test = c.createconnect();
    
    SmartMarket w;
    if (!test) {
        QMessageBox::warning(nullptr, "Erreur BDD", 
                             "Connexion a la base de donnees echouee.\n"
                             "L'application demarre quand meme.");
    }
    
    w.show();
    return a.exec();
}

int main(int argc, char *argv[])
{
    return runApplication(argc, argv);
}
