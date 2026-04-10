#include "smartmarket.h"
#include "ui_smartmarket.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDebug>
#include <QFileDialog>
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
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QUrl>
#include <QRegularExpression>
#include <QProcess>
#include <QFileInfo>

// PDF export via Qt
#include <QPrinter>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QRect>
#include <QFontMetrics>
#include <QDateTime>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

// ── Configuration Ollama (Gratuit, 100% Local) ─────────────────────────────────
// Ollama fonctionne complètement gratuitement sur votre PC
// Installation: https://ollama.ai
// Puis: ollama pull neural-chat (plus rapide) ou ollama pull mistral
static const QString OLLAMA_ENDPOINT = "http://localhost:11434/api/generate";
static const QString OLLAMA_MODEL    = "neural-chat";  // Plus rapide que mistral

// ── Domaines ─────────────────────────────────────────────────────────────────
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
    , publicationModel(new QSqlTableModel(this))
    , pieChartView(nullptr)
    , barChartView(nullptr)
    , networkManager(new QNetworkAccessManager(this))
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

    // Connect CRUD buttons for conferences
    connect(ui->conf_pushButton_26, &QPushButton::clicked, this, &SmartMarket::addConference);
    connect(ui->conf_pushButton_27, &QPushButton::clicked, this, &SmartMarket::modifyConference);
    connect(ui->conf_pushButton_8, &QPushButton::clicked, this, &SmartMarket::deleteConference);

    // Connect CRUD buttons for participants
    connect(ui->conf_pushButton_19, &QPushButton::clicked, this, &SmartMarket::addParticipant);
    connect(ui->conf_pushButton_10, &QPushButton::clicked, this, &SmartMarket::modifyParticipant);
    connect(ui->conf_pushButton_12, &QPushButton::clicked, this, &SmartMarket::deleteParticipant);

    // Connect sorting buttons
    connect(ui->conf_pushButton_17, &QPushButton::clicked, this, &SmartMarket::sortConferencesByDateAsc);
    connect(ui->conf_pushButton_18, &QPushButton::clicked, this, &SmartMarket::sortConferencesByDateDesc);

    // Connect export button
    connect(ui->conf_pushButton_5, &QPushButton::clicked, this, &SmartMarket::exportConferencesToPDF);

    // Connect filtering
    connect(ui->conf_pushButton_21, &QPushButton::clicked, this, &SmartMarket::filterConferences);
    connect(ui->conf_pushButton_20, &QPushButton::clicked, this, &SmartMarket::filterParticipants);

    // Load initial data
    loadConferenceTable();
}

void SmartMarket::loadConferenceTable()
{
    QTableWidget *confTableWidget = ui->conf_tableWidget_7;
    if (!confTableWidget)
        return;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        if (!db.open()) {
            QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
            return;
        }
    }

    if (!ensureConferenceTables(db)) {
        QStringList tables = db.tables(QSql::Tables);
        QMessageBox::critical(this, "Base de données",
                              "Les tables Conference ou Participant sont manquantes ou introuvables.\n"
                              "Tables accessibles : " + tables.join(", "));
        return;
    }

    QString filter = ui->conf_lineEdit_2 ? ui->conf_lineEdit_2->text().trimmed().toLower() : QString();

    QSqlQuery query(db);
    QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                  "(SELECT COUNT(*) FROM SELIM.participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM SELIM.conference c";

    if (!filter.isEmpty()) {
        sql += " WHERE LOWER(c.nom) LIKE :f OR LOWER(c.lieu) LIKE :f OR LOWER(c.theme) LIKE :f";
    }
    sql += " ORDER BY c.idconference";

    query.prepare(sql);
    if (!filter.isEmpty()) {
        query.bindValue(":f", "%" + filter + "%");
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Lecture conférences", "Echec SELECT : " + query.lastError().text());
        return;
    }

    confTableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        confTableWidget->insertRow(row);
        confTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        confTableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        confTableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        confTableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toDate().toString("yyyy-MM-dd")));
        confTableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
        confTableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
        row++;
    }

    loadParticipantTable();
}

void SmartMarket::loadParticipantTable()
{
    QTableWidget *participantTableWidget = ui->conf_tableWidget_2;
    if (!participantTableWidget)
        return;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        if (!db.open()) {
            QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
            return;
        }
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, nom, idconference FROM SELIM.participant ORDER BY id")) {
        QMessageBox::critical(this, "Lecture participants", "Echec SELECT participant : " + query.lastError().text());
        return;
    }

    participantTableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        participantTableWidget->insertRow(row);
        participantTableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        participantTableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        participantTableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        row++;
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
                      "idconference NUMBER(10))";
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
        return true;

    return createConferenceTables(db);
}

