#include "smartmarket.h"
#include "ui_smartmarket.h"
#include "arduino.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include <QGroupBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>
#include <QSslSocket>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QSpinBox>
#include <QDateEdit>
#include <QUrl>
#include <QRegularExpression>
#include <QProcess>
#include <QFileInfo>
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QImage>
#include <QPdfWriter>
#include <QPageLayout>
#include <QStandardPaths>
#include <QSerialPortInfo>
#include <QTimer>
#include <QScrollBar>

// PDF export via Qt
#include <QPrinter>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QRect>
#include <QFontMetrics>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QTextCharFormat>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

// â”€â”€ Configuration Ollama (Gratuit, 100% Local) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
// Ollama fonctionne complÃ¨tement gratuitement sur votre PC
// Installation: https://ollama.ai
// Puis: ollama pull neural-chat (plus rapide) ou ollama pull mistral
static const QString OLLAMA_ENDPOINT = "http://localhost:11434/api/generate";
static const QString OLLAMA_MODEL    = "neural-chat";  // Plus rapide que mistral

static QString formatPercentage(int value, int total)
{
    if (total <= 0)
        return QStringLiteral("0%");

    const double ratio = (100.0 * value) / total;
    const double bounded = qBound(0.0, ratio, 100.0);
    return QString::number(bounded, 'f', 1) + "%";
}

static QString localReviewerAssistantReply(const QString &question)
{
    const QString q = question.trimmed().toLower();

    if (q.isEmpty()) {
        return "Posez une question courte, par exemple:\n✅ ajouter reviewer\n✅ meilleurs reviewers pour publication 42\n✅ suggerer reviewer pour informatique";
    }

    // ✅ NOUVEAU: SYSTEME DE RECOMMANDATION INTELLIGENTE REVIEWERS
    // ✅ DETECTION AMELIOREE: TOUS LES CAS POSSIBLES
    if (q.contains("meilleur") || q.contains("recommand") || q.contains("sugger") || 
        q.contains("assigner") || q.contains("publication") || q.contains("reviewer") ||
        q.contains("domaine") || q.contains("specialite") || q.contains("pour") ||
        q.contains("quel") || q.contains("qui") || q.contains("lequel") ||
        q.contains("meilleurs") || q.contains("recommander") || q.contains("suggerer"))
    {
        // Extraire ID publication si présent
        QRegularExpression rxIdPub("publication\\s*(\\d+)");
        QRegularExpressionMatch match = rxIdPub.match(q);
        QString idPubStr = match.hasMatch() ? match.captured(1) : "";

        // Extraire domaine si présent
        const QStringList DOMAINES_LOCAL = {
            "Informatique", "Mathematiques", "Physique", "Chimie", "Biologie",
            "Medecine", "Droit", "Economie", "Histoire", "Geographie",
            "Philosophie", "Litterature", "Arts", "Sport", "Autre"
        };
        
        QString domaineRecherche;
        for (const QString &d : DOMAINES_LOCAL) {
            if (q.contains(d.toLower())) {
                domaineRecherche = d;
                break;
            }
        }

        // Chercher tous les reviewers dans la base
        Reviewer r;
        QSqlQueryModel *model = r.afficher();

        struct ReviewerScore {
            int id;
            QString nom;
            QString email;
            QString specialite;
            int nbEval;
            double score;
            double matchingScore;
        };

        QList<ReviewerScore> liste;

        for (int i = 0; i < model->rowCount(); ++i) {
            ReviewerScore rs;
            rs.id = model->index(i, 0).data().toInt();
            rs.nom = model->index(i, 1).data().toString();
            rs.email = model->index(i, 2).data().toString();
            rs.specialite = model->index(i, 3).data().toString();
            rs.nbEval = model->index(i, 5).data().toInt();
            rs.score = model->index(i, 6).data().toDouble();

            // Calcul du score de matching
            double scoreMatching = 0.0;

            // 🔹 60% du score: correspondance spécialité / domaine
            if (!domaineRecherche.isEmpty() && !rs.specialite.isEmpty()) {
                if (rs.specialite.toLower().contains(domaineRecherche.toLower()) ||
                    domaineRecherche.toLower().contains(rs.specialite.toLower())) {
                    scoreMatching += 60.0;
                } else {
                    scoreMatching += 20.0;
                }
            } else {
                scoreMatching += 30.0;
            }

            // 🔹 30% du score: score fiabilité (max 10)
            scoreMatching += (rs.score / 10.0) * 30.0;

            // 🔹 10% du score: nombre d'évaluations (expérience)
            scoreMatching += qMin(10.0, rs.nbEval / 5.0);

            rs.matchingScore = scoreMatching;
            liste.append(rs);
        }

        // Trier par score de matching DESC
        std::sort(liste.begin(), liste.end(), [](const ReviewerScore &a, const ReviewerScore &b) {
            return a.matchingScore > b.matchingScore;
        });

        // Construire la réponse
        QString res;

        if (!idPubStr.isEmpty()) {
            res += QString("✅ RECOMMANDATION POUR PUBLICATION ID %1:\n\n").arg(idPubStr);
        } else if (!domaineRecherche.isEmpty()) {
            res += QString("✅ MEILLEURS REVIEWERS POUR DOMAINE \"%1\":\n\n").arg(domaineRecherche.toUpper());
        } else {
            res += "✅ CLASSEMENT GENERAL DES MEILLEURS REVIEWERS:\n\n";
        }

        // Prendre TOP 3
        int limit = qMin(3, liste.size());
        for (int i = 0; i < limit; ++i) {
            const ReviewerScore &rev = liste[i];
            res += QString("%1. 🥇 %2\n")
                      .arg(i+1)
                      .arg(rev.nom);
            res += QString("   📧 Email: %1\n").arg(rev.email);
            res += QString("   🎯 Spécialité: %1\n").arg(rev.specialite.isEmpty() ? "Non renseignée" : rev.specialite);
            res += QString("   ⭐ Score Fiabilité: %1/10\n").arg(rev.score, 0, 'f', 1);
            res += QString("   📊 Nb Evaluations: %1\n").arg(rev.nbEval);
            res += QString("   💯 Score Matching: %1%\n\n").arg(qRound(rev.matchingScore));
        }

        if (liste.isEmpty()) {
            res += "❌ Aucun reviewer trouvé dans la base de données.";
        } else {
            res += "💡 Conseil: Choisissez toujours un reviewer avec une spécialité correspondante et un score > 7.";
        }

        return res;
    }

    if (q.contains("ajout") || q.contains("add") || q.contains("creer")) {
        return "Ajout reviewer:\n"
               "1) Remplir ID (rev_lineEdit_18), Nom (rev_lineEdit_19), Email (rev_lineEdit_20).\n"
               "2) Optionnel: Specialite (lineEdit_3), Affiliation (lineEdit_2), Nb Evaluations (rev_lineEdit_25), Score (rev_lineEdit_26), ID Publication (rev_lineEdit_27).\n"
               "3) Cliquer sur Add.";
    }

    if (q.contains("modif") || q.contains("update") || q.contains("modifier")) {
        return "Modification reviewer:\n"
               "1) Entrer ID reviewer a modifier (rev_lineEdit_18).\n"
               "2) Mettre les nouvelles valeurs (au minimum Nom + Email).\n"
               "3) Cliquer sur Modifier.";
    }

    if (q.contains("suppr") || q.contains("delete") || q.contains("remove")) {
        return "Suppression reviewer:\n"
               "1) Entrer ID reviewer dans rev_lineEdit_18.\n"
               "2) Cliquer sur Suprrimer puis confirmer.";
    }

    if (q.contains("recherche") || q.contains("search") || q.contains("filtre")) {
        return "Recherche reviewer:\n"
               "- ID: rev_idsearch_3\n"
               "- Nom: rev_lineEdit_7\n"
               "- Specialite: lineEdit\n"
               "La table se filtre automatiquement pendant la saisie.";
    }

    if (q.contains("tri") || q.contains("sort")) {
        return "Tri reviewer:\n"
               "- Combo gauche: tri par Score Fiabilite\n"
               "- Combo droite: tri par Nb Evaluations\n"
               "Choisir Croissant ou Decroissant.";
    }

    if (q.contains("reset") || q.contains("reinit")) {
        return "Reset recherche:\n"
               "Cliquer sur le bouton Reset pour vider les champs de recherche et recharger toute la liste.";
    }

    if (q.contains("email")) {
        return "Regle email:\n"
               "L'email doit contenir '@' et '.' et avoir une longueur valide (ex: nom@domaine.com).";
    }

    if (q.contains("score")) {
        return "Regle score fiabilite:\n"
               "Le score doit etre entre 1 et 10.";
    }

    if (q.contains("publication") || q.contains("id pub")) {
        return "ID Publication:\n"
               "Remplir rev_lineEdit_27 avec un ID existant dans la table PUBLICATION.";
    }

    return "Posez votre question! Je peux vous aider sur:\n✅ Ajout / Modif / Suppression reviewer\n✅ Recherche et tri\n✅ ✨ RECOMMANDATION INTELLIGENTE des meilleurs reviewers\n✅ Calcul score matching publication / reviewer";
}

// â”€â”€ Domaines â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
const QStringList SmartMarket::DOMAINES = {
    "Informatique", "Mathematiques", "Physique", "Chimie", "Biologie",
    "Medecine", "Droit", "Economie", "Histoire", "Geographie",
    "Philosophie", "Litterature", "Arts", "Sport", "Autre"
};

// ================================================================
// CONSTRUCTEUR
// ================================================================
SmartMarket::SmartMarket(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartMarket)
    , publicationModel(new QSqlQueryModel(this))
    , pieChartView(nullptr)
    , barChartView(nullptr)
    , networkManager(new QNetworkAccessManager(this))
    , arduinoBridge(nullptr)
    , deadlineTimer(nullptr)
    , arduinoLcd(new Arduino2(this))
    , m_currentArduinoIndex(-1)
{
    ui->setupUi(this);
    ui->lineEdit_10->setEchoMode(QLineEdit::Password);

    connect(ui->pushButton_17, &QPushButton::clicked,
            this, &SmartMarket::openConferenceModule);

    conferenceWidget = nullptr;
    confTableWidget = nullptr;
    participantTableWidget = nullptr;
    confFilterLineEdit = nullptr;
    conferencePageIndex = -1;

    setupConferencePage();

    // Configuration SSL/TLS pour les appels API
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    
    initDomaines();
    initializeArduinoAccess();

    if (ui->rev_comboBox_14) {
        connect(ui->rev_comboBox_14, qOverload<int>(&QComboBox::currentIndexChanged),
                this, &SmartMarket::on_rev_comboBox_14_currentIndexChanged);
    }
    if (ui->rev_comboBox_15) {
        connect(ui->rev_comboBox_15, qOverload<int>(&QComboBox::currentIndexChanged),
                this, &SmartMarket::on_rev_comboBox_15_currentIndexChanged);
    }
    if (ui->rev_idsearch_3) {
        connect(ui->rev_idsearch_3, &QLineEdit::textChanged,
                this, [this](const QString &) { on_rev_pushButton_8_clicked(); });
    }
    if (ui->rev_lineEdit_7) {
        connect(ui->rev_lineEdit_7, &QLineEdit::textChanged,
                this, [this](const QString &) { on_rev_pushButton_8_clicked(); });
    }
    if (ui->lineEdit) {
        connect(ui->lineEdit, &QLineEdit::textChanged,
                this, [this](const QString &) { on_rev_pushButton_8_clicked(); });
    }

    // SETUP CHAT BOX REVIEWERS
    setupReviewerChat();

    // SETUP RECOMMANDATION INTELLIGENTE REVIEWERS
    setupReviewerRecommendation();

    // SETUP RAPPELS DEADLINES (Métier #4)
    setupDeadlineTimer();

    // Connexion du bouton "⏰ Deadlines" dans la sidebar Reviewers
    // findChild est plus robuste que ui-> (fonctionne même sans recompil qmake)
    QPushButton *btnDeadline = this->findChild<QPushButton*>("revv_pushButton_deadline");
    if (btnDeadline) {
        connect(btnDeadline, &QPushButton::clicked,
                this, &SmartMarket::showDeadlineDialog);
        qDebug() << "[Deadline] Bouton deadline connecté avec succès.";
    } else {
        qDebug() << "[Deadline] AVERTISSEMENT: bouton revv_pushButton_deadline introuvable!";
    }

    // Connexion des nouveaux boutons Arduino LCD
    QPushButton *btnArdStart = this->findChild<QPushButton*>("revv_btn_arduino_start");
    QPushButton *btnArdPrev  = this->findChild<QPushButton*>("revv_btn_arduino_prev");
    QPushButton *btnArdNext  = this->findChild<QPushButton*>("revv_btn_arduino_next");

    if (btnArdStart) connect(btnArdStart, &QPushButton::clicked, this, &SmartMarket::on_revv_btn_arduino_start_clicked);
    if (btnArdPrev)  connect(btnArdPrev,  &QPushButton::clicked, this, &SmartMarket::on_revv_btn_arduino_prev_clicked);
    if (btnArdNext)  connect(btnArdNext,  &QPushButton::clicked, this, &SmartMarket::on_revv_btn_arduino_next_clicked);

    ui->stackedWidgetMain->setCurrentIndex(0);
}

SmartMarket::~SmartMarket()
{
    delete ui;
}

// ================================================================
// INIT DOMAINES
// ================================================================
void SmartMarket::initDomaines()
{
    ui->comboBox_5->clear();
    for (const QString &d : DOMAINES)
        ui->comboBox_5->addItem(d);

    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("Tous");
    for (const QString &d : DOMAINES)
        ui->comboBox_2->addItem(d);
}

void SmartMarket::setupConferencePage()
{
    // The conference page (page2) is now defined in the UI file
    // We just need to set the page index and connect the signals
    conferencePageIndex = 2; // page2 is the third page (index 2) in stackedWidgetMain

    // Connect the conference button to navigate to the conference page
    connect(ui->pushButton_17, &QPushButton::clicked, this, &SmartMarket::openConferenceModule);

    // Connect the publications button in the conference sidebar to go back to publications
    connect(ui->conf_pushButton_13, &QPushButton::clicked, this, [this]() {
        ui->stackedWidgetMain->setCurrentIndex(1); // Go back to publications page
    });

    // Connect conference table refresh/filtering
    connect(ui->conf_lineEdit_2, &QLineEdit::textChanged, this, &SmartMarket::loadConferenceTable);
        connect(ui->conf_calendarWidget, &QCalendarWidget::selectionChanged,
            this, &SmartMarket::on_conf_calendarWidget_selectionChanged);

    // Connect sorting buttons
    connect(ui->conf_pushButton_17, &QPushButton::clicked, this, &SmartMarket::sortConferencesByDateAsc);
    connect(ui->conf_pushButton_18, &QPushButton::clicked, this, &SmartMarket::sortConferencesByDateDesc);

    // Load initial data
    loadConferenceTable();
}

void SmartMarket::loadConferenceTable()
{
    QTableWidget *confTableWidget = ui->conf_tableWidget_7;
    if (!confTableWidget) return;

    QString filter = ui->conf_lineEdit_2 ? ui->conf_lineEdit_2->text().trimmed().toLower() : QString();
    Conference c;
    QSqlQueryModel *model = c.rechercher(filter);

    confTableWidget->setRowCount(0);
    for (int i = 0; i < model->rowCount(); ++i) {
        confTableWidget->insertRow(i);
        confTableWidget->setItem(i, 0, new QTableWidgetItem(model->index(i, 0).data().toString()));
        confTableWidget->setItem(i, 1, new QTableWidgetItem(model->index(i, 1).data().toString()));
        confTableWidget->setItem(i, 2, new QTableWidgetItem(model->index(i, 2).data().toString()));
        confTableWidget->setItem(i, 3, new QTableWidgetItem(model->index(i, 3).data().toDate().toString("yyyy-MM-dd")));
        confTableWidget->setItem(i, 4, new QTableWidgetItem(model->index(i, 4).data().toString()));
        confTableWidget->setItem(i, 5, new QTableWidgetItem(model->index(i, 5).data().toString()));
    }

    loadParticipantTable();
    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
    updateConferenceCalendar();
}

void SmartMarket::loadParticipantTable()
{
    QTableWidget *participantTableWidget = ui->conf_tableWidget_2;
    if (!participantTableWidget) return;

    Participant p;
    QSqlQueryModel *model = p.afficher();

    participantTableWidget->setRowCount(0);
    participantTableWidget->setColumnCount(4);
    participantTableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Id Conferences", "UID RFID"});

    for (int i = 0; i < model->rowCount(); ++i) {
        participantTableWidget->insertRow(i);
        participantTableWidget->setItem(i, 0, new QTableWidgetItem(model->index(i, 0).data().toString()));
        participantTableWidget->setItem(i, 1, new QTableWidgetItem(model->index(i, 1).data().toString()));
        participantTableWidget->setItem(i, 2, new QTableWidgetItem(model->index(i, 2).data().toString()));
        participantTableWidget->setItem(i, 3, new QTableWidgetItem(model->index(i, 3).data().toString()));
    }

    updateConferenceParticipantsChart();
}

void SmartMarket::updateConferenceCalendar()
{
    if (!ui->conf_calendarWidget || !ui->conf_tableWidget_7)
        return;

    ui->conf_calendarWidget->setDateTextFormat(QDate(), QTextCharFormat());

    const int rows = ui->conf_tableWidget_7->rowCount();
    QTextCharFormat fmt;
    fmt.setForeground(Qt::white);
    fmt.setBackground(QBrush(Qt::red));
    fmt.setFontWeight(QFont::Bold);

    for (int r = 0; r < rows; ++r)
    {
        const QString dateStr = ui->conf_tableWidget_7->item(r, 3) ? ui->conf_tableWidget_7->item(r, 3)->text() : QString();
        const QDate d = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (d.isValid())
            ui->conf_calendarWidget->setDateTextFormat(d, fmt);
    }
}

