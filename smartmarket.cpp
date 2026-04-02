#include "smartmarket.h"
#include "ui_smartmarket.h"

#include "connection.h"

#include <QHeaderView>
#include <QStandardItem>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <QDate>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QFont>
#include <QFrame>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QDateTime>
#include <QStandardPaths>
#include <algorithm>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// Qt Charts
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

struct Reviewer {
    QString name;
    int evaluationCount;
    float reliability; // 0–100
};

static QVector<Reviewer> reviewersData = {
    {"John Doe", 10, 95},
    {"Jane Smith", 12, 88},
    {"Ali Ben", 8, 82},
    {"Sara Lee", 5, 90}
};

SmartMarket::SmartMarket(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartMarket)
    , publicationModel(new QStandardItemModel(this))
    , conferencePageIndex(-1)
    , reviewersPageIndex(-1)
{
    ui->setupUi(this);
    setupConferencePage();
    setupReviewersPage();
    
    // Masquer le mot de passe
    ui->lineEditLoginPassword->setEchoMode(QLineEdit::Password);
    
    // Initialiser le tableau des publications
    initPublicationTable();
    
    // Créer les graphiques
    createCharts();
    
    // Démarrer sur la page Login (page 0)
    ui->stackedWidgetMain->setCurrentIndex(0);
}

SmartMarket::~SmartMarket()
{
    delete ui;
}