// ================================================================
// INIT TABLE PUBLICATIONS
// ================================================================
void SmartMarket::initPublicationTable()
{
    QSqlQuery testQ;
    if (!testQ.exec("SELECT COUNT(*) FROM SELIM.PUBLICATION")) {
        QMessageBox::critical(this, "Erreur BDD",
                              "Impossible d acces a SELIM.PUBLICATION :\n" + testQ.lastError().text());
        return;
    }
    testQ.next();
    int rowCount = testQ.value(0).toInt();
    qDebug() << "Lignes en BDD :" << rowCount;

    QSqlQuery insertQ;
    insertQ.prepare(
        "INSERT INTO SELIM.PUBLICATION "
        "(IDPUBLICATION, TITRE, SOURCE, DOMAINE, DATEPUBLICATION, STATUT, CONTENU) "
        "VALUES (:id, :titre, :source, :domaine, TO_DATE(:date,'YYYY-MM-DD'), :statut, :contenu)"
    );

    QList<QVariantList> sampleRows = {
        {"Publication sans source", "", "Informatique", "2024-02-01", "Non evaluee", "Test d une publication avec source manquante."},
        {"Publication sans contenu", "arXiv", "Mathematiques", "2024-03-12", "Evaluee", ""},
        {"Article rejete", "IEEE", "Physique", "2023-12-05", "Rejetee", "Etude refusee pour manque de details."},
        {"Article en attente", "Springer", "Chimie", "2024-01-20", "En attente", "Relecture en cours."},
        {"Sans statut", "ACM", "Biologie", "2024-04-10", "", "Publication a verifier."}
    };

    QSqlQuery checkQ;
    QSqlQuery seqQ;
    seqQ.exec("SELECT NVL(MAX(IDPUBLICATION),0) FROM SELIM.PUBLICATION");
    int nextId = 1;
    if (seqQ.next()) nextId = seqQ.value(0).toInt() + 1;

    for (const QVariantList &row : sampleRows) {
        checkQ.prepare("SELECT COUNT(*) FROM SELIM.PUBLICATION WHERE TITRE = :titre");
        checkQ.bindValue(":titre", row[0]);
        if (checkQ.exec() && checkQ.next() && checkQ.value(0).toInt() == 0) {
            insertQ.bindValue(":id", nextId);
            insertQ.bindValue(":titre", row[0]);
            insertQ.bindValue(":source", row[1]);
            insertQ.bindValue(":domaine", row[2]);
            insertQ.bindValue(":date", row[3]);
            insertQ.bindValue(":statut", row[4]);
            insertQ.bindValue(":contenu", row[5]);
            if (!insertQ.exec()) {
                qDebug() << "Echec insertion publication de test :" << insertQ.lastError().text();
            } else {
                qDebug() << "Publication de test ajoutee :" << row[0] << "id=" << nextId;
                nextId++;
            }
        }
    }

    publicationModel->setTable("SELIM.PUBLICATION");
    publicationModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    publicationModel->setHeaderData(0, Qt::Horizontal, "ID");
    publicationModel->setHeaderData(1, Qt::Horizontal, "Titre");
    publicationModel->setHeaderData(2, Qt::Horizontal, "Source");
    publicationModel->setHeaderData(3, Qt::Horizontal, "Domaine");
    publicationModel->setHeaderData(4, Qt::Horizontal, "Date");
    publicationModel->setHeaderData(5, Qt::Horizontal, "Statut");
    publicationModel->setHeaderData(6, Qt::Horizontal, "Contenu");

    bool ok = publicationModel->select();
    if (!ok) {
        QMessageBox::critical(this, "Erreur modele",
                              "Echec select() :\n" + publicationModel->lastError().text());
        return;
    }
    while (publicationModel->canFetchMore())
        publicationModel->fetchMore();

    ui->tableView->setModel(publicationModel);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->show();
}

// ================================================================
// REFRESH
// ================================================================
void SmartMarket::refreshAll()
{
    publicationModel->setFilter("");
    publicationModel->select();
    while (publicationModel->canFetchMore())
        publicationModel->fetchMore();
    ui->tableView->reset();
    refreshCharts();
}