void SmartMarket::on_conf_calendarWidget_selectionChanged()
{
    if (!ui->conf_calendarWidget || !ui->conf_tableWidget_7)
        return;

    const QDate selected = ui->conf_calendarWidget->selectedDate();
    if (!selected.isValid())
        return;

    QStringList lines;
    const int rows = ui->conf_tableWidget_7->rowCount();
    for (int r = 0; r < rows; ++r)
    {
        const QString dateStr = ui->conf_tableWidget_7->item(r, 3) ? ui->conf_tableWidget_7->item(r, 3)->text() : QString();
        const QDate d = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (d != selected)
            continue;

        const QString nom = ui->conf_tableWidget_7->item(r, 1) ? ui->conf_tableWidget_7->item(r, 1)->text() : "(sans nom)";
        const QString lieu = ui->conf_tableWidget_7->item(r, 2) ? ui->conf_tableWidget_7->item(r, 2)->text() : "";
        const QString participants = ui->conf_tableWidget_7->item(r, 5) ? ui->conf_tableWidget_7->item(r, 5)->text() : "";
        lines << nom + (lieu.isEmpty() ? "" : " - " + lieu) + (participants.isEmpty() ? "" : " | Participants: " + participants);
    }

    if (lines.isEmpty())
    {
        QMessageBox::information(this, "Calendrier", "Aucune conference prevue ce jour.");
    }
    else
    {
        QMessageBox::information(this, "Calendrier", lines.join("\n"));
    }
}

void SmartMarket::updateConferenceParticipantsChart()
{
    if (!ui->conf_graphicsView1 || !ui->conf_tableWidget_7)
        return;

    const QSize viewSize = ui->conf_graphicsView1->viewport()->size();
    const int sceneW = viewSize.width() > 0 ? viewSize.width() : 560;
    const int sceneH = viewSize.height() > 0 ? viewSize.height() : 340;

    if (QGraphicsScene *oldScene = ui->conf_graphicsView1->scene()) {
        ui->conf_graphicsView1->setScene(nullptr);
        delete oldScene;
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->conf_graphicsView1->setScene(scene);
    scene->setSceneRect(0, 0, sceneW, sceneH);
    scene->setBackgroundBrush(Qt::white);

    const int rows = ui->conf_tableWidget_7->rowCount();
    if (rows == 0) {
        QGraphicsTextItem *text = scene->addText("Aucune confÃ©rence trouvÃ©e");
        text->setPos(sceneW / 2 - 90, sceneH / 2);
        return;
    }

    QVector<QString> names;
    QVector<int> counts;
    QVector<double> percentages;
    int totalCount = 0;
    for (int r = 0; r < rows; ++r) {
        const QString name = ui->conf_tableWidget_7->item(r, 1) ? ui->conf_tableWidget_7->item(r, 1)->text() : QString();
        const int count = ui->conf_tableWidget_7->item(r, 5) ? ui->conf_tableWidget_7->item(r, 5)->text().toInt() : 0;
        names.append(name);
        counts.append(count);
        totalCount += count;
    }

    if (totalCount <= 0) {
        QGraphicsTextItem *text = scene->addText("Aucun participant renseignÃ©");
        text->setPos(sceneW / 2 - 100, sceneH / 2);
        return;
    }

    for (int count : counts) {
        const double ratio = (100.0 * count) / totalCount;
        percentages.append(qBound(0.0, ratio, 100.0));
    }

    const int leftMargin = 60;
    const int rightMargin = 40;
    const int topMargin = 30;
    const int bottomMargin = 130;
    const int originX = leftMargin;
    const int originY = sceneH - bottomMargin;
    const int axisMaxY = topMargin;
    const int axisMaxX = sceneW - rightMargin;

    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    scene->addLine(originX, axisMaxY, originX, originY, axisPen);
    scene->addLine(originX, originY, axisMaxX, originY, axisPen);

    const double availableWidth = axisMaxX - originX;
    const int barCount = counts.size();
    const int minBarWidth = 18;
    const int spacing = 12;
    int barWidth = static_cast<int>((availableWidth - spacing * (barCount - 1)) / barCount);
    barWidth = qMax(minBarWidth, barWidth);
    const int totalBarsWidth = barCount * barWidth + spacing * (barCount - 1);
    const int startX = originX + qMax(0, static_cast<int>((availableWidth - totalBarsWidth) / 2));

    const double availableHeight = originY - axisMaxY - 10;
    const double scaleY = availableHeight / 100.0;

    QFont tickFont("Segoe UI", 9);
    QPen tickPen(Qt::black);
    tickPen.setWidth(1);

    const int tickCount = 4;
    const double tickStep = 100.0 / tickCount;
    for (int t = 0; t <= tickCount; ++t) {
        const double value = tickStep * t;
        const double yTick = originY - value * scaleY;
        scene->addLine(originX - 6, yTick, originX, yTick, tickPen);
        QGraphicsTextItem *tickLabel = scene->addText(QString::number(static_cast<int>(value)) + "%");
        tickLabel->setFont(tickFont);
        tickLabel->setDefaultTextColor(Qt::black);
        tickLabel->setPos(originX - 55, yTick - 8);
    }

    const QList<QColor> palette = {
        QColor("#e6194b"), QColor("#f58231"), QColor("#ffe119"), QColor("#bfef45"),
        QColor("#3cb44b"), QColor("#42d4f4"), QColor("#4363d8"), QColor("#911eb4"),
        QColor("#f032e6"), QColor("#fabebe"), QColor("#9a6324"), QColor("#808000")
    };

    QPen barPen(Qt::black);
    for (int i = 0; i < barCount; ++i) {
        const int barHeight = static_cast<int>(percentages[i] * scaleY);
        const int x = startX + i * (barWidth + spacing);
        const int y = originY - barHeight;
        QBrush brush(palette[i % palette.size()], Qt::SolidPattern);
        scene->addRect(x, y, barWidth, barHeight, barPen, brush);

        const QString percentLabel = formatPercentage(counts[i], totalCount);
        QGraphicsTextItem *percentItem = scene->addText(percentLabel);
        percentItem->setFont(tickFont);
        percentItem->setDefaultTextColor(Qt::black);
        percentItem->setPos(x, qMax(axisMaxY, y - 38));

        const int xCenter = x + barWidth / 2;
        scene->addLine(xCenter, originY, xCenter, originY + 6, tickPen);

        QString displayName = names[i];
        if (displayName.size() > 16)
            displayName = displayName.left(14) + "...";
        QGraphicsTextItem *nameItem = scene->addText(displayName);
        nameItem->setFont(tickFont);
        nameItem->setDefaultTextColor(Qt::black);
        nameItem->setPos(xCenter - 10, originY + 15);
        nameItem->setRotation(-45);
    }
}

void SmartMarket::updateConferenceDaysChart()
{
    if (!ui->conf_graphicsView1_2 || !ui->conf_tableWidget_7)
        return;

    const QSize viewSize = ui->conf_graphicsView1_2->viewport()->size();
    const int sceneW = viewSize.width() > 0 ? viewSize.width() : 560;
    const int sceneH = viewSize.height() > 0 ? viewSize.height() : 340;

    if (QGraphicsScene *oldScene = ui->conf_graphicsView1_2->scene()) {
        ui->conf_graphicsView1_2->setScene(nullptr);
        delete oldScene;
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->conf_graphicsView1_2->setScene(scene);
    scene->setSceneRect(0, 0, sceneW, sceneH);
    scene->setBackgroundBrush(Qt::white);

    const int rows = ui->conf_tableWidget_7->rowCount();
    if (rows == 0) {
        QGraphicsTextItem *text = scene->addText("Aucune confÃ©rence trouvÃ©e");
        text->setPos(sceneW / 2 - 90, sceneH / 2);
        return;
    }

    QMap<QString, int> dayCountsMap;
    QStringList dayOrder;
    for (int r = 0; r < rows; ++r) {
        const QString dayLabel = ui->conf_tableWidget_7->item(r, 3) ? ui->conf_tableWidget_7->item(r, 3)->text() : QString();
        if (!dayCountsMap.contains(dayLabel))
            dayOrder.append(dayLabel);
        dayCountsMap[dayLabel] += 1;
    }

    QVector<QString> days;
    QVector<int> counts;
    QVector<double> percentages;
    int totalCount = 0;
    for (const QString &d : dayOrder) {
        const int value = dayCountsMap.value(d);
        days.append(d);
        counts.append(value);
        totalCount += value;
    }

    if (totalCount <= 0) {
        QGraphicsTextItem *text = scene->addText("Aucun jour planifiÃ©");
        text->setPos(sceneW / 2 - 80, sceneH / 2);
        return;
    }

    for (int count : counts) {
        const double ratio = (100.0 * count) / totalCount;
        percentages.append(qBound(0.0, ratio, 100.0));
    }

    const int leftMargin = 60;
    const int rightMargin = 40;
    const int topMargin = 30;
    const int bottomMargin = 120;
    const int originX = leftMargin;
    const int originY = sceneH - bottomMargin;
    const int axisMaxY = topMargin;
    const int axisMaxX = sceneW - rightMargin;

    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    scene->addLine(originX, axisMaxY, originX, originY, axisPen);
    scene->addLine(originX, originY, axisMaxX, originY, axisPen);

    QFont tickFont("Segoe UI", 9);
    QPen tickPen(Qt::black);
    tickPen.setWidth(1);

    const int tickCount = 4;
    const double tickStep = 100.0 / tickCount;
    const double availableHeight = originY - axisMaxY - 10;
    const double scaleY = availableHeight / 100.0;

    for (int t = 0; t <= tickCount; ++t) {
        const double value = tickStep * t;
        const double yTick = originY - value * scaleY;
        scene->addLine(originX - 6, yTick, originX, yTick, tickPen);
        QGraphicsTextItem *tickLbl = scene->addText(QString::number(static_cast<int>(value)) + "%");
        tickLbl->setFont(tickFont);
        tickLbl->setDefaultTextColor(Qt::black);
        tickLbl->setPos(originX - 45, yTick - 8);
    }

    const double availableWidth = axisMaxX - originX;
    const int barCount = counts.size();
    const int minBarWidth = 18;
    const int spacing = 12;
    int barWidth = static_cast<int>((availableWidth - spacing * (barCount - 1)) / barCount);
    barWidth = qMax(minBarWidth, barWidth);
    const int totalBarsWidth = barCount * barWidth + spacing * (barCount - 1);
    const int startX = originX + qMax(0, static_cast<int>((availableWidth - totalBarsWidth) / 2));

    const QList<QColor> palette = {
        QColor("#4e79a7"), QColor("#f28e2c"), QColor("#e15759"), QColor("#76b7b2"),
        QColor("#59a14f"), QColor("#edc949"), QColor("#af7aa1"), QColor("#ff9da7"),
        QColor("#9c755f"), QColor("#bab0ab")
    };

    QPen barPen(Qt::black);
    for (int i = 0; i < barCount; ++i) {
        const int barHeight = static_cast<int>(percentages[i] * scaleY);
        const int x = startX + i * (barWidth + spacing);
        const int y = originY - barHeight;
        QBrush brush(palette[i % palette.size()], Qt::SolidPattern);
        scene->addRect(x, y, barWidth, barHeight, barPen, brush);

        const QString percentLabel = formatPercentage(counts[i], totalCount);
        QGraphicsTextItem *percentItem = scene->addText(percentLabel);
        percentItem->setFont(tickFont);
        percentItem->setDefaultTextColor(Qt::black);
        percentItem->setPos(x, qMax(axisMaxY, y - 38));

        const int xCenter = x + barWidth / 2;
        scene->addLine(xCenter, originY, xCenter, originY + 6, tickPen);

        QString displayName = days[i];
        if (displayName.size() > 16)
            displayName = displayName.left(14) + "...";
        QGraphicsTextItem *dayLbl = scene->addText(displayName);
        dayLbl->setFont(tickFont);
        dayLbl->setDefaultTextColor(Qt::black);
        dayLbl->setPos(xCenter - 10, originY + 15);
        dayLbl->setRotation(-45);
    }
}

bool SmartMarket::tableExists(QSqlDatabase &db, const QString &tableName)
{
    const QStringList tables = db.tables(QSql::Tables);
    for (const QString &name : tables) {
        if (QString::compare(name, tableName, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

bool SmartMarket::createConferenceTables(QSqlDatabase &db)
{
    if (!tableExists(db, "conference")) {
        QSqlQuery q(db);
        QString ddl = "CREATE TABLE conference ("
                      "idconference NUMBER(10) PRIMARY KEY, "
                      "nom VARCHAR2(255), "
                      "lieu VARCHAR2(255), "
                      "datedebut DATE, "
                      "theme VARCHAR2(255))";
        if (!q.exec(ddl))
            return false;
    }

    if (!tableExists(db, "participant")) {
        QSqlQuery q(db);
        QString ddl = "CREATE TABLE participant ("
                      "id NUMBER(10) PRIMARY KEY, "
                      "nom VARCHAR2(255), "
                      "idconference NUMBER(10), "
                      "uid_rfid VARCHAR2(32))";
        if (!q.exec(ddl))
            return false;

        QSqlQuery fk(db);
        fk.exec("ALTER TABLE participant ADD CONSTRAINT fk_part_conf "
                "FOREIGN KEY (idconference) REFERENCES conference(idconference)");
    }

    return true;
}

bool SmartMarket::ensureConferenceTables(QSqlDatabase &db)
{
    if (tableExists(db, "conference") && tableExists(db, "participant"))
        return ensureParticipantUidColumn(db);

    return createConferenceTables(db) && ensureParticipantUidColumn(db);
}

bool SmartMarket::ensureParticipantUidColumn(QSqlDatabase &db)
{
    QSqlQuery probe(db);
    if (probe.exec("SELECT uid_rfid FROM ADAM.participant WHERE 1=0"))
        return true;

    QSqlQuery alter(db);
    if (!alter.exec("ALTER TABLE ADAM.participant ADD uid_rfid VARCHAR2(32)"))
        return false;

    QSqlQuery idx(db);
    idx.exec("CREATE UNIQUE INDEX idx_participant_uid_rfid ON ADAM.participant(uid_rfid)");
    return true;
}

void SmartMarket::initializeArduinoAccess()
{
    arduinoBridge = new ArduinoBridge(this);
    connect(arduinoBridge, &ArduinoBridge::uidScanned, this, &SmartMarket::handleArduinoUid);
    connect(arduinoBridge, &ArduinoBridge::rawLineReceived, this, [](const QString &line) {
        qDebug() << "Arduino RX:" << line;
    });
    connect(arduinoBridge, &ArduinoBridge::statusChanged, this, [this](const QString &status) {
        qDebug() << "Arduino:" << status;
        if (statusBar())
            statusBar()->showMessage(status, 5000);
    });
    connect(arduinoBridge, &ArduinoBridge::errorOccurred, this, [](const QString &error) {
        qWarning() << "Arduino error:" << error;
    });

    const QString forcedPort = qEnvironmentVariable("SMARTMARKET_ARDUINO_PORT").trimmed();

    bool baudOk = false;
    const int forcedBaud = qEnvironmentVariable("SMARTMARKET_ARDUINO_BAUD").trimmed().toInt(&baudOk);

    QList<qint32> baudCandidates;
    if (baudOk && forcedBaud > 0) {
        baudCandidates << forcedBaud;
    } else {
        baudCandidates << 9600 << 115200;
    }

    QStringList portCandidates;
    if (!forcedPort.isEmpty()) {
        portCandidates << forcedPort;
    } else {
        const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        QStringList preferredPorts;
        QStringList fallbackPorts;
        QStringList bluetoothPorts;

        for (const QSerialPortInfo &info : ports) {
            const QString details =
                (info.portName() + " " + info.description() + " " + info.manufacturer()).toLower();

            if (details.contains("bluetooth")) {
                bluetoothPorts << info.portName();
                continue;
            }

            if (details.contains("usb") || details.contains("arduino") ||
                details.contains("ch340") || details.contains("cp210") ||
                details.contains("ftdi")) {
                preferredPorts << info.portName();
            } else {
                fallbackPorts << info.portName();
            }
        }

        portCandidates << preferredPorts << fallbackPorts << bluetoothPorts;
    }

    bool connected = false;
    QString lastAttempt;
    for (const QString &portName : portCandidates) {
        for (qint32 baudRate : baudCandidates) {
            if (arduinoBridge->connectPort(portName, baudRate)) {
                connected = true;
                qInfo() << "Arduino connecte sur" << portName << "@" << baudRate;
                if (statusBar())
                    statusBar()->showMessage(QString("Arduino connecte: %1 @ %2").arg(portName).arg(baudRate), 6000);
                break;
            }

            lastAttempt = QString("%1 @ %2 -> %3")
                              .arg(portName)
                              .arg(baudRate)
                              .arg(arduinoBridge->lastError());
        }

        if (connected)
            break;
    }

    if (!connected) {
        qWarning() << "Arduino non connecte. Derniere tentative:" << lastAttempt;
        if (statusBar()) {
            statusBar()->showMessage(
                "Arduino indisponible - definir SMARTMARKET_ARDUINO_PORT et SMARTMARKET_ARDUINO_BAUD", 9000);
        }
    }
}

void SmartMarket::handleArduinoUid(const QString &uid)
{
    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open()) {
        if (arduinoBridge)
            arduinoBridge->sendCommand("DENY");
        return;
    }

    if (!ensureParticipantUidColumn(db)) {
        if (arduinoBridge)
            arduinoBridge->sendCommand("DENY");
        return;
    }

    QSqlQuery q(db);
    q.prepare("SELECT p.nom, p.idconference, NVL(c.lieu, '') "
              "FROM ADAM.participant p "
              "LEFT JOIN ADAM.conference c ON c.idconference = p.idconference "
              "WHERE UPPER(TRIM(p.uid_rfid)) = :uid");
    q.bindValue(":uid", uid.toUpper());

    if (!q.exec()) {
        if (arduinoBridge)
            arduinoBridge->sendCommand("DENY");
        return;
    }

    if (q.next()) {
        const QString nom = q.value(0).toString();
        const QString confId = q.value(1).toString();
        const QString lieu = q.value(2).toString();
        if (arduinoBridge)
            arduinoBridge->sendCommand("OPEN");

        const QString accessMsg = QString("Acces autorise: %1 | Conference: %2 | Lieu: %3")
                                      .arg(nom, confId, lieu.isEmpty() ? "N/A" : lieu);
        qInfo() << accessMsg;
        if (statusBar())
            statusBar()->showMessage(accessMsg, 6000);
    } else {
        if (arduinoBridge)
            arduinoBridge->sendCommand("DENY");
        if (statusBar())
            statusBar()->showMessage("Badge inconnu - acces refuse", 6000);
    }
}

// ================================================================
// INIT TABLE PUBLICATIONS
// ================================================================
void SmartMarket::initPublicationTable()
{
    Publication p;
    publicationModel = p.afficher();
    
    ui->tableView->setModel(publicationModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->show();

    // Connect sorting buttons only if they exist in the currently loaded UI.
    if (QPushButton *sortAscButton = findChild<QPushButton*>("btnSortPubAsc")) {
        connect(sortAscButton, &QPushButton::clicked,
                this, &SmartMarket::on_btnSortPubAsc_clicked, Qt::UniqueConnection);
    }
    if (QPushButton *sortDescButton = findChild<QPushButton*>("btnSortPubDesc")) {
        connect(sortDescButton, &QPushButton::clicked,
                this, &SmartMarket::on_btnSortPubDesc_clicked, Qt::UniqueConnection);
    }
}

// ================================================================
// REFRESH
// ================================================================
void SmartMarket::refreshAll()
{
    Publication p;
    publicationModel = p.afficher();
    ui->tableView->setModel(publicationModel);
    ui->tableView->reset();
    refreshCharts();
}

// ================================================================
// GRAPHIQUES
// ================================================================
void SmartMarket::createCharts()
{
    if (ui->chartWidget && !ui->chartWidget->layout()) {
        QVBoxLayout *l = new QVBoxLayout(ui->chartWidget);
        l->setContentsMargins(0, 0, 0, 0);
    }
    if (ui->domainChartWidget && !ui->domainChartWidget->layout()) {
        QVBoxLayout *l = new QVBoxLayout(ui->domainChartWidget);
        l->setContentsMargins(0, 0, 0, 0);
    }
    refreshCharts();
}

void SmartMarket::refreshCharts()
{
    if (pieChartView && ui->chartWidget && ui->chartWidget->layout()) {
        ui->chartWidget->layout()->removeWidget(pieChartView);
        pieChartView->deleteLater();
        pieChartView = nullptr;
    }
    if (barChartView && ui->domainChartWidget && ui->domainChartWidget->layout()) {
        ui->domainChartWidget->layout()->removeWidget(barChartView);
        barChartView->deleteLater();
        barChartView = nullptr;
    }

    if (!ui->chartWidget || !ui->domainChartWidget)
        return;

    QSqlQuery qStat;
    if (!qStat.exec("SELECT NVL(STATUT, 'Inconnu'), COUNT(*) FROM ADAM.PUBLICATION GROUP BY NVL(STATUT, 'Inconnu')")) {
        QMessageBox::warning(this, "Graphiques", "Impossible de charger les statuts : " + qStat.lastError().text());
        return;
    }

    struct StatRow { QString label; int count; };
    QList<StatRow> statRows;
    int statTotal = 0;
    while (qStat.next()) {
        StatRow row;
        row.label = qStat.value(0).toString().trimmed();
        if (row.label.isEmpty())
            row.label = "Inconnu";
        row.count = qStat.value(1).toInt();
        statRows << row;
        statTotal += row.count;
    }

    QPieSeries *pie = new QPieSeries();
    pie->setHoleSize(0.0);
    pie->setPieSize(0.80);
    for (const StatRow &row : statRows) {
        pie->append(row.label, row.count);
    }

    for (QPieSlice *sl : pie->slices()) {
        const double percent = (statTotal > 0) ? (sl->value() * 100.0 / statTotal) : 0.0;
        sl->setLabelVisible(true);
        sl->setLabelPosition(QPieSlice::LabelOutside);
        sl->setLabel(QString("%1 (%2%)").arg(sl->label()).arg(QString::number(percent, 'f', 1)));
    }

    QChart *pc = new QChart();
    pc->addSeries(pie);
    pc->setTitle("RÃ©partition par statut (pourcentages)");
    pc->legend()->setAlignment(Qt::AlignRight);
    pc->setBackgroundVisible(false);
    pc->setAnimationOptions(QChart::AllAnimations);
    pieChartView = new QChartView(pc);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    ui->chartWidget->layout()->addWidget(pieChartView);

    QSqlQuery qDom;
    if (!qDom.exec(
        "SELECT CASE WHEN TRIM(NVL(DOMAINE,'') ) = '' THEN 'Inconnu' ELSE DOMAINE END AS DOMAINE, "
        "COUNT(*) AS CNT "
        "FROM ADAM.PUBLICATION "
        "GROUP BY CASE WHEN TRIM(NVL(DOMAINE,'')) = '' THEN 'Inconnu' ELSE DOMAINE END "
        "ORDER BY CNT DESC, DOMAINE"
    )) {
        QMessageBox::warning(this, "Graphiques", "Impossible de charger les domaines : " + qDom.lastError().text());
        return;
    }

    QStringList doms;
    QList<int> cnts;
    int domTotal = 0;
    while (qDom.next()) {
        QString d = qDom.value(0).toString();
        doms << d;
        int count = qDom.value(1).toInt();
        cnts << count;
        domTotal += count;
    }

    QBarSet *bs = new QBarSet("Publications");
    bs->setColor(QColor("#1E40AF"));
    for (int c : cnts) {
        const double percent = (domTotal > 0) ? (c * 100.0 / domTotal) : 0.0;
        *bs << percent;
    }
    QBarSeries *bseries = new QBarSeries();
    bseries->append(bs);
    bseries->setLabelsVisible(true);
    bseries->setLabelsFormat("@value%");
    bseries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    QChart *bc = new QChart();
    bc->addSeries(bseries);
    bc->setTitle("Publications par domaine (pourcentages)");
    bc->setAnimationOptions(QChart::SeriesAnimations);
    bc->setBackgroundVisible(false);
    QBarCategoryAxis *axX = new QBarCategoryAxis();
    axX->append(doms);
    bc->addAxis(axX, Qt::AlignBottom);
    bseries->attachAxis(axX);
    QValueAxis *axY = new QValueAxis();
    axY->setLabelFormat("%d%%");
    axY->setRange(0, 100);
    bc->addAxis(axY, Qt::AlignLeft);
    bseries->attachAxis(axY);
    bc->legend()->setVisible(false);
    bc->legend()->setAlignment(Qt::AlignBottom);
    barChartView = new QChartView(bc);
    barChartView->setRenderHint(QPainter::Antialiasing);
    ui->domainChartWidget->layout()->addWidget(barChartView);
}

// ================================================================
// LOGIN
// ================================================================
void SmartMarket::on_pushButton_15_clicked()
{
    QString email    = ui->lineEdit_8->text().trimmed();
    QString password = ui->lineEdit_10->text().trimmed();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Remplissez l email et le mot de passe.");
        return;
    }
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "Attention", "Adresse email invalide.");
        return;
    }
    const bool isSelimAccount = (email.compare("selim@esprit.tn", Qt::CaseInsensitive) == 0
                                 && password == "selim");
    const bool isLegacyAccount = (email.compare("0000@a.t", Qt::CaseInsensitive) == 0
                                  && password == "0000");

    if (isSelimAccount || isLegacyAccount)
    {
        ui->stackedWidgetMain->setCurrentIndex(1);
        initPublicationTable();
        createCharts();
        // Pre-load reviewer table in background so it's ready when user navigates
        loadReviewerTable();
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Email ou mot de passe incorrect !");
        ui->lineEdit_10->clear();
        ui->lineEdit_10->setFocus();
    }
}

void SmartMarket::on_pushButton_13_clicked()
{
    QString email = ui->lineEdit_11->text().trimmed();
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Entrez votre adresse email."); return;
    }
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "Attention", "Adresse email invalide."); return;
    }
    QMessageBox::information(this, "Email envoye",
                             "Email de reinitialisation envoye a : " + email);
    ui->lineEdit_11->clear();
}

