#ifndef OUSSAMA_H
#define OUSSAMA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class OussamaWindow;
}
QT_END_NAMESPACE

class OussamaWindow : public QMainWindow
{
    Q_OBJECT

public:
    OussamaWindow(QWidget *parent = nullptr);
    ~OussamaWindow();

private:
    Ui::OussamaWindow *ui;
};
#endif // OUSSAMA_H