// ================================================================
// GRAPHIQUES
// ================================================================
void SmartMarket::createCharts()
{
    if (!ui->chartWidget->layout()) {
        QVBoxLayout *l = new QVBoxLayout(ui->chartWidget);
        l->setContentsMargins(0,0,0,0);
    }
    if (!ui->domainChartWidget->layout()) {
        QVBoxLayout *l = new QVBoxLayout(ui->domainChartWidget);
        l->setContentsMargins(0,0,0,0);
    }
    refreshCharts();
}

void SmartMarket::refreshCharts()
{
    if (pieChartView) {
        ui->chartWidget->layout()->removeWidget(pieChartView);
        delete pieChartView; pieChartView = nullptr;
    }
    if (barChartView) {
        ui->domainChartWidget->layout()->removeWidget(barChartView);
        delete barChartView; barChartView = nullptr;
    }

    QSqlQuery qStat;
    qStat.exec("SELECT STATUT, COUNT(*) FROM SELIM.PUBLICATION GROUP BY STATUT");
    QPieSeries *pie = new QPieSeries();
    while (qStat.next()) {
        QString s = qStat.value(0).toString();
        pie->append(s.isEmpty() ? "Inconnu" : s, qStat.value(1).toInt());
    }
    for (auto *sl : pie->slices()) {
        sl->setLabelVisible(true);
        sl->setLabel(QString("%1 (%2)").arg(sl->label()).arg((int)sl->value()));
    }
    QChart *pc = new QChart();
    pc->addSeries(pie);
    pc->setTitle("Repartition par statut");
    pc->legend()->setAlignment(Qt::AlignRight);
    pc->setBackgroundVisible(false);
    pc->setAnimationOptions(QChart::AllAnimations);
    pieChartView = new QChartView(pc);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    ui->chartWidget->layout()->addWidget(pieChartView);

    QSqlQuery qDom;
    qDom.exec(
        "SELECT CASE WHEN TRIM(NVL(DOMAINE,'') ) = '' THEN 'Inconnu' ELSE DOMAINE END AS DOMAINE, "
        "COUNT(*) AS CNT "
        "FROM SELIM.PUBLICATION "
        "GROUP BY CASE WHEN TRIM(NVL(DOMAINE,'')) = '' THEN 'Inconnu' ELSE DOMAINE END "
        "ORDER BY CNT DESC, DOMAINE"
    );
    QStringList doms;
    QList<int> cnts;
    int maxCount = 0;
    while (qDom.next()) {
        QString d = qDom.value(0).toString();
        doms << d;
        int count = qDom.value(1).toInt();
        cnts << count;
        maxCount = qMax(maxCount, count);
    }

    QBarSet *bs = new QBarSet("Publications");
    bs->setColor(QColor("#1E40AF"));
    for (int c : cnts) *bs << c;
    QBarSeries *bseries = new QBarSeries();
    bseries->append(bs);
    bseries->setLabelsVisible(true);
    bseries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    QChart *bc = new QChart();
    bc->addSeries(bseries);
    bc->setTitle("Publications par domaine");
    bc->setAnimationOptions(QChart::SeriesAnimations);
    bc->setBackgroundVisible(false);
    QBarCategoryAxis *axX = new QBarCategoryAxis();
    axX->append(doms);
    bc->addAxis(axX, Qt::AlignBottom);
    bseries->attachAxis(axX);
    QValueAxis *axY = new QValueAxis();
    axY->setLabelFormat("%d");
    axY->setRange(0, qMax(1, maxCount + 1));
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
    if (email.compare("Selim.ASCHI@esprit.tn", Qt::CaseInsensitive) == 0
        && password == "selim")
    {
        ui->stackedWidgetMain->setCurrentIndex(1);
        initPublicationTable();
        createCharts();
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
// CRUD — AJOUTER
// ================================================================
void SmartMarket::on_pushButton_19_clicked()
{
    QString titre   = ui->lineEdit_16->text().trimmed();
    QString source  = ui->lineEdit_17->text().trimmed();
    QString contenu = ui->lineEdit_18->toPlainText().trimmed();
    QString domaine = ui->comboBox_5->currentText().trimmed();
    QString date    = ui->dateEdit_5->date().toString("yyyy-MM-dd");

    if (titre.isEmpty() || source.isEmpty() || domaine.isEmpty()) {
        QMessageBox::warning(this, "Attention",
                             "Remplissez obligatoirement : Titre, Source, Domaine.");
        return;
    }
    if (titre.length() > 20) {
        QMessageBox::warning(this, "Attention", "Titre : max 20 caracteres."); return;
    }
    if (source.length() > 20) {
        QMessageBox::warning(this, "Attention", "Source : max 20 caracteres."); return;
    }
    if (domaine.length() > 20) {
        QMessageBox::warning(this, "Attention", "Domaine : max 20 caracteres."); return;
    }

    QSqlQuery seqQ;
    seqQ.exec("SELECT NVL(MAX(IDPUBLICATION),0)+1 FROM SELIM.PUBLICATION");
    int newId = 1;
    if (seqQ.next()) newId = seqQ.value(0).toInt();

    QSqlQuery q;
    q.prepare(
        "INSERT INTO SELIM.PUBLICATION "
        "(IDPUBLICATION, TITRE, SOURCE, DOMAINE, DATEPUBLICATION, STATUT, CONTENU) "
        "VALUES (:id, :titre, :source, :domaine, "
        "TO_DATE(:date,'YYYY-MM-DD'), 'Non evaluee', :contenu)"
        );
    q.bindValue(":id",      newId);
    q.bindValue(":titre",   titre);
    q.bindValue(":source",  source);
    q.bindValue(":domaine", domaine);
    q.bindValue(":date",    date);
    q.bindValue(":contenu", contenu);

    if (q.exec()) {
        QMessageBox::information(this, "Succes",
                                 QString("Publication ajoutee ! ID : %1").arg(newId));
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
        refreshAll();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec ajout :\n" + q.lastError().text());
    }
}

// ================================================================
// CRUD — MODIFIER
// ================================================================
void SmartMarket::on_pushButton_20_clicked()
{
    QString titre   = ui->lineEdit_16->text().trimmed();
    QString source  = ui->lineEdit_17->text().trimmed();
    QString contenu = ui->lineEdit_18->toPlainText().trimmed();
    QString domaine = ui->comboBox_5->currentText().trimmed();
    QString date    = ui->dateEdit_5->date().toString("yyyy-MM-dd");
    QString idStr   = ui->lineEdit_19->text().trimmed();

    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Entrez l ID de la publication a modifier."); return;
    }
    bool ok; int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Attention", "L ID doit etre un entier positif."); return;
    }
    if (titre.isEmpty() || source.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Remplissez Titre et Source."); return;
    }
    if (titre.length() > 20) {
        QMessageBox::warning(this, "Attention", "Titre : max 20 caracteres."); return;
    }
    if (domaine.length() > 20) {
        QMessageBox::warning(this, "Attention", "Domaine : max 20 caracteres."); return;
    }

    QSqlQuery chk;
    chk.prepare("SELECT COUNT(*) FROM SELIM.PUBLICATION WHERE IDPUBLICATION=:id");
    chk.bindValue(":id", id); chk.exec();
    if (chk.next() && chk.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", QString("ID %1 introuvable.").arg(id)); return;
    }

    QSqlQuery q;
    q.prepare(
        "UPDATE SELIM.PUBLICATION "
        "SET TITRE=:titre, SOURCE=:source, DOMAINE=:domaine, "
        "DATEPUBLICATION=TO_DATE(:date,'YYYY-MM-DD'), CONTENU=:contenu "
        "WHERE IDPUBLICATION=:id"
        );
    q.bindValue(":titre",   titre);
    q.bindValue(":source",  source);
    q.bindValue(":domaine", domaine);
    q.bindValue(":date",    date);
    q.bindValue(":contenu", contenu);
    q.bindValue(":id",      id);

    if (q.exec()) {
        QMessageBox::information(this, "Succes",
                                 QString("Publication ID %1 modifiee !").arg(id));
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
        refreshAll();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec modification :\n" + q.lastError().text());
    }
}