// ================================================================
// CRUD â€” AJOUTER
// ================================================================
void SmartMarket::on_pushButton_19_clicked()
{
    QString titre   = ui->lineEdit_16->text().trimmed();
    QString source  = ui->lineEdit_17->text().trimmed();
    QString contenu = ui->lineEdit_18->toPlainText().trimmed();
    QString domaine = ui->comboBox_5->currentText().trimmed();
    QDate date      = ui->dateEdit_5->date();

    if (titre.isEmpty() || source.isEmpty() || domaine.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Remplissez obligatoirement : Titre, Source, Domaine.");
        return;
    }

    QSqlQuery seqQ;
    seqQ.exec("SELECT NVL(MAX(IDPUBLICATION),0)+1 FROM ADAM.PUBLICATION");
    int newId = 1;
    if (seqQ.next()) newId = seqQ.value(0).toInt();

    Publication p(newId, titre, source, domaine, date, "Non evaluee", contenu);

    if (p.ajouter()) {
        QMessageBox::information(this, "Succes", QString("Publication ajoutee ! ID : %1").arg(newId));
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
        refreshAll();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec ajout.");
    }
}

// ================================================================
// CRUD â€” MODIFIER
// ================================================================
void SmartMarket::on_pushButton_20_clicked()
{
    QString titre   = ui->lineEdit_16->text().trimmed();
    QString source  = ui->lineEdit_17->text().trimmed();
    QString contenu = ui->lineEdit_18->toPlainText().trimmed();
    QString domaine = ui->comboBox_5->currentText().trimmed();
    QDate date      = ui->dateEdit_5->date();
    QString idStr   = ui->lineEdit_19->text().trimmed();

    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Entrez l ID de la publication a modifier."); return;
    }
    bool ok; int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Attention", "L ID doit etre un entier positif."); return;
    }

    Publication p(id, titre, source, domaine, date, "Non evaluee", contenu);

    if (p.modifier()) {
        QMessageBox::information(this, "Succes", QString("Publication ID %1 modifiee !").arg(id));
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
        refreshAll();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec modification.");
    }
}

// ================================================================
// CRUD â€” SUPPRIMER
// ================================================================
void SmartMarket::on_pushButton_8_clicked()
{
    QString idStr = ui->lineEdit_9->text().trimmed();
    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Entrez l ID a supprimer."); return;
    }
    bool ok; int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Attention", "L ID doit etre un entier positif."); return;
    }

    Publication p;
    if (p.supprimer(id)) {
        QMessageBox::information(this, "Succes", QString("Publication ID %1 supprimee !").arg(id));
        ui->lineEdit_9->clear();
        refreshAll();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec suppression.");
    }
}

// ================================================================
// RECHERCHER
// ================================================================
void SmartMarket::on_pushButton_9_clicked()
{
    QString titre   = ui->lineEdit_20->text().trimmed();
    QString source  = ui->lineEdit_21->text().trimmed();
    QString domaine = ui->comboBox_2->currentText().trimmed();
    QString statut  = ui->lineEdit_22->text().trimmed();

    Publication p;
    publicationModel = p.rechercher(titre, source, domaine, statut);
    ui->tableView->setModel(publicationModel);

    // Save filter for exports
    lastPubFilter = "";
    if (!titre.isEmpty())
        lastPubFilter += QString("UPPER(TITRE) LIKE UPPER('%%%1%%') AND ").arg(titre);
    if (!source.isEmpty())
        lastPubFilter += QString("UPPER(SOURCE) LIKE UPPER('%%%1%%') AND ").arg(source);
    if (!domaine.isEmpty() && domaine.toUpper() != "TOUS")
        lastPubFilter += QString("UPPER(DOMAINE) = UPPER('%1') AND ").arg(domaine);
    if (!statut.isEmpty())
        lastPubFilter += QString("UPPER(STATUT) LIKE UPPER('%%%1%%') AND ").arg(statut);
    if (lastPubFilter.endsWith(" AND "))
        lastPubFilter.chop(5);

    if (publicationModel->rowCount() == 0)
        QMessageBox::information(this, "Recherche", "Aucune publication trouvee.");
}

// ================================================================
// REINITIALISER
// ================================================================
void SmartMarket::on_pushButton_5_clicked()
{
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_22->clear();
    ui->lineEdit_23->clear();
    ui->comboBox_2->setCurrentIndex(0);
    
    lastPubFilter = "";
    Publication p;
    publicationModel = p.afficher();
    ui->tableView->setModel(publicationModel);
}

// ================================================================
// TRI PUBLICATIONS
// ================================================================
void SmartMarket::on_btnSortPubAsc_clicked()
{
    Publication p;
    publicationModel = p.trier("DATEPUBLICATION", "ASC");
    ui->tableView->setModel(publicationModel);
}

void SmartMarket::on_btnSortPubDesc_clicked()
{
    Publication p;
    publicationModel = p.trier("DATEPUBLICATION", "DESC");
    ui->tableView->setModel(publicationModel);
}

// ================================================================
// EXPORT PDF â€” 100% FONCTIONNEL avec QPrinter + QPainter
// ================================================================
void SmartMarket::on_pushButton_21_clicked()
{
    bool pdf    = ui->radioButton_7->isChecked();
    bool excel  = ui->radioButton_8->isChecked();

    if (!pdf && !excel) {
        QMessageBox::warning(this, "Attention", "Choisissez PDF ou Excel."); return;
    }

    if (excel) {
        bool toutesPublications = ui->radioButton_5->isChecked();
        exporterExcel(toutesPublications);
        return;
    }

    // PDF
    bool toutesPublications = ui->radioButton_5->isChecked();
    exporterPDF(toutesPublications);
}

void SmartMarket::exporterPDF(bool toutesPublications)
{
    // Demander chemin de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(
        this, "Enregistrer le PDF",
        QDir::homePath() + "/publications_" +
            QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pdf",
        "Fichiers PDF (*.pdf)"
        );
    if (filePath.isEmpty()) return;

    // PrÃ©parer les donnÃ©es
    QSqlQuery q;
    QString sql = "SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, "
                  "TO_CHAR(DATEPUBLICATION,'DD/MM/YYYY'), STATUT, CONTENU "
                  "FROM ADAM.PUBLICATION";

    // Si on exporte seulement les publications filtrÃ©es
    if (!toutesPublications) {
        if (!lastPubFilter.isEmpty())
            sql += " WHERE " + lastPubFilter;
    }
    sql += " ORDER BY DATEPUBLICATION DESC";

    if (!q.exec(sql)) {
        QMessageBox::critical(this, "Erreur BDD",
                              "Impossible de recuperer les donnees :\n" + q.lastError().text());
        return;
    }

    // Charger toutes les lignes
    struct PubRow {
        QString id, titre, source, domaine, date, statut, contenu;
    };
    QList<PubRow> rows;
    while (q.next()) {
        PubRow r;
        r.id      = q.value(0).toString();
        r.titre   = q.value(1).toString();
        r.source  = q.value(2).toString();
        r.domaine = q.value(3).toString();
        r.date    = q.value(4).toString();
        r.statut  = q.value(5).toString();
        r.contenu = q.value(6).toString();
        rows << r;
    }

    if (rows.isEmpty()) {
        QMessageBox::warning(this, "Export PDF",
                             "Aucune publication a exporter.");
        return;
    }

    // â”€â”€ CrÃ©er le PDF via QPrinter â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur PDF",
                              "Impossible d initialiser le rendu PDF.\n"
                              "Verifiez les permissions du fichier.");
        return;
    }

    // Echelle fixe pour garder des tailles de texte lisibles et stables.
    const double scale = 1.0;
    const int pageW    = painter.device()->width();
    const int pageH    = painter.device()->height();
    const int marginX  = (int)(15 * scale * 3.78);  // mm â†’ pixels approx
    const int marginY  = (int)(15 * scale * 3.78);
    const int contentW = pageW - 2 * marginX;

    // Polices
    QFont titleFont("Arial", 16);
    titleFont.setBold(true);
    QFont headerFont("Arial", 9);
    headerFont.setBold(true);
    QFont sectionFont("Arial", 11);
    sectionFont.setBold(true);
    QFont bodyFont("Arial", 8);
    QFont metaFont("Arial", 7);

    // Couleurs
    QColor colorHeader(15, 42, 68);      // #0F2A44 â€” bleu marine
    QColor colorAlt(240, 245, 255);      // bleu trÃ¨s clair
    QColor colorBorder(180, 200, 230);
    QColor colorTitle(255, 255, 255);
    QColor colorText(30, 30, 30);

    // Largeurs de colonnes (proportionnelles)
    const int COL_COUNT = 7;
    int colW[COL_COUNT];
    double proportions[] = { 0.06, 0.18, 0.13, 0.12, 0.09, 0.12, 0.30 };
    for (int i = 0; i < COL_COUNT; ++i)
        colW[i] = (int)(contentW * proportions[i]);

    const int rowH     = 28;
    const int headerH  = 36;
    const int titleH   = 55;

    // â”€â”€ Fonction lambda pour dessiner l'en-tÃªte de tableau â”€â”€â”€â”€â”€â”€
    auto drawTableHeader = [&](int y) {
        const QString headers[] = { "ID", "Titre", "Source", "Domaine",
                                   "Date", "Statut", "Contenu" };
        int x = marginX;
        for (int i = 0; i < COL_COUNT; ++i) {
            // Fond
            painter.fillRect(x, y, colW[i], headerH, colorHeader);
            // Texte
            painter.setFont(headerFont);
            painter.setPen(colorTitle);
            painter.drawText(QRect(x + 4, y,
                             colW[i] - 8, headerH),
                             Qt::AlignVCenter | Qt::AlignLeft, headers[i]);
            x += colW[i];
        }
    };

    // â”€â”€ Fonction lambda pour dessiner une ligne de donnÃ©es â”€â”€â”€â”€â”€â”€â”€
    auto drawDataRow = [&](int y, const PubRow &r, bool alternate) {
        const QString vals[] = { r.id, r.titre, r.source, r.domaine,
                                r.date, r.statut, r.contenu };
        int x = marginX;
        for (int i = 0; i < COL_COUNT; ++i) {
            // Fond
            QColor bg = alternate ? colorAlt : QColor(255, 255, 255);
            painter.fillRect(x, y, colW[i], rowH, bg);
            // Bordure
            painter.setPen(QPen(colorBorder, 1));
            painter.drawRect(x, y, colW[i], rowH);
            // Texte (tronquÃ© si nÃ©cessaire)
            painter.setFont(bodyFont);
            painter.setPen(colorText);
            QFontMetrics fm(bodyFont);
            QString txt = fm.elidedText(vals[i], Qt::ElideRight,
                                colW[i] - 10);
            painter.drawText(QRect(x + 4, y,
                             colW[i] - 8, rowH),
                             Qt::AlignVCenter | Qt::AlignLeft, txt);
            x += colW[i];
        }
    };

    // â•â•â•â•â•â•â•â• PAGE(S) â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•â•
    int currentY   = marginY;
    int pageNumber = 1;
    bool firstPage = true;

    for (int rowIdx = 0; rowIdx < rows.size(); ) {

        // â”€â”€ En-tÃªte de page â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        if (firstPage) {
            // BanniÃ¨re titre
            painter.fillRect(marginX, currentY, contentW, titleH, colorHeader);
            painter.setFont(titleFont);
            painter.setPen(colorTitle);
            painter.drawText(QRect(marginX, currentY, contentW, titleH),
                             Qt::AlignCenter,
                             "RAPPORT DES PUBLICATIONS â€” SmartMarket");
            currentY += titleH + 8;

            // MÃ©ta-info
            painter.setFont(metaFont);
            painter.setPen(Qt::darkGray);
            painter.drawText(marginX, currentY + 14,
                             QString("Genere le : %1   |   Total : %2 publication(s)   |   Filtre : %3")
                                 .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
                                 .arg(rows.size())
                                 .arg(toutesPublications ? "Toutes" : "FiltrÃ©es"));
            currentY += 30;
            firstPage = false;
        }

        // En-tÃªte du tableau
        drawTableHeader(currentY);
        currentY += headerH;

        // â”€â”€ Lignes de donnÃ©es jusqu'en bas de page â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
        bool alternate = false;
        while (rowIdx < rows.size()) {
            // Espace restant ?
            if (currentY + rowH > pageH - marginY - 20) {
                // Pied de page
                painter.setFont(metaFont);
                painter.setPen(Qt::gray);
                painter.drawText(
                  QRect(marginX, pageH - marginY - 20,
                      contentW, 20),
                    Qt::AlignCenter,
                    QString("Page %1  â€”  SmartMarket").arg(pageNumber));

                printer.newPage();
                pageNumber++;
                currentY = marginY;
                drawTableHeader(currentY);
                currentY += headerH;
                alternate = false;
            }

            drawDataRow(currentY, rows[rowIdx], alternate);
            currentY += rowH;
            alternate = !alternate;
            ++rowIdx;
        }
    }

    // Pied de derniÃ¨re page
    painter.setFont(metaFont);
    painter.setPen(Qt::gray);
    painter.drawText(
          QRect(marginX, pageH - marginY - 20,
              contentW, 20),
        Qt::AlignCenter,
        QString("Page %1  â€”  SmartMarket").arg(pageNumber));

    painter.end();

    QMessageBox::information(this, "Export PDF",
                             QString("PDF genere avec succes !\n%1\n\n"
                                     "%2 publication(s) exportee(s).")
                                 .arg(filePath).arg(rows.size()));
}

