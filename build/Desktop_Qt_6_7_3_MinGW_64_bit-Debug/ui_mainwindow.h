/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QTableWidget *tableWidget_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QLineEdit *idsearch;
    QLabel *label_7;
    QLineEdit *lineEdit_5;
    QLabel *label_21;
    QComboBox *comboBox_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_8;
    QPushButton *pushButton_17;
    QPushButton *pushButton_18;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_27;
    QComboBox *comboBox_7;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_26;
    QComboBox *comboBox_6;
    QWidget *tab_2;
    QLabel *label_2;
    QGroupBox *groupBox_9;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QComboBox *comboBox_3;
    QDateEdit *dateEdit_3;
    QPushButton *pushButton_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *lineEdit_16;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_18;
    QLabel *label;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_30;
    QWidget *tab_3;
    QGroupBox *groupBox_5;
    QLabel *label_3;
    QComboBox *comboBox_2;
    QDateEdit *dateEdit;
    QDateEdit *dateEdit_2;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pushButton_5;
    QLabel *label_24;
    QWidget *widget_6;
    QProgressBar *progressBar;
    QProgressBar *progressBar_2;
    QProgressBar *progressBar_3;
    QProgressBar *progressBar_4;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QProgressBar *progressBar_5;
    QProgressBar *progressBar_6;
    QProgressBar *progressBar_7;
    QProgressBar *progressBar_8;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_25;
    QGroupBox *groupBox_11;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QLabel *label_29;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1298, 824);
        MainWindow->setStyleSheet(QString::fromUtf8("/* =====================================================\n"
"   TH\303\210ME GLOBAL \342\200\223 BLEU MARINE PREMIUM\n"
"   ===================================================== */\n"
"\n"
"/* ---------- TYPOGRAPHIE ---------- */\n"
"* {\n"
"    font-family: \"Inter\", \"Segoe UI\", \"Roboto\", \"Helvetica Neue\", Arial, sans-serif;\n"
"    font-size: 14px;\n"
"    color: #E5E7EB;\n"
"}\n"
"\n"
"/* ---------- FEN\303\212TRE PRINCIPALE ---------- */\n"
"QMainWindow {\n"
"    background-color: #0B1F33;\n"
"}\n"
"\n"
"/* ---------- BASE WIDGET ---------- */\n"
"QWidget {\n"
"    background-color: transparent;\n"
"}\n"
"\n"
"/* ---------- GROUPBOX / PANNEAUX ---------- */\n"
"QGroupBox {\n"
"    background-color: #0F2A44;\n"
"    border: 1px solid #1E3A8A;\n"
"    border-radius: 14px;\n"
"    margin-top: 22px;\n"
"    padding: 18px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 18px;\n"
"    font-size: 15px;\n"
"    font-weight: 800"
                        ";\n"
"    color: #FFFFFF;\n"
"}\n"
"\n"
"/* ---------- LABELS ---------- */\n"
"QLabel {\n"
"    color: #E5E7EB;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"/* ---------- TAB WIDGET ---------- */\n"
"QTabWidget::pane {\n"
"    background-color: #0F2A44;\n"
"    border: 1px solid #1E3A8A;\n"
"    border-radius: 14px;\n"
"    margin-top: 10px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: #0B1F33;\n"
"    color: #CBD5E1;\n"
"    padding: 14px 32px;\n"
"    margin-right: 10px;\n"
"    border-radius: 12px;\n"
"    font-weight: 600;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #1E40AF;\n"
"    color: #FFFFFF;\n"
"    font-weight: 900;\n"
"}\n"
"\n"
"QTabBar::tab:hover:!selected {\n"
"    background-color: #163B63;\n"
"    color: #FFFFFF;\n"
"}\n"
"\n"
"/* ---------- INPUTS ---------- */\n"
"QLineEdit,\n"
"QComboBox,\n"
"QDateEdit,\n"
"QSpinBox,\n"
"QDoubleSpinBox {\n"
"    background-color: #0B1F33;\n"
"    border: 1px solid #1E3A8A;\n"
"    border-radius: 10px;\n"
"    padding: 12px;\n"
""
                        "    color: #FFFFFF;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"QLineEdit:focus,\n"
"QComboBox:focus,\n"
"QDateEdit:focus,\n"
"QSpinBox:focus,\n"
"QDoubleSpinBox:focus {\n"
"    border-color: #3B82F6;\n"
"    background-color: #0F2A44;\n"
"}\n"
"\n"
"/* ---------- BOUTONS ---------- */\n"
"QPushButton {\n"
"    background-color: #1E40AF;\n"
"    color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    padding: 14px 30px;\n"
"    font-weight: 800;\n"
"    letter-spacing: 0.4px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #2563EB;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1D4ED8;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #475569;\n"
"    color: #CBD5E1;\n"
"}\n"
"\n"
"/* ---------- TABLES ---------- */\n"
"QTableWidget {\n"
"    background-color: #0F2A44;\n"
"    border: 1px solid #1E3A8A;\n"
"    border-radius: 14px;\n"
"    gridline-color: #1E3A8A;\n"
"    alternate-background-color: #0B1F33;\n"
"    selection-background-color: #1E40AF;\n"
"}\n"
"\n"
"QTabl"
                        "eWidget::item {\n"
"    padding: 14px;\n"
"    color: #E5E7EB;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #0B1F33;\n"
"    color: #FFFFFF;\n"
"    padding: 14px;\n"
"    font-weight: 900;\n"
"    border: none;\n"
"}\n"
"\n"
"/* ---------- CHECKBOX / RADIO ---------- */\n"
"QCheckBox,\n"
"QRadioButton {\n"
"    color: #E5E7EB;\n"
"    font-weight: 600;\n"
"}\n"
"\n"
"QCheckBox::indicator,\n"
"QRadioButton::indicator {\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"/* ---------- PROGRESS BAR ---------- */\n"
"QProgressBar {\n"
"    background-color: #0B1F33;\n"
"    border: 1px solid #1E3A8A;\n"
"    border-radius: 12px;\n"
"    text-align: center;\n"
"    font-weight: 800;\n"
"    color: #FFFFFF;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #2563EB;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"/* ---------- MENU BAR ---------- */\n"
"QMenuBar {\n"
"    background-color: #071526;\n"
"    color: #FFFFFF;\n"
"    font-weight: 800;\n"
"}\n"
""
                        "\n"
"QMenuBar::item:selected {\n"
"    background-color: rgba(255, 255, 255, 0.15);\n"
"}\n"
"\n"
"/* ---------- STATUS BAR ---------- */\n"
"QStatusBar {\n"
"    background-color: #071526;\n"
"    color: #CBD5E1;\n"
"}\n"
"\n"
"/* ---------- SIDEBAR (si objectName = groupBox_11) ---------- */\n"
"#groupBox_11 {\n"
"    background-color: #071526;\n"
"    border: none;\n"
"}\n"
"\n"
"#groupBox_11 QPushButton {\n"
"    background-color: transparent;\n"
"    color: #E5E7EB;\n"
"    padding: 18px 24px;\n"
"    border-radius: 14px;\n"
"    font-size: 17px;\n"
"    font-weight: 700;\n"
"}\n"
"\n"
"#groupBox_11 QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.15);\n"
"}\n"
"\n"
"#pushButton_13 {\n"
"    background-color: rgba(255,255,255,0.25);\n"
"    font-weight: 900;\n"
"}\n"
"\n"
"\n"
"\n"
""));
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(210, 0, 1091, 721));
        tabWidget->setStyleSheet(QString::fromUtf8("/* =========================\n"
"   TYPOGRAPHIE PREMIUM\n"
"   ========================= */\n"
"* {\n"
"    font-family: \"Inter\", \"Segoe UI\", \"Roboto\", \"Helvetica Neue\", Arial, sans-serif;\n"
"    font-size: 14px;\n"
"    color: #374151;\n"
"}\n"
"\n"
"/* =========================\n"
"   FEN\303\212TRE PRINCIPALE\n"
"   ========================= */\n"
"QMainWindow {\n"
"    background-color: #F4F6F9;\n"
"}\n"
"\n"
"/* =========================\n"
"   TITRES & LABELS\n"
"   ========================= */\n"
"QLabel {\n"
"    font-weight: 500;\n"
"    color: #1F2937;\n"
"}\n"
"\n"
"QGroupBox {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    border: 1px solid #E5E7EB;\n"
"    margin-top: 24px;\n"
"    padding: 18px;\n"
"    font-weight: 600;\n"
"    color: #0F2A44;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 16px;\n"
"    font-size: 15px;\n"
"    font-weight: 700;\n"
"}\n"
"\n"
"/* =================="
                        "=======\n"
"   ONGLET (TAB BAR)\n"
"   ========================= */\n"
"QTabWidget::pane {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    border: 1px solid #E5E7EB;\n"
"    margin-top: 10px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background-color: #E5E7EB;\n"
"    border-radius: 10px;\n"
"    padding: 14px 28px;\n"
"    margin-right: 8px;\n"
"    font-weight: 500;\n"
"    color: #374151;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background-color: #0F2A44;\n"
"    color: #FFFFFF;\n"
"    font-weight: 700;\n"
"}\n"
"\n"
"QTabBar::tab:hover:!selected {\n"
"    background-color: #DBEAFE;\n"
"    color: #1E3A8A;\n"
"}\n"
"\n"
"/* =========================\n"
"   INPUTS (\303\211L\303\211GANTS)\n"
"   ========================= */\n"
"QLineEdit, QComboBox, QDateEdit {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #D1D5DB;\n"
"    padding: 12px;\n"
"    font-size: 14px;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QD"
                        "ateEdit:focus {\n"
"    border-color: #2563EB;\n"
"    background-color: #F0F9FF;\n"
"}\n"
"\n"
"/* =========================\n"
"   BOUTONS PREMIUM\n"
"   ========================= */\n"
"QPushButton {\n"
"    background-color: #0F2A44;\n"
"    color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    padding: 12px 26px;\n"
"    font-weight: 700;\n"
"    letter-spacing: 0.3px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #163B63;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #2563EB;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #9CA3AF;\n"
"    color: #E5E7EB;\n"
"}\n"
"\n"
"/* =========================\n"
"   TABLES (PRO DASHBOARD)\n"
"   ========================= */\n"
"QTableWidget {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 12px;\n"
"    border: 1px solid #E5E7EB;\n"
"    gridline-color: #E5E7EB;\n"
"    alternate-background-color: #F9FAFB;\n"
"    selection-background-color: #DBEAFE;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 14px;\n"
""
                        "    font-weight: 500;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #0F2A44;\n"
"    color: #FFFFFF;\n"
"    padding: 14px;\n"
"    font-weight: 700;\n"
"    border: none;\n"
"}\n"
"\n"
"/* =========================\n"
"   SIDEBAR MODERNE\n"
"   ========================= */\n"
"#groupBox_11 {\n"
"    background-color: #0F2A44;\n"
"    border: none;\n"
"}\n"
"\n"
"#groupBox_11 QPushButton {\n"
"    background-color: transparent;\n"
"    color: #FFFFFF;\n"
"    padding: 16px 22px;\n"
"    border-radius: 12px;\n"
"    font-size: 16px;\n"
"    font-weight: 600;\n"
"}\n"
"\n"
"#groupBox_11 QPushButton:hover {\n"
"    background-color: rgba(255,255,255,0.15);\n"
"}\n"
"\n"
"#pushButton_13 {\n"
"    background-color: rgba(255,255,255,0.25);\n"
"    font-weight: 800;\n"
"}\n"
"\n"
"/* =========================\n"
"   PROGRESS BAR\n"
"   ========================= */\n"
"QProgressBar {\n"
"    background-color: #FFFFFF;\n"
"    border-radius: 10px;\n"
"    border: 1px solid #D1D5DB;\n"
"    text-align:"
                        " center;\n"
"    font-weight: 700;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"    background-color: #2563EB;\n"
"    border-radius: 8px;\n"
"}\n"
"\n"
"/* =========================\n"
"   MENU & STATUS\n"
"   ========================= */\n"
"QMenuBar {\n"
"    background-color: #0F2A44;\n"
"    color: #FFFFFF;\n"
"    font-weight: 700;\n"
"}\n"
"\n"
"QStatusBar {\n"
"    background-color: #F4F6F9;\n"
"    color: #6B7280;\n"
"}\n"
""));
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        tableWidget_2 = new QTableWidget(tab_5);
        if (tableWidget_2->columnCount() < 8)
            tableWidget_2->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(0, 80, 1011, 391));
        widget_2 = new QWidget(tab_5);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(30, 20, 777, 69));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        label_6 = new QLabel(widget_2);
        label_6->setObjectName("label_6");

        horizontalLayout->addWidget(label_6);

        idsearch = new QLineEdit(widget_2);
        idsearch->setObjectName("idsearch");

        horizontalLayout->addWidget(idsearch);

        label_7 = new QLabel(widget_2);
        label_7->setObjectName("label_7");

        horizontalLayout->addWidget(label_7);

        lineEdit_5 = new QLineEdit(widget_2);
        lineEdit_5->setObjectName("lineEdit_5");

        horizontalLayout->addWidget(lineEdit_5);

        label_21 = new QLabel(widget_2);
        label_21->setObjectName("label_21");

        horizontalLayout->addWidget(label_21);

        comboBox_5 = new QComboBox(widget_2);
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");

        horizontalLayout->addWidget(comboBox_5);

        pushButton_6 = new QPushButton(widget_2);
        pushButton_6->setObjectName("pushButton_6");

        horizontalLayout->addWidget(pushButton_6);

        pushButton = new QPushButton(widget_2);
        pushButton->setObjectName("pushButton");

        horizontalLayout->addWidget(pushButton);

        widget = new QWidget(tab_5);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(330, 500, 497, 65));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pushButton_8 = new QPushButton(widget);
        pushButton_8->setObjectName("pushButton_8");

        horizontalLayout_2->addWidget(pushButton_8);

        pushButton_17 = new QPushButton(widget);
        pushButton_17->setObjectName("pushButton_17");

        horizontalLayout_2->addWidget(pushButton_17);

        pushButton_18 = new QPushButton(tab_5);
        pushButton_18->setObjectName("pushButton_18");
        pushButton_18->setGeometry(QRect(880, 590, 154, 43));
        widget_3 = new QWidget(tab_5);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(0, 460, 301, 151));
        verticalLayout = new QVBoxLayout(widget_3);
        verticalLayout->setObjectName("verticalLayout");
        widget_4 = new QWidget(widget_3);
        widget_4->setObjectName("widget_4");
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_27 = new QLabel(widget_4);
        label_27->setObjectName("label_27");

        horizontalLayout_3->addWidget(label_27);

        comboBox_7 = new QComboBox(widget_4);
        comboBox_7->addItem(QString());
        comboBox_7->addItem(QString());
        comboBox_7->setObjectName("comboBox_7");

        horizontalLayout_3->addWidget(comboBox_7);


        verticalLayout->addWidget(widget_4);

        widget_5 = new QWidget(widget_3);
        widget_5->setObjectName("widget_5");
        horizontalLayout_4 = new QHBoxLayout(widget_5);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_26 = new QLabel(widget_5);
        label_26->setObjectName("label_26");

        horizontalLayout_4->addWidget(label_26);

        comboBox_6 = new QComboBox(widget_5);
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->setObjectName("comboBox_6");

        horizontalLayout_4->addWidget(comboBox_6);


        verticalLayout->addWidget(widget_5);

        tabWidget->addTab(tab_5, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 100, 63, 20));
        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName("groupBox_9");
        groupBox_9->setGeometry(QRect(10, 10, 991, 611));
        lineEdit_9 = new QLineEdit(groupBox_9);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(180, 40, 301, 26));
        lineEdit_10 = new QLineEdit(groupBox_9);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(180, 90, 301, 26));
        lineEdit_11 = new QLineEdit(groupBox_9);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(180, 140, 301, 26));
        comboBox_3 = new QComboBox(groupBox_9);
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(180, 190, 171, 26));
        dateEdit_3 = new QDateEdit(groupBox_9);
        dateEdit_3->setObjectName("dateEdit_3");
        dateEdit_3->setGeometry(QRect(180, 240, 110, 26));
        pushButton_9 = new QPushButton(groupBox_9);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(20, 410, 291, 41));
        label_10 = new QLabel(groupBox_9);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(20, 40, 63, 20));
        label_11 = new QLabel(groupBox_9);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(20, 90, 63, 20));
        label_12 = new QLabel(groupBox_9);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(20, 190, 71, 20));
        label_13 = new QLabel(groupBox_9);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(20, 140, 111, 20));
        label_14 = new QLabel(groupBox_9);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(20, 240, 151, 20));
        lineEdit_16 = new QLineEdit(groupBox_9);
        lineEdit_16->setObjectName("lineEdit_16");
        lineEdit_16->setGeometry(QRect(180, 280, 301, 26));
        lineEdit_17 = new QLineEdit(groupBox_9);
        lineEdit_17->setObjectName("lineEdit_17");
        lineEdit_17->setGeometry(QRect(180, 330, 301, 26));
        lineEdit_18 = new QLineEdit(groupBox_9);
        lineEdit_18->setObjectName("lineEdit_18");
        lineEdit_18->setGeometry(QRect(180, 370, 301, 26));
        label = new QLabel(groupBox_9);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 280, 91, 20));
        label_8 = new QLabel(groupBox_9);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(20, 330, 63, 31));
        label_9 = new QLabel(groupBox_9);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 370, 63, 20));
        label_30 = new QLabel(groupBox_9);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(630, 330, 91, 71));
        label_30->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/Downloads/publication icon.png);"));
        label_30->setPixmap(QPixmap(QString::fromUtf8(":/ressouces/publication icon.png")));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        groupBox_5 = new QGroupBox(tab_3);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(10, 10, 391, 181));
        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 30, 71, 20));
        comboBox_2 = new QComboBox(groupBox_5);
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(90, 30, 191, 26));
        dateEdit = new QDateEdit(groupBox_5);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(120, 60, 110, 26));
        dateEdit_2 = new QDateEdit(groupBox_5);
        dateEdit_2->setObjectName("dateEdit_2");
        dateEdit_2->setGeometry(QRect(120, 90, 110, 26));
        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 60, 121, 20));
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 90, 63, 20));
        pushButton_5 = new QPushButton(groupBox_5);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(272, 128, 111, 41));
        label_24 = new QLabel(groupBox_5);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(20, 0, 171, 20));
        widget_6 = new QWidget(tab_3);
        widget_6->setObjectName("widget_6");
        widget_6->setGeometry(QRect(400, 0, 681, 271));
        progressBar = new QProgressBar(widget_6);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(220, 30, 461, 23));
        progressBar->setValue(24);
        progressBar_2 = new QProgressBar(widget_6);
        progressBar_2->setObjectName("progressBar_2");
        progressBar_2->setGeometry(QRect(220, 80, 461, 23));
        progressBar_2->setValue(67);
        progressBar_3 = new QProgressBar(widget_6);
        progressBar_3->setObjectName("progressBar_3");
        progressBar_3->setGeometry(QRect(220, 180, 461, 23));
        progressBar_3->setValue(40);
        progressBar_4 = new QProgressBar(widget_6);
        progressBar_4->setObjectName("progressBar_4");
        progressBar_4->setGeometry(QRect(220, 130, 461, 23));
        progressBar_4->setValue(60);
        label_15 = new QLabel(widget_6);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 30, 141, 20));
        label_16 = new QLabel(widget_6);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(10, 80, 121, 20));
        label_17 = new QLabel(widget_6);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(10, 130, 151, 20));
        label_18 = new QLabel(widget_6);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(10, 180, 241, 20));
        progressBar_5 = new QProgressBar(tab_3);
        progressBar_5->setObjectName("progressBar_5");
        progressBar_5->setGeometry(QRect(150, 350, 771, 23));
        progressBar_5->setValue(52);
        progressBar_6 = new QProgressBar(tab_3);
        progressBar_6->setObjectName("progressBar_6");
        progressBar_6->setGeometry(QRect(150, 400, 771, 23));
        progressBar_6->setValue(24);
        progressBar_7 = new QProgressBar(tab_3);
        progressBar_7->setObjectName("progressBar_7");
        progressBar_7->setGeometry(QRect(150, 450, 771, 23));
        progressBar_7->setValue(34);
        progressBar_8 = new QProgressBar(tab_3);
        progressBar_8->setObjectName("progressBar_8");
        progressBar_8->setGeometry(QRect(150, 500, 771, 23));
        progressBar_8->setValue(48);
        progressBar_8->setOrientation(Qt::Orientation::Horizontal);
        label_19 = new QLabel(tab_3);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(32, 350, 141, 20));
        label_20 = new QLabel(tab_3);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(32, 400, 141, 20));
        label_22 = new QLabel(tab_3);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(20, 450, 131, 20));
        label_23 = new QLabel(tab_3);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(32, 500, 141, 20));
        label_25 = new QLabel(tab_3);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(360, 300, 171, 20));
        label_25->setMaximumSize(QSize(171, 16777215));
        QFont font;
        font.setFamilies({QString::fromUtf8("Inter")});
        font.setWeight(QFont::Medium);
        label_25->setFont(font);
        label_25->setStyleSheet(QString::fromUtf8(""));
        tabWidget->addTab(tab_3, QString());
        groupBox_11 = new QGroupBox(centralwidget);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setGeometry(QRect(0, -40, 211, 781));
        pushButton_13 = new QPushButton(groupBox_11);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(20, 330, 161, 51));
        pushButton_14 = new QPushButton(groupBox_11);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(20, 450, 161, 51));
        pushButton_15 = new QPushButton(groupBox_11);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(20, 580, 161, 51));
        pushButton_16 = new QPushButton(groupBox_11);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(20, 700, 161, 61));
        label_29 = new QLabel(groupBox_11);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(-110, 150, 291, 171));
        label_29->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/imagetouse.png-removebg-preview.png);"));
        label_31 = new QLabel(groupBox_11);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(50, 370, 101, 91));
        label_31->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/Downloads/reviewer icon.png);"));
        label_32 = new QLabel(groupBox_11);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(20, 500, 151, 101));
        label_32->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/Downloads/conference icon.png);"));
        label_33 = new QLabel(groupBox_11);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(20, 620, 151, 101));
        label_33->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/Downloads/equipement icon.png);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1298, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Affiliation", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Nb \303\211valuations", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Score Fiabilit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "ID pub", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Id:", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Sp\303\251ciali\303\251", nullptr));
        comboBox_5->setItemText(0, QCoreApplication::translate("MainWindow", "AI", nullptr));
        comboBox_5->setItemText(1, QCoreApplication::translate("MainWindow", "CS", nullptr));
        comboBox_5->setItemText(2, QCoreApplication::translate("MainWindow", "WD", nullptr));

        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        pushButton_17->setText(QCoreApplication::translate("MainWindow", "Suprrimer", nullptr));
        pushButton_18->setText(QCoreApplication::translate("MainWindow", "Exporter", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "Tri par score Fia", nullptr));
        comboBox_7->setItemText(0, QCoreApplication::translate("MainWindow", "Croissant-decroissant", nullptr));
        comboBox_7->setItemText(1, QCoreApplication::translate("MainWindow", "Decroissant-croissant", nullptr));

        label_26->setText(QCoreApplication::translate("MainWindow", "Tri par Nb eva", nullptr));
        comboBox_6->setItemText(0, QCoreApplication::translate("MainWindow", "Croissant-decroissant", nullptr));
        comboBox_6->setItemText(1, QCoreApplication::translate("MainWindow", "Decroissant-croissant", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Liste des reviewers", nullptr));
        label_2->setText(QString());
        groupBox_9->setTitle(QCoreApplication::translate("MainWindow", "ajout de publication", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "svt", nullptr));

        pushButton_9->setText(QCoreApplication::translate("MainWindow", "cliquez ici pour ajouter ", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "ID:", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Email:", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Affiliation", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Nb Evaluations", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Score Fiabilit\303\251\n"
"", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "ID pub", nullptr));
        label_30->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Ajouter des reviewers", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "filtres", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Specialit\303\251", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "svt", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "Date debut:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Date fin:", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "g\303\251n\303\251rer", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Statistiques par Sp\303\251cialit\303\251", nullptr));
        progressBar->setFormat(QString());
        progressBar_2->setFormat(QString());
        progressBar_4->setFormat(QString());
        label_15->setText(QCoreApplication::translate("MainWindow", "Nombre de reviewers", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Total \303\251valuations", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Score moyen fiabilit\303\251", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Pourcentage par rapport au total", nullptr));
        progressBar_5->setFormat(QString());
        progressBar_6->setFormat(QString());
        progressBar_7->setFormat(QString());
        progressBar_8->setFormat(QCoreApplication::translate("MainWindow", "%p%", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Total Reviewers", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Total \303\251valuations", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "Score moyen fiabilit\303\251", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Reviewer actif (%)", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">Statistiques Globales</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Statistiques de reviewers", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("MainWindow", "Official desktop app", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Publications", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "Reviewers", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Conf\303\251rences", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "Equipements", nullptr));
        label_29->setText(QString());
        label_31->setText(QString());
        label_32->setText(QString());
        label_33->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
