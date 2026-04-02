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
    void on_btnLoginEntrer_clicked();       // Bouton "Entrer" du Login
    void on_btnLoginEnvoyer_clicked();      // Bouton "Envoyer" (mot de passe oublié)
    
    // ========== MODULE PUBLICATION - CRUD ==========
    void on_pushButton_19_clicked();        // Ajouter une publication
    void on_pushButton_20_clicked();        // Modifier une publication
    void on_pushButton_8_clicked();         // Supprimer une publication
    void on_pushButton_9_clicked();         // Rechercher des publications
    void on_pushButton_5_clicked();         // Réinitialiser la recherche
    void on_pushButton_21_clicked();        // Exporter les publications

    // ========== MODULE CONFERENCE - CRUD ==========
    void on_conf_pushButton_26_clicked();   // Ajouter une conférence
    void on_conf_pushButton_8_clicked();    // Supprimer une conférence par ID
    void on_conf_pushButton_27_clicked();   // Modifier une conférence par ID
    void on_conf_pushButton_19_clicked();   // Ajouter un participant
    void on_conf_pushButton_12_clicked();   // Supprimer un participant
    void on_conf_pushButton_10_clicked();   // Modifier un participant
    void on_conf_pushButton_20_clicked();   // Filtrer les participants par nom
    void on_conf_pushButton_5_clicked();    // Exporter conférences/participants en PDF
    void on_conf_pushButton_21_clicked();   // Filtrer les conférences
    void on_conf_calendarWidget_selectionChanged(); // Afficher les conférences du jour sélectionné
    
    // ========== MODULE PUBLICATION - ANALYSE ==========
    void on_pushButton_6_clicked();         // Choisir 2 publications pour comparer
    void on_pushButton_7_clicked();         // Vérifier la complétude
    
    // ========== SIDEBAR - NAVIGATION MODULES ==========
    void on_pushButton_14_clicked();        // Publications
    void on_pushButton_16_clicked();        // Reviewers
    void on_pushButton_17_clicked();        // Conférences
    void on_pushButton_18_clicked();        // Équipements

private:
    Ui::SmartMarket *ui;
    QStandardItemModel *publicationModel;
    int conferencePageIndex;
    int reviewersPageIndex;
    
    void initPublicationTable();
    void loadConferenceTable();
    void createCharts();
    void setupConferencePage();
    void setupReviewersPage();
    void setupConferenceCharts();
    void setupReviewersCharts();
    void reviewerUpdateKPIs();
    void reviewerCreateBarChart();
    void reviewerCreateLineChart();
    void reviewerCreatePieChart();
    void reviewerSetupNavigation();
    int addEmbeddedWindow(QMainWindow *window);
    void updateConferenceParticipantsChart();
    void updateConferenceDaysChart();
    void updateConferenceCalendar();
    void loadParticipantTable();
};

#endif // SMARTMARKET_H
