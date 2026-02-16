#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QVBoxLayout>
#include <QString>
#include <QVector>
#include <QDebug>

// dummy data for demonstration; replace with real model later
struct Reviewer {
    QString name;
    int evaluationCount;
    float reliability; // 0–100
};

static QVector<Reviewer> reviewers = {
    { "John Doe",  10, 95 },
    { "Jane Smith",12, 88 },
    { "Ali Ben",    8, 82 },
    { "Sara Lee",   5, 90 }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    updateKPIs();          // fill the four KPI QLabel widgets
    createBarChart();      // left chartFrameBar
    createLineChart();     // right chartFrameLine
    createPieChart();      // bottom chartFramePie
    setupNavigation();      // wire up left‑side buttons to the stacked pages
}

MainWindow::~MainWindow()
{
    delete ui;
}

// -----------------------------------------------------
// navigation helper
// -----------------------------------------------------
void MainWindow::setupNavigation()
{
    qDebug() << "setupNavigation() called";
    qDebug() << "btnList:" << ui->btnList;
    qDebug() << "btnStatistics:" << ui->btnStatistics;
    qDebug() << "btnList_2:" << ui->btnList_2;
    qDebug() << "btnStatistics_2:" << ui->btnStatistics_2;
    qDebug() << "stackedWidget:" << ui->stackedWidget;
    const int listIndex = ui->stackedWidget->indexOf(ui->stackedWidgetPage1);
    const int statsIndex = ui->stackedWidget->indexOf(ui->page);
    qDebug() << "listIndex:" << listIndex << "statsIndex:" << statsIndex;
    
    // Setup internal Reviewers page navigation
    // - Page 1: Statistics (has btnList_2 and btnStatistics_2)
    // - Page 2: List (has btnList and btnStatistics)

    // Lambda to manage button states
    auto updateButtonStates = [this, listIndex, statsIndex](int pageIndex) {
        bool isListPage = (pageIndex == listIndex);
        bool isStatsPage = (pageIndex == statsIndex);
        // Page 1 buttons
        ui->btnList_2->setChecked(isListPage);         // List button
        ui->btnStatistics_2->setChecked(isStatsPage);  // Statistics button
        // Page 2 buttons
        ui->btnList->setChecked(isListPage);           // List button
        ui->btnStatistics->setChecked(isStatsPage);    // Statistics button
    };

    // Page 1 buttons (btnList_2, btnStatistics_2) - on Statistics page
    connect(ui->btnList_2, &QPushButton::clicked,
            this, [=]() { 
                qDebug() << "btnList_2 clicked - switching to page 2 (List)";
                ui->stackedWidget->setCurrentIndex(listIndex); 
                updateButtonStates(listIndex);
            });
    connect(ui->btnStatistics_2, &QPushButton::clicked,
            this, [=]() { 
                qDebug() << "btnStatistics_2 clicked - staying on page 1 (Statistics)";
                ui->stackedWidget->setCurrentIndex(statsIndex); 
                updateButtonStates(statsIndex);
            });

    // Page 2 buttons (btnList, btnStatistics) - on List page
    connect(ui->btnList, &QPushButton::clicked,
            this, [=]() { 
                qDebug() << "btnList clicked - staying on page 2 (List)";
                ui->stackedWidget->setCurrentIndex(listIndex); 
                updateButtonStates(listIndex);
            });
    connect(ui->btnStatistics, &QPushButton::clicked,
            this, [=]() { 
                qDebug() << "btnStatistics clicked - switching to page 1 (Statistics)";
                ui->stackedWidget->setCurrentIndex(statsIndex); 
                updateButtonStates(statsIndex);
            });

    // Make buttons checkable for visual feedback
    ui->btnList->setCheckable(true);
    ui->btnStatistics->setCheckable(true);
    ui->btnList_2->setCheckable(true);
    ui->btnStatistics_2->setCheckable(true);

    // Start on Statistics page
    ui->stackedWidget->setCurrentIndex(statsIndex);
    updateButtonStates(statsIndex);
}

void MainWindow::updateKPIs()
{
    int total     = reviewers.size();
    float avgRel  = 0;
    int maxEvals  = 0;
    QString topName, mostEvalsName;
    float topRel  = 0;

    for (auto &r : reviewers) {
        avgRel += r.reliability;
        if (r.evaluationCount > maxEvals) {
            maxEvals       = r.evaluationCount;
            mostEvalsName  = r.name;
        }
        if (r.reliability > topRel) {
            topRel   = r.reliability;
            topName  = r.name;
        }
    }
    if (total) avgRel /= total;

    ui->kpiTotalReviewers->setText(QString::number(total));
    ui->kpiAvgReliability->setText(QString::number(avgRel) + "%");
    ui->kpiMostEvaluations->setText(mostEvalsName + " – " + QString::number(maxEvals));
    ui->kpiTopReviewer->setText(topName + " – " + QString::number(topRel) + "%");
}

void MainWindow::createBarChart()
{
    auto *set = new QBarSet("Evaluations");
    QStringList cats;
    for (auto &r : reviewers) {
        *set << r.evaluationCount;
        cats << r.name;
    }

    auto *series = new QBarSeries();
    series->append(set);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Number of Evaluations per Reviewer");
    chart->createDefaultAxes();
    // horizontal axis is a QBarCategoryAxis after createDefaultAxes()
    if (auto *axis = qobject_cast<QBarCategoryAxis*>(chart->axes(Qt::Horizontal).first())) {
        axis->append(cats);
    }

    auto *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    // remove any placeholder label left in the frame
    if (auto *lbl = ui->chartFrameBar->findChild<QLabel*>())
        lbl->deleteLater();

    auto *lay = new QVBoxLayout(ui->chartFrameBar);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    view->setParent(ui->chartFrameBar);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(view);
}

void MainWindow::createLineChart()
{
    auto *series = new QLineSeries();
    int x = 0;
    for (auto &r : reviewers)
        series->append(x++, r.evaluationCount);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Evaluation Trend");
    chart->createDefaultAxes();

    auto *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    if (auto *lbl = ui->chartFrameLine->findChild<QLabel*>())
        lbl->deleteLater();

    auto *lay = new QVBoxLayout(ui->chartFrameLine);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    view->setParent(ui->chartFrameLine);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(view);
}

void MainWindow::createPieChart()
{
    auto *series = new QPieSeries();
    int c90_100 = 0, c80_90 = 0, cBelow = 0;
    for (auto &r : reviewers) {
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

    if (auto *lbl = ui->chartFramePie->findChild<QLabel*>())
        lbl->deleteLater();

    auto *lay = new QVBoxLayout(ui->chartFramePie);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    view->setParent(ui->chartFramePie);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lay->addWidget(view);
}
