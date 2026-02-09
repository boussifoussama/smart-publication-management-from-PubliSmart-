#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHeaderView>
#include <QStandardItem>
#include <QVBoxLayout>

// Qt Charts
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // ================== TABLEAU ==================
    model->setRowCount(4);
    model->setColumnCount(5);

    model->setHeaderData(0, Qt::Horizontal, "Domaine");
    model->setHeaderData(1, Qt::Horizontal, "Titre");
    model->setHeaderData(2, Qt::Horizontal, "Source");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Statut");

    model->setItem(0, 0, new QStandardItem("SVT"));
    model->setItem(0, 1, new QStandardItem("Molécules"));
    model->setItem(0, 2, new QStandardItem("Journal"));
    model->setItem(0, 3, new QStandardItem("01/11/2000"));
    model->setItem(0, 4, new QStandardItem("Évaluée"));

    model->setItem(1, 0, new QStandardItem("Informatique"));
    model->setItem(1, 1, new QStandardItem("IA"));
    model->setItem(1, 2, new QStandardItem("Magazine"));
    model->setItem(1, 3, new QStandardItem("01/09/2009"));
    model->setItem(1, 4, new QStandardItem("Non évaluée"));

    model->setItem(2, 0, new QStandardItem("Informatique"));
    model->setItem(2, 1, new QStandardItem("Deep Learning"));
    model->setItem(2, 2, new QStandardItem("Magazine of Science"));
    model->setItem(2, 3, new QStandardItem("06/09/2005"));
    model->setItem(2, 4, new QStandardItem("Rejetée"));

    model->setItem(3, 0, new QStandardItem("Physique"));
    model->setItem(3, 1, new QStandardItem("Atomes"));
    model->setItem(3, 2, new QStandardItem("Journal Parisien"));
    model->setItem(3, 3, new QStandardItem("06/07/2008"));
    model->setItem(3, 4, new QStandardItem("Rejetée"));

    // Connecter les deux vues
    ui->tableView->setModel(model);
    ui->tableView_2->setModel(model);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ================== PIE CHART STATUT ==================
    int eval = 0, nonEval = 0, rejet = 0;
    for (int row = 0; row < model->rowCount(); ++row) {
        QString statut = model->item(row, 4)->text();
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
    pieLayout->addWidget(pieChartView);

    // ================== BAR CHART DOMAINES ==================
    QStringList domaines = {"SVT", "Informatique", "Physique", "Math", "Chimie", "Biologie"};
    QMap<QString, int> domainCounts;
    for (const QString &d : domaines) domainCounts[d] = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        QString domaine = model->item(row, 0)->text();
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

    // Axe X (Catégories)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(domaines);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    // Axe Y (Valeurs)
    QValueAxis *axisY = new QValueAxis();
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *barLayout = new QVBoxLayout(ui->domainChartWidget);
    barLayout->addWidget(barChartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}