// ================================================================
// CRUD — SUPPRIMER
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

    QSqlQuery chk;
    chk.prepare("SELECT TITRE FROM SELIM.PUBLICATION WHERE IDPUBLICATION=:id");
    chk.bindValue(":id", id); chk.exec();
    if (!chk.next()) {
        QMessageBox::warning(this, "Erreur", QString("ID %1 introuvable.").arg(id)); return;
    }
    QString titre = chk.value(0).toString();

    auto rep = QMessageBox::question(this, "Confirmation",
                                     QString("Supprimer \"%1\" (ID %2) ?").arg(titre).arg(id),
                                     QMessageBox::Yes | QMessageBox::No);
    if (rep != QMessageBox::Yes) return;

    QSqlQuery q;
    q.prepare("DELETE FROM SELIM.PUBLICATION WHERE IDPUBLICATION=:id");
    q.bindValue(":id", id);

    if (q.exec()) {
        QMessageBox::information(this, "Succes",
                                 QString("Publication ID %1 supprimee !").arg(id));
        ui->lineEdit_9->clear();
        refreshAll();
    } else {
        QMessageBox::critical(this, "Erreur BDD", "Echec suppression :\n" + q.lastError().text());
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

    QString filter;
    if (!titre.isEmpty())
        filter += QString("UPPER(TITRE) LIKE UPPER('%%%1%%') AND ").arg(titre);
    if (!source.isEmpty())
        filter += QString("UPPER(SOURCE) LIKE UPPER('%%%1%%') AND ").arg(source);
    if (!domaine.isEmpty() && domaine.toUpper() != "TOUS")
        filter += QString("UPPER(DOMAINE) = UPPER('%1') AND ").arg(domaine);
    if (!statut.isEmpty())
        filter += QString("UPPER(STATUT) LIKE UPPER('%%%1%%') AND ").arg(statut);
    if (filter.endsWith(" AND "))
        filter.chop(5);

    publicationModel->setFilter(filter);
    publicationModel->select();
    while (publicationModel->canFetchMore())
        publicationModel->fetchMore();

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
    publicationModel->setFilter("");
    publicationModel->select();
    while (publicationModel->canFetchMore())
        publicationModel->fetchMore();
}

