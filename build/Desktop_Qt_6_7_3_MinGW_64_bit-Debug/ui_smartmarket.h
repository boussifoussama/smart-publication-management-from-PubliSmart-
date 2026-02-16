/********************************************************************************
** Form generated from reading UI file 'smartmarket.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMARTMARKET_H
#define UI_SMARTMARKET_H

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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SmartMarket
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidgetMain;
    QWidget *pageLogin;
    QGroupBox *groupBox_11_login;
    QPushButton *pushButton_13;
    QPushButton *pushButton_15;
    QLabel *label_29;
    QLineEdit *lineEdit_8;
    QLineEdit *lineEdit_10;
    QLineEdit *lineEdit_11;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QGroupBox *groupBox;
    QLabel *label_5;
    QWidget *page;
    QGroupBox *groupBox_11;
    QPushButton *pushButton_14;
    QPushButton *pushButton_16;
    QPushButton *pushButton_17;
    QPushButton *pushButton_18;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_34;
    QTabWidget *tabWidget;
    QWidget *tab_5;
    QGroupBox *groupBox_8;
    QLineEdit *lineEdit_9;
    QLabel *label_21;
    QLabel *label_6;
    QPushButton *pushButton_8;
    QTableView *tableView;
    QGroupBox *groupBox_12;
    QLineEdit *lineEdit_16;
    QLineEdit *lineEdit_17;
    QLineEdit *lineEdit_18;
    QComboBox *comboBox_5;
    QDateEdit *dateEdit_5;
    QPushButton *pushButton_19;
    QLabel *label_36;
    QLabel *label_37;
    QLabel *label_38;
    QLabel *label_39;
    QLabel *label_40;
    QLabel *label_41;
    QLabel *label_42;
    QLineEdit *lineEdit_19;
    QPushButton *pushButton_20;
    QLabel *label_43;
    QGroupBox *groupBox_4;
    QLineEdit *lineEdit_20;
    QPushButton *pushButton_5;
    QLineEdit *lineEdit_21;
    QLabel *label_44;
    QComboBox *comboBox_2;
    QPushButton *pushButton_9;
    QLineEdit *lineEdit_22;
    QLabel *label_45;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QLineEdit *lineEdit_23;
    QLabel *label_49;
    QLabel *label_50;
    QGroupBox *groupBox_5;
    QRadioButton *radioButton_5;
    QRadioButton *radioButton_6;
    QGroupBox *groupBox_13;
    QRadioButton *radioButton_7;
    QRadioButton *radioButton_8;
    QCheckBox *checkBox_3;
    QPushButton *pushButton_21;
    QTableWidget *tableWidget_7;
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
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_35;
    QWidget *domainChartWidget;
    QWidget *chartWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SmartMarket)
    {
        if (SmartMarket->objectName().isEmpty())
            SmartMarket->setObjectName("SmartMarket");
        SmartMarket->resize(1298, 768);
        SmartMarket->setStyleSheet(QString::fromUtf8("/* ===== TH\303\210ME GLOBAL \342\200\223 BLEU MARINE PREMIUM ===== */\n"
"* { font-family: \"Inter\", \"Segoe UI\", \"Roboto\", \"Helvetica Neue\", Arial, sans-serif; font-size: 14px; color: #E5E7EB; }\n"
"QMainWindow { background-color: #0B1F33; }\n"
"QWidget { background-color: transparent; }\n"
"QGroupBox { background-color: #0F2A44; border: 1px solid #1E3A8A; border-radius: 14px; margin-top: 22px; padding: 18px; }\n"
"QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 18px; font-size: 15px; font-weight: 800; color: #FFFFFF; }\n"
"QLabel { color: #E5E7EB; font-weight: 500; }\n"
"QTabWidget::pane { background-color: #0F2A44; border: 1px solid #1E3A8A; border-radius: 14px; margin-top: 10px; }\n"
"QTabBar::tab { background-color: #0B1F33; color: #CBD5E1; padding: 14px 32px; margin-right: 10px; border-radius: 12px; font-weight: 600; }\n"
"QTabBar::tab:selected { background-color: #1E40AF; color: #FFFFFF; font-weight: 900; }\n"
"QTabBar::tab:hover:!selected { background-co"
                        "lor: #163B63; color: #FFFFFF; }\n"
"QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox { background-color: #0B1F33; border: 1px solid #1E3A8A; border-radius: 10px; padding: 12px; color: #FFFFFF; font-weight: 500; }\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border-color: #3B82F6; background-color: #0F2A44; }\n"
"QPushButton { background-color: #1E40AF; color: #FFFFFF; border-radius: 12px; padding: 14px 30px; font-weight: 800; letter-spacing: 0.4px; }\n"
"QPushButton:hover { background-color: #2563EB; }\n"
"QPushButton:pressed { background-color: #1D4ED8; }\n"
"QPushButton:disabled { background-color: #475569; color: #CBD5E1; }\n"
"QTableWidget, QTableView { background-color: #0F2A44; border: 1px solid #1E3A8A; border-radius: 14px; gridline-color: #1E3A8A; alternate-background-color: #0B1F33; selection-background-color: #1E40AF; }\n"
"QTableWidget::item, QTableView::item { padding: 14px; color: #E5E7EB; font-weight: 500; }\n"
"QHeaderView::section { back"
                        "ground-color: #0B1F33; color: #FFFFFF; padding: 14px; font-weight: 900; border: none; }\n"
"QCheckBox, QRadioButton { color: #E5E7EB; font-weight: 600; }\n"
"QCheckBox::indicator, QRadioButton::indicator { width: 16px; height: 16px; }\n"
"QProgressBar { background-color: #0B1F33; border: 1px solid #1E3A8A; border-radius: 12px; text-align: center; font-weight: 800; color: #FFFFFF; }\n"
"QProgressBar::chunk { background-color: #2563EB; border-radius: 10px; }\n"
"QMenuBar { background-color: #071526; color: #FFFFFF; font-weight: 800; }\n"
"QMenuBar::item:selected { background-color: rgba(255, 255, 255, 0.15); }\n"
"QStatusBar { background-color: #071526; color: #CBD5E1; }\n"
"#groupBox_11 { background-color: #071526; border: none; }\n"
"#groupBox_11 QPushButton { background-color: transparent; color: #E5E7EB; padding: 18px 24px; border-radius: 14px; font-size: 17px; font-weight: 700; }\n"
"#groupBox_11 QPushButton:hover { background-color: rgba(255,255,255,0.15); }\n"
"#pushButton_13 { background-color: rgba(255,"
                        "255,255,0.25); font-weight: 900; }\n"
