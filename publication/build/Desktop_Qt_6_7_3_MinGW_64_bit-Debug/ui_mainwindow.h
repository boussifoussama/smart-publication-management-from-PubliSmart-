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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QGroupBox *groupBox_8;
    QLineEdit *lineEdit_8;
    QPushButton *pushButton_8;
    QLabel *label_21;
    QLabel *label_5;
    QTableView *tableView;
    QWidget *tab_2;
    QLabel *label_2;
    QGroupBox *groupBox_9;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QComboBox *comboBox_3;
    QDateEdit *dateEdit_3;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QGroupBox *groupBox_10;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QLineEdit *lineEdit_12;
    QLineEdit *lineEdit_13;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QComboBox *comboBox_4;
    QDateEdit *dateEdit_4;
    QLabel *label_3;
    QLabel *label_4;
    QWidget *tab_3;
    QWidget *chartWidget;
    QWidget *domainChartWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QComboBox *comboBox;
    QPushButton *pushButton_3;
    QLineEdit *lineEdit_4;
    QLabel *label_22;
    QLabel *label_23;
    QLabel *label_24;
    QLabel *label_25;
    QLineEdit *lineEdit_3;
    QGroupBox *groupBox_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_3;
    QGroupBox *groupBox_3;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_4;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_4;
    QLabel *label_8;
    QLabel *label_9;
    QTableWidget *tableWidget_6;
    QTableView *tableView_2;
    QWidget *tab_4;
    QGroupBox *groupBox_6;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton_6;
    QLabel *label_26;
    QLabel *label_27;
    QLineEdit *lineEdit_5;
    QTableWidget *tableWidget_4;
    QProgressBar *progressBar;
    QTableWidget *tableWidget_5;
    QGroupBox *groupBox_7;
    QPushButton *pushButton_7;
    QLabel *label_28;
    QLineEdit *lineEdit_7;
    QProgressBar *progressBar_2;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_34;
    QGroupBox *groupBox_11;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1298, 768);
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(210, 10, 1091, 711));
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
        groupBox_8 = new QGroupBox(tab_5);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(650, -10, 441, 221));
        lineEdit_8 = new QLineEdit(groupBox_8);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(70, 110, 291, 26));
        lineEdit_8->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
""));
        lineEdit_8->setDragEnabled(false);
        pushButton_8 = new QPushButton(groupBox_8);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(160, 150, 121, 41));
        label_21 = new QLabel(groupBox_8);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(70, 60, 291, 20));
        label_5 = new QLabel(groupBox_8);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(120, 30, 191, 20));
        label_5->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        tableView = new QTableView(tab_5);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(10, 10, 611, 621));
        tabWidget->addTab(tab_5, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 100, 63, 20));
        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName("groupBox_9");
        groupBox_9->setGeometry(QRect(10, 20, 521, 601));
        lineEdit_9 = new QLineEdit(groupBox_9);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(180, 40, 301, 26));
        lineEdit_9->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
""));
        lineEdit_10 = new QLineEdit(groupBox_9);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(180, 90, 301, 26));
        lineEdit_10->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        lineEdit_11 = new QLineEdit(groupBox_9);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(180, 140, 301, 26));
        lineEdit_11->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        comboBox_3 = new QComboBox(groupBox_9);
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(180, 190, 171, 26));
        comboBox_3->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: white;\n"
"    color: black;\n"
"    selection-background-color: #0078d7;\n"
"    selection-color: white;\n"
"}\n"
""));
        dateEdit_3 = new QDateEdit(groupBox_9);
        dateEdit_3->setObjectName("dateEdit_3");
        dateEdit_3->setGeometry(QRect(180, 240, 110, 26));
        dateEdit_3->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QDateEdit::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 20px;\n"