// ================================================================
// EXPORT PDF — 100% FONCTIONNEL avec QPrinter + QPainter
// ================================================================
void SmartMarket::on_pushButton_21_clicked()
{
    bool pdf    = ui->radioButton_7->isChecked();
    bool excel  = ui->radioButton_8->isChecked();

    if (!pdf && !excel) {
        QMessageBox::warning(this, "Attention", "Choisissez PDF ou Excel."); return;
    }

    if (excel) {
        QMessageBox::information(this, "Export Excel",
                                 "Export Excel : fonctionnalite reservee a une prochaine version.\n"
                                 "Utilisez le format PDF disponible maintenant.");
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

    // Préparer les données
    QSqlQuery q;
    QString sql = "SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, "
                  "TO_CHAR(DATEPUBLICATION,'DD/MM/YYYY'), STATUT, CONTENU "
                  "FROM SELIM.PUBLICATION";

    // Si on exporte seulement les publications filtrées
    if (!toutesPublications) {
        QString currentFilter = publicationModel->filter();
        if (!currentFilter.isEmpty())
            sql += " WHERE " + currentFilter;
    }
    sql += " ORDER BY IDPUBLICATION";

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

    // ── Créer le PDF via QPrinter ────────────────────────────────
    QPrinter printer(QPrinter::HighResolution);
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

    // Facteur de mise à l'échelle pour haute résolution
    const double scale = printer.resolution() / 96.0;
    const int pageW    = painter.device()->width();
    const int pageH    = painter.device()->height();
    const int marginX  = (int)(15 * scale * 3.78);  // mm → pixels approx
    const int marginY  = (int)(15 * scale * 3.78);
    const int contentW = pageW - 2 * marginX;

    // Polices
    QFont titleFont("Arial", (int)(16 * scale));
    titleFont.setBold(true);
    QFont headerFont("Arial", (int)(9 * scale));
    headerFont.setBold(true);
    QFont bodyFont("Arial", (int)(8 * scale));
    QFont metaFont("Arial", (int)(7 * scale));

    // Couleurs
    QColor colorHeader(15, 42, 68);      // #0F2A44 — bleu marine
    QColor colorAlt(240, 245, 255);      // bleu très clair
    QColor colorBorder(180, 200, 230);
    QColor colorTitle(255, 255, 255);
    QColor colorText(30, 30, 30);

    // Largeurs de colonnes (proportionnelles)
    const int COL_COUNT = 7;
    int colW[COL_COUNT];
    double proportions[] = { 0.06, 0.18, 0.13, 0.12, 0.09, 0.12, 0.30 };
    for (int i = 0; i < COL_COUNT; ++i)
        colW[i] = (int)(contentW * proportions[i]);

    const int rowH     = (int)(28 * scale);
    const int headerH  = (int)(36 * scale);
    const int titleH   = (int)(55 * scale);

    // ── Fonction lambda pour dessiner l'en-tête de tableau ──────
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
            painter.drawText(QRect(x + (int)(4*scale), y,
                                   colW[i] - (int)(8*scale), headerH),
                             Qt::AlignVCenter | Qt::AlignLeft, headers[i]);
            x += colW[i];
        }
    };

    // ── Fonction lambda pour dessiner une ligne de données ───────
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
            // Texte (tronqué si nécessaire)
            painter.setFont(bodyFont);
            painter.setPen(colorText);
            QFontMetrics fm(bodyFont);
            QString txt = fm.elidedText(vals[i], Qt::ElideRight,
                                        colW[i] - (int)(10*scale));
            painter.drawText(QRect(x + (int)(4*scale), y,
                                   colW[i] - (int)(8*scale), rowH),
                             Qt::AlignVCenter | Qt::AlignLeft, txt);
            x += colW[i];
        }
    };

    // ════════ PAGE(S) ════════════════════════════════════════════
    int currentY   = marginY;
    int pageNumber = 1;
    bool firstPage = true;

    for (int rowIdx = 0; rowIdx < rows.size(); ) {

        // ── En-tête de page ─────────────────────────────────────
        if (firstPage) {
            // Bannière titre
            painter.fillRect(marginX, currentY, contentW, titleH, colorHeader);
            painter.setFont(titleFont);
            painter.setPen(colorTitle);
            painter.drawText(QRect(marginX, currentY, contentW, titleH),
                             Qt::AlignCenter,
                             "RAPPORT DES PUBLICATIONS — SmartMarket");
            currentY += titleH + (int)(8 * scale);

            // Méta-info
            painter.setFont(metaFont);
            painter.setPen(Qt::darkGray);
            painter.drawText(marginX, currentY + (int)(14*scale),
                             QString("Genere le : %1   |   Total : %2 publication(s)   |   Filtre : %3")
                                 .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"))
                                 .arg(rows.size())
                                 .arg(toutesPublications ? "Toutes" : "Filtrées"));
            currentY += (int)(30 * scale);
            firstPage = false;
        }

        // En-tête du tableau
        drawTableHeader(currentY);
        currentY += headerH;

        // ── Lignes de données jusqu'en bas de page ───────────────
        bool alternate = false;
        while (rowIdx < rows.size()) {
            // Espace restant ?
            if (currentY + rowH > pageH - marginY - (int)(20*scale)) {
                // Pied de page
                painter.setFont(metaFont);
                painter.setPen(Qt::gray);
                painter.drawText(
                    QRect(marginX, pageH - marginY - (int)(20*scale),
                          contentW, (int)(20*scale)),
                    Qt::AlignCenter,
                    QString("Page %1  —  SmartMarket").arg(pageNumber));

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

    // Pied de dernière page
    painter.setFont(metaFont);
    painter.setPen(Qt::gray);
    painter.drawText(
        QRect(marginX, pageH - marginY - (int)(20*scale),
              contentW, (int)(20*scale)),
        Qt::AlignCenter,
        QString("Page %1  —  SmartMarket").arg(pageNumber));

    painter.end();

    QMessageBox::information(this, "Export PDF",
                             QString("PDF genere avec succes !\n%1\n\n"
                                     "%2 publication(s) exportee(s).")
                                 .arg(filePath).arg(rows.size()));
}

// ================================================================
// SIMILARITE IA — Appel Anthropic
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

    // Récupérer les deux publications
    QSqlQuery q;
    q.prepare("SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, CONTENU, STATUT "
              "FROM SELIM.PUBLICATION WHERE IDPUBLICATION IN (:a, :b)");
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
    ui->tableWidget_4->setItem(0, 3, new QTableWidgetItem("⏳ Traitement IA"));
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

    // Appel à Ollama (100% gratuit, local)
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
            details += QString("\n\nRéponse serveur:\n%1").arg(QString::fromUtf8(serverResponse));
        }
        
        // Indice si Ollama n'est pas lancé
        if (details.contains("Connection refused") || details.contains("localhost")) {
            details += "\n\n💡 SOLUTION: Lancez Ollama d'abord!\n"
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

    // Extraire le texte de la réponse Ollama
    QJsonObject root = doc.object();
    QString text = root["response"].toString();  // Ollama retourne "response"

    // Parser le JSON imbriqué
    // Nettoyer les éventuels blocs ```json
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
// COMPLETUDE IA — Appel Anthropic
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
              "FROM SELIM.PUBLICATION WHERE IDPUBLICATION=:id");
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

    // Complétude basique (champs remplis)
    int total = 6, remplis = 0;
    QStringList vals = {titre, source, domaine, date, statut, contenu};
    for (const QString &v : vals) if (!v.isEmpty()) remplis++;
    int pct = (remplis * 100) / total;
    ui->progressBar_2->setValue(pct);

    // Préparer tableau 5 — aperçu des champs
    compIdStr = idStr; compTitre = titre;

    ui->tableWidget_5->clearContents();
    ui->tableWidget_5->setColumnCount(7);
    ui->tableWidget_5->setHorizontalHeaderLabels(
        {"Titre", "Domaine", "Source", "Date",
         "Contenu", "Statut", "Score IA"});
    ui->tableWidget_5->setRowCount(2);

    // Ligne 1 : valeurs
    ui->tableWidget_5->setItem(0, 0, new QTableWidgetItem(titre.isEmpty()   ? "❌ Manquant" : titre));
    ui->tableWidget_5->setItem(0, 1, new QTableWidgetItem(domaine.isEmpty() ? "❌ Manquant" : domaine));
    ui->tableWidget_5->setItem(0, 2, new QTableWidgetItem(source.isEmpty()  ? "❌ Manquant" : source));
    ui->tableWidget_5->setItem(0, 3, new QTableWidgetItem(date.isEmpty()    ? "❌ Manquant" : date));
    ui->tableWidget_5->setItem(0, 4, new QTableWidgetItem(contenu.isEmpty() ? "❌ Manquant" :
                                                              contenu.left(40) + (contenu.length()>40?"…":"")));
    ui->tableWidget_5->setItem(0, 5, new QTableWidgetItem(statut.isEmpty()  ? "❌ Manquant" : statut));
    ui->tableWidget_5->setItem(0, 6, new QTableWidgetItem("Analyse IA en cours..."));

    // Ligne 2 : analyse IA globale (placeholder)
    ui->tableWidget_5->setSpan(1, 0, 1, 7);
    ui->tableWidget_5->setItem(1, 0, new QTableWidgetItem("⏳ L'IA analyse la qualite de la publication..."));
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

    // Appel à Ollama (100% gratuit, local)
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
            details += QString("\n\nRéponse serveur:\n%1").arg(QString::fromUtf8(serverResponse));
        }
        
        // Indice si Ollama n'est pas lancé
        if (details.contains("Connection refused") || details.contains("localhost")) {
            details += "\n\n💡 SOLUTION: Lancez Ollama d'abord!\n"
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

    // Extraire le texte de la réponse Ollama
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

    // Mettre à jour tableau
    ui->tableWidget_5->setItem(0, 6,
                               new QTableWidgetItem(QString("%1/100 — %2").arg(score).arg(qualite)));

    QString missing = manquants.isEmpty() ? "Aucun" : manquants.join(", ");
    QString analyse = "Champs manquants : " + missing +
                      "\nRecommandations : " + recommandations;
    ui->tableWidget_5->setItem(1, 0, new QTableWidgetItem("Analyse IA : " + analyse));
    ui->tableWidget_5->resizeColumnsToContents();
    ui->tableWidget_5->resizeRowToContents(1);

    QMessageBox msg(QMessageBox::Information,
                    "Compl\xc3\xa9tude — R\xc3\xa9sultat IA",
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
{ QMessageBox::information(this, "Info", "Module Equipements a integrer."); }

void SmartMarket::openConferenceModule()
{
    ui->stackedWidgetMain->setCurrentIndex(2); // page2 is index 2
    loadConferenceTable();
}

// Conference CRUD Operations
void SmartMarket::addConference()
{
    QString id = ui->conf_lineEdit_26->text().trimmed();
    QString nom = ui->conf_lineEdit_27->text().trimmed();
    QString lieu = ui->conf_lineEdit_24->text().trimmed();
    QString theme = ui->conf_lineEdit_25->text().trimmed();
    QDate date = ui->conf_dateEdit_6->date();

    if (id.isEmpty() || nom.isEmpty() || lieu.isEmpty() || theme.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, "Base de données", "Connexion impossible.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO SELIM.conference (idconference, nom, lieu, datedebut, theme) VALUES (:id, :nom, :lieu, :date, :theme)");
    query.bindValue(":id", id.toInt());
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":date", date);
    query.bindValue(":theme", theme);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Conférence ajoutée avec succès.");
        loadConferenceTable();
        // Clear fields
        ui->conf_lineEdit_26->clear();
        ui->conf_lineEdit_27->clear();
        ui->conf_lineEdit_24->clear();
        ui->conf_lineEdit_25->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout: " + query.lastError().text());
    }
}