""));
        centralwidget = new QWidget(SmartMarket);
        centralwidget->setObjectName("centralwidget");
        stackedWidgetMain = new QStackedWidget(centralwidget);
        stackedWidgetMain->setObjectName("stackedWidgetMain");
        stackedWidgetMain->setGeometry(QRect(0, 0, 1298, 730));
        pageLogin = new QWidget();
        pageLogin->setObjectName("pageLogin");
        groupBox_11_login = new QGroupBox(pageLogin);
        groupBox_11_login->setObjectName("groupBox_11_login");
        groupBox_11_login->setGeometry(QRect(10, -40, 651, 861));
        pushButton_13 = new QPushButton(groupBox_11_login);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(220, 580, 161, 51));
        pushButton_15 = new QPushButton(groupBox_11_login);
        pushButton_15->setObjectName("pushButton_15");
        pushButton_15->setGeometry(QRect(230, 370, 161, 51));
        label_29 = new QLabel(groupBox_11_login);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(70, -50, 481, 361));
        label_29->setStyleSheet(QString::fromUtf8("image: url(:/imagetouse.png-removebg-preview.png);\n"
""));
        lineEdit_8 = new QLineEdit(groupBox_11_login);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(300, 240, 291, 26));
        lineEdit_8->setStyleSheet(QString::fromUtf8("QLineEdit { color: black; background-color: white; border: 2px solid #555; border-radius: 5px; padding: 5px; font-size: 14px; }"));
        lineEdit_8->setDragEnabled(false);
        lineEdit_10 = new QLineEdit(groupBox_11_login);
        lineEdit_10->setObjectName("lineEdit_10");
        lineEdit_10->setGeometry(QRect(300, 300, 291, 26));
        lineEdit_10->setStyleSheet(QString::fromUtf8("QLineEdit { color: black; background-color: white; border: 2px solid #555; border-radius: 5px; padding: 5px; font-size: 14px; }"));
        lineEdit_11 = new QLineEdit(groupBox_11_login);
        lineEdit_11->setObjectName("lineEdit_11");
        lineEdit_11->setGeometry(QRect(300, 510, 291, 26));
        lineEdit_11->setStyleSheet(QString::fromUtf8("QLineEdit { color: black; background-color: white; border: 2px solid #555; border-radius: 5px; padding: 5px; font-size: 14px; }"));
        label = new QLabel(groupBox_11_login);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 240, 111, 20));
        label_2 = new QLabel(groupBox_11_login);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(170, 300, 111, 20));
        label_3 = new QLabel(groupBox_11_login);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 210, 141, 20));
        label_3->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        label_4 = new QLabel(groupBox_11_login);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(50, 450, 511, 21));
        label_4->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        groupBox = new QGroupBox(pageLogin);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(640, -100, 771, 1241));
        groupBox->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(-50, 80, 731, 741));
        label_5->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/menuimage.png);"));
        stackedWidgetMain->addWidget(pageLogin);
        page = new QWidget();
        page->setObjectName("page");
        groupBox_11 = new QGroupBox(page);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setGeometry(QRect(10, -40, 201, 771));
        pushButton_14 = new QPushButton(groupBox_11);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(20, 330, 161, 51));
        pushButton_16 = new QPushButton(groupBox_11);
        pushButton_16->setObjectName("pushButton_16");
        pushButton_16->setGeometry(QRect(20, 450, 161, 51));
        pushButton_17 = new QPushButton(groupBox_11);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setGeometry(QRect(20, 580, 161, 51));
        pushButton_18 = new QPushButton(groupBox_11);
        pushButton_18->setObjectName("pushButton_18");
        pushButton_18->setGeometry(QRect(20, 700, 161, 51));
        label_30 = new QLabel(groupBox_11);
        label_30->setObjectName("label_30");
        label_30->setGeometry(QRect(-50, 40, 291, 171));
        label_30->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/imagetouse.png-removebg-preview.png);"));
        label_31 = new QLabel(groupBox_11);
        label_31->setObjectName("label_31");
        label_31->setGeometry(QRect(50, 270, 91, 71));
        label_31->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/publication icon.png);"));
        label_32 = new QLabel(groupBox_11);
        label_32->setObjectName("label_32");
        label_32->setGeometry(QRect(50, 370, 101, 91));
        label_32->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/reviewer icon.png);"));
        label_33 = new QLabel(groupBox_11);
        label_33->setObjectName("label_33");
        label_33->setGeometry(QRect(20, 500, 151, 101));
        label_33->setStyleSheet(QString::fromUtf8("\n"
"image: url(:/conference icon.png);"));
        label_34 = new QLabel(groupBox_11);
        label_34->setObjectName("label_34");
        label_34->setGeometry(QRect(20, 620, 151, 101));
        label_34->setStyleSheet(QString::fromUtf8("\n"
"\n"
"image: url(:/equipement icon.png);"));
        tabWidget = new QTabWidget(page);
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
        groupBox_8->setGeometry(QRect(660, -10, 431, 121));
        lineEdit_9 = new QLineEdit(groupBox_8);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(10, 80, 291, 21));
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
        lineEdit_9->setDragEnabled(false);
        label_21 = new QLabel(groupBox_8);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(20, 50, 291, 20));
        label_6 = new QLabel(groupBox_8);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 19, 191, 31));
        label_6->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        pushButton_8 = new QPushButton(groupBox_8);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(310, 50, 111, 41));
        tableView = new QTableView(tab_5);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(10, 10, 641, 161));
        groupBox_12 = new QGroupBox(tab_5);
        groupBox_12->setObjectName("groupBox_12");
        groupBox_12->setGeometry(QRect(660, 120, 421, 421));
        lineEdit_16 = new QLineEdit(groupBox_12);
        lineEdit_16->setObjectName("lineEdit_16");
        lineEdit_16->setGeometry(QRect(110, 40, 291, 26));
        lineEdit_16->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