void SmartMarket::exporterExcel(bool toutesPublications)
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Enregistrer le fichier Excel",
        QDir::homePath() + "/publications_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".xls",
        "Fichiers Excel 97-2003 (*.xls)"
    );
    if (filePath.isEmpty())
        return;

    QSqlQuery q;
    QString sql = "SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, "
                  "TO_CHAR(DATEPUBLICATION,'DD/MM/YYYY'), STATUT, CONTENU "
                  "FROM ADAM.PUBLICATION";

    if (!toutesPublications) {
        if (!lastPubFilter.isEmpty())
            sql += " WHERE " + lastPubFilter;
    }
    sql += " ORDER BY DATEPUBLICATION DESC";

    if (!q.exec(sql)) {
        QMessageBox::critical(this, "Erreur BDD",
                              "Impossible de recuperer les donnees :\n" + q.lastError().text());
        return;
    }

    struct PubRow {
        QString id, titre, source, domaine, date, statut, contenu;
    };

    QList<PubRow> rows;
    while (q.next()) {
        PubRow r;
        r.id      = q.value(0).toString();
        r.titre   = q.value(1).toString();
        r.source  = q.value(2).toString();
        r.domaine = q.value(3).toString();
        r.date    = q.value(4).toString();
        r.statut  = q.value(5).toString();
        r.contenu = q.value(6).toString();
        rows << r;
    }

    if (rows.isEmpty()) {
        QMessageBox::warning(this, "Export Excel", "Aucune publication a exporter.");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::critical(this, "Export Excel", "Impossible d'ouvrir le fichier en ecriture : " + filePath);
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument("1.0");
    xml.writeProcessingInstruction("mso-application", "progid=\"Excel.Sheet\"");

    xml.writeStartElement("Workbook");
    xml.writeAttribute("xmlns", "urn:schemas-microsoft-com:office:spreadsheet");
    xml.writeAttribute("xmlns:o", "urn:schemas-microsoft-com:office:office");
    xml.writeAttribute("xmlns:x", "urn:schemas-microsoft-com:office:excel");
    xml.writeAttribute("xmlns:ss", "urn:schemas-microsoft-com:office:spreadsheet");

    xml.writeStartElement("Styles");

    xml.writeStartElement("Style");
    xml.writeAttribute("ss:ID", "Title");
    xml.writeStartElement("Font");
    xml.writeAttribute("ss:Bold", "1");
    xml.writeAttribute("ss:Size", "16");
    xml.writeAttribute("ss:Color", "#0F2A44");
    xml.writeEndElement();
    xml.writeEndElement();

    xml.writeStartElement("Style");
    xml.writeAttribute("ss:ID", "Meta");
    xml.writeStartElement("Font");
    xml.writeAttribute("ss:Size", "10");
    xml.writeAttribute("ss:Color", "#4B5563");
    xml.writeEndElement();
    xml.writeEndElement();

    xml.writeStartElement("Style");
    xml.writeAttribute("ss:ID", "Header");
    xml.writeStartElement("Font");
    xml.writeAttribute("ss:Bold", "1");
    xml.writeAttribute("ss:Color", "#FFFFFF");
    xml.writeEndElement();
    xml.writeStartElement("Interior");
    xml.writeAttribute("ss:Color", "#0F2A44");
    xml.writeAttribute("ss:Pattern", "Solid");
    xml.writeEndElement();
    xml.writeEndElement();

    xml.writeStartElement("Style");
    xml.writeAttribute("ss:ID", "Row");
    xml.writeStartElement("Font");
    xml.writeAttribute("ss:Size", "9");
    xml.writeEndElement();
    xml.writeEndElement();

    xml.writeEndElement(); // Styles

    xml.writeStartElement("Worksheet");
    xml.writeAttribute("ss:Name", "Publications");
    xml.writeStartElement("Table");

    const QString headers[] = {"ID", "Titre", "Source", "Domaine", "Date", "Statut", "Contenu"};

    xml.writeStartElement("Row");
    xml.writeStartElement("Cell");
    xml.writeAttribute("ss:StyleID", "Title");
    xml.writeAttribute("ss:MergeAcross", "6");
    xml.writeStartElement("Data");
    xml.writeAttribute("ss:Type", "String");
    xml.writeCharacters("RAPPORT DES PUBLICATIONS â€” SmartMarket");
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();

    xml.writeStartElement("Row");
    xml.writeStartElement("Cell");
    xml.writeAttribute("ss:StyleID", "Meta");
    xml.writeAttribute("ss:MergeAcross", "6");
    xml.writeStartElement("Data");
    xml.writeAttribute("ss:Type", "String");
    xml.writeCharacters(QString("Genere le : %1   |   Total : %2 publication(s)   |   Filtre : %3")
                            .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
                            .arg(rows.size())
                            .arg(toutesPublications ? "Toutes" : "FiltrÃ©es"));
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();

    xml.writeStartElement("Row");
    for (const QString &header : headers) {
        xml.writeStartElement("Cell");
        xml.writeAttribute("ss:StyleID", "Header");
        xml.writeStartElement("Data");
        xml.writeAttribute("ss:Type", "String");
        xml.writeCharacters(header);
        xml.writeEndElement();
        xml.writeEndElement();
    }
    xml.writeEndElement();

    for (const PubRow &row : rows) {
        xml.writeStartElement("Row");
        const QString values[] = {row.id, row.titre, row.source, row.domaine, row.date, row.statut, row.contenu};
        for (const QString &value : values) {
            xml.writeStartElement("Cell");
            xml.writeAttribute("ss:StyleID", "Row");
            xml.writeStartElement("Data");
            xml.writeAttribute("ss:Type", "String");
            xml.writeCharacters(value);
            xml.writeEndElement();
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }

    xml.writeEndElement(); // Table
    xml.writeEndElement(); // Worksheet
    xml.writeEndElement(); // Workbook
    xml.writeEndDocument();
    file.close();

    QMessageBox::information(this, "Export Excel",
                             QString("Fichier Excel genere avec succes !\n%1\n\n%2 publication(s) exportee(s).")
                                 .arg(filePath).arg(rows.size()));
}

// ================================================================
// SIMILARITE IA â€” Appel Anthropic
// ================================================================
void SmartMarket::on_pushButton_6_clicked()
{
    QString id1 = ui->lineEdit_5->text().trimmed();
    QString id2 = ui->lineEdit_6->text().trimmed();

    if (id1.isEmpty() || id2.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Entrez les deux IDs."); return;
    }
    if (id1 == id2) {
        QMessageBox::warning(this, "Attention", "Les IDs doivent etre differents."); return;
    }

    // RÃ©cupÃ©rer les deux publications
    QSqlQuery q;
    q.prepare("SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, CONTENU, STATUT "
              "FROM ADAM.PUBLICATION WHERE IDPUBLICATION IN (:a, :b)");
    q.bindValue(":a", id1.toInt());
    q.bindValue(":b", id2.toInt());
    q.exec();

    QString titreA, contenuA, sourceA, domaineA, statutA;
    QString titreB, contenuB, sourceB, domaineB, statutB;
    int foundCount = 0;
    while (q.next()) {
        int curId = q.value(0).toInt();
        if (curId == id1.toInt()) {
            titreA   = q.value(1).toString();
            sourceA  = q.value(2).toString();
            domaineA = q.value(3).toString();
            contenuA = q.value(4).toString();
            statutA  = q.value(5).toString();
            foundCount++;
        } else if (curId == id2.toInt()) {
            titreB   = q.value(1).toString();
            sourceB  = q.value(2).toString();
            domaineB = q.value(3).toString();
            contenuB = q.value(4).toString();
            statutB  = q.value(5).toString();
            foundCount++;
        }
    }

    if (foundCount < 2) {
        QMessageBox::warning(this, "Erreur",
                             "Un ou les deux IDs introuvables dans la base."); return;
    }

    // Sauvegarder pour le callback
    simId1     = id1; simId2     = id2;
    simTitre1  = titreA; simTitre2  = titreB;
    simContenu1 = contenuA; simContenu2 = contenuB;

    // Vider le tableau et afficher un message d'attente
    ui->tableWidget_4->clearContents();
    ui->tableWidget_4->setRowCount(1);
    ui->tableWidget_4->setColumnCount(4);
    ui->tableWidget_4->setHorizontalHeaderLabels(
        {"Publication 1", "Publication 2", "Taux de similarite", "Statut"});
    ui->tableWidget_4->setItem(0, 0, new QTableWidgetItem(titreA));
    ui->tableWidget_4->setItem(0, 1, new QTableWidgetItem(titreB));
    ui->tableWidget_4->setItem(0, 2, new QTableWidgetItem("Analyse en cours..."));
    ui->tableWidget_4->setItem(0, 3, new QTableWidgetItem("â³ Traitement IA"));
    ui->tableWidget_4->resizeColumnsToContents();

    ui->progressBar->setValue(10);

    // Appel IA
    callAnthropicSimilarite(titreA, contenuA, titreB, contenuB);
}

void SmartMarket::callAnthropicSimilarite(const QString &titre1, const QString &contenu1,
                                          const QString &titre2, const QString &contenu2)
{
    QString prompt = QString(
                         "Tu es un expert en analyse de publications scientifiques.\n"
                         "Analyse la similarite entre ces deux publications et reponds UNIQUEMENT en JSON valide.\n\n"
                         "Publication 1:\n"
                         "- Titre: %1\n"
                         "- Contenu: %2\n\n"
                         "Publication 2:\n"
                         "- Titre: %3\n"
                         "- Contenu: %4\n\n"
                         "Reponds en JSON avec exactement ces champs:\n"
                         "{\n"
                         "  \"taux\": <entier entre 0 et 100>,\n"
                         "  \"statut\": \"<Tres similaires | Moderement similaires | Peu similaires | Non similaires>\",\n"
                         "  \"justification\": \"<explication concise en 1-2 phrases>\"\n"
                         "}"
                         ).arg(titre1, contenu1.left(500), titre2, contenu2.left(500));

    // Appel Ã  Ollama (100% gratuit, local)
    QJsonObject body;
    body["model"]  = OLLAMA_MODEL;
    body["prompt"] = prompt;
    body["stream"] = false;

    QNetworkRequest req{QUrl(OLLAMA_ENDPOINT)};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSimilariteReply(reply);
    });
}

void SmartMarket::onSimilariteReply(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        ui->tableWidget_4->setItem(0, 2, new QTableWidgetItem("Erreur reseau"));
        ui->tableWidget_4->setItem(0, 3,
                                   new QTableWidgetItem("Echec : " + reply->errorString()));
        ui->progressBar->setValue(0);
        
        QString details = reply->errorString();
        QByteArray serverResponse = reply->readAll();
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (httpStatus > 0) {
            details += QString("\n\nCode HTTP: %1").arg(httpStatus);
        }
        if (!serverResponse.isEmpty()) {
            details += QString("\n\nRÃ©ponse serveur:\n%1").arg(QString::fromUtf8(serverResponse));
        }
        
        // Indice si Ollama n'est pas lancÃ©
        if (details.contains("Connection refused") || details.contains("localhost")) {
            details += "\n\nðŸ’¡ SOLUTION: Lancez Ollama d'abord!\n"
                      "1. Installez Ollama: https://ollama.ai\n"
                      "2. Dans le Terminal: ollama run mistral\n"
                      "3. Relancez SmartMarket";
        }
        
        QMessageBox::critical(this, "Erreur IA",
                              "Erreur reseau :\n" + details);
        return;
    }

    QByteArray raw = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (doc.isNull()) {
        ui->tableWidget_4->setItem(0, 2, new QTableWidgetItem("Erreur JSON"));
        ui->progressBar->setValue(0);
        return;
    }

    // Extraire le texte de la rÃ©ponse Ollama
    QJsonObject root = doc.object();
    QString text = root["response"].toString();  // Ollama retourne "response"

    // Parser le JSON imbriquÃ©
    // Nettoyer les Ã©ventuels blocs ```json
    text = text.trimmed();
    if (text.startsWith("```")) {
        text = text.mid(text.indexOf('\n') + 1);
        if (text.endsWith("```")) text.chop(3);
    }

    QJsonDocument inner = QJsonDocument::fromJson(text.toUtf8());
    int taux = 0;
    QString statut = "Inconnu";
    QString justification;

    if (!inner.isNull() && inner.isObject()) {
        QJsonObject o = inner.object();
        taux          = o["taux"].toInt();
        statut        = o["statut"].toString();
        justification = o["justification"].toString();
    } else {
        // Fallback : chercher un nombre dans le texte
        QRegularExpression rx("(\\d+)");
        QRegularExpressionMatch match = rx.match(text);
        if (match.hasMatch())
            taux = match.captured(1).toInt();
        statut = "Voir justification";
        justification = text;
    }

    taux = qBound(0, taux, 100);
    ui->progressBar->setValue(taux);

    ui->tableWidget_4->setItem(0, 2,
                               new QTableWidgetItem(QString("%1%").arg(taux)));
    ui->tableWidget_4->setItem(0, 3,
                               new QTableWidgetItem(statut));

    // Ajouter une ligne de justification
    ui->tableWidget_4->setRowCount(2);
    ui->tableWidget_4->setSpan(1, 0, 1, 4);
    ui->tableWidget_4->setItem(1, 0,
                               new QTableWidgetItem("Analyse IA : " + justification));

    ui->tableWidget_4->resizeColumnsToContents();

    QMessageBox::information(this, "Resultat Similarite",
                             QString("Publications %1 & %2\n\n"
                                     "Taux de similarite : %3%\n"
                                     "Statut : %4\n\n"
                                     "%5")
                                 .arg(simId1, simId2)
                                 .arg(taux)
                                 .arg(statut)
                                 .arg(justification));
}