void SmartMarket::modifyConference()
{
    QString id = ui->conf_lineEdit_31->text().trimmed();
    QString nom = ui->conf_lineEdit_28->text().trimmed();
    QString lieu = ui->conf_lineEdit_29->text().trimmed();
    QString theme = ui->conf_lineEdit_30->text().trimmed();
    QDate date = ui->conf_dateEdit_4->date();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "ID manquant", "Veuillez saisir l'ID de la conférence à modifier.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, "Base de données", "Connexion impossible.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE SELIM.conference SET nom=:nom, lieu=:lieu, datedebut=:date, theme=:theme WHERE idconference=:id");
    query.bindValue(":id", id.toInt());
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":date", date);
    query.bindValue(":theme", theme);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Conférence modifiée avec succès.");
        loadConferenceTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification: " + query.lastError().text());
    }
}

void SmartMarket::deleteConference()
{
    QString id = ui->conf_lineEdit_8->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "ID manquant", "Veuillez saisir l'ID de la conférence à supprimer.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette conférence?") != QMessageBox::Yes)
        return;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, "Base de données", "Connexion impossible.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM SELIM.conference WHERE idconference=:id");
    query.bindValue(":id", id.toInt());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Conférence supprimée avec succès.");
        loadConferenceTable();
        ui->conf_lineEdit_8->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression: " + query.lastError().text());
    }
}