""));
        lineEdit_17 = new QLineEdit(groupBox_12);
        lineEdit_17->setObjectName("lineEdit_17");
        lineEdit_17->setGeometry(QRect(110, 90, 291, 26));
        lineEdit_17->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        lineEdit_18 = new QLineEdit(groupBox_12);
        lineEdit_18->setObjectName("lineEdit_18");
        lineEdit_18->setGeometry(QRect(120, 140, 281, 26));
        lineEdit_18->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        comboBox_5 = new QComboBox(groupBox_12);
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");
        comboBox_5->setGeometry(QRect(100, 190, 301, 26));
        comboBox_5->setStyleSheet(QString::fromUtf8("QComboBox {\n"
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
        dateEdit_5 = new QDateEdit(groupBox_12);
        dateEdit_5->setObjectName("dateEdit_5");
        dateEdit_5->setGeometry(QRect(160, 240, 241, 26));
        dateEdit_5->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
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
        pushButton_19 = new QPushButton(groupBox_12);
        pushButton_19->setObjectName("pushButton_19");
        pushButton_19->setGeometry(QRect(10, 370, 111, 41));
        label_36 = new QLabel(groupBox_12);
        label_36->setObjectName("label_36");
        label_36->setGeometry(QRect(20, 40, 63, 20));
        label_37 = new QLabel(groupBox_12);
        label_37->setObjectName("label_37");
        label_37->setGeometry(QRect(20, 90, 63, 20));
        label_38 = new QLabel(groupBox_12);
        label_38->setObjectName("label_38");
        label_38->setGeometry(QRect(20, 190, 71, 20));
        label_39 = new QLabel(groupBox_12);
        label_39->setObjectName("label_39");
        label_39->setGeometry(QRect(20, 140, 111, 20));
        label_40 = new QLabel(groupBox_12);
        label_40->setObjectName("label_40");
        label_40->setGeometry(QRect(20, 240, 151, 20));
        label_41 = new QLabel(groupBox_12);
        label_41->setObjectName("label_41");
        label_41->setGeometry(QRect(20, 290, 221, 20));
        label_42 = new QLabel(groupBox_12);
        label_42->setObjectName("label_42");
        label_42->setGeometry(QRect(20, 330, 391, 20));
        lineEdit_19 = new QLineEdit(groupBox_12);
        lineEdit_19->setObjectName("lineEdit_19");
        lineEdit_19->setGeometry(QRect(220, 290, 181, 26));
        lineEdit_19->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        pushButton_20 = new QPushButton(groupBox_12);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(300, 370, 111, 41));
        label_43 = new QLabel(groupBox_12);
        label_43->setObjectName("label_43");
        label_43->setGeometry(QRect(20, 0, 281, 20));
        label_43->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        groupBox_4 = new QGroupBox(tab_5);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(10, 180, 291, 291));
        lineEdit_20 = new QLineEdit(groupBox_4);
        lineEdit_20->setObjectName("lineEdit_20");
        lineEdit_20->setGeometry(QRect(110, 30, 113, 26));
        lineEdit_20->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        pushButton_5 = new QPushButton(groupBox_4);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(0, 240, 131, 41));
        lineEdit_21 = new QLineEdit(groupBox_4);
        lineEdit_21->setObjectName("lineEdit_21");
        lineEdit_21->setGeometry(QRect(110, 70, 113, 26));
        lineEdit_21->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        label_44 = new QLabel(groupBox_4);
        label_44->setObjectName("label_44");
        label_44->setGeometry(QRect(30, 110, 71, 20));
        comboBox_2 = new QComboBox(groupBox_4);
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(110, 110, 141, 26));
        comboBox_2->setStyleSheet(QString::fromUtf8("QComboBox {\n"
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
        pushButton_9 = new QPushButton(groupBox_4);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(142, 240, 141, 41));
        lineEdit_22 = new QLineEdit(groupBox_4);
        lineEdit_22->setObjectName("lineEdit_22");
        lineEdit_22->setGeometry(QRect(110, 190, 113, 26));
        lineEdit_22->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        label_45 = new QLabel(groupBox_4);
        label_45->setObjectName("label_45");
        label_45->setGeometry(QRect(30, 30, 63, 20));
        label_46 = new QLabel(groupBox_4);
        label_46->setObjectName("label_46");
        label_46->setGeometry(QRect(30, 70, 63, 20));
        label_47 = new QLabel(groupBox_4);
        label_47->setObjectName("label_47");
        label_47->setGeometry(QRect(30, 150, 63, 20));
        label_48 = new QLabel(groupBox_4);
        label_48->setObjectName("label_48");
        label_48->setGeometry(QRect(30, 190, 63, 20));
        lineEdit_23 = new QLineEdit(groupBox_4);
        lineEdit_23->setObjectName("lineEdit_23");
        lineEdit_23->setGeometry(QRect(110, 150, 113, 26));
        lineEdit_23->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    color: black;\n"
"    background-color: white;\n"
"    border: 2px solid #555;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"    font-size: 14px;\n"
"}\n"
""));
        label_49 = new QLabel(groupBox_4);
        label_49->setObjectName("label_49");
        label_49->setGeometry(QRect(30, -11, 201, 31));
        label_49->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        label_50 = new QLabel(tab_5);
        label_50->setObjectName("label_50");
        label_50->setGeometry(QRect(40, 480, 191, 20));
        label_50->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        groupBox_5 = new QGroupBox(tab_5);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(20, 500, 121, 91));
        radioButton_5 = new QRadioButton(groupBox_5);
        radioButton_5->setObjectName("radioButton_5");
        radioButton_5->setGeometry(QRect(10, 30, 110, 24));
        radioButton_6 = new QRadioButton(groupBox_5);
        radioButton_6->setObjectName("radioButton_6");
        radioButton_6->setGeometry(QRect(10, 60, 110, 24));
        groupBox_13 = new QGroupBox(tab_5);
        groupBox_13->setObjectName("groupBox_13");
        groupBox_13->setGeometry(QRect(160, 500, 271, 91));
        radioButton_7 = new QRadioButton(groupBox_13);
        radioButton_7->setObjectName("radioButton_7");
        radioButton_7->setGeometry(QRect(10, 30, 110, 24));
        radioButton_8 = new QRadioButton(groupBox_13);
        radioButton_8->setObjectName("radioButton_8");
        radioButton_8->setGeometry(QRect(10, 60, 110, 24));
        checkBox_3 = new QCheckBox(groupBox_13);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(100, 60, 171, 24));
        pushButton_21 = new QPushButton(tab_5);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(90, 600, 121, 41));
        tableWidget_7 = new QTableWidget(tab_5);
        if (tableWidget_7->columnCount() < 1)
            tableWidget_7->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(0, __qtablewidgetitem);
        tableWidget_7->setObjectName("tableWidget_7");
        tableWidget_7->setGeometry(QRect(440, 520, 121, 111));
        tableView_2 = new QTableView(tab_5);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(320, 190, 331, 281));
        tabWidget->addTab(tab_5, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        groupBox_6 = new QGroupBox(tab_4);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(10, 10, 201, 151));
        lineEdit_6 = new QLineEdit(groupBox_6);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(150, 70, 41, 26));
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
        pushButton_6->setGeometry(QRect(10, 100, 121, 41));
        label_26 = new QLabel(groupBox_6);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(10, 30, 211, 20));
        label_27 = new QLabel(groupBox_6);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(10, 70, 211, 20));
        lineEdit_5 = new QLineEdit(groupBox_6);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(150, 30, 41, 26));
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
        tableWidget_4->setGeometry(QRect(250, 20, 421, 151));
        progressBar = new QProgressBar(tab_4);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(220, 80, 31, 23));
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
        tableWidget_5->setGeometry(QRect(10, 290, 661, 91));
        groupBox_7 = new QGroupBox(tab_4);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(10, 170, 261, 121));
        pushButton_7 = new QPushButton(groupBox_7);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(10, 60, 111, 41));
        label_28 = new QLabel(groupBox_7);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(10, 30, 201, 20));
        lineEdit_7 = new QLineEdit(groupBox_7);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(210, 30, 41, 26));
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
        progressBar_2->setGeometry(QRect(290, 250, 118, 21));
        progressBar_2->setValue(24);
        label_7 = new QLabel(tab_4);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(10, 10, 171, 20));
        label_7->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";\n"
