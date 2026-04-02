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
    
    // Démarrer sur la page Conférences (première page disponible)
    if (conferencePageIndex >= 0)
        ui->stackedWidgetMain->setCurrentIndex(conferencePageIndex);
}

SmartMarket::~SmartMarket()
{
    delete ui;
}

void SmartMarket::initPublicationTable()
{
    // Stub: widgets for publication table have been removed
}

void SmartMarket::createCharts()
{
    // Stub: chart widgets have been removed from UI
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
            if (conferencePageIndex >= 0)
                ui->stackedWidgetMain->setCurrentIndex(conferencePageIndex);
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
    // Stub: reviewers page (page3) has been removed from UI
    reviewersPageIndex = -1;
}

void SmartMarket::reviewerUpdateKPIs()
{
    // Stub: KPI widgets have been removed
}

void SmartMarket::reviewerCreateBarChart()
{
    // Stub: chart widgets have been removed
}

void SmartMarket::reviewerCreateLineChart()
{
    // Stub: chart widgets have been removed
}

void SmartMarket::reviewerCreatePieChart()
{
    // Stub: chart widgets have been removed
}

void SmartMarket::reviewerSetupNavigation()
{
    // Stub: navigation widgets have been removed
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
    // Stub: login page has been removed
}

void SmartMarket::on_btnLoginEnvoyer_clicked()
{
    // Stub: login page has been removed
}

// ============================================================
// MODULE PUBLICATION - CRUD
// ============================================================

void SmartMarket::on_pushButton_19_clicked()
{
    // Stub: publication widgets have been removed
}

void SmartMarket::on_pushButton_20_clicked()
{
    // Stub: publication widgets have been removed
}

void SmartMarket::on_pushButton_8_clicked()
{
    // Stub: publication widgets have been removed
}

void SmartMarket::on_pushButton_9_clicked()
{
    // Stub: publication widgets have been removed
}

void SmartMarket::on_pushButton_5_clicked()
{
    // Stub: publication widgets have been removed
}

void SmartMarket::on_pushButton_21_clicked()
{
    // Stub: publication widgets have been removed
}

// ============================================================
// MODULE PUBLICATION - ANALYSE
// ============================================================

void SmartMarket::on_pushButton_6_clicked()
{
    // Stub: publication analysis widgets have been removed
}

void SmartMarket::on_pushButton_7_clicked()
{
    // Stub: publication analysis widgets have been removed
}

// ============================================================
// SIDEBAR - NAVIGATION ENTRE MODULES
// ============================================================

void SmartMarket::on_pushButton_14_clicked()
{
    if (conferencePageIndex >= 0)
        ui->stackedWidgetMain->setCurrentIndex(conferencePageIndex);
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
            "Le module Reviewers n'est pas disponible.");
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
            "Le module Conférences n'est pas disponible.");
    }
}

void SmartMarket::on_pushButton_18_clicked()
{
    QMessageBox::information(this, "Module Équipements", 
        "Le module Équipements sera intégré prochainement.");
}
