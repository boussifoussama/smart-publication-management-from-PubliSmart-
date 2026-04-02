#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>

// forward declarations kept for other QtCharts classes if needed
namespace QtCharts {
    class QPieSlice; // example
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    // helpers used internally by this window
    void updateKPIs();
    void createBarChart();
    void createLineChart();
    void createPieChart();

    // navigation support (buttons on left, stacked widget for pages)
    void setupNavigation();
};

#endif // MAINWINDOW_H