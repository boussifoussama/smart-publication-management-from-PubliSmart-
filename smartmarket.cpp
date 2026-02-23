#include "smartmarket.h"
#include "ui_smartmarket.h"

#include <QHeaderView>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>

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
    const QStringList conferences = {"Conf A", "Conf B", "Conf C", "Conf D"};

    if (ui->conf_graphicsView1)
    {
        QGraphicsScene *scene1 = new QGraphicsScene(this);
        ui->conf_graphicsView1->setScene(scene1);
        scene1->setSceneRect(0, 0, 500, 300);

        QPen axisPen(Qt::black);
        axisPen.setWidth(2);
        scene1->addLine(50, 20, 50, 260, axisPen);
        scene1->addLine(50, 260, 480, 260, axisPen);

        QList<int> participants = {120, 80, 150, 100};
        const int barWidth = 40;
        const int spacing = 35;
        const int startX = 90;
        const int baseY = 260;
        QBrush barBrush(Qt::blue);

        for (int i = 0; i < participants.size() && i < conferences.size(); ++i)
        {
            int barHeight = participants[i];

            scene1->addRect(startX + i * (barWidth + spacing),
                            baseY - barHeight,
                            barWidth,
                            barHeight,
                            QPen(Qt::NoPen),
                            barBrush);

            scene1->addText(QString::number(participants[i]))
                ->setPos(startX + i * (barWidth + spacing),
                         baseY - barHeight - 20);

            scene1->addText(conferences[i])
                ->setPos(startX + i * (barWidth + spacing) - 5,
                         baseY + 5);
        }

        scene1->addText("Participants per Conference")->setPos(150, 0);
    }

    if (ui->conf_graphicsView1_2)
    {
        QGraphicsScene *scene2 = new QGraphicsScene(this);
        ui->conf_graphicsView1_2->setScene(scene2);
        scene2->setSceneRect(0, 0, 500, 300);

        QPen axisPen(Qt::black);
        axisPen.setWidth(2);
        scene2->addLine(50, 20, 50, 260, axisPen);
        scene2->addLine(50, 260, 480, 260, axisPen);

        QList<int> publications = {10, 6, 14, 9};
        const int barWidth = 40;
        const int spacing = 35;
        const int startX = 90;
        const int baseY = 260;
        QBrush barBrush2(Qt::darkGreen);

        for (int i = 0; i < publications.size() && i < conferences.size(); ++i)
        {
            int barHeight = publications[i] * 10; // mise à l'échelle

            scene2->addRect(startX + i * (barWidth + spacing),
                            baseY - barHeight,
                            barWidth,
                            barHeight,
                            QPen(Qt::NoPen),
                            barBrush2);

            scene2->addText(QString::number(publications[i]))
                ->setPos(startX + i * (barWidth + spacing),
                         baseY - barHeight - 20);

            scene2->addText(conferences[i])
                ->setPos(startX + i * (barWidth + spacing) - 5,
                         baseY + 5);
        }

        scene2->addText("Publications per Conference")->setPos(150, 0);
    }
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

    setupConferenceCharts();
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