// ================================================================
// COMPLETUDE IA â€” Appel Anthropic
// ================================================================
void SmartMarket::on_pushButton_7_clicked()
{
    QString idStr = ui->lineEdit_7->text().trimmed();
    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Entrez l ID."); return;
    }

    QSqlQuery q;
    q.prepare("SELECT TITRE, SOURCE, DOMAINE, "
              "TO_CHAR(DATEPUBLICATION,'DD/MM/YYYY'), STATUT, CONTENU "
              "FROM ADAM.PUBLICATION WHERE IDPUBLICATION=:id");
    q.bindValue(":id", idStr.toInt());
    q.exec();

    if (!q.next()) {
        QMessageBox::warning(this, "Erreur", "Publication introuvable."); return;
    }

    QString titre   = q.value(0).toString();
    QString source  = q.value(1).toString();
    QString domaine = q.value(2).toString();
    QString date    = q.value(3).toString();
    QString statut  = q.value(4).toString();
    QString contenu = q.value(5).toString();

    // ComplÃ©tude basique (champs remplis)
    int total = 6, remplis = 0;
    QStringList vals = {titre, source, domaine, date, statut, contenu};
    for (const QString &v : vals) if (!v.isEmpty()) remplis++;
    int pct = (remplis * 100) / total;
    ui->progressBar_2->setValue(pct);

    // PrÃ©parer tableau 5 â€” aperÃ§u des champs
    compIdStr = idStr; compTitre = titre;

    ui->tableWidget_5->clearContents();
    ui->tableWidget_5->setColumnCount(7);
    ui->tableWidget_5->setHorizontalHeaderLabels(
        {"Titre", "Domaine", "Source", "Date",
         "Contenu", "Statut", "Score IA"});
    ui->tableWidget_5->setRowCount(2);

    // Ligne 1 : valeurs
    ui->tableWidget_5->setItem(0, 0, new QTableWidgetItem(titre.isEmpty()   ? "âŒ Manquant" : titre));
    ui->tableWidget_5->setItem(0, 1, new QTableWidgetItem(domaine.isEmpty() ? "âŒ Manquant" : domaine));
    ui->tableWidget_5->setItem(0, 2, new QTableWidgetItem(source.isEmpty()  ? "âŒ Manquant" : source));
    ui->tableWidget_5->setItem(0, 3, new QTableWidgetItem(date.isEmpty()    ? "âŒ Manquant" : date));
    ui->tableWidget_5->setItem(0, 4, new QTableWidgetItem(contenu.isEmpty() ? "âŒ Manquant" :
                                                              contenu.left(40) + (contenu.length()>40?"â€¦":"")));
    ui->tableWidget_5->setItem(0, 5, new QTableWidgetItem(statut.isEmpty()  ? "âŒ Manquant" : statut));
    ui->tableWidget_5->setItem(0, 6, new QTableWidgetItem("Analyse IA en cours..."));

    // Ligne 2 : analyse IA globale (placeholder)
    ui->tableWidget_5->setSpan(1, 0, 1, 7);
    ui->tableWidget_5->setItem(1, 0, new QTableWidgetItem("â³ L'IA analyse la qualite de la publication..."));
    ui->tableWidget_5->resizeColumnsToContents();

    // Appel IA
    callAnthropicCompletude(idStr, titre, source, domaine, date, statut, contenu);
}

void SmartMarket::callAnthropicCompletude(const QString &idStr,
                                          const QString &titre,
                                          const QString &source,
                                          const QString &domaine,
                                          const QString &date,
                                          const QString &statut,
                                          const QString &contenu)
{
    QString prompt = QString(
                         "Tu es un expert en qualite de publications scientifiques.\n"
                         "Evalue la completude et la qualite de cette publication. "
                         "Reponds UNIQUEMENT en JSON valide.\n\n"
                         "Donnees de la publication (ID: %1):\n"
                         "- Titre: \"%2\"\n"
                         "- Source: \"%3\"\n"
                         "- Domaine: \"%4\"\n"
                         "- Date: \"%5\"\n"
                         "- Statut: \"%6\"\n"
                         "- Contenu: \"%7\"\n\n"
                         "Reponds avec exactement ce JSON:\n"
                         "{\n"
                         "  \"score\": <entier entre 0 et 100>,\n"
                         "  \"champs_manquants\": [\"liste\", \"des\", \"champs\", \"vides\"],\n"
                         "  \"qualite_contenu\": \"<Excellent | Bon | Moyen | Insuffisant | Absent>\",\n"
                         "  \"recommandations\": \"<conseils concis pour ameliorer la publication>\"\n"
                         "}"
                         ).arg(idStr, titre, source, domaine, date, statut, contenu.left(600));

    // Appel Ã  Ollama (100% gratuit, local)
    QJsonObject body;
    body["model"]  = OLLAMA_MODEL;
    body["prompt"] = prompt;
    body["stream"] = false;

    QNetworkRequest req{QUrl(OLLAMA_ENDPOINT)};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = networkManager->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onCompletudReply(reply);
    });
}

void SmartMarket::onCompletudReply(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        ui->tableWidget_5->setItem(0, 6, new QTableWidgetItem("Erreur"));
        ui->tableWidget_5->setItem(1, 0,
                                   new QTableWidgetItem("Erreur reseau : " + reply->errorString()));
        
        QString details = reply->errorString();
        QByteArray serverResponse = reply->readAll();
        int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        
        if (httpStatus > 0) {
            details += QString("\n\nCode HTTP: %1").arg(httpStatus);
        }
        if (!serverResponse.isEmpty()) {
            details += QString("\n\nRÃ©ponse serveur:\n%1").arg(QString::fromUtf8(serverResponse));
        }
        
        // Indice si Ollama n'est pas lancÃ©
        if (details.contains("Connection refused") || details.contains("localhost")) {
            details += "\n\nðŸ’¡ SOLUTION: Lancez Ollama d'abord!\n"
                      "1. Installez Ollama: https://ollama.ai\n"
                      "2. Dans le Terminal: ollama run mistral\n"
                      "3. Relancez SmartMarket";
        }
        
        QMessageBox::critical(this, "Erreur IA",
                              "Erreur reseau :\n" + details);
        return;
    }

    QByteArray raw = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (doc.isNull()) {
        ui->tableWidget_5->setItem(0, 6, new QTableWidgetItem("Erreur JSON"));
        return;
    }

    // Extraire le texte de la rÃ©ponse Ollama
    QJsonObject root = doc.object();
    QString text = root["response"].toString();  // Ollama retourne "response"

    // Nettoyer et parser
    text = text.trimmed();
    if (text.startsWith("```")) {
        text = text.mid(text.indexOf('\n') + 1);
        if (text.endsWith("```")) text.chop(3);
    }

    QJsonDocument inner = QJsonDocument::fromJson(text.toUtf8());
    int score = 0;
    QString qualite, recommandations;
    QStringList manquants;

    if (!inner.isNull() && inner.isObject()) {
        QJsonObject o  = inner.object();
        score          = o["score"].toInt();
        qualite        = o["qualite_contenu"].toString();
        recommandations = o["recommandations"].toString();
        QJsonArray arr = o["champs_manquants"].toArray();
        for (const QJsonValue &v : arr) manquants << v.toString();
    } else {
        score = 50;
        recommandations = text;
        qualite = "Voir recommandations";
    }

    score = qBound(0, score, 100);
    ui->progressBar_2->setValue(score);

    // Mettre Ã  jour tableau
    ui->tableWidget_5->setItem(0, 6,
                               new QTableWidgetItem(QString("%1/100 â€” %2").arg(score).arg(qualite)));

    QString missing = manquants.isEmpty() ? "Aucun" : manquants.join(", ");
    QString analyse = "Champs manquants : " + missing +
                      "\nRecommandations : " + recommandations;
    ui->tableWidget_5->setItem(1, 0, new QTableWidgetItem("Analyse IA : " + analyse));
    ui->tableWidget_5->resizeColumnsToContents();
    ui->tableWidget_5->resizeRowToContents(1);

    QMessageBox msg(QMessageBox::Information,
                    "Compl\xc3\xa9tude â€” R\xc3\xa9sultat IA",
                    QString("<b>Publication ID %1 :</b> %2").arg(compIdStr, compTitre),
                    QMessageBox::Ok,
                    this);
    msg.setTextFormat(Qt::RichText);
    msg.setInformativeText(
        QString("<b>Score de compl\xc3\xa9tude :</b> %1/100<br>"
                "<b>Qualit\xc3\xa9 du contenu :</b> %2<br>"
                "<b>Champs manquants :</b> %3<br><br>"
                "<b>Recommandations :</b><br>%4")
            .arg(score)
            .arg(qualite)
            .arg(missing)
            .arg(recommandations.toHtmlEscaped().replace("\n", "<br>")));
    msg.setDetailedText(text);
    msg.exec();
}

// ================================================================
// SIDEBAR
// ================================================================
void SmartMarket::on_pushButton_13_sidebar_clicked()
{ ui->stackedWidgetMain->setCurrentIndex(1); }

void SmartMarket::on_pushButton_14_clicked()
{ QMessageBox::information(this, "Info", "Deja sur Publications."); }

void SmartMarket::on_pushButton_15_sidebar_clicked()
{ openConferenceModule(); }

void SmartMarket::on_pushButton_16_clicked()
{
    ui->stackedWidgetMain->setCurrentIndex(3); // Open page_2 (Reviewers page)
    loadReviewerTable(); // Always load fresh data when opening reviewers page
}

void SmartMarket::openConferenceModule()
{
    ui->stackedWidgetMain->setCurrentIndex(2); // page2 is index 2
    loadConferenceTable();
}

// Conference CRUD Operations
void SmartMarket::on_conf_pushButton_26_clicked()
{
    const QString idText = ui->conf_lineEdit_26->text().trimmed();
    const QString nom = ui->conf_lineEdit_27->text().trimmed();
    const QString lieu = ui->conf_lineEdit_24->text().trimmed();
    const QDate date = ui->conf_dateEdit_6->date();
    const QString theme = ui->conf_lineEdit_25->text().trimmed();

    bool idOk = false;
    const int id = idText.toInt(&idOk);

    const int maxId = 9999999; // NUMBER(7)

    if (!idOk || nom.isEmpty() || lieu.isEmpty() || !date.isValid())
    {
        QMessageBox::warning(this, "Saisie incomplete", "Veuillez renseigner ID (numerique), Nom, Lieu et Date.");
        return;
    }

    if (id < 0 || id > maxId)
    {
        QMessageBox::warning(this, "ID invalide", "ID doit etre un entier entre 0 et " + QString::number(maxId) + ".");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de donnees", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO ADAM.conference (idconference, nom, lieu, datedebut, theme, nombreparticipants) "
              "VALUES (:id, :nom, :lieu, :date, :theme, 0)");
    q.bindValue(":id", id);
    q.bindValue(":nom", nom);
    q.bindValue(":lieu", lieu);
    q.bindValue(":date", date);
    q.bindValue(":theme", theme);

    if (!q.exec())
    {
        QMessageBox::critical(this, "Insertion conference",
                              "Echec INSERT : " + q.lastError().text() +
                              "\nValeurs : ID=" + QString::number(id) +
                              ", Nom=" + nom +
                              ", Lieu=" + lieu +
                              ", Date=" + date.toString("yyyy-MM-dd") +
                              ", Theme=" + theme);
        return;
    }

    QMessageBox::information(this, "Succes", "Conference ajoutee dans la base.");

    loadConferenceTable();

    ui->conf_lineEdit_26->clear();
    ui->conf_lineEdit_27->clear();
    ui->conf_lineEdit_24->clear();
    ui->conf_dateEdit_6->setDate(QDate::currentDate());
    ui->conf_lineEdit_25->clear();
}

void SmartMarket::addConference()
{
    on_conf_pushButton_26_clicked();
}

void SmartMarket::on_conf_pushButton_27_clicked()
{
    const QString idText = ui->conf_lineEdit_31->text().trimmed();
    const QString nom = ui->conf_lineEdit_28->text().trimmed();
    const QString lieu = ui->conf_lineEdit_29->text().trimmed();
    const QDate date = ui->conf_dateEdit_4->date();
    const QString theme = ui->conf_lineEdit_30->text().trimmed();

    bool idOk = false;
    const int id = idText.toInt(&idOk);
    const int maxId = 9999999; // NUMBER(7)

    if (!idOk || nom.isEmpty() || lieu.isEmpty() || !date.isValid())
    {
        QMessageBox::warning(this, "Saisie incomplete", "Renseignez ID (numerique), Nom, Lieu et Date.");
        return;
    }

    if (id < 0 || id > maxId)
    {
        QMessageBox::warning(this, "ID invalide", "ID doit etre entre 0 et " + QString::number(maxId) + ".");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de donnees", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("UPDATE ADAM.conference "
              "SET nom = :nom, lieu = :lieu, datedebut = :date, theme = :theme "
              "WHERE idconference = :id");
    q.bindValue(":nom", nom);
    q.bindValue(":lieu", lieu);
    q.bindValue(":date", date);
    q.bindValue(":theme", theme);
    q.bindValue(":id", id);

    if (!q.exec())
    {
        QMessageBox::critical(this, "Mise a jour conference",
                              "Echec UPDATE : " + q.lastError().text() +
                              "\nValeurs : ID=" + QString::number(id) +
                              ", Nom=" + nom +
                              ", Lieu=" + lieu +
                              ", Date=" + date.toString("yyyy-MM-dd") +
                              ", Theme=" + theme);
        return;
    }

    if (q.numRowsAffected() == 0)
    {
        QMessageBox::information(this, "Mise a jour", "Aucune conference trouvee avec cet ID.");
    }
    else
    {
        QMessageBox::information(this, "Mise a jour", "Conference mise a jour.");
    }

    loadConferenceTable();

    ui->conf_lineEdit_31->clear();
    ui->conf_lineEdit_28->clear();
    ui->conf_lineEdit_29->clear();
    ui->conf_dateEdit_4->setDate(QDate::currentDate());
    ui->conf_lineEdit_30->clear();
}

void SmartMarket::modifyConference()
{
    on_conf_pushButton_27_clicked();
}

void SmartMarket::deleteConference()
{
    int id = ui->conf_lineEdit_8->text().toInt();
    Conference c;
    if (c.supprimer(id)) {
        QMessageBox::information(this, "SuccÃ¨s", "ConfÃ©rence supprimÃ©e.");
        loadConferenceTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Echec suppression confÃ©rence.");
    }
}