"    border-left: 1px solid #555;\n"
"}\n"
"\n"
"QDateEdit QAbstractItemView {\n"
"    background-color: white;\n"
"    color: black;\n"
"    selection-background-color: #0078d7;\n"
"    selection-color: white;\n"
"}\n"
""));
        pushButton_9 = new QPushButton(groupBox_9);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(30, 290, 291, 41));
        pushButton_10 = new QPushButton(groupBox_9);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(392, 560, 111, 41));
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
        groupBox_10 = new QGroupBox(tab_2);
        groupBox_10->setObjectName("groupBox_10");
        groupBox_10->setGeometry(QRect(559, 20, 521, 601));
        label_15 = new QLabel(groupBox_10);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(20, 40, 221, 20));
        label_16 = new QLabel(groupBox_10);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(20, 90, 63, 20));
        label_17 = new QLabel(groupBox_10);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(20, 140, 101, 20));
        label_18 = new QLabel(groupBox_10);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(20, 190, 63, 20));
        label_19 = new QLabel(groupBox_10);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(20, 240, 81, 20));
        label_20 = new QLabel(groupBox_10);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(20, 290, 141, 20));
        pushButton_11 = new QPushButton(groupBox_10);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(20, 340, 301, 41));
        pushButton_12 = new QPushButton(groupBox_10);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(392, 558, 111, 41));
        lineEdit_12 = new QLineEdit(groupBox_10);
        lineEdit_12->setObjectName("lineEdit_12");
        lineEdit_12->setGeometry(QRect(160, 90, 301, 26));
        lineEdit_12->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        lineEdit_13 = new QLineEdit(groupBox_10);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(320, 40, 181, 26));
        lineEdit_13->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        lineEdit_14 = new QLineEdit(groupBox_10);
        lineEdit_14->setObjectName("lineEdit_14");
        lineEdit_14->setGeometry(QRect(160, 140, 301, 26));
        lineEdit_14->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        lineEdit_15 = new QLineEdit(groupBox_10);
        lineEdit_15->setObjectName("lineEdit_15");
        lineEdit_15->setGeometry(QRect(160, 190, 301, 26));
        lineEdit_15->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        comboBox_4 = new QComboBox(groupBox_10);
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");
        comboBox_4->setGeometry(QRect(160, 230, 136, 26));
        comboBox_4->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: white;\n"
"    color: black;\n"
"    selection-background-color: #0078d7;\n"
"    selection-color: white;\n"
"}\n"
""));
        dateEdit_4 = new QDateEdit(groupBox_10);
        dateEdit_4->setObjectName("dateEdit_4");
        dateEdit_4->setGeometry(QRect(160, 290, 110, 26));
        dateEdit_4->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QDateEdit::drop-down {\n"
"    subcontrol-origin: padding;\n"
"    subcontrol-position: top right;\n"
"    width: 20px;\n"
"    border-left: 1px solid #555;\n"
"}\n"
"\n"
"QDateEdit QAbstractItemView {\n"
"    background-color: white;\n"
"    color: black;\n"
"    selection-background-color: #0078d7;\n"
"    selection-color: white;\n"
"}\n"
""));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 10, 171, 20));
        label_3->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(570, 10, 221, 20));
        label_4->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        chartWidget = new QWidget(tab_3);
        chartWidget->setObjectName("chartWidget");
        chartWidget->setGeometry(QRect(430, 20, 641, 611));
        domainChartWidget = new QWidget(tab_3);
        domainChartWidget->setObjectName("domainChartWidget");
        domainChartWidget->setGeometry(QRect(20, 19, 401, 611));
        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 60, 291, 301));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(110, 30, 113, 26));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(0, 240, 131, 41));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(110, 70, 113, 26));
        lineEdit_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(30, 110, 71, 20));
        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(110, 110, 141, 26));
        comboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: white;\n"