void SmartMarket::initPublicationTable()
{
    // ================== TABLEAU DES PUBLICATIONS ==================
    publicationModel->setRowCount(4);
    publicationModel->setColumnCount(5);

    publicationModel->setHeaderData(0, Qt::Horizontal, "Domaine");
    publicationModel->setHeaderData(1, Qt::Horizontal, "Titre");
    publicationModel->setHeaderData(2, Qt::Horizontal, "Source");
    publicationModel->setHeaderData(3, Qt::Horizontal, "Date");
    publicationModel->setHeaderData(4, Qt::Horizontal, "Statut");

    publicationModel->setItem(0, 0, new QStandardItem("SVT"));
    publicationModel->setItem(0, 1, new QStandardItem("Molécules"));
    publicationModel->setItem(0, 2, new QStandardItem("Journal"));
    publicationModel->setItem(0, 3, new QStandardItem("01/11/2000"));
    publicationModel->setItem(0, 4, new QStandardItem("Évaluée"));

    publicationModel->setItem(1, 0, new QStandardItem("Informatique"));
    publicationModel->setItem(1, 1, new QStandardItem("IA"));
    publicationModel->setItem(1, 2, new QStandardItem("Magazine"));
    publicationModel->setItem(1, 3, new QStandardItem("01/09/2009"));
    publicationModel->setItem(1, 4, new QStandardItem("Non évaluée"));

    publicationModel->setItem(2, 0, new QStandardItem("Informatique"));
    publicationModel->setItem(2, 1, new QStandardItem("Deep Learning"));
    publicationModel->setItem(2, 2, new QStandardItem("Magazine of Science"));
    publicationModel->setItem(2, 3, new QStandardItem("06/09/2005"));
    publicationModel->setItem(2, 4, new QStandardItem("Rejetée"));

    publicationModel->setItem(3, 0, new QStandardItem("Physique"));
    publicationModel->setItem(3, 1, new QStandardItem("Atomes"));
    publicationModel->setItem(3, 2, new QStandardItem("Journal Parisien"));
    publicationModel->setItem(3, 3, new QStandardItem("06/07/2008"));
    publicationModel->setItem(3, 4, new QStandardItem("Rejetée"));

    ui->tableView->setModel(publicationModel);
    ui->tableView_2->setModel(publicationModel);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void SmartMarket::createCharts()
{
    // ================== PIE CHART - RÉPARTITION PAR STATUT ==================
    int eval = 0, nonEval = 0, rejet = 0;
    for (int row = 0; row < publicationModel->rowCount(); ++row) {
        QString statut = publicationModel->item(row, 4)->text();
        if (statut == "Évaluée") eval++;
        else if (statut == "Non évaluée") nonEval++;
        else if (statut == "Rejetée") rejet++;
    }

    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Évaluée", eval);
    pieSeries->append("Non évaluée", nonEval);
    pieSeries->append("Rejetée", rejet);

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Répartition des publications par statut");
    pieChart->legend()->setAlignment(Qt::AlignRight);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *pieLayout = new QVBoxLayout(ui->chartWidget);
    pieLayout->setContentsMargins(0, 0, 0, 0);
    pieLayout->addWidget(pieChartView);

    // ================== BAR CHART - PUBLICATIONS PAR DOMAINE ==================
    QStringList domaines = {"SVT", "Informatique", "Physique", "Math", "Chimie", "Biologie"};
    QMap<QString, int> domainCounts;
    for (const QString &d : domaines) domainCounts[d] = 0;

    for (int row = 0; row < publicationModel->rowCount(); ++row) {
        QString domaine = publicationModel->item(row, 0)->text();
        if (domainCounts.contains(domaine)) domainCounts[domaine]++;
    }

    QBarSet *barSet = new QBarSet("Publications");
    for (const QString &d : domaines)
        *barSet << domainCounts[d];

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("Nombre de publications par domaine");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(domaines);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *barLayout = new QVBoxLayout(ui->domainChartWidget);
    barLayout->setContentsMargins(0, 0, 0, 0);
    barLayout->addWidget(barChartView);
}

void SmartMarket::setupConferenceCharts()
{
    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
}

void SmartMarket::setupConferencePage()
{
    conferencePageIndex = ui->stackedWidgetMain->indexOf(ui->page2);

    if (conferencePageIndex < 0)
        return;

    // Bouton retour vers publications
    if (auto btnConfBack = ui->page2->findChild<QPushButton*>("conf_pushButton_13"))
    {
        connect(btnConfBack, &QPushButton::clicked, this, [this]() {
            ui->stackedWidgetMain->setCurrentIndex(1);
        });
    }

    // Nav vers reviewers
    if (auto btnConfReviewers = ui->page2->findChild<QPushButton*>("conf_pushButton_14"))
    {
        connect(btnConfReviewers, &QPushButton::clicked, this, [this]() {
            if (reviewersPageIndex >= 0)
                ui->stackedWidgetMain->setCurrentIndex(reviewersPageIndex);
            else
                QMessageBox::warning(this, "Module Reviewers", "Le module Reviewers n'est pas disponible.");
        });
    }

    // Nav vers conférences (page actuelle)
    if (auto btnConfSelf = ui->page2->findChild<QPushButton*>("conf_pushButton_15"))
    {
        connect(btnConfSelf, &QPushButton::clicked, this, [this]() {
            ui->stackedWidgetMain->setCurrentIndex(conferencePageIndex);
        });
    }

    // Nav vers équipements
    if (auto btnConfEquip = ui->page2->findChild<QPushButton*>("conf_pushButton_16"))
    {
        connect(btnConfEquip, &QPushButton::clicked, this, [this]() {
            QMessageBox::information(this, "Module Équipements", "Le module Équipements sera intégré prochainement.");
        });
    }

    // Tri croissant des conférences par date
    if (auto btnSortAsc = ui->page2->findChild<QPushButton*>("conf_pushButton_18"))
    {
        connect(btnSortAsc, &QPushButton::clicked, this, [this]() {
            if (!ui->conf_tableWidget_7)
                return;

            ui->conf_tableWidget_7->setSortingEnabled(true);
            ui->conf_tableWidget_7->sortItems(3, Qt::AscendingOrder);
        });
    }

    // Tri décroissant des conférences par date
    if (auto btnSortDesc = ui->page2->findChild<QPushButton*>("conf_pushButton_17"))
    {
        connect(btnSortDesc, &QPushButton::clicked, this, [this]() {
            if (!ui->conf_tableWidget_7)
                return;

            ui->conf_tableWidget_7->setSortingEnabled(true);
            ui->conf_tableWidget_7->sortItems(3, Qt::DescendingOrder);
        });
    }

    setupConferenceCharts();

    // Charger les données existantes au démarrage
    loadConferenceTable();
}

void SmartMarket::loadConferenceTable()
{
    if (!ui->conf_tableWidget_7)
        return;

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    const QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                        "       (SELECT COUNT(*) FROM participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                        "FROM conference c ORDER BY c.idconference";

    if (!query.exec(sql))
    {
        QMessageBox::critical(this, "Lecture conférences", "Echec SELECT : " + query.lastError().text());
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

    updateConferenceParticipantsChart();
    updateConferenceDaysChart();
    updateConferenceCalendar();

    // Charger également les participants pour maintenir les deux vues synchronisées
    loadParticipantTable();
}

void SmartMarket::loadParticipantTable()
{
    if (!ui->conf_tableWidget_2)
        return;

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, nom, idconference FROM participant ORDER BY id"))
    {
        QMessageBox::critical(this, "Lecture participants", "Echec SELECT participant : " + query.lastError().text());
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
}

void SmartMarket::on_conf_pushButton_20_clicked()
{
    if (!ui->conf_tableWidget_2)
        return;

    const QString filter = ui->conf_lineEdit ? ui->conf_lineEdit->text().trimmed() : QString();

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    if (filter.isEmpty())
    {
        query.prepare("SELECT id, nom, idconference FROM participant ORDER BY id");
    }
    else
    {
        query.prepare("SELECT id, nom, idconference FROM participant WHERE LOWER(nom) LIKE LOWER(:f) ORDER BY id");
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

    // Déterminer la colonne à filtrer selon le radio sélectionné
    QString whereClause;
    if (ui->conf_radioButton_17 && ui->conf_radioButton_17->isChecked())
        whereClause = "LOWER(c.nom) LIKE LOWER(:f)";
    else if (ui->conf_radioButton_18 && ui->conf_radioButton_18->isChecked())
        whereClause = "LOWER(c.lieu) LIKE LOWER(:f)";
    else if (ui->conf_radioButton_19 && ui->conf_radioButton_19->isChecked())
        whereClause = "TO_CHAR(c.datedebut, 'YYYY-MM-DD') = :d";
    else if (ui->conf_radioButton_20 && ui->conf_radioButton_20->isChecked())
        whereClause = "LOWER(c.theme) LIKE LOWER(:f)";

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
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

void SmartMarket::updateConferenceParticipantsChart()
{
    if (!ui->conf_graphicsView1 || !ui->conf_tableWidget_7)
        return;

    // S'adapter à la taille courante de la vue
    const QSize viewSize = ui->conf_graphicsView1->viewport()->size();
    const int sceneW = viewSize.width() > 0 ? viewSize.width() : 560;
    const int sceneH = viewSize.height() > 0 ? viewSize.height() : 340;

    if (auto oldScene = ui->conf_graphicsView1->scene())
    {
        delete oldScene;
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
        ui->conf_graphicsView1->setScene(scene);
        scene->setSceneRect(0, 0, sceneW, sceneH);
        scene->setBackgroundBrush(Qt::white);
    scene->setBackgroundBrush(Qt::white);

    const int rows = ui->conf_tableWidget_7->rowCount();
    if (rows == 0)
    {
        scene->addText("Aucune conférence trouvée")->setPos(sceneW / 2 - 60, sceneH / 2);
        return;
    }

    QVector<QString> names;
    QVector<int> counts;
    int maxCount = 0;
    for (int r = 0; r < rows; ++r)
    {
        const QString name = ui->conf_tableWidget_7->item(r, 1) ? ui->conf_tableWidget_7->item(r, 1)->text() : QString();
        const int count = ui->conf_tableWidget_7->item(r, 5) ? ui->conf_tableWidget_7->item(r, 5)->text().toInt() : 0;
        names.append(name);
        counts.append(count);
        if (count > maxCount)
            maxCount = count;
    }

    if (maxCount <= 0)
    {
        scene->addText("Aucun participant renseigné")->setPos(sceneW / 2 - 60, sceneH / 2);
        return;
    }

    // Marges et axes
    const int leftMargin = 60;
    const int rightMargin = 40;
    const int topMargin = 30;
    const int bottomMargin = 130; // plus d'espace pour les labels X inclinés
    const int originX = leftMargin;
    const int originY = sceneH - bottomMargin;
    const int axisMaxY = topMargin;
    const int axisMaxX = sceneW - rightMargin;

    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    // Y axis = participants
    scene->addLine(originX, axisMaxY, originX, originY, axisPen);
    // X axis = conference names
    scene->addLine(originX, originY, axisMaxX, originY, axisPen);

    // Barre verticale : noms sur l'axe X, participants sur Y
    const double availableWidth = axisMaxX - originX;
    const int barCount = counts.size();
    const int minBarWidth = 18;
    const int spacing = 12;
    int barWidth = static_cast<int>((availableWidth - spacing * (barCount - 1)) / barCount);
    barWidth = std::max(minBarWidth, barWidth);
    const int totalBarsWidth = barCount * barWidth + spacing * (barCount - 1);
    const int startX = originX + std::max(0, static_cast<int>((availableWidth - totalBarsWidth) / 2));

    const double availableHeight = originY - axisMaxY - 10; // leave a little gap at top
    const double scaleY = maxCount > 0 ? availableHeight / maxCount : 0.0;

    QFont tickFont("Segoe UI", 9);
    QFont labelFont("Segoe UI", 10, QFont::Bold);

    // Graduations sur l'axe Y (participants)
    const int tickCount = 4;
    const double tickStep = tickCount > 0 ? static_cast<double>(maxCount) / tickCount : 0.0;
    QPen tickPen(Qt::black);
    tickPen.setWidth(1);
    for (int t = 0; t <= tickCount; ++t)
    {
        const double value = tickStep * t;
        const double yTick = originY - value * scaleY;
        scene->addLine(originX - 6, yTick, originX, yTick, tickPen);
        auto tickLabel = scene->addText(QString::number(static_cast<int>(value)) + "");
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
    for (int i = 0; i < barCount; ++i)
    {
        const int barHeight = static_cast<int>(counts[i] * scaleY);
        const int x = startX + i * (barWidth + spacing);
        const int y = originY - barHeight;
        QBrush brush(palette[i % palette.size()], Qt::SolidPattern);
        scene->addRect(x, y, barWidth, barHeight, barPen, brush);

        // Nombre au-dessus
        auto countLabel = scene->addText(QString::number(counts[i]));
        countLabel->setFont(tickFont);
        countLabel->setDefaultTextColor(Qt::black);
        countLabel->setPos(x, y - 18);

        // Nom en dessous (x-axis labels)
        const int xCenter = x + barWidth / 2;
        scene->addLine(xCenter, originY, xCenter, originY + 6, tickPen);

        // Nom de la conférence sous la barre, incliné pour tenir si long
        QString displayName = names[i];
        if (displayName.size() > 16)
            displayName = displayName.left(14) + "..."; // éviter les overlaps
        auto nameItem = scene->addText(displayName);
        nameItem->setFont(tickFont);
        nameItem->setDefaultTextColor(Qt::black);
        nameItem->setPos(xCenter - 10, originY + 15);
        nameItem->setRotation(-45);
    }

    // Pas de légendes ou titre supplémentaires pour un rendu épuré
}

void SmartMarket::updateConferenceCalendar()
{
    if (!ui->conf_calendarWidget || !ui->conf_tableWidget_7)
        return;

    // Clear previous formats
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
        lines << nom + (lieu.isEmpty() ? "" : " – " + lieu) + (participants.isEmpty() ? "" : " | Participants: " + participants);
    }

    if (lines.isEmpty())
    {
        QMessageBox::information(this, "Calendrier", "Aucune conférence prévue ce jour.");
    }
    else
    {
        QMessageBox::information(this, "Calendrier", lines.join("\n"));
    }
}

void SmartMarket::updateConferenceDaysChart()
{
    if (!ui->conf_graphicsView1_2 || !ui->conf_tableWidget_7)
        return;

    const QSize viewSize = ui->conf_graphicsView1_2->viewport()->size();
    const int sceneW = viewSize.width() > 0 ? viewSize.width() : 560;
    const int sceneH = viewSize.height() > 0 ? viewSize.height() : 340;

    if (auto oldScene = ui->conf_graphicsView1_2->scene())
    {
        delete oldScene;
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
        ui->conf_graphicsView1_2->setScene(scene);
        scene->setSceneRect(0, 0, sceneW, sceneH);
        scene->setBackgroundBrush(Qt::white);

    const int rows = ui->conf_tableWidget_7->rowCount();
    if (rows == 0)
    {
        scene->addText("Aucune conférence trouvée")->setPos(sceneW / 2 - 60, sceneH / 2);
        return;
    }

    // Regrouper par jour (date) et conserver l'ordre d'apparition
    QMap<QString, int> dayCountsMap;
    QStringList dayOrder;
    for (int r = 0; r < rows; ++r)
    {
        const QString dayLabel = ui->conf_tableWidget_7->item(r, 3) ? ui->conf_tableWidget_7->item(r, 3)->text() : QString();
        if (!dayCountsMap.contains(dayLabel))
            dayOrder.append(dayLabel);
        dayCountsMap[dayLabel] += 1;
    }

    QVector<QString> days;
    QVector<int> counts;
    int maxCount = 0;
    for (const QString &d : dayOrder)
    {
        days.append(d);
        counts.append(dayCountsMap.value(d));
        maxCount = std::max(maxCount, dayCountsMap.value(d));
    }

    if (maxCount <= 0)
    {
        scene->addText("Aucun jour planifié")->setPos(sceneW / 2 - 60, sceneH / 2);
        return;
    }

    // Marges et axes
    const int leftMargin = 60;
    const int rightMargin = 40;
    const int topMargin = 30;
    const int bottomMargin = 120; // plus d'espace pour les dates sur l'axe X
    const int originX = leftMargin;
    const int originY = sceneH - bottomMargin;
    const int axisMaxY = topMargin;
    const int axisMaxX = sceneW - rightMargin;

    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    scene->addLine(originX, axisMaxY, originX, originY, axisPen); // Y axis: number of conferences
    scene->addLine(originX, originY, axisMaxX, originY, axisPen); // X axis: days

    QFont tickFont("Segoe UI", 9);
    QFont labelFont("Segoe UI", 10, QFont::Bold);

    // Y ticks (counts)
    const int tickCount = 4;
    const double tickStep = tickCount > 0 ? static_cast<double>(maxCount) / tickCount : 0.0;
    QPen tickPen(Qt::black);
    tickPen.setWidth(1);
    const double availableHeight = originY - axisMaxY - 10;
    const double scaleY = maxCount > 0 ? availableHeight / maxCount : 0.0;
    for (int t = 0; t <= tickCount; ++t)
    {
        const double value = tickStep * t;
        const double yTick = originY - value * scaleY;
        scene->addLine(originX - 6, yTick, originX, yTick, tickPen);
        auto tickLbl = scene->addText(QString::number(static_cast<int>(value)));
        tickLbl->setFont(tickFont);
        tickLbl->setDefaultTextColor(Qt::black);
        tickLbl->setPos(originX - 45, yTick - 8);
    }

    // Bar layout on X axis (days)
    const double availableWidth = axisMaxX - originX;
    const int barCount = counts.size();
    const int minBarWidth = 18;
    const int spacing = 12;
    int barWidth = static_cast<int>((availableWidth - spacing * (barCount - 1)) / barCount);
    barWidth = std::max(minBarWidth, barWidth);
    const int totalBarsWidth = barCount * barWidth + spacing * (barCount - 1);
    const int startX = originX + std::max(0, static_cast<int>((availableWidth - totalBarsWidth) / 2));

    const QList<QColor> palette = {
        QColor("#4e79a7"), QColor("#f28e2c"), QColor("#e15759"), QColor("#76b7b2"),
        QColor("#59a14f"), QColor("#edc949"), QColor("#af7aa1"), QColor("#ff9da7"),
        QColor("#9c755f"), QColor("#bab0ab")
    };

    QPen barPen(Qt::black);
    for (int i = 0; i < barCount; ++i)
    {
        const int barHeight = static_cast<int>(counts[i] * scaleY);
        const int x = startX + i * (barWidth + spacing);
        const int y = originY - barHeight;
        QBrush brush(palette[i % palette.size()], Qt::SolidPattern);
        scene->addRect(x, y, barWidth, barHeight, barPen, brush);

        // Count above bar
        auto cntLbl = scene->addText(QString::number(counts[i]));
        cntLbl->setFont(tickFont);
        cntLbl->setDefaultTextColor(Qt::black);
        cntLbl->setPos(x, y - 18);

        // Day label centered under the bar with tick mark
        const int xCenter = x + barWidth / 2;
        scene->addLine(xCenter, originY, xCenter, originY + 6, tickPen);
        auto dayLbl = scene->addText(days[i]);
        dayLbl->setFont(tickFont);
        dayLbl->setDefaultTextColor(Qt::black);
        dayLbl->setPos(xCenter - 10, originY + 15);
        dayLbl->setRotation(-45);
    }

    // Pas de titre ou légendes supplémentaires pour un rendu épuré
}

void SmartMarket::setupReviewersPage()
{
    reviewersPageIndex = ui->stackedWidgetMain->indexOf(ui->page3);

    if (reviewersPageIndex < 0)
        return;

    // Sidebar navigation (page3)
    if (auto btnPublications = ui->page3->findChild<QPushButton*>("rev_btnPublications"))
    {
        connect(btnPublications, &QPushButton::clicked, this, [this]() {
            ui->stackedWidgetMain->setCurrentIndex(1);
        });
    }

    if (auto btnReviewers = ui->page3->findChild<QPushButton*>("rev_btnReviewers"))
    {
        connect(btnReviewers, &QPushButton::clicked, this, [this]() {
            ui->stackedWidgetMain->setCurrentIndex(reviewersPageIndex);
        });
    }

    if (auto btnConferences = ui->page3->findChild<QPushButton*>("rev_btnConferences"))
    {
        connect(btnConferences, &QPushButton::clicked, this, [this]() {
            if (conferencePageIndex >= 0)
                ui->stackedWidgetMain->setCurrentIndex(conferencePageIndex);
            else
                QMessageBox::warning(this, "Module Conferences", "Le module Conferences n'est pas disponible.");
        });
    }

    if (auto btnEquipments = ui->page3->findChild<QPushButton*>("rev_btnEquipments"))
    {
        connect(btnEquipments, &QPushButton::clicked, this, [this]() {
            QMessageBox::information(this, "Module Equipements", "Le module Equipements sera integre prochainement.");
        });
    }

    reviewerUpdateKPIs();
    reviewerCreateBarChart();
    reviewerCreateLineChart();
    reviewerCreatePieChart();
    reviewerSetupNavigation();
}

void SmartMarket::reviewerUpdateKPIs()
{
    int total = reviewersData.size();
    float avgRel = 0;
    int maxEvals = 0;
    QString topName;
    QString mostEvalsName;
    float topRel = 0;

    for (const auto &r : reviewersData)
    {
        avgRel += r.reliability;
        if (r.evaluationCount > maxEvals)
        {
            maxEvals = r.evaluationCount;
            mostEvalsName = r.name;
        }
        if (r.reliability > topRel)
        {
            topRel = r.reliability;
            topName = r.name;
        }
    }
    if (total) avgRel /= total;

    if (ui->rev_kpiTotalReviewers)
        ui->rev_kpiTotalReviewers->setText(QString::number(total));
    if (ui->rev_kpiAvgReliability)
        ui->rev_kpiAvgReliability->setText(QString::number(avgRel) + "%");
    if (ui->rev_kpiMostEvaluations)
        ui->rev_kpiMostEvaluations->setText(mostEvalsName + " – " + QString::number(maxEvals));
    if (ui->rev_kpiTopReviewer)
        ui->rev_kpiTopReviewer->setText(topName + " – " + QString::number(topRel) + "%");
}

void SmartMarket::reviewerCreateBarChart()
{
    if (!ui->rev_chartFrameBar)
        return;

    auto *set = new QBarSet("Evaluations");
    QStringList cats;
    for (const auto &r : reviewersData)
    {
        *set << r.evaluationCount;
        cats << r.name;
    }

    auto *series = new QBarSeries();
    series->append(set);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Number of Evaluations per Reviewer");
    chart->createDefaultAxes();
    if (!chart->axes(Qt::Horizontal).isEmpty())
    {
        if (auto *axis = qobject_cast<QBarCategoryAxis*>(chart->axes(Qt::Horizontal).first()))
            axis->append(cats);
    }

    auto *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    if (auto *lbl = ui->rev_chartFrameBar->findChild<QLabel*>())
        lbl->deleteLater();

    auto *lay = new QVBoxLayout(ui->rev_chartFrameBar);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    view->setParent(ui->rev_chartFrameBar);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(view);
}

void SmartMarket::reviewerCreateLineChart()
{
    if (!ui->rev_chartFrameLine)
        return;

    auto *series = new QLineSeries();
    int x = 0;
    for (const auto &r : reviewersData)
        series->append(x++, r.evaluationCount);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Evaluation Trend");
    chart->createDefaultAxes();

    auto *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    if (auto *lbl = ui->rev_chartFrameLine->findChild<QLabel*>())
        lbl->deleteLater();

    auto *lay = new QVBoxLayout(ui->rev_chartFrameLine);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    view->setParent(ui->rev_chartFrameLine);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(view);
}

void SmartMarket::reviewerCreatePieChart()
{
    if (!ui->rev_chartFramePie)
        return;

    auto *series = new QPieSeries();
    int c90_100 = 0, c80_90 = 0, cBelow = 0;
    for (const auto &r : reviewersData)
    {
        if (r.reliability >= 90) ++c90_100;
        else if (r.reliability >= 80) ++c80_90;
        else ++cBelow;
    }
    series->append("90–100%", c90_100);
    series->append("80–90%", c80_90);
    series->append("<80%", cBelow);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Reviewer Reliability Distribution");

    auto *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    if (auto *lbl = ui->rev_chartFramePie->findChild<QLabel*>())
        lbl->deleteLater();

    auto *lay = new QVBoxLayout(ui->rev_chartFramePie);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);
    view->setParent(ui->rev_chartFramePie);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(view);
}

void SmartMarket::reviewerSetupNavigation()
{
    if (!ui->rev_stackedWidget || !ui->rev_stackedWidgetPage1 || !ui->rev_page)
        return;

    const int listIndex = ui->rev_stackedWidget->indexOf(ui->rev_stackedWidgetPage1);
    const int statsIndex = ui->rev_stackedWidget->indexOf(ui->rev_page);

    auto updateButtons = [this, statsIndex, listIndex](int index) {
        const bool isStats = (index == statsIndex);
        const bool isList = (index == listIndex);

        if (ui->rev_btnList)
            ui->rev_btnList->setChecked(isList);
        if (ui->rev_btnStatistics)
            ui->rev_btnStatistics->setChecked(isStats);
        if (ui->rev_btnList_2)
            ui->rev_btnList_2->setChecked(isList);
        if (ui->rev_btnStatistics_2)
            ui->rev_btnStatistics_2->setChecked(isStats);
    };

    if (ui->rev_btnList_2)
    {
        ui->rev_btnList_2->setCheckable(true);
        connect(ui->rev_btnList_2, &QPushButton::clicked, this, [this, listIndex, updateButtons]() {
            ui->rev_stackedWidget->setCurrentIndex(listIndex);
            updateButtons(listIndex);
        });
    }

    if (ui->rev_btnStatistics_2)
    {
        ui->rev_btnStatistics_2->setCheckable(true);
        connect(ui->rev_btnStatistics_2, &QPushButton::clicked, this, [this, statsIndex, updateButtons]() {
            ui->rev_stackedWidget->setCurrentIndex(statsIndex);
            updateButtons(statsIndex);
        });
    }

    if (ui->rev_btnList)
    {
        ui->rev_btnList->setCheckable(true);
        connect(ui->rev_btnList, &QPushButton::clicked, this, [this, listIndex, updateButtons]() {
            ui->rev_stackedWidget->setCurrentIndex(listIndex);
            updateButtons(listIndex);
        });
    }

    if (ui->rev_btnStatistics)
    {
        ui->rev_btnStatistics->setCheckable(true);
        connect(ui->rev_btnStatistics, &QPushButton::clicked, this, [this, statsIndex, updateButtons]() {
            ui->rev_stackedWidget->setCurrentIndex(statsIndex);
            updateButtons(statsIndex);
        });
    }

    ui->rev_stackedWidget->setCurrentIndex(statsIndex);
    updateButtons(statsIndex);
}

int SmartMarket::addEmbeddedWindow(QMainWindow *window)
{
    if (!window)
        return -1;

    QWidget *central = window->centralWidget();
    if (!central)
        return -1;

    central->setParent(nullptr);
    window->setCentralWidget(nullptr);

    QWidget *container = new QWidget(this);
    auto layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(central);

    return ui->stackedWidgetMain->addWidget(container);
}

// ============================================================
// NAVIGATION - MODULE LOGIN
// ============================================================

void SmartMarket::on_conf_pushButton_26_clicked()
{
    const QString idText = ui->conf_lineEdit_26->text().trimmed();
    const QString nom = ui->conf_lineEdit_27->text().trimmed();
    const QString lieu = ui->conf_lineEdit_24->text().trimmed();
    const QDate date = ui->conf_dateEdit_6->date();
    const QString theme = ui->conf_lineEdit_25->text().trimmed();

    bool idOk = false;
    const int id = idText.toInt(&idOk);

    const int maxId = 9'999'999;    // NUMBER(7)

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

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("INSERT INTO conference (idconference, nom, lieu, datedebut, theme, nombreparticipants) "
              "VALUES (:id, :nom, :lieu, :date, :theme, 0)");
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

    // Mise à jour de la vue
    loadConferenceTable();

    // Nettoyer les champs
    ui->conf_lineEdit_26->clear();
    ui->conf_lineEdit_27->clear();
    ui->conf_lineEdit_24->clear();
    ui->conf_dateEdit_6->setDate(QDate::currentDate());
    ui->conf_lineEdit_25->clear();
}

void SmartMarket::on_conf_pushButton_8_clicked()
{
    const QString idText = ui->conf_lineEdit_8->text().trimmed();
    bool ok = false;
    const int id = idText.toInt(&ok);

    if (!ok)
    {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID numérique valide.");
        return;
    }

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("DELETE FROM conference WHERE idconference = :id");
    q.bindValue(":id", id);

    if (!q.exec())
    {
        QMessageBox::critical(this, "Suppression conférence", "Echec DELETE : " + q.lastError().text());
        return;
    }

    // Note: numRowsAffected peut renvoyer -1 selon le driver; on rafraîchit quand même.
    if (q.numRowsAffected() == 0)
    {
        QMessageBox::information(this, "Suppression", "Aucune conférence trouvée avec cet ID.");
    }
    else
    {
        QMessageBox::information(this, "Suppression", "Conférence supprimée.");
    }

    loadConferenceTable();
    ui->conf_lineEdit_8->clear();
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
    const int maxId = 9'999'999;      // NUMBER(7)

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

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery q(db);
    q.prepare("UPDATE conference "
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
    {
        QMessageBox::information(this, "Mise à jour", "Aucune conférence trouvée avec cet ID.");
    }
    else
    {
        QMessageBox::information(this, "Mise à jour", "Conférence mise à jour.");
    }

    loadConferenceTable();

    ui->conf_lineEdit_31->clear();
    ui->conf_lineEdit_28->clear();
    ui->conf_lineEdit_29->clear();
    ui->conf_dateEdit_4->setDate(QDate::currentDate());
    ui->conf_lineEdit_30->clear();
}

void SmartMarket::on_conf_pushButton_19_clicked()
{
    const QString idText = ui->conf_lineEdit_21->text().trimmed();
    const QString nom = ui->conf_lineEdit_20->text().trimmed();
    const QString confIdText = ui->conf_lineEdit_22->text().trimmed();

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

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    // Vérifier l'existence de la conférence cible avant insertion
    QSqlQuery checkConf(db);
    checkConf.prepare("SELECT 1 FROM conference WHERE idconference = :id");
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
    insert.prepare("INSERT INTO participant (id, nom, idconference) VALUES (:id, :nom, :idconference)");
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
    loadConferenceTable(); // recalcule le nombre de participants par conférence

    ui->conf_lineEdit_21->clear();
    ui->conf_lineEdit_20->clear();
    ui->conf_lineEdit_22->clear();
}

void SmartMarket::on_conf_pushButton_12_clicked()
{
    const QString idText = ui->conf_lineEdit_13->text().trimmed();
    bool ok = false;
    const int participantId = idText.toInt(&ok);

    if (!ok || idText.isEmpty())
    {
        QMessageBox::warning(this, "ID participant", "L'ID à supprimer doit être numérique.");
        return;
    }

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    QSqlQuery check(db);
    check.prepare("SELECT 1 FROM participant WHERE id = :id");
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
    del.prepare("DELETE FROM participant WHERE id = :id");
    del.bindValue(":id", participantId);

    if (!del.exec())
    {
        QMessageBox::critical(this, "Suppression participant", "Echec DELETE : " + del.lastError().text());
        return;
    }

    QMessageBox::information(this, "Suppression", "Participant supprimé.");
    loadParticipantTable();
    loadConferenceTable(); // recalcule le nombre de participants par conférence
    ui->conf_lineEdit_13->clear();
}

void SmartMarket::on_conf_pushButton_10_clicked()
{
    const QString idText = ui->conf_lineEdit_23->text().trimmed();
    const QString nom = ui->conf_lineEdit_15->text().trimmed();
    const QString confIdText = ui->conf_lineEdit_14->text().trimmed();

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

    QSqlDatabase db = Connection::getInstance()->getDatabase();
    if (!db.isOpen() && !db.open())
    {
        QMessageBox::critical(this, "Base de données", "Connexion BD indisponible : " + db.lastError().text());
        return;
    }

    // Vérifier l'existence du participant
    QSqlQuery checkParticipant(db);
    checkParticipant.prepare("SELECT 1 FROM participant WHERE id = :id");
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

    // Vérifier l'existence de la conférence cible
    QSqlQuery checkConf(db);
    checkConf.prepare("SELECT 1 FROM conference WHERE idconference = :id");
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
    update.prepare("UPDATE participant SET nom = :nom, idconference = :idconference WHERE id = :id");
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
    loadConferenceTable(); // recalcule le nombre de participants par conférence

    ui->conf_lineEdit_23->clear();
    ui->conf_lineEdit_15->clear();
    ui->conf_lineEdit_14->clear();
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

        y += 20; // espace après tableau
    };

    auto drawChart = [&](QGraphicsView *view, const QString &title)
    {
        if (!view || !view->scene())
            return;

        const int exportW = pageW - 2 * margin;
        const int exportH = 350;

        QImage img(view->viewport()->size() * view->devicePixelRatio(), QImage::Format_ARGB32);
        img.setDevicePixelRatio(view->devicePixelRatio());
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

    // Conférences
    drawTable(ui->conf_tableWidget_7, "Conférences", {"ID", "Nom", "Lieu", "Date", "Thème", "Participants"});

    // Participants
    drawTable(ui->conf_tableWidget_2, "Participants", {"ID", "Nom", "Id Conférence"});

    // Graphiques
    drawChart(ui->conf_graphicsView1, "Participants par conférence");
    drawChart(ui->conf_graphicsView1_2, "Conférences par date");

    painter.end();

    QMessageBox::information(this, "Export PDF", "PDF généré : " + filePath);
}

void SmartMarket::on_btnLoginEntrer_clicked()
{
    QString email = ui->lineEditLoginEmail->text().trimmed();
    QString password = ui->lineEditLoginPassword->text().trimmed();

    if(email.compare("oussama", Qt::CaseInsensitive) == 0
        && password == "oussama")
    {
        QMessageBox::information(this, "Succès", "Connexion réussie ! Bienvenue dans SmartMarket.");
        
        // 🔥 Navigation vers la page Publication (page 1)
        ui->stackedWidgetMain->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Email ou mot de passe incorrect !");
    }
}

void SmartMarket::on_btnLoginEnvoyer_clicked()
{
    QString email = ui->lineEditResetEmail->text().trimmed();
    
    if(email.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer votre adresse email.");
        return;
    }
    
    QMessageBox::information(this, "Email envoyé", 
        "Un email de réinitialisation a été envoyé à : " + email);
    
    ui->lineEditResetEmail->clear();

    // Naviguer vers la page Publications
    ui->stackedWidgetMain->setCurrentIndex(1);
}

// ============================================================
// MODULE PUBLICATION - CRUD
// ============================================================

void SmartMarket::on_pushButton_19_clicked()
{
    QString titre = ui->lineEdit_16->text().trimmed();
    QString source = ui->lineEdit_17->text().trimmed();
    QString createur = ui->lineEdit_18->text().trimmed();
    QString domaine = ui->comboBox_5->currentText();
    QString date = ui->dateEdit_5->date().toString("dd/MM/yyyy");
    QString idModif = ui->lineEdit_19->text().trimmed();
    
    if(titre.isEmpty() || source.isEmpty() || createur.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    if(idModif.isEmpty())
    {
        int newRow = publicationModel->rowCount();
        publicationModel->insertRow(newRow);
        
        publicationModel->setItem(newRow, 0, new QStandardItem(domaine));
        publicationModel->setItem(newRow, 1, new QStandardItem(titre));
        publicationModel->setItem(newRow, 2, new QStandardItem(source));
        publicationModel->setItem(newRow, 3, new QStandardItem(date));
        publicationModel->setItem(newRow, 4, new QStandardItem("Non évaluée"));
        
        QMessageBox::information(this, "Succès", "Publication ajoutée avec succès !");
        
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
    }
}

void SmartMarket::on_pushButton_20_clicked()
{
    QString titre = ui->lineEdit_16->text().trimmed();
    QString source = ui->lineEdit_17->text().trimmed();
    QString createur = ui->lineEdit_18->text().trimmed();
    QString domaine = ui->comboBox_5->currentText();
    QString date = ui->dateEdit_5->date().toString("dd/MM/yyyy");
    QString idModif = ui->lineEdit_19->text().trimmed();
    
    if(idModif.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID de la publication à modifier.");
        return;
    }
    
    int rowToModify = idModif.toInt() - 1;
    
    if(rowToModify >= 0 && rowToModify < publicationModel->rowCount())
    {
        publicationModel->setItem(rowToModify, 0, new QStandardItem(domaine));
        publicationModel->setItem(rowToModify, 1, new QStandardItem(titre));
        publicationModel->setItem(rowToModify, 2, new QStandardItem(source));
        publicationModel->setItem(rowToModify, 3, new QStandardItem(date));
        
        QMessageBox::information(this, "Succès", "Publication modifiée avec succès !");
        
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "ID de publication introuvable.");
    }
}

void SmartMarket::on_pushButton_8_clicked()
{
    QString idSupprimer = ui->lineEdit_9->text().trimmed();
    
    if(idSupprimer.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID de la publication à supprimer.");
        return;
    }
    
    int rowToDelete = idSupprimer.toInt() - 1;
    
    if(rowToDelete >= 0 && rowToDelete < publicationModel->rowCount())
    {
        publicationModel->removeRow(rowToDelete);
        QMessageBox::information(this, "Succès", "Publication supprimée avec succès !");
        ui->lineEdit_9->clear();
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "ID de publication introuvable.");
    }
}

void SmartMarket::on_pushButton_9_clicked()
{
    QString titre = ui->lineEdit_20->text().trimmed();
    QString source = ui->lineEdit_21->text().trimmed();
    QString domaine = ui->comboBox_2->currentText();
    QString score = ui->lineEdit_23->text().trimmed();
    QString statut = ui->lineEdit_22->text().trimmed();
    
    QMessageBox::information(this, "Recherche", 
        "Recherche effectuée avec les critères :\n"
        "Titre: " + titre + "\n"
        "Source: " + source + "\n"
        "Domaine: " + domaine + "\n"
        "Score: " + score + "\n"
        "Statut: " + statut);
}

void SmartMarket::on_pushButton_5_clicked()
{
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_23->clear();
    ui->lineEdit_22->clear();
    ui->comboBox_2->setCurrentIndex(0);
    
    QMessageBox::information(this, "Réinitialisation", "Critères de recherche réinitialisés.");
}

void SmartMarket::on_pushButton_21_clicked()
{
    bool toutes = ui->radioButton_5->isChecked();
    bool pdf = ui->radioButton_7->isChecked();
    bool excel = ui->radioButton_8->isChecked();
    bool includeVides = ui->checkBox_3->isChecked();

    QString format = "PDF";
    if (excel)
        format = "Excel";
    else if (pdf)
        format = "PDF";
    QString scope = toutes ? "toutes les publications" : "les publications filtrées";
    
    QMessageBox::information(this, "Export", 
        "Export en " + format + " de " + scope + ".\n"
        "Inclure champs vides : " + (includeVides ? "Oui" : "Non"));
}

// ============================================================
// MODULE PUBLICATION - ANALYSE
// ============================================================

void SmartMarket::on_pushButton_6_clicked()
{
    QString id1 = ui->lineEdit_5->text().trimmed();
    QString id2 = ui->lineEdit_6->text().trimmed();
    
    if(id1.isEmpty() || id2.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer les deux IDs de publication.");
        return;
    }
    
    int similarite = 75;
    
    QMessageBox::information(this, "Détection de similarité", 
        "Comparaison des publications " + id1 + " et " + id2 + ".\n"
        "Taux de similarité : " + QString::number(similarite) + "%");
    
    ui->progressBar->setValue(similarite);
}

void SmartMarket::on_pushButton_7_clicked()
{
    QString id = ui->lineEdit_7->text().trimmed();
    
    if(id.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID de la publication.");
        return;
    }
    
    int completude = 85;
    
    QMessageBox::information(this, "Vérification de complétude", 
        "Publication " + id + " vérifiée.\n"
        "Complétude : " + QString::number(completude) + "%");
    
    ui->progressBar_2->setValue(completude);
}

// ============================================================
// SIDEBAR - NAVIGATION ENTRE MODULES
// ============================================================

void SmartMarket::on_pushButton_14_clicked()
{
    ui->stackedWidgetMain->setCurrentIndex(1);
}

void SmartMarket::on_pushButton_16_clicked()
{
    if (reviewersPageIndex >= 0)
    {
        ui->stackedWidgetMain->setCurrentIndex(reviewersPageIndex);
    }
    else
    {
        QMessageBox::warning(this, "Module Reviewers", 
            "Le module Reviewers n'est pas disponible (chargement de mainwindow.ui echoue)." );
    }
}

void SmartMarket::on_pushButton_17_clicked()
{
    if (conferencePageIndex >= 0)
    {
        ui->stackedWidgetMain->setCurrentIndex(conferencePageIndex);
    }
    else
    {
        QMessageBox::warning(this, "Module Conférences", 
            "Le module Conférences n'est pas disponible (chargement de l'interface oussama.ui échoué).");
    }
}

void SmartMarket::on_pushButton_18_clicked()
{
    QMessageBox::information(this, "Module Équipements", 
        "Le module Équipements sera intégré prochainement.");
}