void SmartMarket::on_conf_pushButton_8_clicked()
{
    const QString idText = ui->conf_lineEdit_8->text().trimmed();
    bool ok = false;
    const int id = idText.toInt(&ok);

    if (!ok)
    {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID numerique valide.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de donnees", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("DELETE FROM ADAM.conference WHERE idconference = :id");
    q.bindValue(":id", id);

    if (!q.exec())
    {
        QMessageBox::critical(this, "Suppression conference", "Echec DELETE : " + q.lastError().text());
        return;
    }

    // Note: numRowsAffected can return -1 depending on the driver.
    if (q.numRowsAffected() == 0)
    {
        QMessageBox::information(this, "Suppression", "Aucune conference trouvee avec cet ID.");
    }
    else
    {
        QMessageBox::information(this, "Suppression", "Conference supprimee.");
    }

    loadConferenceTable();
    ui->conf_lineEdit_8->clear();
}

void SmartMarket::on_conf_pushButton_19_clicked()
{
    const int id = ui->conf_lineEdit_21 ? ui->conf_lineEdit_21->text().toInt() : 0;
    const QString nom = ui->conf_lineEdit_20 ? ui->conf_lineEdit_20->text().trimmed() : QString();
    const int confId = ui->conf_lineEdit_22 ? ui->conf_lineEdit_22->text().toInt() : 0;

    Participant p(id, nom, confId, "");
    
    QInputDialog uidDialog(this);
    if (uidDialog.exec() == QDialog::Accepted) {
        p.setUidRfid(uidDialog.textValue().trimmed().toUpper());
        if (p.ajouter()) {
            QMessageBox::information(this, "SuccÃ¨s", "Participant ajoutÃ©.");
            loadParticipantTable();
            loadConferenceTable();
        } else {
             QMessageBox::critical(this, "Erreur", "Echec ajout participant.");
        }
    }
}

void SmartMarket::addParticipant()
{
    on_conf_pushButton_19_clicked();
}

void SmartMarket::on_conf_pushButton_10_clicked()
{
    const int id = ui->conf_lineEdit_23 ? ui->conf_lineEdit_23->text().toInt() : 0;
    const QString nom = ui->conf_lineEdit_15 ? ui->conf_lineEdit_15->text().trimmed() : QString();
    const int confId = ui->conf_lineEdit_14 ? ui->conf_lineEdit_14->text().toInt() : 0;

    Participant p(id, nom, confId, "");
    
    QInputDialog uidDialog(this);
    if (uidDialog.exec() == QDialog::Accepted) {
        p.setUidRfid(uidDialog.textValue().trimmed().toUpper());
        if (p.modifier()) {
            QMessageBox::information(this, "SuccÃ¨s", "Participant modifiÃ©.");
            loadParticipantTable();
            loadConferenceTable();
        } else {
             QMessageBox::critical(this, "Erreur", "Echec modification participant.");
        }
    }
}

void SmartMarket::modifyParticipant()
{
    on_conf_pushButton_10_clicked();
}

void SmartMarket::on_conf_pushButton_12_clicked()
{
    int id = ui->conf_lineEdit_13 ? ui->conf_lineEdit_13->text().toInt() : 0;
    Participant p;
    if (p.supprimer(id)) {
        QMessageBox::information(this, "Suppression", "Participant supprimÃ©.");
        loadParticipantTable();
        loadConferenceTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Echec suppression participant.");
    }
}

void SmartMarket::deleteParticipant()
{
    on_conf_pushButton_12_clicked();
}

void SmartMarket::sortConferencesByDateAsc()
{
    QTableWidget *table = ui->conf_tableWidget_7;
    table->sortItems(3, Qt::AscendingOrder); // Column 3 is date
}

void SmartMarket::sortConferencesByDateDesc()
{
    QTableWidget *table = ui->conf_tableWidget_7;
    table->sortItems(3, Qt::DescendingOrder); // Column 3 is date
}

void SmartMarket::on_conf_pushButton_5_clicked()
{
    if (!ui->conf_tableWidget_7 || !ui->conf_tableWidget_2 || !ui->conf_graphicsView1 || !ui->conf_graphicsView1_2)
    {
        QMessageBox::warning(this, "Export PDF", "Composants manquants pour l'export (tables ou graphiques).");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Enregistrer le PDF",
        QDir::homePath() + "/conferences_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pdf",
        "Fichiers PDF (*.pdf)"
    );
    if (filePath.isEmpty())
        return;

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer))
    {
        QMessageBox::critical(this, "Export PDF", "Impossible d'ouvrir le PDF en Ã©criture : " + filePath);
        return;
    }

    const int pageW = painter.device()->width();
    const int pageH = painter.device()->height();
    const int marginX = 15 * 3.78;
    const int marginY = 15 * 3.78;
    const int contentW = pageW - 2 * marginX;

    QFont titleFont("Arial", 16);
    titleFont.setBold(true);
    QFont headerFont("Arial", 9);
    headerFont.setBold(true);
    QFont sectionFont("Arial", 11);
    sectionFont.setBold(true);
    QFont bodyFont("Arial", 8);
    QFont metaFont("Arial", 7);

    QColor colorHeader(15, 42, 68);
    QColor colorAlt(240, 245, 255);
    QColor colorBorder(180, 200, 230);
    QColor colorTitle(255, 255, 255);
    QColor colorText(30, 30, 30);

    const int COL_COUNT_CONF = 6;
    int confColW[COL_COUNT_CONF];
    double confProps[] = {0.08, 0.26, 0.18, 0.16, 0.18, 0.14};
    for (int i = 0; i < COL_COUNT_CONF; ++i)
        confColW[i] = static_cast<int>(contentW * confProps[i]);

    const int COL_COUNT_PART = 4;
    int partColW[COL_COUNT_PART];
    double partProps[] = {0.10, 0.38, 0.22, 0.30};
    for (int i = 0; i < COL_COUNT_PART; ++i)
        partColW[i] = static_cast<int>(contentW * partProps[i]);

    const int rowH = 24;
    const int headerH = 30;
    const int titleH = 48;
    const int chartH = 220;
    const int chartGap = 18;

    auto drawHeader = [&](const QString &title, const QString &subtitle, int &currentY) {
        painter.fillRect(marginX, currentY, contentW, titleH, colorHeader);
        painter.setFont(titleFont);
        painter.setPen(colorTitle);
        painter.drawText(QRect(marginX + 12, currentY, contentW - 24, titleH - 18), Qt::AlignLeft | Qt::AlignVCenter, title);
        painter.setFont(metaFont);
        painter.drawText(QRect(marginX + 12, currentY + 20, contentW - 24, 18), Qt::AlignLeft | Qt::AlignVCenter, subtitle);
        currentY += titleH + 8;
    };

    auto drawTableHeader = [&](int y, const QStringList &headers, const int *widths, int cols) {
        int x = marginX;
        painter.setFont(headerFont);
        for (int i = 0; i < cols; ++i) {
            painter.fillRect(x, y, widths[i], headerH, colorHeader);
            painter.setPen(colorTitle);
            painter.drawText(QRect(x + 4, y, widths[i] - 8, headerH), Qt::AlignVCenter | Qt::AlignLeft, headers[i]);
            x += widths[i];
        }
    };

    auto drawTableRow = [&](int y, QTableWidget *table, int row, const int *widths, int cols) {
        int x = marginX;
        const QColor rowBg = (row % 2 == 0) ? Qt::white : colorAlt;
        painter.setFont(bodyFont);
        for (int c = 0; c < cols; ++c) {
            const QString text = table->item(row, c) ? table->item(row, c)->text() : QString();
            QFontMetrics fm(bodyFont);
            const QString clipped = fm.elidedText(text, Qt::ElideRight, widths[c] - 10);

            painter.fillRect(x, y, widths[c], rowH, rowBg);
            painter.setPen(QPen(colorBorder, 1));
            painter.drawRect(x, y, widths[c], rowH);
            painter.setPen(colorText);
            painter.drawText(QRect(x + 4, y, widths[c] - 8, rowH), Qt::AlignVCenter | Qt::AlignLeft, clipped);
            x += widths[c];
        }
    };

    auto renderChart = [&](QGraphicsView *view, const QString &title, int targetWidth, int targetHeight) -> QImage {
        Q_UNUSED(title);
        QRectF sceneRect = view->scene()->itemsBoundingRect();
        if (sceneRect.isEmpty() || !sceneRect.isValid())
            sceneRect = view->scene()->sceneRect();
        if (sceneRect.isEmpty() || !sceneRect.isValid())
            sceneRect = QRectF(QPointF(0, 0), view->viewport()->size());
        sceneRect = sceneRect.adjusted(-8, -8, 8, 8);

        QImage img(QSize(qMax(1, targetWidth * 2), qMax(1, targetHeight * 2)), QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::white);
        QPainter chartPainter(&img);
        chartPainter.setRenderHint(QPainter::Antialiasing);
        chartPainter.setRenderHint(QPainter::TextAntialiasing);
        view->scene()->render(&chartPainter, QRectF(QPointF(0, 0), QSizeF(img.size())), sceneRect);
        chartPainter.end();
        return img.scaled(targetWidth, targetHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    };

    auto newPageIfNeeded = [&](int &currentY, int blockHeight) {
        if (currentY + blockHeight > pageH - marginY) {
            printer.newPage();
            currentY = marginY;
        }
    };

    int currentY = marginY;
    drawHeader("RAPPORT DES CONFERENCES", QString("Genere le : %1   |   Export PDF propre").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")), currentY);

    painter.setPen(Qt::black);
    painter.setFont(headerFont);

    newPageIfNeeded(currentY, titleH + headerH + (ui->conf_tableWidget_7->rowCount() * rowH) + 2 * chartH + 120);

    painter.setFont(headerFont);
    painter.drawText(QRect(marginX, currentY, contentW, 18), Qt::AlignLeft | Qt::AlignVCenter, "ConfÃ©rences");
    currentY += 22;
    drawTableHeader(currentY, {"ID", "Nom", "Lieu", "Date", "ThÃ¨me", "Participants"}, confColW, COL_COUNT_CONF);
    currentY += headerH;
    for (int r = 0; r < ui->conf_tableWidget_7->rowCount(); ++r) {
        if (currentY + rowH > pageH - marginY) {
            printer.newPage();
            currentY = marginY;
            painter.drawText(QRect(marginX, currentY, contentW, 18), Qt::AlignLeft | Qt::AlignVCenter, "ConfÃ©rences (suite)");
            currentY += 22;
            drawTableHeader(currentY, {"ID", "Nom", "Lieu", "Date", "ThÃ¨me", "Participants"}, confColW, COL_COUNT_CONF);
            currentY += headerH;
        }
        drawTableRow(currentY, ui->conf_tableWidget_7, r, confColW, COL_COUNT_CONF);
        currentY += rowH;
    }

    currentY += 18;
    if (currentY + headerH + (ui->conf_tableWidget_2->rowCount() * rowH) + chartH + 60 > pageH - marginY) {
        printer.newPage();
        currentY = marginY;
    }

    painter.drawText(QRect(marginX, currentY, contentW, 18), Qt::AlignLeft | Qt::AlignVCenter, "Participants");
    currentY += 22;
    drawTableHeader(currentY, {"ID", "Nom", "Id ConfÃ©rence", "UID RFID"}, partColW, COL_COUNT_PART);
    currentY += headerH;
    for (int r = 0; r < ui->conf_tableWidget_2->rowCount(); ++r) {
        if (currentY + rowH > pageH - marginY) {
            printer.newPage();
            currentY = marginY;
            painter.drawText(QRect(marginX, currentY, contentW, 18), Qt::AlignLeft | Qt::AlignVCenter, "Participants (suite)");
            currentY += 22;
            drawTableHeader(currentY, {"ID", "Nom", "Id ConfÃ©rence", "UID RFID"}, partColW, COL_COUNT_PART);
            currentY += headerH;
        }
        drawTableRow(currentY, ui->conf_tableWidget_2, r, partColW, COL_COUNT_PART);
        currentY += rowH;
    }

    currentY += 20;
    if (currentY + chartH + chartGap > pageH - marginY) {
        printer.newPage();
        currentY = marginY;
    }

    painter.setFont(sectionFont);
    painter.setPen(Qt::black);
    painter.drawText(QRect(marginX, currentY, contentW, 18), Qt::AlignLeft | Qt::AlignVCenter, "Participants par confÃ©rence");
    currentY += 24;
    QImage chart1 = renderChart(ui->conf_graphicsView1, "Participants par confÃ©rence", contentW, chartH);
    painter.drawImage(QRect(marginX, currentY, contentW, chartH), chart1);
    currentY += chartH + chartGap;

    if (currentY + 24 + chartH > pageH - marginY) {
        printer.newPage();
        currentY = marginY;
    }

    painter.drawText(QRect(marginX, currentY, contentW, 18), Qt::AlignLeft | Qt::AlignVCenter, "ConfÃ©rences par date");
    currentY += 24;
    QImage chart2 = renderChart(ui->conf_graphicsView1_2, "ConfÃ©rences par date", contentW, chartH);
    painter.drawImage(QRect(marginX, currentY, contentW, chartH), chart2);

    painter.end();

    QMessageBox::information(this, "Export PDF", "PDF gÃ©nÃ©rÃ© : " + filePath);
}

void SmartMarket::exportConferencesToPDF()
{
    on_conf_pushButton_5_clicked();
}

int SmartMarket::applyConferenceFilterToTable()
{
    if (!ui->conf_tableWidget_7) return 0;

    QString filter = ui->conf_lineEdit_2 ? ui->conf_lineEdit_2->text().trimmed() : QString();
    Conference c;
    QSqlQueryModel *model = c.rechercher(filter);

    ui->conf_tableWidget_7->setRowCount(0);
    for (int i = 0; i < model->rowCount(); ++i) {
        ui->conf_tableWidget_7->insertRow(i);
        for (int j = 0; j < model->columnCount(); ++j) {
            ui->conf_tableWidget_7->setItem(i, j, new QTableWidgetItem(model->index(i, j).data().toString()));
        }
    }

    if (model->rowCount() == 0)
        QMessageBox::information(this, "Filtrage", "Aucune confÃ©rence pour ce filtre.");

    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
    updateConferenceCalendar();

    return model->rowCount();
}

void SmartMarket::on_conf_pushButton_21_clicked()
{
    if (!ui->conf_tableWidget_7)
        return;

    const QString filterText = ui->conf_lineEdit_2 ? ui->conf_lineEdit_2->text().trimmed() : QString();
    QString dateFilter;
    if (ui->conf_radioButton_19 && ui->conf_radioButton_19->isChecked())
    {
        if (ui->dateEdit)
            dateFilter = ui->dateEdit->date().toString("yyyy-MM-dd");
        if (dateFilter.isEmpty())
        {
            QMessageBox::warning(this, "Filtrage", "Veuillez choisir une date pour filtrer.");
            return;
        }
    }

    // Determiner la colonne a filtrer selon le radio selectionne
    QString whereClause;
    if (ui->conf_radioButton_17 && ui->conf_radioButton_17->isChecked())
        whereClause = "LOWER(c.nom) LIKE LOWER(:f)";
    else if (ui->conf_radioButton_18 && ui->conf_radioButton_18->isChecked())
        whereClause = "LOWER(c.lieu) LIKE LOWER(:f)";
    else if (ui->conf_radioButton_19 && ui->conf_radioButton_19->isChecked())
        whereClause = "TO_CHAR(c.datedebut, 'YYYY-MM-DD') = :d";
    else if (ui->conf_radioButton_20 && ui->conf_radioButton_20->isChecked())
        whereClause = "LOWER(c.theme) LIKE LOWER(:f)";

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de donnees", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                  "       (SELECT COUNT(*) FROM ADAM.participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM ADAM.conference c ";

    const bool isDateFilter = ui->conf_radioButton_19 && ui->conf_radioButton_19->isChecked();

    if (!whereClause.isEmpty() && ((isDateFilter && !dateFilter.isEmpty()) || (!isDateFilter && !filterText.isEmpty())))
        sql += "WHERE " + whereClause + " ";

    sql += "ORDER BY c.idconference";

    query.prepare(sql);
    if (!whereClause.isEmpty())
    {
        if (isDateFilter)
        {
            query.bindValue(":d", dateFilter);
        }
        else if (!filterText.isEmpty())
        {
            query.bindValue(":f", "%" + filterText + "%");
        }
    }

    if (!query.exec())
    {
        QMessageBox::critical(this, "Filtrage conferences", "Echec SELECT : " + query.lastError().text());
        return;
    }

    ui->conf_tableWidget_7->setRowCount(0);
    int row = 0;
    while (query.next())
    {
        ui->conf_tableWidget_7->insertRow(row);
        ui->conf_tableWidget_7->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->conf_tableWidget_7->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->conf_tableWidget_7->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        const QString dateStr = query.value(3).toDate().toString("yyyy-MM-dd");
        ui->conf_tableWidget_7->setItem(row, 3, new QTableWidgetItem(dateStr));
        ui->conf_tableWidget_7->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->conf_tableWidget_7->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        ++row;
    }

    if (row == 0)
        QMessageBox::information(this, "Filtrage", "Aucune conference pour ce filtre.");

    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
    updateConferenceCalendar();
}

void SmartMarket::filterConferences()
{
    applyConferenceFilterToTable();
}

void SmartMarket::on_conf_pushButton_20_clicked()
{
    if (!ui->conf_tableWidget_2) return;

    QString filter = ui->conf_lineEdit ? ui->conf_lineEdit->text().trimmed() : QString();
    Participant p;
    QSqlQueryModel *model = p.rechercher(filter);

    ui->conf_tableWidget_2->setRowCount(0);
    for (int i = 0; i < model->rowCount(); ++i) {
        ui->conf_tableWidget_2->insertRow(i);
        for (int j = 0; j < model->columnCount(); ++j) {
            ui->conf_tableWidget_2->setItem(i, j, new QTableWidgetItem(model->index(i, j).data().toString()));
        }
    }

    if (model->rowCount() == 0)
        QMessageBox::information(this, "Filtrage", "Aucun participant pour ce filtre.");
}

void SmartMarket::filterParticipants()
{
    on_conf_pushButton_20_clicked();
}

void SmartMarket::on_conf_pushButton_14_clicked()
{
    // Open page_2 (Reviewers page) - index 3 in stackedWidgetMain
    ui->stackedWidgetMain->setCurrentIndex(3);
    loadReviewerTable();
}

// ================================================================
// REVIEWERS PAGE (page_2) NAVIGATION
// ================================================================
void SmartMarket::on_revv_pushButton_22_clicked()
{
    // Navigate to Publications page (index 1)
    ui->stackedWidgetMain->setCurrentIndex(1);
}

void SmartMarket::on_revv_pushButton_24_clicked()
{
    // Navigate to Conferences page (page2, index 2)
    ui->stackedWidgetMain->setCurrentIndex(2);
    loadConferenceTable();
}

// ================================================================
// REVIEWERS CRUD - Ajouter reviewer
// ================================================================
void SmartMarket::on_rev_pushButton_12_clicked()
{
    // Get values from UI fields
    QString idStr = ui->rev_lineEdit_18->text().trimmed();
    QString nom = ui->rev_lineEdit_19->text().trimmed();
    QString email = ui->rev_lineEdit_20->text().trimmed();
    QString specialite = ui->lineEdit_3->text().trimmed();
    QString affiliation = ui->lineEdit_2->text().trimmed();
    QString nbEvalStr = ui->rev_lineEdit_25->text().trimmed();
    QString scoreStr = ui->rev_lineEdit_26->text().trimmed();
    QString idPubStr = ui->rev_lineEdit_27->text().trimmed();

    // Validation
    if (idStr.isEmpty() || nom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Attention", 
            "Remplissez obligatoirement : ID, Nom, Email.");
        return;
    }

    bool idOk = false, nbOk = false, scoreOk = false, idPubOk = false;
    int id = idStr.toInt(&idOk);
    int nbEval = nbEvalStr.toInt(&nbOk);
    double score = scoreStr.toDouble(&scoreOk);
    int idPub = idPubStr.toInt(&idPubOk);

    // ✅ CONTROLE DE SAISI ID REVIEWER
    if (!idOk || id <= 0) {
        QMessageBox::warning(this, "⚠️  ID invalide", "L'ID Reviewer doit etre un NUMERO entier positif !");
        ui->rev_lineEdit_18->setStyleSheet("border: 2px solid red;");
        return;
    } else {
        ui->rev_lineEdit_18->setStyleSheet("");
    }

    // ✅ CONTROLE DE SAISI EMAIL
    if (!email.contains("@") || !email.contains(".") || email.length() < 5) {
        QMessageBox::warning(this, "⚠️  Email invalide", "Veuillez entrer une adresse email valide (ex: nom@domaine.com)");
        ui->rev_lineEdit_20->setStyleSheet("border: 2px solid red;");
        return;
    } else {
        ui->rev_lineEdit_20->setStyleSheet("");
    }

    // ✅ CONTROLE DE SAISI SCORE FIABILITE (1-10)
    if (scoreOk) {
        if (score < 1 || score > 10) {
            QMessageBox::warning(this, "⚠️  Score invalide", "Le Score Fiabilité doit etre un nombre **ENTRE 1 ET 10** !");
            ui->rev_lineEdit_26->setStyleSheet("border: 2px solid red;");
            return;
        } else {
            ui->rev_lineEdit_26->setStyleSheet("");
        }
    }

    // Create reviewer object
    Reviewer r(id, nom, email, specialite, affiliation, 
               nbOk ? nbEval : 0, scoreOk ? score : 0.0, idPubOk ? idPub : 0);

    if (r.ajouter()) {
        QMessageBox::information(this, "Succes", 
            QString("Reviewer ajoute ! ID : %1").arg(id));
        // Clear fields
        ui->rev_lineEdit_18->clear();
        ui->rev_lineEdit_19->clear();
        ui->rev_lineEdit_20->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_2->clear();
        ui->rev_lineEdit_25->clear();
        ui->rev_lineEdit_26->clear();
        ui->rev_lineEdit_27->clear();
        // Refresh table
        loadReviewerTable();
    } else {
        QMessageBox::critical(this, "Erreur BDD", 
            "Echec ajout reviewer.\n\n" + r.getLastError());
    }
}

void SmartMarket::on_rev_pushButton_21_clicked()
{
    const QString idStr = ui->rev_lineEdit_18->text().trimmed();
    bool idOk = false;
    const int id = idStr.toInt(&idOk);

    if (!idOk || id <= 0) {
        QMessageBox::warning(this, "Attention", "Entrez un ID reviewer valide pour supprimer.");
        return;
    }

    const QMessageBox::StandardButton confirm = QMessageBox::question(
        this,
        "Confirmation",
        QString("Supprimer le reviewer ID %1 ?").arg(id),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (confirm != QMessageBox::Yes)
        return;

    Reviewer r;
    if (r.supprimer(id)) {
        QMessageBox::information(this, "Succes", "Reviewer supprime.");
        ui->rev_lineEdit_18->clear();
        loadReviewerTable();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec suppression reviewer.\n\n" + r.getLastError());
    }
}

void SmartMarket::on_rev_pushButton_10_clicked()
{
    QString idStr = ui->rev_lineEdit_18->text().trimmed();
    QString nom = ui->rev_lineEdit_19->text().trimmed();
    QString email = ui->rev_lineEdit_20->text().trimmed();
    QString specialite = ui->lineEdit_3->text().trimmed();
    QString affiliation = ui->lineEdit_2->text().trimmed();
    QString nbEvalStr = ui->rev_lineEdit_25->text().trimmed();
    QString scoreStr = ui->rev_lineEdit_26->text().trimmed();
    QString idPubStr = ui->rev_lineEdit_27->text().trimmed();

    bool idOk = false, nbOk = false, scoreOk = false, idPubOk = false;
    int id = idStr.toInt(&idOk);
    int nbEval = nbEvalStr.toInt(&nbOk);
    double score = scoreStr.toDouble(&scoreOk);
    int idPub = idPubStr.toInt(&idPubOk);

    if (!idOk || id <= 0) {
        QMessageBox::warning(this, "Attention", "Entrez un ID reviewer valide pour modifier.");
        return;
    }

    if (nom.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Remplissez au minimum Nom et Email pour la modification.");
        return;
    }

    Reviewer r(id, nom, email, specialite, affiliation,
               nbOk ? nbEval : 0, scoreOk ? score : 0.0, idPubOk ? idPub : 0);

    if (r.modifier()) {
        QMessageBox::information(this, "Succes", "Reviewer modifie.");
        loadReviewerTable();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec modification reviewer.\n\n" + r.getLastError());
    }
}

void SmartMarket::on_rev_pushButton_19_clicked()
{
    QTableWidget *table = ui->rev_tableWidget_3;
    if (!table) {
        QMessageBox::warning(this, "Export PDF", "Table reviewer introuvable.");
        return;
    }

    if (table->rowCount() == 0) {
        QMessageBox::information(this, "Export PDF", "Aucune donnee reviewer a exporter.");
        return;
    }

    QString defaultPath = QDir::homePath() + "/reviewers_export_"
            + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".pdf";

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Exporter reviewers en PDF",
        defaultPath,
        "PDF Files (*.pdf)"
    );

    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
        filePath += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Export PDF", "Impossible de generer le PDF.");
        return;
    }

    const QFont titleFont("Arial", 14, QFont::Bold);
    const QFont headerFont("Arial", 9, QFont::Bold);
    const QFont bodyFont("Arial", 8);

    const QRect pageRect = printer.pageRect(QPrinter::DevicePixel).toRect();
    const int left = pageRect.left() + 30;
    const int right = pageRect.right() - 30;
    int y = pageRect.top() + 30;

    painter.setFont(titleFont);
    painter.drawText(QRect(left, y, right - left, 28), Qt::AlignLeft | Qt::AlignVCenter,
                     "Rapport Reviewers");
    y += 28;

    painter.setFont(bodyFont);
    painter.drawText(QRect(left, y, right - left, 18), Qt::AlignLeft | Qt::AlignVCenter,
                     "Genere le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    y += 26;

    const int columns = table->columnCount();
    if (columns <= 0) {
        painter.end();
        QMessageBox::warning(this, "Export PDF", "Aucune colonne a exporter.");
        return;
    }

    const int tableWidth = right - left;
    const int colW = tableWidth / columns;
    const int headerH = 24;
    const int rowH = 20;

    auto drawHeader = [&]() {
        painter.setFont(headerFont);
        for (int c = 0; c < columns; ++c) {
            const int x = left + c * colW;
            painter.fillRect(x, y, colW, headerH, QColor("#12304E"));
            painter.setPen(Qt::white);
            const QString text = table->horizontalHeaderItem(c)
                    ? table->horizontalHeaderItem(c)->text()
                    : QString("Col%1").arg(c + 1);
            painter.drawText(QRect(x + 4, y, colW - 8, headerH), Qt::AlignVCenter | Qt::AlignLeft, text);
            painter.setPen(Qt::black);
            painter.drawRect(x, y, colW, headerH);
        }
        y += headerH;
    };

    drawHeader();

    painter.setFont(bodyFont);
    for (int r = 0; r < table->rowCount(); ++r) {
        if (y + rowH > pageRect.bottom() - 20) {
            printer.newPage();
            y = pageRect.top() + 30;
            drawHeader();
            painter.setFont(bodyFont);
        }

        for (int c = 0; c < columns; ++c) {
            const int x = left + c * colW;
            const QString text = table->item(r, c) ? table->item(r, c)->text() : "";
            const QString clipped = QFontMetrics(bodyFont).elidedText(text, Qt::ElideRight, colW - 8);
            painter.drawRect(x, y, colW, rowH);
            painter.drawText(QRect(x + 4, y, colW - 8, rowH), Qt::AlignVCenter | Qt::AlignLeft, clipped);
        }
        y += rowH;
    }

    painter.end();
    QMessageBox::information(this, "Export PDF", "PDF genere: " + filePath);
}

// ================================================================
// LOAD REVIEWER TABLE
// ================================================================
void SmartMarket::loadReviewerTable()
{
    QTableWidget *table = ui->rev_tableWidget_3;
    if (!table) return;

    Reviewer r;
    QSqlQueryModel *model = r.afficher();

    table->setSortingEnabled(false);
    table->setRowCount(0);
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels({
        "ID", "Nom", "Email", "Specialite", 
        "Affiliation", "Nb Evaluations", "Score Fiabilite", "ID Publication"
    });

    // Dark theme styling for better readability
    table->setStyleSheet(R"(
        QTableWidget {
            background-color: #1e1e2e;
            alternate-background-color: #2d2d44;
            color: #ffffff;
            gridline-color: #44475a;
            selection-background-color: #6272a4;
            selection-color: #ffffff;
        }
        QHeaderView::section {
            background-color: #44475a;
            color: #f8f8f2;
            padding: 5px;
            border: 1px solid #6272a4;
            font-weight: bold;
        }
        QTableWidget::item {
            padding: 4px;
        }
    )");
    
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);

    for (int i = 0; i < model->rowCount(); ++i) {
        table->insertRow(i);
        for (int j = 0; j < model->columnCount(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(model->index(i, j).data().toString());
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            table->setItem(i, j, item);
        }
    }

    table->setSortingEnabled(true);
    applyReviewerSort();
    refreshReviewerCharts();
}

void SmartMarket::applyReviewerSort()
{
    QTableWidget *table = ui->rev_tableWidget_3;
    if (!table || table->rowCount() == 0)
        return;

    int column = reviewerSortColumn;

    // Fallback: si la colonne score n'existe pas dans le tableau charge,
    // on trie par Nb Evaluations.
    if (column >= table->columnCount())
        column = 5;

    table->sortItems(column, reviewerSortOrder);
}

void SmartMarket::on_rev_comboBox_14_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    reviewerSortColumn = 5; // Nb Evaluations
    reviewerSortOrder = (ui->rev_comboBox_14 && ui->rev_comboBox_14->currentIndex() == 0)
            ? Qt::AscendingOrder
            : Qt::DescendingOrder;
    applyReviewerSort();
}