"    color: black;\n"
"    selection-background-color: #0078d7;\n"
"    selection-color: white;\n"
"}\n"
""));
        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(142, 240, 141, 41));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(110, 190, 113, 26));
        lineEdit_4->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(30, 30, 63, 20));
        label_23 = new QLabel(groupBox);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(30, 70, 63, 20));
        label_24 = new QLabel(groupBox);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(30, 150, 63, 20));
        label_25 = new QLabel(groupBox);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(30, 190, 63, 20));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(110, 150, 113, 26));
        lineEdit_3->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(20, 390, 271, 101));
        radioButton = new QRadioButton(groupBox_2);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(10, 30, 110, 24));
        radioButton_3 = new QRadioButton(groupBox_2);
        radioButton_3->setObjectName("radioButton_3");
        radioButton_3->setGeometry(QRect(10, 60, 110, 24));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(310, 390, 271, 101));
        radioButton_2 = new QRadioButton(groupBox_3);
        radioButton_2->setObjectName("radioButton_2");
        radioButton_2->setGeometry(QRect(10, 30, 110, 24));
        radioButton_4 = new QRadioButton(groupBox_3);
        radioButton_4->setObjectName("radioButton_4");
        radioButton_4->setGeometry(QRect(10, 60, 110, 24));
        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(100, 30, 151, 24));
        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setGeometry(QRect(100, 60, 171, 24));
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(30, 550, 121, 41));
        label_8 = new QLabel(tab);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 360, 191, 20));
        label_8->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        label_9 = new QLabel(tab);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(40, 30, 201, 20));
        label_9->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        tableWidget_6 = new QTableWidget(tab);
        if (tableWidget_6->columnCount() < 1)
            tableWidget_6->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_6->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableWidget_6->setObjectName("tableWidget_6");
        tableWidget_6->setGeometry(QRect(970, 520, 121, 111));
        tableView_2 = new QTableView(tab);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(330, 80, 741, 281));
        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        groupBox_6 = new QGroupBox(tab_4);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(20, 100, 451, 201));
        lineEdit_6 = new QLineEdit(groupBox_6);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(230, 70, 201, 26));
        lineEdit_6->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        pushButton_6 = new QPushButton(groupBox_6);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(170, 140, 121, 41));
        label_26 = new QLabel(groupBox_6);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(10, 30, 211, 20));
        label_27 = new QLabel(groupBox_6);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(10, 70, 211, 20));
        lineEdit_5 = new QLineEdit(groupBox_6);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(230, 30, 201, 26));
        lineEdit_5->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        tableWidget_4 = new QTableWidget(tab_4);
        if (tableWidget_4->columnCount() < 4)
            tableWidget_4->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_4->setHorizontalHeaderItem(3, __qtablewidgetitem4);
        tableWidget_4->setObjectName("tableWidget_4");
        tableWidget_4->setGeometry(QRect(570, 140, 511, 151));
        progressBar = new QProgressBar(tab_4);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(490, 200, 71, 23));
        progressBar->setValue(24);
        tableWidget_5 = new QTableWidget(tab_4);
        if (tableWidget_5->columnCount() < 7)
            tableWidget_5->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(5, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_5->setHorizontalHeaderItem(6, __qtablewidgetitem11);
        tableWidget_5->setObjectName("tableWidget_5");
        tableWidget_5->setGeometry(QRect(200, 530, 881, 101));
        groupBox_7 = new QGroupBox(tab_4);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(20, 400, 441, 121));
        pushButton_7 = new QPushButton(groupBox_7);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(172, 68, 111, 41));
        label_28 = new QLabel(groupBox_7);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(10, 30, 201, 20));
        lineEdit_7 = new QLineEdit(groupBox_7);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(210, 30, 221, 26));
        lineEdit_7->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        progressBar_2 = new QProgressBar(tab_4);
        progressBar_2->setObjectName("progressBar_2");
        progressBar_2->setGeometry(QRect(40, 572, 118, 21));
        progressBar_2->setValue(24);
        label_6 = new QLabel(tab_4);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(30, 50, 171, 20));
        label_6->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";\n"
