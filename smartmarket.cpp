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
#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QImage>
#include <QPdfWriter>
#include <QStandardPaths>

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
        connect(ui->conf_calendarWidget, &QCalendarWidget::selectionChanged,
            this, &SmartMarket::on_conf_calendarWidget_selectionChanged);

    // Connect CRUD buttons for conferences
    connect(ui->conf_pushButton_8, &QPushButton::clicked, this, &SmartMarket::deleteConference);


    // Connect sorting buttons
    connect(ui->conf_pushButton_17, &QPushButton::clicked, this, &SmartMarket::sortConferencesByDateAsc);
    connect(ui->conf_pushButton_18, &QPushButton::clicked, this, &SmartMarket::sortConferencesByDateDesc);

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
                  "(SELECT COUNT(*) FROM OUSSAMA.participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM OUSSAMA.conference c";

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
    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
    updateConferenceCalendar();
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
    if (!query.exec("SELECT id, nom, idconference FROM OUSSAMA.participant ORDER BY id")) {
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

    updateConferenceParticipantsChart();
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
        QGraphicsTextItem *text = scene->addText("Aucune conférence trouvée");
        text->setPos(sceneW / 2 - 90, sceneH / 2);
        return;
    }

    QVector<QString> names;
    QVector<int> counts;
    int maxCount = 0;
    for (int r = 0; r < rows; ++r) {
        const QString name = ui->conf_tableWidget_7->item(r, 1) ? ui->conf_tableWidget_7->item(r, 1)->text() : QString();
        const int count = ui->conf_tableWidget_7->item(r, 5) ? ui->conf_tableWidget_7->item(r, 5)->text().toInt() : 0;
        names.append(name);
        counts.append(count);
        if (count > maxCount)
            maxCount = count;
    }

    if (maxCount <= 0) {
        QGraphicsTextItem *text = scene->addText("Aucun participant renseigné");
        text->setPos(sceneW / 2 - 100, sceneH / 2);
        return;
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
    const double scaleY = maxCount > 0 ? availableHeight / maxCount : 0.0;

    QFont tickFont("Segoe UI", 9);
    QPen tickPen(Qt::black);
    tickPen.setWidth(1);

    const int tickCount = 4;
    const double tickStep = tickCount > 0 ? static_cast<double>(maxCount) / tickCount : 0.0;
    for (int t = 0; t <= tickCount; ++t) {
        const double value = tickStep * t;
        const double yTick = originY - value * scaleY;
        scene->addLine(originX - 6, yTick, originX, yTick, tickPen);
        QGraphicsTextItem *tickLabel = scene->addText(QString::number(static_cast<int>(value)));
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
        const int barHeight = static_cast<int>(counts[i] * scaleY);
        const int x = startX + i * (barWidth + spacing);
        const int y = originY - barHeight;
        QBrush brush(palette[i % palette.size()], Qt::SolidPattern);
        scene->addRect(x, y, barWidth, barHeight, barPen, brush);

        QGraphicsTextItem *countLabel = scene->addText(QString::number(counts[i]));
        countLabel->setFont(tickFont);
        countLabel->setDefaultTextColor(Qt::black);
        countLabel->setPos(x, y - 18);

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
        QGraphicsTextItem *text = scene->addText("Aucune conférence trouvée");
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
    int maxCount = 0;
    for (const QString &d : dayOrder) {
        const int value = dayCountsMap.value(d);
        days.append(d);
        counts.append(value);
        maxCount = qMax(maxCount, value);
    }

    if (maxCount <= 0) {
        QGraphicsTextItem *text = scene->addText("Aucun jour planifié");
        text->setPos(sceneW / 2 - 80, sceneH / 2);
        return;
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
    const double tickStep = tickCount > 0 ? static_cast<double>(maxCount) / tickCount : 0.0;
    const double availableHeight = originY - axisMaxY - 10;
    const double scaleY = maxCount > 0 ? availableHeight / maxCount : 0.0;

    for (int t = 0; t <= tickCount; ++t) {
        const double value = tickStep * t;
        const double yTick = originY - value * scaleY;
        scene->addLine(originX - 6, yTick, originX, yTick, tickPen);
        QGraphicsTextItem *tickLbl = scene->addText(QString::number(static_cast<int>(value)));
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
        const int barHeight = static_cast<int>(counts[i] * scaleY);
        const int x = startX + i * (barWidth + spacing);
        const int y = originY - barHeight;
        QBrush brush(palette[i % palette.size()], Qt::SolidPattern);
        scene->addRect(x, y, barWidth, barHeight, barPen, brush);

        QGraphicsTextItem *cntLbl = scene->addText(QString::number(counts[i]));
        cntLbl->setFont(tickFont);
        cntLbl->setDefaultTextColor(Qt::black);
        cntLbl->setPos(x, y - 18);

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

void SmartMarket::updateConferenceCalendar()
{
    if (!ui->conf_calendarWidget || !ui->conf_tableWidget_7)
        return;

    const QList<QDate> selectedDates = ui->conf_calendarWidget->selectedDate().isValid()
        ? QList<QDate>{ui->conf_calendarWidget->selectedDate()}
        : QList<QDate>();

    ui->conf_calendarWidget->setDateTextFormat(QDate(), QTextCharFormat());

    const int rows = ui->conf_tableWidget_7->rowCount();
    QTextCharFormat fmt;
    fmt.setForeground(Qt::white);
    fmt.setBackground(QBrush(Qt::red));
    fmt.setFontWeight(QFont::Bold);

    for (int r = 0; r < rows; ++r) {
        const QString dateStr = ui->conf_tableWidget_7->item(r, 3)
            ? ui->conf_tableWidget_7->item(r, 3)->text()
            : QString();

        const QDate d = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (d.isValid())
            ui->conf_calendarWidget->setDateTextFormat(d, fmt);
    }

    if (!selectedDates.isEmpty())
        ui->conf_calendarWidget->setSelectedDate(selectedDates.first());
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

    for (int r = 0; r < rows; ++r) {
        const QString dateStr = ui->conf_tableWidget_7->item(r, 3)
            ? ui->conf_tableWidget_7->item(r, 3)->text()
            : QString();

        const QDate d = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (d != selected)
            continue;

        const QString nom = ui->conf_tableWidget_7->item(r, 1)
            ? ui->conf_tableWidget_7->item(r, 1)->text()
            : "(sans nom)";

        const QString lieu = ui->conf_tableWidget_7->item(r, 2)
            ? ui->conf_tableWidget_7->item(r, 2)->text()
            : "";

        const QString participants = ui->conf_tableWidget_7->item(r, 5)
            ? ui->conf_tableWidget_7->item(r, 5)->text()
            : "";

        lines << nom + (lieu.isEmpty() ? "" : " - " + lieu)
                    + (participants.isEmpty() ? "" : " | Participants: " + participants);
    }

    if (lines.isEmpty())
        QMessageBox::information(this, "Calendrier", "Aucune conférence prévue ce jour.");
    else
        QMessageBox::information(this, "Calendrier", lines.join("\n"));
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
    if (!testQ.exec("SELECT COUNT(*) FROM OUSSAMA.PUBLICATION")) {
        QMessageBox::critical(this, "Erreur BDD",
                              "Impossible d acces a OUSSAMA.PUBLICATION :\n" + testQ.lastError().text());
        return;
    }
    testQ.next();
    int rowCount = testQ.value(0).toInt();
    qDebug() << "Lignes en BDD :" << rowCount;

    QSqlQuery insertQ;
    insertQ.prepare(
        "INSERT INTO OUSSAMA.PUBLICATION "
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
    seqQ.exec("SELECT NVL(MAX(IDPUBLICATION),0) FROM OUSSAMA.PUBLICATION");
    int nextId = 1;
    if (seqQ.next()) nextId = seqQ.value(0).toInt() + 1;

    for (const QVariantList &row : sampleRows) {
        checkQ.prepare("SELECT COUNT(*) FROM OUSSAMA.PUBLICATION WHERE TITRE = :titre");
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

    publicationModel->setTable("OUSSAMA.PUBLICATION");
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
    qStat.exec("SELECT STATUT, COUNT(*) FROM OUSSAMA.PUBLICATION GROUP BY STATUT");
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
        "FROM OUSSAMA.PUBLICATION "
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
    if (email.compare("oussama@esprit.tn", Qt::CaseInsensitive) == 0
        && password == "oussama")
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
    seqQ.exec("SELECT NVL(MAX(IDPUBLICATION),0)+1 FROM OUSSAMA.PUBLICATION");
    int newId = 1;
    if (seqQ.next()) newId = seqQ.value(0).toInt();

    QSqlQuery q;
    q.prepare(
        "INSERT INTO OUSSAMA.PUBLICATION "
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
    chk.prepare("SELECT COUNT(*) FROM OUSSAMA.PUBLICATION WHERE IDPUBLICATION=:id");
    chk.bindValue(":id", id); chk.exec();
    if (chk.next() && chk.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", QString("ID %1 introuvable.").arg(id)); return;
    }

    QSqlQuery q;
    q.prepare(
        "UPDATE OUSSAMA.PUBLICATION "
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
    chk.prepare("SELECT TITRE FROM OUSSAMA.PUBLICATION WHERE IDPUBLICATION=:id");
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
    q.prepare("DELETE FROM OUSSAMA.PUBLICATION WHERE IDPUBLICATION=:id");
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
                  "FROM OUSSAMA.PUBLICATION";

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
              "FROM OUSSAMA.PUBLICATION WHERE IDPUBLICATION IN (:a, :b)");
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
              "FROM OUSSAMA.PUBLICATION WHERE IDPUBLICATION=:id");
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
void SmartMarket::on_conf_pushButton_26_clicked()
{
    const QString idText = ui->conf_lineEdit_26 ? ui->conf_lineEdit_26->text().trimmed() : QString();
    const QString nom = ui->conf_lineEdit_27 ? ui->conf_lineEdit_27->text().trimmed() : QString();
    const QString lieu = ui->conf_lineEdit_24 ? ui->conf_lineEdit_24->text().trimmed() : QString();
    const QDate date = ui->conf_dateEdit_6 ? ui->conf_dateEdit_6->date() : QDate();
    const QString theme = ui->conf_lineEdit_25 ? ui->conf_lineEdit_25->text().trimmed() : QString();

    bool idOk = false;
    const int id = idText.toInt(&idOk);

    const int maxId = 9999999;

    if (!idOk || nom.isEmpty() || lieu.isEmpty() || !date.isValid())
    {
        QMessageBox::warning(this, "Saisie incomplète", "Veuillez renseigner ID (numérique), Nom, Lieu et Date.");
        return;
    }

    if (id < 0 || id > maxId)
    {
        QMessageBox::warning(this, "ID invalide", "ID doit être un entier entre 0 et " + QString::number(maxId) + ".");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO OUSSAMA.conference (idconference, nom, lieu, datedebut, theme) "
              "VALUES (:id, :nom, :lieu, :date, :theme)");
    q.bindValue(":id", id);
    q.bindValue(":nom", nom);
    q.bindValue(":lieu", lieu);
    q.bindValue(":date", date);
    q.bindValue(":theme", theme);

    if (!q.exec())
    {
        QMessageBox::critical(this, "Insertion conférence",
                              "Echec INSERT : " + q.lastError().text() +
                              "\nValeurs : ID=" + QString::number(id) +
                              ", Nom=" + nom +
                              ", Lieu=" + lieu +
                              ", Date=" + date.toString("yyyy-MM-dd") +
                              ", Theme=" + theme);
        return;
    }

    QMessageBox::information(this, "Succès", "Conférence ajoutée dans la base.");

    loadConferenceTable();

    if (ui->conf_lineEdit_26)
        ui->conf_lineEdit_26->clear();
    if (ui->conf_lineEdit_27)
        ui->conf_lineEdit_27->clear();
    if (ui->conf_lineEdit_24)
        ui->conf_lineEdit_24->clear();
    if (ui->conf_dateEdit_6)
        ui->conf_dateEdit_6->setDate(QDate::currentDate());
    if (ui->conf_lineEdit_25)
        ui->conf_lineEdit_25->clear();
}

void SmartMarket::addConference()
{
    on_conf_pushButton_26_clicked();
}

void SmartMarket::on_conf_pushButton_27_clicked()
{
    const QString idText = ui->conf_lineEdit_31 ? ui->conf_lineEdit_31->text().trimmed() : QString();
    const QString nom = ui->conf_lineEdit_28 ? ui->conf_lineEdit_28->text().trimmed() : QString();
    const QString lieu = ui->conf_lineEdit_29 ? ui->conf_lineEdit_29->text().trimmed() : QString();
    const QDate date = ui->conf_dateEdit_4 ? ui->conf_dateEdit_4->date() : QDate();
    const QString theme = ui->conf_lineEdit_30 ? ui->conf_lineEdit_30->text().trimmed() : QString();

    bool idOk = false;
    const int id = idText.toInt(&idOk);
    const int maxId = 9999999;

    if (!idOk || nom.isEmpty() || lieu.isEmpty() || !date.isValid())
    {
        QMessageBox::warning(this, "Saisie incomplète", "Renseignez ID (numérique), Nom, Lieu et Date.");
        return;
    }

    if (id < 0 || id > maxId)
    {
        QMessageBox::warning(this, "ID invalide", "ID doit être entre 0 et " + QString::number(maxId) + ".");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("UPDATE OUSSAMA.conference "
              "SET nom = :nom, lieu = :lieu, datedebut = :date, theme = :theme "
              "WHERE idconference = :id");
    q.bindValue(":nom", nom);
    q.bindValue(":lieu", lieu);
    q.bindValue(":date", date);
    q.bindValue(":theme", theme);
    q.bindValue(":id", id);

    if (!q.exec())
    {
        QMessageBox::critical(this, "Mise à jour conférence",
                              "Echec UPDATE : " + q.lastError().text() +
                              "\nValeurs : ID=" + QString::number(id) +
                              ", Nom=" + nom +
                              ", Lieu=" + lieu +
                              ", Date=" + date.toString("yyyy-MM-dd") +
                              ", Theme=" + theme);
        return;
    }

    if (q.numRowsAffected() == 0)
        QMessageBox::information(this, "Mise à jour", "Aucune conférence trouvée avec cet ID.");
    else
        QMessageBox::information(this, "Mise à jour", "Conférence mise à jour.");

    loadConferenceTable();

    if (ui->conf_lineEdit_31)
        ui->conf_lineEdit_31->clear();
    if (ui->conf_lineEdit_28)
        ui->conf_lineEdit_28->clear();
    if (ui->conf_lineEdit_29)
        ui->conf_lineEdit_29->clear();
    if (ui->conf_dateEdit_4)
        ui->conf_dateEdit_4->setDate(QDate::currentDate());
    if (ui->conf_lineEdit_30)
        ui->conf_lineEdit_30->clear();
}

void SmartMarket::modifyConference()
{
    on_conf_pushButton_27_clicked();
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
    query.prepare("DELETE FROM OUSSAMA.conference WHERE idconference=:id");
    query.bindValue(":id", id.toInt());

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Conférence supprimée avec succès.");
        loadConferenceTable();
        ui->conf_lineEdit_8->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression: " + query.lastError().text());
    }
}

void SmartMarket::on_conf_pushButton_19_clicked()
{
    const QString idText = ui->conf_lineEdit_21 ? ui->conf_lineEdit_21->text().trimmed() : QString();
    const QString nom = ui->conf_lineEdit_20 ? ui->conf_lineEdit_20->text().trimmed() : QString();
    const QString confIdText = ui->conf_lineEdit_22 ? ui->conf_lineEdit_22->text().trimmed() : QString();

    bool idOk = false;
    const int participantId = idText.toInt(&idOk);
    bool confIdOk = false;
    const int conferenceId = confIdText.toInt(&confIdOk);

    const QRegularExpression nameRegex("^[A-Za-z\\s]+$");

    if (!idOk || idText.isEmpty())
    {
        QMessageBox::warning(this, "ID participant", "L'ID doit contenir uniquement des chiffres.");
        return;
    }

    if (nom.isEmpty() || !nameRegex.match(nom).hasMatch())
    {
        QMessageBox::warning(this, "Nom participant", "Le nom doit contenir uniquement des lettres et des espaces.");
        return;
    }

    if (!confIdOk || confIdText.isEmpty())
    {
        QMessageBox::warning(this, "ID conférence", "L'ID conférence doit être numérique.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery checkConf(db);
    checkConf.prepare("SELECT 1 FROM OUSSAMA.conference WHERE idconference = :id");
    checkConf.bindValue(":id", conferenceId);
    if (!checkConf.exec())
    {
        QMessageBox::critical(this, "Validation", "Echec vérification conférence : " + checkConf.lastError().text());
        return;
    }

    if (!checkConf.next())
    {
        QMessageBox::warning(this, "Validation", "Aucune conférence avec cet ID. Veuillez en choisir une existante.");
        return;
    }

    QSqlQuery insert(db);
    insert.prepare("INSERT INTO OUSSAMA.participant (id, nom, idconference) VALUES (:id, :nom, :idconference)");
    insert.bindValue(":id", participantId);
    insert.bindValue(":nom", nom);
    insert.bindValue(":idconference", conferenceId);

    if (!insert.exec())
    {
        QMessageBox::critical(this, "Insertion participant", "Echec INSERT : " + insert.lastError().text());
        return;
    }

    QMessageBox::information(this, "Succès", "Participant ajouté dans la base.");
    loadParticipantTable();
    loadConferenceTable();

    if (ui->conf_lineEdit_21)
        ui->conf_lineEdit_21->clear();
    if (ui->conf_lineEdit_20)
        ui->conf_lineEdit_20->clear();
    if (ui->conf_lineEdit_22)
        ui->conf_lineEdit_22->clear();
}

void SmartMarket::addParticipant()
{
    on_conf_pushButton_19_clicked();
}

void SmartMarket::on_conf_pushButton_10_clicked()
{
    const QString idText = ui->conf_lineEdit_23 ? ui->conf_lineEdit_23->text().trimmed() : QString();
    const QString nom = ui->conf_lineEdit_15 ? ui->conf_lineEdit_15->text().trimmed() : QString();
    const QString confIdText = ui->conf_lineEdit_14 ? ui->conf_lineEdit_14->text().trimmed() : QString();

    bool idOk = false;
    const int participantId = idText.toInt(&idOk);
    bool confIdOk = false;
    const int conferenceId = confIdText.toInt(&confIdOk);

    const QRegularExpression nameRegex("^[A-Za-z\\s]+$");

    if (!idOk || idText.isEmpty())
    {
        QMessageBox::warning(this, "ID participant", "L'ID à modifier doit être numérique.");
        return;
    }

    if (nom.isEmpty() || !nameRegex.match(nom).hasMatch())
    {
        QMessageBox::warning(this, "Nom participant", "Le nom doit contenir uniquement des lettres et des espaces.");
        return;
    }

    if (!confIdOk || confIdText.isEmpty())
    {
        QMessageBox::warning(this, "ID conférence", "L'ID conférence doit être numérique.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery checkParticipant(db);
    checkParticipant.prepare("SELECT 1 FROM OUSSAMA.participant WHERE id = :id");
    checkParticipant.bindValue(":id", participantId);
    if (!checkParticipant.exec())
    {
        QMessageBox::critical(this, "Validation", "Echec vérification participant : " + checkParticipant.lastError().text());
        return;
    }

    if (!checkParticipant.next())
    {
        QMessageBox::information(this, "Mise à jour", "Aucun participant avec cet ID.");
        return;
    }

    QSqlQuery checkConf(db);
    checkConf.prepare("SELECT 1 FROM OUSSAMA.conference WHERE idconference = :id");
    checkConf.bindValue(":id", conferenceId);
    if (!checkConf.exec())
    {
        QMessageBox::critical(this, "Validation", "Echec vérification conférence : " + checkConf.lastError().text());
        return;
    }

    if (!checkConf.next())
    {
        QMessageBox::warning(this, "Validation", "Aucune conférence avec cet ID. Veuillez en choisir une existante.");
        return;
    }

    QSqlQuery update(db);
    update.prepare("UPDATE OUSSAMA.participant SET nom = :nom, idconference = :idconference WHERE id = :id");
    update.bindValue(":nom", nom);
    update.bindValue(":idconference", conferenceId);
    update.bindValue(":id", participantId);

    if (!update.exec())
    {
        QMessageBox::critical(this, "Mise à jour participant", "Echec UPDATE : " + update.lastError().text());
        return;
    }

    QMessageBox::information(this, "Mise à jour", "Participant modifié.");
    loadParticipantTable();
    loadConferenceTable();

    if (ui->conf_lineEdit_23)
        ui->conf_lineEdit_23->clear();
    if (ui->conf_lineEdit_15)
        ui->conf_lineEdit_15->clear();
    if (ui->conf_lineEdit_14)
        ui->conf_lineEdit_14->clear();
}

void SmartMarket::modifyParticipant()
{
    on_conf_pushButton_10_clicked();
}

void SmartMarket::on_conf_pushButton_12_clicked()
{
    const QString idText = ui->conf_lineEdit_13 ? ui->conf_lineEdit_13->text().trimmed() : QString();
    bool ok = false;
    const int participantId = idText.toInt(&ok);

    if (!ok || idText.isEmpty())
    {
        QMessageBox::warning(this, "ID participant", "L'ID à supprimer doit être numérique.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery check(db);
    check.prepare("SELECT 1 FROM OUSSAMA.participant WHERE id = :id");
    check.bindValue(":id", participantId);
    if (!check.exec())
    {
        QMessageBox::critical(this, "Validation", "Echec vérification participant : " + check.lastError().text());
        return;
    }

    if (!check.next())
    {
        QMessageBox::information(this, "Suppression", "Aucun participant avec cet ID.");
        return;
    }

    QSqlQuery del(db);
    del.prepare("DELETE FROM OUSSAMA.participant WHERE id = :id");
    del.bindValue(":id", participantId);

    if (!del.exec())
    {
        QMessageBox::critical(this, "Suppression participant", "Echec DELETE : " + del.lastError().text());
        return;
    }

    QMessageBox::information(this, "Suppression", "Participant supprimé.");
    loadParticipantTable();
    loadConferenceTable();
    if (ui->conf_lineEdit_13)
        ui->conf_lineEdit_13->clear();
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

    QString basePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = basePath.isEmpty() ? "export_conferences.pdf" : basePath + "/export_conferences.pdf";

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive())
    {
        QMessageBox::critical(this, "Export PDF", "Impossible d'ouvrir le PDF en écriture : " + filePath);
        return;
    }

    const int pageW = writer.width();
    const int pageH = writer.height();
    const int margin = 70;
    int y = margin;

    QFont titleFont("Segoe UI", 14, QFont::Bold);
    QFont headerFont("Segoe UI", 10, QFont::Bold);
    QFont cellFont("Segoe UI", 9);
    QPen gridPen(Qt::black);
    gridPen.setWidth(1);

    auto newPageIfNeeded = [&](int blockHeight) {
        if (y + blockHeight > pageH - margin)
        {
            writer.newPage();
            y = margin;
        }
    };

    auto drawTable = [&](QTableWidget *table, const QString &title, const QVector<QString> &headers)
    {
        if (!table)
            return;

        const int cols = headers.size();
        if (cols == 0)
            return;

        const int colWidth = (pageW - 2 * margin) / cols;
        const int rowHeight = 24;
        const int headerHeight = 28;
        const int totalHeight = headerHeight + table->rowCount() * rowHeight + 30;
        newPageIfNeeded(totalHeight + 30);

        painter.setFont(titleFont);
        painter.drawText(margin, y, pageW - 2 * margin, headerHeight, Qt::AlignLeft | Qt::AlignVCenter, title);
        y += headerHeight + 4;

        painter.setFont(headerFont);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#f0f0f0"));
        painter.drawRect(margin, y, colWidth * cols, headerHeight);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(gridPen);
        for (int c = 0; c < cols; ++c)
        {
            painter.drawText(margin + c * colWidth, y, colWidth, headerHeight, Qt::AlignLeft | Qt::AlignVCenter, headers[c]);
            painter.drawRect(margin + c * colWidth, y, colWidth, headerHeight);
        }
        y += headerHeight;

        painter.setFont(cellFont);
        for (int r = 0; r < table->rowCount(); ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                const QString text = table->item(r, c) ? table->item(r, c)->text() : "";
                painter.drawText(margin + c * colWidth, y, colWidth, rowHeight, Qt::AlignLeft | Qt::AlignVCenter, text);
                painter.drawRect(margin + c * colWidth, y, colWidth, rowHeight);
            }
            y += rowHeight;
            if (y > pageH - margin)
            {
                writer.newPage();
                y = margin;
                painter.setFont(headerFont);
                painter.setPen(Qt::NoPen);
                painter.setBrush(QColor("#f0f0f0"));
                painter.drawRect(margin, y, colWidth * cols, headerHeight);
                painter.setBrush(Qt::NoBrush);
                painter.setPen(gridPen);
                for (int c = 0; c < cols; ++c)
                {
                    painter.drawText(margin + c * colWidth, y, colWidth, headerHeight, Qt::AlignLeft | Qt::AlignVCenter, headers[c]);
                    painter.drawRect(margin + c * colWidth, y, colWidth, headerHeight);
                }
                y += headerHeight;
                painter.setFont(cellFont);
            }
        }

        y += 20;
    };

    auto drawChart = [&](QGraphicsView *view, const QString &title)
    {
        if (!view || !view->scene())
            return;

        const int exportW = pageW - 2 * margin;
        const int exportH = 350;

        QImage img(view->viewport()->size() * view->devicePixelRatioF(), QImage::Format_ARGB32);
        img.setDevicePixelRatio(view->devicePixelRatioF());
        img.fill(Qt::white);
        QPainter imgPainter(&img);
        view->scene()->render(&imgPainter);
        imgPainter.end();

        QImage scaled = img.scaled(exportW, exportH, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        newPageIfNeeded(title.isEmpty() ? exportH + 10 : exportH + 40);
        if (!title.isEmpty())
        {
            painter.setFont(titleFont);
            painter.drawText(margin, y, exportW, 24, Qt::AlignLeft | Qt::AlignVCenter, title);
            y += 28;
        }

        painter.drawImage(QRect(margin, y, scaled.width(), scaled.height()), scaled);
        y += scaled.height() + 20;
    };

    drawTable(ui->conf_tableWidget_7, "Conférences", {"ID", "Nom", "Lieu", "Date", "Thème", "Participants"});
    drawTable(ui->conf_tableWidget_2, "Participants", {"ID", "Nom", "Id Conférence"});
    drawChart(ui->conf_graphicsView1, "Participants par conférence");
    drawChart(ui->conf_graphicsView1_2, "Conférences par date");

    painter.end();

    QMessageBox::information(this, "Export PDF", "PDF généré : " + filePath);
}

void SmartMarket::exportConferencesToPDF()
{
    on_conf_pushButton_5_clicked();
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
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                  "       (SELECT COUNT(*) FROM participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM conference c ";

    const bool isDateFilter = ui->conf_radioButton_19 && ui->conf_radioButton_19->isChecked();

    if (!whereClause.isEmpty() && ((isDateFilter && !dateFilter.isEmpty()) || (!isDateFilter && !filterText.isEmpty())))
        sql += "WHERE " + whereClause + " ";

    sql += "ORDER BY c.idconference";

    query.prepare(sql);
    if (!whereClause.isEmpty())
    {
        if (isDateFilter)
            query.bindValue(":d", dateFilter);
        else if (!filterText.isEmpty())
            query.bindValue(":f", "%" + filterText + "%");
    }

    if (!query.exec())
    {
        QMessageBox::critical(this, "Filtrage conférences", "Echec SELECT : " + query.lastError().text());
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
        QMessageBox::information(this, "Filtrage", "Aucune conférence pour ce filtre.");

    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
    updateConferenceCalendar();
}

void SmartMarket::filterConferences()
{
    on_conf_pushButton_21_clicked();
}

void SmartMarket::on_conf_pushButton_20_clicked()
{
    if (!ui->conf_tableWidget_2)
        return;

    const QString filter = ui->conf_lineEdit ? ui->conf_lineEdit->text().trimmed() : QString();

    QSqlDatabase db = QSqlDatabase::database();
    if ((!db.isValid() || !db.isOpen()) && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    if (filter.isEmpty())
    {
        query.prepare("SELECT id, nom, idconference FROM OUSSAMA.participant ORDER BY id");
    }
    else
    {
        query.prepare("SELECT id, nom, idconference FROM OUSSAMA.participant WHERE LOWER(nom) LIKE LOWER(:f) ORDER BY id");
        query.bindValue(":f", "%" + filter + "%");
    }

    if (!query.exec())
    {
        QMessageBox::critical(this, "Filtrage participants", "Echec SELECT : " + query.lastError().text());
        return;
    }

    ui->conf_tableWidget_2->setRowCount(0);
    int row = 0;
    while (query.next())
    {
        ui->conf_tableWidget_2->insertRow(row);
        ui->conf_tableWidget_2->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->conf_tableWidget_2->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->conf_tableWidget_2->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        ++row;
    }

    if (row == 0)
    {
        QMessageBox::information(this, "Filtrage", "Aucun participant pour ce filtre.");
    }
}

void SmartMarket::filterParticipants()
{
    on_conf_pushButton_20_clicked();
}


