#ifndef SMARTMARKET_H
#define SMARTMARKET_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include "publication.h"
#include "conference.h"
#include "participant.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SmartMarket; }
QT_END_NAMESPACE

class QChartView;
class QTableWidget;
class QLineEdit;
class ArduinoBridge;

class SmartMarket : public QMainWindow
{
    Q_OBJECT

public:
    SmartMarket(QWidget *parent = nullptr);
    ~SmartMarket();

private slots:
    // Login
    void on_pushButton_15_clicked();   // Se connecter
    void on_pushButton_13_clicked();   // Envoyer (reset mdp)

    // CRUD
    void on_pushButton_19_clicked();   // Ajouter
    void on_pushButton_20_clicked();   // Modifier
    void on_pushButton_8_clicked();    // Supprimer

    // Recherche / Export
    void on_pushButton_9_clicked();    // Rechercher
    void on_pushButton_5_clicked();    // Réinitialiser
    void on_pushButton_21_clicked();   // Exporter PDF

    // Similarité / Complétude
    void on_pushButton_6_clicked();    // Similarité
    void on_pushButton_7_clicked();    // Complétude

    // Sidebar
    void on_pushButton_13_sidebar_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_15_sidebar_clicked();
    void on_pushButton_16_clicked();

    // Module conférences externe
    void openConferenceModule();

    // Réponses réseau IA
    void onSimilariteReply(QNetworkReply *reply);
    void onCompletudReply(QNetworkReply *reply);

<<<<<<< Updated upstream
=======
    // Conference UI slots
    void on_conf_pushButton_5_clicked();
    void on_conf_pushButton_21_clicked();
    void on_conf_pushButton_20_clicked();
    void on_conf_pushButton_19_clicked();
    void on_conf_pushButton_12_clicked();
    void on_conf_pushButton_10_clicked();
    void on_conf_pushButton_26_clicked();
    void on_conf_pushButton_27_clicked();

    // Publication sorting
    void on_btnSortPubAsc_clicked();
    void on_btnSortPubDesc_clicked();

>>>>>>> Stashed changes
private:
    Ui::SmartMarket *ui;
    QSqlQueryModel  *publicationModel;
    QString         lastPubFilter;
    QChartView      *pieChartView;
    QChartView      *barChartView;
    QNetworkAccessManager *networkManager;

    // Données contextuelles pour les callbacks IA
    QString simId1, simId2, simTitre1, simTitre2, simContenu1, simContenu2;
    QString compIdStr, compTitre;

    // Conferences
    QWidget *conferenceWidget;
    QTableWidget *confTableWidget;
    QTableWidget *participantTableWidget;
    QLineEdit *confFilterLineEdit;
    ArduinoBridge *arduinoBridge;
    int conferencePageIndex;

    static const QStringList DOMAINES;

    void initDomaines();
    void initPublicationTable();
    void setupConferencePage();
    void loadConferenceTable();
    void loadParticipantTable();
    bool ensureConferenceTables(QSqlDatabase &db);
    bool createConferenceTables(QSqlDatabase &db);
    bool ensureParticipantUidColumn(QSqlDatabase &db);
    bool tableExists(QSqlDatabase &db, const QString &tableName);
    void initializeArduinoAccess();
    void handleArduinoUid(const QString &uid);
    void showConferencePage();
    void refreshAll();
    void createCharts();
    void refreshCharts();

    // Conference CRUD operations
    void addConference();
    void modifyConference();
    void deleteConference();
    void addParticipant();
    void modifyParticipant();
    void deleteParticipant();
    void sortConferencesByDateAsc();
    void sortConferencesByDateDesc();
    void exportConferencesToPDF();
    int applyConferenceFilterToTable();
    void filterConferences();
    void filterParticipants();

    // IA helpers
    void callAnthropicSimilarite(const QString &titre1, const QString &contenu1,
                                 const QString &titre2, const QString &contenu2);
    void callAnthropicCompletude(const QString &idStr,
                                 const QString &titre, const QString &source,
                                 const QString &domaine, const QString &date,
                                 const QString &statut, const QString &contenu);

    // Export PDF helper
    void exporterPDF(bool toutesPublications);
    void exporterExcel(bool toutesPublications);
};

#endif // SMARTMARKET_H
