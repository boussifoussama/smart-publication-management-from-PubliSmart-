#include "oussama.h"
#include "ui_oussama.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>

OussamaWindow::OussamaWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::OussamaWindow)
{
    ui->setupUi(this);

    // ================= GRAPHICS VIEW 1 =================
    // Participants per conference
    QGraphicsScene *scene1 = new QGraphicsScene(this);
    ui->graphicsView1->setScene(scene1);
    scene1->setSceneRect(0, 0, 500, 300);

    // Axes
    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    scene1->addLine(50, 20, 50, 260, axisPen);
    scene1->addLine(50, 260, 480, 260, axisPen);

    // Data
    QString conferences[] = { "Conf A", "Conf B", "Conf C", "Conf D" };
    int participants[] = { 120, 80, 150, 100 };
    int count = 4;

    int barWidth = 40;
    int spacing = 35;
    int startX = 90;
    int baseY = 260;

    QBrush barBrush(Qt::blue);

    for (int i = 0; i < count; i++)
    {
        int barHeight = participants[i];

        // Bar
        scene1->addRect(
            startX + i * (barWidth + spacing),
            baseY - barHeight,
            barWidth,
            barHeight,
            QPen(Qt::NoPen),
            barBrush
            );

        // Participant number above bar
        scene1->addText(QString::number(participants[i]))
            ->setPos(startX + i * (barWidth + spacing),
                     baseY - barHeight - 20);

        // Conference label below bar
        scene1->addText(conferences[i])
            ->setPos(startX + i * (barWidth + spacing) - 5,
                     baseY + 5);
    }

    // Title
    scene1->addText("Participants per Conference")
        ->setPos(150, 0);


    // ================= GRAPHICS VIEW 1_2 =================
    // Publications per conference
    QGraphicsScene *scene2 = new QGraphicsScene(this);
    ui->graphicsView1_2->setScene(scene2);
    scene2->setSceneRect(0, 0, 500, 300);

    // Axes
    scene2->addLine(50, 20, 50, 260, axisPen);
    scene2->addLine(50, 260, 480, 260, axisPen);

    // Data
    int publications[] = { 10, 6, 14, 9 };

    QBrush barBrush2(Qt::darkGreen);

    for (int i = 0; i < count; i++)
    {
        int barHeight = publications[i] * 10; // scaled for visibility

        // Bar
        scene2->addRect(
            startX + i * (barWidth + spacing),
            baseY - barHeight,
            barWidth,
            barHeight,
            QPen(Qt::NoPen),
            barBrush2
            );

        // Publication number above bar
        scene2->addText(QString::number(publications[i]))
            ->setPos(startX + i * (barWidth + spacing),
                     baseY - barHeight - 20);

        // Conference label below bar
        scene2->addText(conferences[i])
            ->setPos(startX + i * (barWidth + spacing) - 5,
                     baseY + 5);
    }

    // Title
    scene2->addText("Publications per Conference")
        ->setPos(150, 0);
}

OussamaWindow::~OussamaWindow()
{
    delete ui;
}