"font: 900 9pt \"Segoe UI\";"));
        label_7 = new QLabel(tab_4);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(30, 350, 181, 20));
        label_7->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        label_34 = new QLabel(tab_4);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(660, 490, 221, 20));
        label_34->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        tabWidget->addTab(tab_4, QString());
        groupBox_11 = new QGroupBox(centralwidget);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setGeometry(QRect(0, -40, 211, 771));
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
        pushButton_16->setGeometry(QRect(20, 700, 161, 51));
        label_29 = new QLabel(groupBox_11);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(-50, 40, 291, 171));
        label_29->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/imagetouse.png-removebg-preview.png);"));
        label_30 = new QLabel(groupBox_11);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(50, 270, 91, 71));
        label_30->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/Downloads/publication icon.png);"));
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
        menubar->setGeometry(QRect(0, 0, 1298, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "supprimer une publication", nullptr));
        lineEdit_8->setText(QString());
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Effacer", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "donner le id de la publication a supprimer :", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "supression publications :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Liste des publications", nullptr));
        label_2->setText(QString());
        groupBox_9->setTitle(QCoreApplication::translate("MainWindow", "ajout de publication", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "svt", nullptr));

        pushButton_9->setText(QCoreApplication::translate("MainWindow", "cliquez ici pour ajouter le contenu", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Titre :", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "source :", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Domaine :", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Nom createur :", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Date de publication :", nullptr));
        groupBox_10->setTitle(QCoreApplication::translate("MainWindow", "modification publication", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Id de la publication a modifier  :", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Titre :", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Nom createur :", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Source :", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Domaine :", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Date de publication :", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "cliquer ici pour modifier le contenu", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("MainWindow", "svt", nullptr));

        label_3->setText(QCoreApplication::translate("MainWindow", "Ajout de publication :", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "modification de publication :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Modification et ajout", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Statistiques de publication", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Crit\303\250res de recherche", nullptr));
        lineEdit->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "R\303\251initialiser", nullptr));
        lineEdit_2->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Domaine:", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "svt", nullptr));

        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        lineEdit_4->setText(QString());
        label_22->setText(QCoreApplication::translate("MainWindow", "Titre", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Source", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Score", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Statue", nullptr));
        lineEdit_3->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Publications \303\240 exporter", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "Toutes", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "Filtr\303\251es", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Format", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        radioButton_4->setText(QCoreApplication::translate("MainWindow", "Excel", nullptr));
        checkBox->setText(QCoreApplication::translate("MainWindow", "Inclure statistiques", nullptr));
        checkBox_2->setText(QCoreApplication::translate("MainWindow", "Inclure champs vides", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Exporter", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "export de la publication :", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Recherche de publication:", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_6->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "succ\303\250s ou non", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Recherche et export", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "choisir les publications pour comparer :", nullptr));
        lineEdit_6->setText(QString());
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "choisir", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "indentifiant de la publication 1 :", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "indentifiant de la publication 2 :", nullptr));
        lineEdit_5->setText(QString());
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_4->horizontalHeaderItem(0);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "pub1", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_4->horizontalHeaderItem(1);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "pub2", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_4->horizontalHeaderItem(2);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Taux de similarit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_4->horizontalHeaderItem(3);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_5->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Titre ", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_5->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Domaine ", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_5->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Source ", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_5->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_5->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "contenu ", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_5->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "statut ", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_5->horizontalHeaderItem(6);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "score ", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "choisir la publication pour v\303\251rifier sa completude :", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "choisir", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "indentifiant de la publication : ", nullptr));
        lineEdit_7->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "detecrtion similarit\303\251 :", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "verification de completude :", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "table des \303\251lements manquants  :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "D\303\251tection de similarit\303\251 et v\303\251rification de compl\303\251tude", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("MainWindow", "Official desktop app", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Publications", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "Reviewers", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Conf\303\251rences", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "Equipements", nullptr));
        label_29->setText(QString());
        label_30->setText(QString());
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