void SmartMarket::addParticipant()
{
    QString nom = ui->conf_lineEdit_20->text().trimmed();
    QString idConf = ui->conf_lineEdit_22->text().trimmed();

    if (nom.isEmpty() || idConf.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, "Base de données", "Connexion impossible.");
        return;
    }

    // Get next ID
    QSqlQuery idQuery(db);
    idQuery.exec("SELECT NVL(MAX(id), 0) + 1 FROM SELIM.participant");
    int nextId = 1;
    if (idQuery.next()) {
        nextId = idQuery.value(0).toInt();
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO SELIM.participant (id, nom, idconference) VALUES (:id, :nom, :idconf)");
    query.bindValue(":id", nextId);
    query.bindValue(":nom", nom);
    query.bindValue(":idconf", idConf.toInt());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Participant ajouté avec succès.");
        loadParticipantTable();
        ui->conf_lineEdit_20->clear();
        ui->conf_lineEdit_22->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout: " + query.lastError().text());
    }
}

void SmartMarket::modifyParticipant()
{
    QString id = ui->conf_lineEdit_23->text().trimmed();
    QString nom = ui->conf_lineEdit_14->text().trimmed();
    QString idConf = ui->conf_lineEdit_15->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "ID manquant", "Veuillez saisir l'ID du participant à modifier.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, "Base de données", "Connexion impossible.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE SELIM.participant SET nom=:nom, idconference=:idconf WHERE id=:id");
    query.bindValue(":id", id.toInt());
    query.bindValue(":nom", nom);
    query.bindValue(":idconf", idConf.toInt());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Participant modifié avec succès.");
        loadParticipantTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification: " + query.lastError().text());
    }
}

void SmartMarket::deleteParticipant()
{
    QString id = ui->conf_lineEdit_13->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "ID manquant", "Veuillez saisir l'ID du participant à supprimer.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce participant?") != QMessageBox::Yes)
        return;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        QMessageBox::critical(this, "Base de données", "Connexion impossible.");
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM SELIM.participant WHERE id=:id");
    query.bindValue(":id", id.toInt());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Participant supprimé avec succès.");
        loadParticipantTable();
        ui->conf_lineEdit_13->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression: " + query.lastError().text());
    }
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

void SmartMarket::exportConferencesToPDF()
{
    // Basic PDF export implementation
    QMessageBox::information(this, "Export PDF", "Fonctionnalité d'export PDF à implémenter.");
}

void SmartMarket::filterConferences()
{
    loadConferenceTable(); // Re-filter based on current filter text
}

void SmartMarket::filterParticipants()
{
    QString filter = ui->conf_lineEdit->text().trimmed().toLower();
    QTableWidget *table = ui->conf_tableWidget_2;

    for (int row = 0; row < table->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            if (item && item->text().toLower().contains(filter)) {
                match = true;
                break;
            }
        }
        table->setRowHidden(row, !match && !filter.isEmpty());
    }
}