"font: 900 9pt \"Segoe UI\";"));
        label_8 = new QLabel(tab_4);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(10, 170, 211, 20));
        label_8->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        label_35 = new QLabel(tab_4);
        label_35->setObjectName("label_35");
        label_35->setGeometry(QRect(420, 250, 241, 20));
        label_35->setStyleSheet(QString::fromUtf8("font: 900 9pt \"Segoe UI\";"));
        domainChartWidget = new QWidget(tab_4);
        domainChartWidget->setObjectName("domainChartWidget");
        domainChartWidget->setGeometry(QRect(10, 390, 1061, 251));
        chartWidget = new QWidget(tab_4);
        chartWidget->setObjectName("chartWidget");
        chartWidget->setGeometry(QRect(680, 10, 391, 371));
        tabWidget->addTab(tab_4, QString());
        stackedWidgetMain->addWidget(page);
        SmartMarket->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SmartMarket);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1298, 24));
        SmartMarket->setMenuBar(menubar);
        statusbar = new QStatusBar(SmartMarket);
        statusbar->setObjectName("statusbar");
        SmartMarket->setStatusBar(statusbar);

        retranslateUi(SmartMarket);

        stackedWidgetMain->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SmartMarket);
    } // setupUi

    void retranslateUi(QMainWindow *SmartMarket)
    {
        SmartMarket->setWindowTitle(QCoreApplication::translate("SmartMarket", "SmartMarket", nullptr));
        groupBox_11_login->setTitle(QCoreApplication::translate("SmartMarket", "Official desktop app", nullptr));
        pushButton_13->setText(QCoreApplication::translate("SmartMarket", "Envoyer", nullptr));
        pushButton_15->setText(QCoreApplication::translate("SmartMarket", "entrer", nullptr));
        label_29->setText(QString());
        lineEdit_8->setText(QString());
        label->setText(QCoreApplication::translate("SmartMarket", "l'adresse mail :", nullptr));
        label_2->setText(QCoreApplication::translate("SmartMarket", "le mot de passe :", nullptr));
        label_3->setText(QCoreApplication::translate("SmartMarket", "se connecter :", nullptr));
        label_4->setText(QCoreApplication::translate("SmartMarket", "mot de passe oubli\303\251e ? pas de probleme entrez votre adresse mail : ", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SmartMarket", "GroupBox", nullptr));
        label_5->setText(QCoreApplication::translate("SmartMarket", "TextLabel", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("SmartMarket", "Official desktop app", nullptr));
        pushButton_14->setText(QCoreApplication::translate("SmartMarket", "Publications", nullptr));
        pushButton_16->setText(QCoreApplication::translate("SmartMarket", "Reviewers", nullptr));
        pushButton_17->setText(QCoreApplication::translate("SmartMarket", "Conf\303\251rences", nullptr));
        pushButton_18->setText(QCoreApplication::translate("SmartMarket", "Equipements", nullptr));
        label_30->setText(QString());
        label_31->setText(QString());
        label_32->setText(QString());
        label_33->setText(QString());
        label_34->setText(QString());
        groupBox_8->setTitle(QCoreApplication::translate("SmartMarket", "supprimer une publication", nullptr));
        lineEdit_9->setText(QString());
        label_21->setText(QCoreApplication::translate("SmartMarket", "donner le id de la publication a supprimer :", nullptr));
        label_6->setText(QCoreApplication::translate("SmartMarket", "supression publications :", nullptr));
        pushButton_8->setText(QCoreApplication::translate("SmartMarket", "Effacer", nullptr));
        groupBox_12->setTitle(QCoreApplication::translate("SmartMarket", "ajout de publication", nullptr));
        comboBox_5->setItemText(0, QCoreApplication::translate("SmartMarket", "svt", nullptr));

        pushButton_19->setText(QCoreApplication::translate("SmartMarket", "ajouter", nullptr));
        label_36->setText(QCoreApplication::translate("SmartMarket", "Titre :", nullptr));
        label_37->setText(QCoreApplication::translate("SmartMarket", "source :", nullptr));
        label_38->setText(QCoreApplication::translate("SmartMarket", "Domaine :", nullptr));
        label_39->setText(QCoreApplication::translate("SmartMarket", "Nom createur :", nullptr));
        label_40->setText(QCoreApplication::translate("SmartMarket", "Date de publication :", nullptr));
        label_41->setText(QCoreApplication::translate("SmartMarket", "Id de la publication a modifier :", nullptr));
        label_42->setText(QCoreApplication::translate("SmartMarket", "(si vous voulez ajouter une publication laissez ce champ vide)", nullptr));
        pushButton_20->setText(QCoreApplication::translate("SmartMarket", "modifier", nullptr));
        label_43->setText(QCoreApplication::translate("SmartMarket", "Ajout ou modification de publication :", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("SmartMarket", "Crit\303\250res de recherche", nullptr));
        lineEdit_20->setText(QString());
        pushButton_5->setText(QCoreApplication::translate("SmartMarket", "R\303\251initialiser", nullptr));
        lineEdit_21->setText(QString());
        label_44->setText(QCoreApplication::translate("SmartMarket", "Domaine:", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("SmartMarket", "svt", nullptr));

        pushButton_9->setText(QCoreApplication::translate("SmartMarket", "Rechercher", nullptr));
        lineEdit_22->setText(QString());
        label_45->setText(QCoreApplication::translate("SmartMarket", "Titre", nullptr));
        label_46->setText(QCoreApplication::translate("SmartMarket", "Source", nullptr));
        label_47->setText(QCoreApplication::translate("SmartMarket", "Score", nullptr));
        label_48->setText(QCoreApplication::translate("SmartMarket", "Statue", nullptr));
        lineEdit_23->setText(QString());
        label_49->setText(QCoreApplication::translate("SmartMarket", "Recherche de publication:", nullptr));
        label_50->setText(QCoreApplication::translate("SmartMarket", "export des publications :", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("SmartMarket", "Publications \303\240 exporter", nullptr));
        radioButton_5->setText(QCoreApplication::translate("SmartMarket", "Toutes", nullptr));
        radioButton_6->setText(QCoreApplication::translate("SmartMarket", "Filtr\303\251es", nullptr));
        groupBox_13->setTitle(QCoreApplication::translate("SmartMarket", "Format", nullptr));
        radioButton_7->setText(QCoreApplication::translate("SmartMarket", "PDF", nullptr));
        radioButton_8->setText(QCoreApplication::translate("SmartMarket", "Excel", nullptr));
        checkBox_3->setText(QCoreApplication::translate("SmartMarket", "Inclure champs vides", nullptr));
        pushButton_21->setText(QCoreApplication::translate("SmartMarket", "Exporter", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_7->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("SmartMarket", "succ\303\250s ou non", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("SmartMarket", "Liste des publications", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("SmartMarket", "choisir les publications pour comparer :", nullptr));
        lineEdit_6->setText(QString());
        pushButton_6->setText(QCoreApplication::translate("SmartMarket", "choisir", nullptr));
        label_26->setText(QCoreApplication::translate("SmartMarket", "id de la publication 1 :", nullptr));
        label_27->setText(QCoreApplication::translate("SmartMarket", "id de la publication 2 :", nullptr));
        lineEdit_5->setText(QString());
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_4->horizontalHeaderItem(0);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("SmartMarket", "pub1", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_4->horizontalHeaderItem(1);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("SmartMarket", "pub2", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_4->horizontalHeaderItem(2);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("SmartMarket", "Taux de similarit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_4->horizontalHeaderItem(3);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("SmartMarket", "statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_5->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("SmartMarket", "Titre ", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_5->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("SmartMarket", "Domaine ", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_5->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("SmartMarket", "Source ", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_5->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("SmartMarket", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_5->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("SmartMarket", "contenu ", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_5->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("SmartMarket", "statut ", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_5->horizontalHeaderItem(6);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("SmartMarket", "score ", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("SmartMarket", "choisir la publication pour v\303\251rifier sa completude :", nullptr));
        pushButton_7->setText(QCoreApplication::translate("SmartMarket", "choisir", nullptr));
        label_28->setText(QCoreApplication::translate("SmartMarket", "indentifiant de la publication : ", nullptr));
        lineEdit_7->setText(QString());
        label_7->setText(QCoreApplication::translate("SmartMarket", "detection similarit\303\251 :", nullptr));
        label_8->setText(QCoreApplication::translate("SmartMarket", "verification de completude :", nullptr));
        label_35->setText(QCoreApplication::translate("SmartMarket", "table des \303\251lements manquants  :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("SmartMarket", "D\303\251tection de similarit\303\251 et v\303\251rification de compl\303\251tude", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SmartMarket: public Ui_SmartMarket {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMARTMARKET_H