void SmartMarket::on_rev_comboBox_15_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    reviewerSortColumn = 6; // Score Fiabilite (si disponible)
    reviewerSortOrder = (ui->rev_comboBox_15 && ui->rev_comboBox_15->currentIndex() == 0)
            ? Qt::AscendingOrder
            : Qt::DescendingOrder;
    applyReviewerSort();
}

// ================================================================
// REVIEWERS CHARTS AUTO-GENERATED
// ================================================================
void SmartMarket::on_rev_pushButton_8_clicked()
{
    QString idSearch = ui->rev_idsearch_3 ? ui->rev_idsearch_3->text().trimmed() : QString();
    QString nomSearch = ui->rev_lineEdit_7 ? ui->rev_lineEdit_7->text().trimmed() : QString();
    QString specialiteSearch = ui->lineEdit ? ui->lineEdit->text().trimmed() : QString();
    
    QTableWidget *table = ui->rev_tableWidget_3;
    if (!table) return;

    table->setSortingEnabled(false);
    table->setRowCount(0);

    Reviewer r;
    QSqlQueryModel *model = nullptr;

    if (idSearch.isEmpty() && nomSearch.isEmpty() && specialiteSearch.isEmpty()) {
        // Si champ vide afficher tous les reviewers
        model = r.afficher();
    } else {
        model = r.rechercher(idSearch, nomSearch, specialiteSearch);
    }

    if (!model) {
        table->setSortingEnabled(true);
        QMessageBox::critical(this, "Erreur BDD", "Echec recherche reviewer.\n\n" + r.getLastError());
        return;
    }

    for (int i = 0; i < model->rowCount(); ++i) {
        table->insertRow(i);
        for (int j = 0; j < model->columnCount(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(model->index(i, j).data().toString());
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            table->setItem(i, j, item);
        }
    }

    table->setSortingEnabled(true);
    applyReviewerSort();
    refreshReviewerCharts();

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun reviewer trouvé pour cette recherche.");
    }
}

void SmartMarket::on_rev_pushButton_3_clicked()
{
    // Reset champs recherche
    if (ui->rev_idsearch_3) {
        ui->rev_idsearch_3->clear();
    }
    if (ui->rev_lineEdit_7) {
        ui->rev_lineEdit_7->clear();
    }
    if (ui->lineEdit) {
        ui->lineEdit->clear();
    }
    // Recharger tous les reviewers
    loadReviewerTable();
}

void SmartMarket::refreshReviewerCharts()
{
    // Cleanup old charts
    if (reviewerPieChartView && ui->rev_chartFramePie && ui->rev_chartFramePie->layout()) {
        ui->rev_chartFramePie->layout()->removeWidget(reviewerPieChartView);
        reviewerPieChartView->deleteLater();
        reviewerPieChartView = nullptr;
    }
    if (reviewerBarChartView && ui->rev_chartFrameLine && ui->rev_chartFrameLine->layout()) {
        ui->rev_chartFrameLine->layout()->removeWidget(reviewerBarChartView);
        reviewerBarChartView->deleteLater();
        reviewerBarChartView = nullptr;
    }

    // Ensure frames have layouts
    if (ui->rev_chartFramePie && !ui->rev_chartFramePie->layout()) {
        QVBoxLayout *l = new QVBoxLayout(ui->rev_chartFramePie);
        l->setContentsMargins(0, 0, 0, 0);
    }
    if (ui->rev_chartFrameLine && !ui->rev_chartFrameLine->layout()) {
        QVBoxLayout *l = new QVBoxLayout(ui->rev_chartFrameLine);
        l->setContentsMargins(0, 0, 0, 0);
    }

    if (!ui->rev_chartFramePie || !ui->rev_chartFrameLine)
        return;

    QTableWidget *table = ui->rev_tableWidget_3;
    if (!table || table->rowCount() == 0)
        return;

    // ✅ NOUVELLE VERSION: STATISTIQUES CALCULEES DIRECTEMENT DE LA TABLE AFFICHÉE (FILTRÉE)
    // Les graphiques suivent EXACTEMENT ce qui est affiché après recherche / filtre
    // ================== PIE CHART: Score Fiabilite Distribution ==================
    int excellent = 0, bon = 0, moyen = 0, insuffisant = 0;
    QBarSet *evalBarSet = new QBarSet("Nombre d'evaluations");
    QStringList reviewerNames;

    for (int i = 0; i < table->rowCount(); ++i) {
        double score = table->item(i, 6) ? table->item(i, 6)->text().toDouble() : 0;
        int nbEval = table->item(i, 5) ? table->item(i, 5)->text().toInt() : 0;
        QString nom = table->item(i, 1) ? table->item(i, 1)->text() : "";

        // Categorize score (NOUVELLE REPARTITION: 1-10 scale)
        if (score >= 7) excellent++;
        else if (score >= 4 && score <= 6) bon++;
        else if (score < 4) insuffisant++;

        // Add to bar chart
        *evalBarSet << nbEval;
        reviewerNames << nom.left(10); // Trim long names
    }

    // Create Pie Series
    QPieSeries *pieSeries = new QPieSeries();
    if (excellent > 0) pieSeries->append(QString("Excellent (%1)").arg(excellent), excellent);
    if (bon > 0) pieSeries->append(QString("Bon (%1)").arg(bon), bon);
    if (moyen > 0) pieSeries->append(QString("Moyen (%1)").arg(moyen), moyen);
    if (insuffisant > 0) pieSeries->append(QString("Insuffisant (%1)").arg(insuffisant), insuffisant);

    // Set colors
    QList<QColor> pieColors = { QColor("#059669"), QColor("#10b981"), QColor("#f59e0b"), QColor("#dc2626") };
    int colorIdx = 0;
    for (QPieSlice *slice : pieSeries->slices()) {
        slice->setColor(pieColors[colorIdx % pieColors.size()]);
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        colorIdx++;
    }

    // Create Pie Chart
    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Répartition des scores de fiabilité");
    pieChart->legend()->setAlignment(Qt::AlignRight);
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->setBackgroundVisible(false);

    reviewerPieChartView = new QChartView(pieChart);
    reviewerPieChartView->setRenderHint(QPainter::Antialiasing);
    if (ui->rev_chartFramePie->layout())
        ui->rev_chartFramePie->layout()->addWidget(reviewerPieChartView);

    // ================== BAR CHART: Nombre Evaluations par Reviewer ==================
    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(evalBarSet);
    evalBarSet->setColor(QColor("#3b82f6"));
    barSeries->setLabelsVisible(true);

    // Create Bar Chart
    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Nombre d'évaluations par reviewer");
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setBackgroundVisible(false);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(reviewerNames);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(false);

    reviewerBarChartView = new QChartView(barChart);
    reviewerBarChartView->setRenderHint(QPainter::Antialiasing);
    if (ui->rev_chartFrameLine->layout())
        ui->rev_chartFrameLine->layout()->addWidget(reviewerBarChartView);
}



// ================================================================
// CHAT BOX REVIEWERS - IMPLEMENTATION COMPLETE
// ================================================================
void SmartMarket::setupReviewerChat()
{
    if (!ui->revv_tab_6) return;

    // Bouton Toggle Chat PLACE SOUS LE BOUTON SUPPRIMER COMME DEMANDE
    QPushButton *chatToggleBtn = new QPushButton("💬 Assistant Chat", ui->revv_tab_6);
    chatToggleBtn->setGeometry(20, 420, 200, 40);
    chatToggleBtn->setStyleSheet("QPushButton { background-color: #2563eb; color: white; border-radius: 6px; font-weight: bold; font-size: 13px; } QPushButton:hover { background-color: #1d4ed8; }");

    // Panneau Chat Principal sur le cote droit
    QWidget *chatPanel = new QWidget(ui->revv_tab_6);
    chatPanel->setObjectName("chatPanelWidget");
    chatPanel->setGeometry(740, 60, 320, 580);
    chatPanel->setStyleSheet("QWidget { background-color: #f8fafc; border: 1px solid #cbd5e1; border-radius: 8px; }");
    chatPanel->hide();

    // Zone historique messages
    QTextEdit *chatHistory = new QTextEdit(chatPanel);
    chatHistory->setObjectName("chatHistory");
    chatHistory->setGeometry(10, 10, 300, 490);
    chatHistory->setReadOnly(true);
    chatHistory->setStyleSheet("QTextEdit { border: none; background-color: white; }");
    chatHistory->setFont(QFont("Segoe UI", 10));

    // Champ texte message
    QLineEdit *chatInput = new QLineEdit(chatPanel);
    chatInput->setObjectName("chatInput");
    chatInput->setGeometry(10, 510, 210, 32);
    chatInput->setPlaceholderText("Posez votre question ici...");
    chatInput->setStyleSheet("QLineEdit { border: 1px solid #cbd5e1; border-radius: 4px; padding: 4px; }");

    // Bouton Envoyer
    QPushButton *sendBtn = new QPushButton("Envoyer", chatPanel);
    sendBtn->setGeometry(230, 510, 80, 32);
    sendBtn->setStyleSheet("QPushButton { background-color: #2563eb; color: white; border-radius: 4px; }");

    // Connexions
    connect(chatToggleBtn, &QPushButton::clicked, this, &SmartMarket::toggleChatPanel);
    connect(sendBtn, &QPushButton::clicked, this, &SmartMarket::sendChatMessage);
    connect(chatInput, &QLineEdit::returnPressed, this, &SmartMarket::sendChatMessage);

    // Charger historique
    loadChatHistory();

    // Message de bienvenue
    if (chatHistory->toPlainText().isEmpty()) {
        appendChatMessage("👋 Bonjour! Je suis votre assistant Reviewer. Posez-moi n'importe quelle question sur l'ajout, modification, suppression, recherche des reviewers.", false);
    }
}

void SmartMarket::toggleChatPanel()
{
    QWidget *chatPanel = ui->revv_tab_6->findChild<QWidget*>("chatPanelWidget");
    if (chatPanel) {
        chatPanel->setVisible(!chatPanel->isVisible());
    }
}

