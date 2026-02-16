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
    // Setup internal Reviewers page navigation
    // The Reviewers section has 2 pages in stackedWidget:
    // - Page 1 (stackedWidgetPage1): Reviewer List with btnList and btnStatistics
    // - Page 2 (page): Statistics view with btnList_2 and btnStatistics_2

    // Lambda to manage button states when switching pages
    auto updateReviewersPageState = [this](int pageIndex) {
        bool isPage1 = ( pageIndex == 1);
        bool isPage2 = (pageIndex == 2);
        ui->btnList->setChecked(isPage1);
        ui->btnStatistics->setChecked(isPage2);
        ui->btnList_2->setChecked(isPage1);
        ui->btnStatistics_2->setChecked(isPage2);
    };

    // Setup page 1 buttons (List and Statistics)
    connect(ui->btnList, &QPushButton::clicked,
            this, [=]() { 
                ui->stackedWidget->setCurrentIndex(1); 
                updateReviewersPageState(1);
            });
    connect(ui->btnStatistics, &QPushButton::clicked,
            this, [=]() { 
                ui->stackedWidget->setCurrentIndex(2); 
                updateReviewersPageState(2);
            });

    // Setup page 2 buttons (mirror of page 1)
    connect(ui->btnList_2, &QPushButton::clicked,
            this, [=]() { 
                ui->stackedWidget->setCurrentIndex(1); 
                updateReviewersPageState(1);
            });
    connect(ui->btnStatistics_2, &QPushButton::clicked,
            this, [=]() { 
                ui->stackedWidget->setCurrentIndex(2); 
                updateReviewersPageState(2);
            });

    // Make buttons checkable for visual feedback
    ui->btnList->setCheckable(true);
    ui->btnStatistics->setCheckable(true);
    ui->btnList_2->setCheckable(true);
    ui->btnStatistics_2->setCheckable(true);

    // Start on Reviewer List page
    ui->stackedWidget->setCurrentIndex(1);
    updateReviewersPageState(1);
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
