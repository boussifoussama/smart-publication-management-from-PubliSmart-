#ifndef SMARTMARKET_H
#define SMARTMARKET_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class SmartMarket; }
QT_END_NAMESPACE

class SmartMarket : public QMainWindow
{
    Q_OBJECT

public:
    SmartMarket(QWidget *parent = nullptr);
    ~SmartMarket();

private slots:
    // ========== NAVIGATION LOGIN ==========
    void on_pushButton_15_clicked();        // Bouton "Entrer" du Login
    void on_pushButton_13_clicked();        // Bouton "Envoyer" (mot de passe oublié)
    
    // ========== MODULE PUBLICATION - CRUD ==========
    void on_pushButton_17_clicked();        // Ajouter une publication
    void on_pushButton_18_clicked();        // Modifier une publication
    void on_pushButton_8_clicked();         // Supprimer une publication
    void on_pushButton_9_clicked();         // Rechercher des publications
    void on_pushButton_5_clicked();         // Réinitialiser la recherche
    void on_pushButton_19_clicked();        // Exporter les publications
    
    // ========== MODULE PUBLICATION - ANALYSE ==========
    void on_pushButton_6_clicked();         // Choisir 2 publications pour comparer
    void on_pushButton_7_clicked();         // Vérifier la complétude
    
    // ========== SIDEBAR - NAVIGATION MODULES ==========
    void on_pushButton_13_sidebar_clicked(); // Publications (actif)
    void on_pushButton_14_clicked();        // Reviewers
    void on_pushButton_15_sidebar_clicked(); // Conférences
    void on_pushButton_16_clicked();        // Équipements

private:
    Ui::SmartMarket *ui;
    QStandardItemModel *publicationModel;
    
    void initPublicationTable();
    void createCharts();
};

#endif // SMARTMARKET_H