void SmartMarket::sendChatMessage()
{
    QLineEdit *chatInput = ui->revv_tab_6->findChild<QLineEdit*>("chatInput");
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");

    if (!chatInput || !chatHistory) return;

    QString question = chatInput->text().trimmed();
    if (question.isEmpty()) return;

    // Ajouter message utilisateur
    appendChatMessage(question, true);
    chatInput->clear();

    // Reponse IA instantanée
    QString response = localReviewerAssistantReply(question);

    // Simuler "est en train d'écrire"
    QTimer::singleShot(600, this, [this, response]() {
        appendChatMessage(response, false);
        saveChatHistory();
    });
}

void SmartMarket::appendChatMessage(const QString &text, bool isUser)
{
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");
    if (!chatHistory) return;

    QString style = isUser
        ? "margin-left: 60px; background-color: #dbeafe; color: #1e40af; padding: 8px 12px; border-radius: 12px 12px 2px 12px;"
        : "margin-right: 60px; background-color: #f1f5f9; color: #1e293b; padding: 8px 12px; border-radius: 12px 12px 12px 2px;";

    QString html = QString("<div style='%1'>%2</div><div style='height: 8px;'></div>").arg(style, text.toHtmlEscaped().replace("\n", "<br>"));

    chatHistory->moveCursor(QTextCursor::End);
    chatHistory->insertHtml(html);
    chatHistory->verticalScrollBar()->setValue(chatHistory->verticalScrollBar()->maximum());
}

void SmartMarket::saveChatHistory()
{
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");
    if (!chatHistory) return;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/chat_history_reviewer.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << chatHistory->toHtml();
    }
}

void SmartMarket::loadChatHistory()
{
    QTextEdit *chatHistory = ui->revv_tab_6->findChild<QTextEdit*>("chatHistory");
    if (!chatHistory) return;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/chat_history_reviewer.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        chatHistory->setHtml(in.readAll());
    }
}

// ================================================================
// RECOMMANDATION INTELLIGENTE REVIEWERS
// ================================================================
void SmartMarket::setupReviewerRecommendation()
{
    // Implementation will be added in next update
}

void SmartMarket::findBestReviewersForPublication()
{
    // Implementation will be added in next update
}

double SmartMarket::calculateMatchingScore(Reviewer &r, const QString &domainePublication)
{
    Q_UNUSED(r);
    Q_UNUSED(domainePublication);
    return 0.0;
}

void SmartMarket::assignReviewerToPublication()
{
    // Implementation will be added in next update
}

// ================================================================
// METIER #4 : RAPPELS DEADLINES
// ================================================================

void SmartMarket::setupDeadlineTimer()
{
    // Assurer que la colonne existe dès le lancement
    Reviewer::ensureDeadlineColumn();

    deadlineTimer = new QTimer(this);
    connect(deadlineTimer, &QTimer::timeout,
            this, &SmartMarket::checkDeadlines);

    // Vérification silencieuse toutes les 30 minutes
    deadlineTimer->start(30 * 60 * 1000);

    // Vérification immédiate 3 secondes après le lancement (laisse la BDD se connecter)
    QTimer::singleShot(3000, this, &SmartMarket::checkDeadlines);
}

void SmartMarket::checkDeadlines()
{
    Reviewer rv;
    QSqlQueryModel *retard  = rv.getReviewersEnRetard();
    QSqlQueryModel *prochains = rv.getReviewersProchesDeadline(7);

    int nbRetard  = retard  ? retard->rowCount()  : 0;
    int nbProches = prochains ? prochains->rowCount() : 0;

    // Notifications seulement si des alertes existent
    if (nbRetard > 0 || nbProches > 0)
    {
        QString msg;
        if (nbRetard > 0)
            msg += QString("⚠️ %1 reviewer(s) EN RETARD d'évaluation !\n").arg(nbRetard);
        if (nbProches > 0)
            msg += QString("⏰ %1 reviewer(s) dont la deadline approche (≤7j).").arg(nbProches);

        // Alerte dans la barre de statut (non-intrusive)
        statusBar()->showMessage(msg, 15000);
        statusBar()->setStyleSheet(
            nbRetard > 0
            ? "background-color: #dc2626; color: white; font-weight: bold; padding: 4px;"
            : "background-color: #f59e0b; color: white; font-weight: bold; padding: 4px;"
        );
    }
    else
    {
        statusBar()->clearMessage();
        statusBar()->setStyleSheet("");
    }

    if (retard)   delete retard;
    if (prochains) delete prochains;
}

void SmartMarket::showDeadlineDialog()
{
    qDebug() << "[Deadline] showDeadlineDialog() appelé.";

    // ── Vérifier la connexion BDD avant tout ──────────────────────
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::warning(this, "⏰ Deadlines",
            "La base de données n'est pas connectée.\n"
            "Veuillez vous connecter d'abord via l'écran de connexion.");
        return;
    }

    // ── Créer la colonne si absente (Oracle) ─────────────────────
    Reviewer::ensureDeadlineColumn();

    // ── Fenêtre principale ─────────────────────────────────────────
    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("⏰ Deadlines Reviewers");
    dlg->resize(920, 560);
    dlg->setStyleSheet(
        "QDialog { background-color: #0d1b2a; }"
        "QLabel  { color: #e2e8f0; font-size: 13px; }"
        "QGroupBox { color: #93c5fd; font-weight: bold; border: 1px solid #1e40af;"
        "            border-radius: 8px; margin-top: 8px; padding: 8px; }"
        "QTableWidget { background-color: #1e293b; color: #f1f5f9;"
        "               gridline-color: #334155; border-radius: 6px;"
        "               selection-background-color: #2563eb; }"
        "QHeaderView::section { background-color: #1e3a5f; color: #93c5fd;"
        "                       padding: 6px; border: none; font-weight: bold; }"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(dlg);
    mainLayout->setSpacing(14);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    // ── Titre ────────────────────────────────────────────────────
    QLabel *title = new QLabel("⏰  Deadlines Reviewers — Suivi des échéances");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #60a5fa; padding-bottom: 4px;");
    mainLayout->addWidget(title);

    // ── Tableau principal ─────────────────────────────────────────
    QGroupBox *gbTable = new QGroupBox("📊 État de toutes les deadlines");
    QVBoxLayout *gbLayout = new QVBoxLayout(gbTable);

    QTableWidget *table = new QTableWidget();
    table->setObjectName("deadlineTable");
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Spécialité", "ID Pub", "Deadline", "État"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setStyleSheet("QTableWidget { alternate-background-color: #1a2942; }");

    // ── Charger les reviewers ayant une deadline ──────────────────
    QSqlQuery q(db);
    bool queryOk = q.exec(
        "SELECT IDREVIEWER, NOM, SPECIALITE, IDPUBLICATION, DEADLINE_EVALUATION "
        "FROM ADAM.REVIEWER "
        "WHERE DEADLINE_EVALUATION IS NOT NULL "
        "ORDER BY DEADLINE_EVALUATION ASC"
    );

    int row = 0;
    if (!queryOk) {
        qDebug() << "[Deadline] Erreur SQL SELECT:" << q.lastError().text();
        QLabel *errLbl = new QLabel("⚠️ Erreur SQL: " + q.lastError().text());
        errLbl->setStyleSheet("color: #f87171; font-size: 12px;");
        gbLayout->addWidget(errLbl);
    } else {
        QDate today = QDate::currentDate();
        while (q.next()) {
            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
            table->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
            table->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
            table->setItem(row, 3, new QTableWidgetItem(q.value(3).toString()));

            QDate deadline = q.value(4).toDate();
            table->setItem(row, 4, new QTableWidgetItem(deadline.toString("dd/MM/yyyy")));

            QString etat;
            QColor  bg;
            int diff = today.daysTo(deadline);
            if      (diff < 0)  { etat = QString("🔴 EN RETARD (%1j)").arg(-diff);       bg = QColor("#7f1d1d"); }
            else if (diff <= 3) { etat = QString("🟠 URGENT (%1j restants)").arg(diff);   bg = QColor("#78350f"); }
            else if (diff <= 7) { etat = QString("🟡 BIENTÔT (%1j restants)").arg(diff);  bg = QColor("#713f12"); }
            else                { etat = QString("🟢 OK (%1j restants)").arg(diff);        bg = QColor("#14532d"); }

            QTableWidgetItem *etatItem = new QTableWidgetItem(etat);
            etatItem->setBackground(bg);
            etatItem->setForeground(Qt::white);
            table->setItem(row, 5, etatItem);

            if (diff < 0) {
                for (int c = 0; c < 5; ++c)
                    if (table->item(row, c))
                        table->item(row, c)->setBackground(QColor("#450a0a"));
            }
            ++row;
        }
    }

    if (row == 0) {
        QLabel *noDataLbl = new QLabel("ℹ️  Aucune deadline assignée pour le moment.");
        noDataLbl->setAlignment(Qt::AlignCenter);
        noDataLbl->setStyleSheet("color: #94a3b8; font-size: 13px; padding: 20px;");
        gbLayout->addWidget(noDataLbl);
    }
    gbLayout->addWidget(table);
    mainLayout->addWidget(gbTable);

    // ── Boutons bas ───────────────────────────────────────────────
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    QPushButton *btnAdd     = new QPushButton("➕  Assigner Deadline");
    QPushButton *btnDelete  = new QPushButton("🗑  Supprimer deadline");
    QPushButton *btnRefresh = new QPushButton("🔄 Rafraîchir");
    QPushButton *btnClose   = new QPushButton("❌ Fermer");

    btnAdd->setStyleSheet(
        "background-color: #059669; color: white; border-radius: 8px; padding: 8px 18px; font-weight: bold;");
    btnDelete->setStyleSheet(
        "background-color: #dc2626; color: white; border-radius: 8px; padding: 8px 18px; font-weight: bold;");
    btnRefresh->setStyleSheet(
        "background-color: #2563eb; color: white; border-radius: 8px; padding: 8px 18px; font-weight: bold;");
    btnClose->setStyleSheet(
        "background-color: #374151; color: white; border-radius: 8px; padding: 8px 18px; font-weight: bold;");

    bottomLayout->addWidget(btnAdd);
    bottomLayout->addSpacing(8);
    bottomLayout->addWidget(btnDelete);
    bottomLayout->addSpacing(12);
    bottomLayout->addWidget(btnRefresh);
    bottomLayout->addStretch();
    bottomLayout->addWidget(btnClose);
    mainLayout->addLayout(bottomLayout);

    // ── Connexions ────────────────────────────────────────────────

    // Assigner une nouvelle deadline
    connect(btnAdd, &QPushButton::clicked, dlg, [=]() {
        QDialog *dlgAdd = new QDialog(dlg);
        dlgAdd->setWindowTitle("➕ Nouvelle Deadline");
        dlgAdd->resize(400, 250);
        dlgAdd->setStyleSheet(dlg->styleSheet()); // Même style sombre

        QVBoxLayout *l = new QVBoxLayout(dlgAdd);
        l->setSpacing(15);

        l->addWidget(new QLabel("Sélectionner un Reviewer :"));
        QComboBox *cbRev = new QComboBox();
        cbRev->setStyleSheet("QComboBox { background-color: #1e293b; color: white; padding: 5px; border-radius: 4px; }");
        
        // Charger les reviewers sans deadline
        QSqlQuery qRev(db);
        qRev.exec("SELECT IDREVIEWER, NOM FROM ADAM.REVIEWER WHERE DEADLINE_EVALUATION IS NULL ORDER BY NOM");
        while (qRev.next()) {
            cbRev->addItem(QString("%1 (ID: %2)").arg(qRev.value(1).toString(), qRev.value(0).toString()), qRev.value(0));
        }
        l->addWidget(cbRev);

        l->addWidget(new QLabel("Date d'échéance :"));
        QDateEdit *de = new QDateEdit(QDate::currentDate().addDays(7));
        de->setCalendarPopup(true);
        de->setStyleSheet("QDateEdit { background-color: #1e293b; color: white; padding: 5px; border-radius: 4px; }");
        l->addWidget(de);

        QHBoxLayout *bl = new QHBoxLayout();
        QPushButton *bOk = new QPushButton("Assigner");
        QPushButton *bCan = new QPushButton("Annuler");
        bOk->setStyleSheet("background-color: #059669; color: white; border-radius: 6px; padding: 6px; font-weight: bold;");
        bCan->setStyleSheet("background-color: #374151; color: white; border-radius: 6px; padding: 6px;");
        bl->addWidget(bOk);
        bl->addWidget(bCan);
        l->addLayout(bl);

        connect(bCan, &QPushButton::clicked, dlgAdd, &QDialog::reject);
        connect(bOk, &QPushButton::clicked, dlgAdd, [=]() {
            int id = cbRev->currentData().toInt();
            if (id <= 0) {
                QMessageBox::warning(dlgAdd, "Erreur", "Veuillez sélectionner un reviewer.");
                return;
            }
            Reviewer rv;
            if (rv.setDeadline(id, de->date())) {
                QMessageBox::information(dlgAdd, "Succès", "Deadline assignée avec succès.");
                dlgAdd->accept();
                // Rafraîchir le dialogue parent
                btnRefresh->click();
            } else {
                QMessageBox::critical(dlgAdd, "Erreur", rv.getLastError());
            }
        });

        dlgAdd->exec();
    });

    // Supprimer la deadline du reviewer sélectionné dans le tableau
    connect(btnDelete, &QPushButton::clicked, dlg, [=]() {
        int selRow = table->currentRow();
        if (selRow < 0) {
            QMessageBox::information(dlg, "Sélection requise",
                "Veuillez d'abord sélectionner un reviewer dans le tableau.");
            return;
        }
        QString idStr  = table->item(selRow, 0) ? table->item(selRow, 0)->text() : "";
        QString nomStr = table->item(selRow, 1) ? table->item(selRow, 1)->text() : "";
        if (idStr.isEmpty()) return;

        if (QMessageBox::question(dlg, "Confirmer la suppression",
                QString("Supprimer la deadline du reviewer #%1 (%2) ?")
                    .arg(idStr, nomStr))
            == QMessageBox::Yes)
        {
            Reviewer rv;
            if (rv.supprimerDeadline(idStr.toInt())) {
                table->removeRow(selRow);
                checkDeadlines();
                QMessageBox::information(dlg, "✔ Supprimé",
                    QString("Deadline du reviewer #%1 supprimée.").arg(idStr));
            } else {
                QMessageBox::critical(dlg, "Erreur", rv.getLastError());
            }
        }
    });

    // Rafraîchir : fermer + rouvrir
    connect(btnRefresh, &QPushButton::clicked, dlg, [=]() {
        checkDeadlines();
        dlg->accept();
        QTimer::singleShot(0, this, &SmartMarket::showDeadlineDialog);
    });

    connect(btnClose, &QPushButton::clicked, dlg, &QDialog::accept);

    dlg->exec();
    delete dlg;
}

// ================================================================
// ARDUINO LCD DEADLINE NAVIGATION
// ================================================================

void SmartMarket::on_revv_btn_arduino_start_clicked()
{
    qDebug() << "[ArduinoLCD] Tentative de démarrage...";

    // 🔍 AUTO DETECTION AUTOMATIQUE - SCANNE TOUS LES PORTS
    statusBar()->showMessage("🔍 Recherche Arduino sur TOUS les ports...");
    
    // ESSAYE TOUS LES PORTS DISPONIBLES AUTOMATIQUEMENT
    QString portTrouve = arduinoLcd->autoConnect();
    
    if (portTrouve.isEmpty()) {
        QStringList tousLesPorts = Arduino2::availablePorts();
        QString msg;
        
        if (tousLesPorts.isEmpty()) {
            msg = "❌ AUCUN port série détecté.\n\nVérifiez que l'Arduino est bien branché en USB.";
        } else {
            msg = "❌ Aucun Arduino n'a répondu.\n\n✅ Ports testés: " + tousLesPorts.join(" | ");
        }
        
        QMessageBox::warning(this, "Arduino LCD", msg);
        return;
    }
    
    // ✅ ARDUINO TROUVE !
    QMessageBox::information(this, "Arduino LCD", 
        "✅ SUCCES ! Arduino connecté automatiquement sur le port: " + portTrouve);

    // 2. Charger les deadlines depuis la base de données
    m_arduinoDeadlines.clear();
    QSqlQuery q("SELECT NOM, DEADLINE_EVALUATION FROM ADAM.REVIEWER "
                "WHERE DEADLINE_EVALUATION IS NOT NULL "
                "ORDER BY DEADLINE_EVALUATION ASC");
    
    while (q.next()) {
        DeadlineInfo info;
        info.nom = q.value(0).toString();
        info.date = q.value(1).toDate();
        m_arduinoDeadlines.append(info);
    }

    if (m_arduinoDeadlines.isEmpty()) {
        QMessageBox::information(this, "Arduino LCD", "Aucune deadline trouvée dans la base.");
        arduinoLcd->clearDisplay();
        return;
    }

    // 3. Afficher la plus récente (index 0)
    m_currentArduinoIndex = 0;
    updateArduinoDisplay();
    
    statusBar()->showMessage("Arduino LCD : Navigation démarrée (1/" + QString::number(m_arduinoDeadlines.size()) + ")", 5000);
}

void SmartMarket::on_revv_btn_arduino_prev_clicked()
{
    if (m_arduinoDeadlines.isEmpty()) return;
    
    m_currentArduinoIndex--;
    if (m_currentArduinoIndex < 0) 
        m_currentArduinoIndex = m_arduinoDeadlines.size() - 1; // Boucle à la fin

    updateArduinoDisplay();
}

void SmartMarket::on_revv_btn_arduino_next_clicked()
{
    if (m_arduinoDeadlines.isEmpty()) return;
    
    m_currentArduinoIndex++;
    if (m_currentArduinoIndex >= m_arduinoDeadlines.size()) 
        m_currentArduinoIndex = 0; // Boucle au début

    updateArduinoDisplay();
}

void SmartMarket::updateArduinoDisplay()
{
    if (m_currentArduinoIndex < 0 || m_currentArduinoIndex >= m_arduinoDeadlines.size())
        return;

    const DeadlineInfo &info = m_arduinoDeadlines.at(m_currentArduinoIndex);
    arduinoLcd->displayDeadline(info.nom, info.date);
    
    qDebug() << "[ArduinoLCD] Affichage :" << info.nom << info.date.toString();
}
