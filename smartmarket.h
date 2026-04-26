#ifndef SMARTMARKET_H
#define SMARTMARKET_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include "publication.h"
#include "conference.h"
#include "participant.h"
#include "reviewer.h"
#include "arduino2.h"

// Structure simple pour le cache Arduino
struct DeadlineInfo {
    QString nom;
    QDate date;
};

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

    // Conference UI slots
    void on_conf_pushButton_5_clicked();
    void on_conf_pushButton_8_clicked();
    void on_conf_pushButton_21_clicked();
    void on_conf_pushButton_20_clicked();
    void on_conf_pushButton_19_clicked();
    void on_conf_pushButton_12_clicked();
    void on_conf_pushButton_10_clicked();
    void on_conf_pushButton_26_clicked();
    void on_conf_pushButton_27_clicked();
    void on_conf_pushButton_14_clicked();

    // Reviewers page (page_2) UI slots
    void on_revv_pushButton_22_clicked();
    void on_revv_pushButton_24_clicked();
    void on_rev_pushButton_12_clicked();
    void on_rev_pushButton_21_clicked();
    void on_rev_pushButton_10_clicked();
    void on_rev_pushButton_19_clicked();
    void on_rev_pushButton_8_clicked();
    void on_rev_pushButton_3_clicked();
    void on_rev_comboBox_14_currentIndexChanged(int index);
    void on_rev_comboBox_15_currentIndexChanged(int index);

    // Publication sorting
    void on_btnSortPubAsc_clicked();
    void on_btnSortPubDesc_clicked();

    // Arduino LCD Deadline Navigation
    void on_revv_btn_arduino_start_clicked();
    void on_revv_btn_arduino_prev_clicked();
    void on_revv_btn_arduino_next_clicked();
    void updateArduinoDisplay();

private:
    Ui::SmartMarket *ui;
    QSqlQueryModel  *publicationModel;
    QString         lastPubFilter;
    QChartView      *pieChartView;
    QChartView      *barChartView;
    QChartView      *reviewerPieChartView;
    QChartView      *reviewerBarChartView;
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
    void updateConferenceParticipantsChart();
    void updateConferenceDaysChart();
    void updateConferenceCalendar();
    void on_conf_calendarWidget_selectionChanged();
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
    void loadReviewerTable();
    void applyReviewerSort();
    void refreshReviewerCharts();

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

    int reviewerSortColumn = 5; // 5: Nb Evaluations, 6: Score Fiabilite
    Qt::SortOrder reviewerSortOrder = Qt::AscendingOrder;

    // CHAT BOX REVIEWERS
    void setupReviewerChat();
    void sendChatMessage();
    void appendChatMessage(const QString &text, bool isUser);
    void saveChatHistory();
    void loadChatHistory();
    void toggleChatPanel();

    // RECOMMANDATION INTELLIGENTE REVIEWERS
    void setupReviewerRecommendation();
    void findBestReviewersForPublication();
    double calculateMatchingScore(Reviewer &r, const QString &domainePublication);
    void assignReviewerToPublication();

    QStringList chatHistory;

    // ── Métier #4 : Rappels Deadlines ──────────────────────────────
    QTimer *deadlineTimer;               // Timer périodique de vérification
    void setupDeadlineTimer();           // Démarre le timer (appelé dans le constructeur)
    void checkDeadlines();               // Vérification silencieuse (appelée par le timer)
    void showDeadlineDialog();           // Fenêtre complète : retards + assignation deadline

    // Arduino LCD
    Arduino2 *arduinoLcd;
    QList<DeadlineInfo> m_arduinoDeadlines;
    int m_currentArduinoIndex;
};

#endif // SMARTMARKET_H
