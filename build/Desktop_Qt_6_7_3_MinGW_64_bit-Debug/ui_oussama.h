/********************************************************************************
** Form generated from reading UI file 'oussama.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUSSAMA_H
#define UI_OUSSAMA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
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
    QTableWidget *tableWidget_7;
    QPushButton *pushButton_17;
    QPushButton *pushButton_18;
    QGroupBox *groupBox_5;
    QRadioButton *radioButton_9;
    QRadioButton *radioButton_10;
    QGroupBox *groupBox_12;
    QRadioButton *radioButton_11;
    QRadioButton *radioButton_12;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QPushButton *pushButton_5;
    QGroupBox *groupBox_7;
    QRadioButton *radioButton_16;
    QRadioButton *radioButton_17;
    QRadioButton *radioButton_18;
    QLabel *label_6;
    QRadioButton *radioButton_19;
    QRadioButton *radioButton_20;
    QRadioButton *radioButton_21;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_21;
    QLabel *label_44;
    QLineEdit *lineEdit_24;
    QLabel *label_45;
    QLabel *label_46;
    QSpinBox *spinBox_3;
    QLabel *label_47;
    QLabel *label_48;
    QLabel *label_49;
    QLineEdit *lineEdit_25;
    QLineEdit *lineEdit_26;
    QDateEdit *dateEdit_6;
    QLineEdit *lineEdit_27;
    QPushButton *pushButton_26;
    QLabel *label_17;
    QLabel *label_23;
    QLineEdit *lineEdit_28;
    QLineEdit *lineEdit_29;
    QLineEdit *lineEdit_30;
    QLineEdit *lineEdit_31;
    QDateEdit *dateEdit_4;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_28;
    QLabel *label_29;
    QSpinBox *spinBox_4;
    QPushButton *pushButton_27;
    QGraphicsView *graphicsView1;
    QGraphicsView *graphicsView1_2;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *tab_4;
    QLineEdit *lineEdit_20;
    QLabel *label_25;
    QPushButton *pushButton_10;
    QLineEdit *lineEdit_13;
    QPushButton *pushButton_12;
    QLabel *label_22;
    QTableWidget *tableWidget_2;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QLabel *label_19;
    QLabel *label_24;
    QLineEdit *lineEdit_21;
    QLabel *label_20;
    QPushButton *pushButton_19;
    QLabel *label_16;
    QLineEdit *lineEdit_22;
    QLabel *label_18;
    QLineEdit *lineEdit_23;
    QGroupBox *groupBox_6;
    QRadioButton *radioButton_13;
    QRadioButton *radioButton_14;
    QRadioButton *radioButton_15;
    QPushButton *pushButton_20;
    QLineEdit *lineEdit;
    QLabel *label_5;
    QCalendarWidget *calendarWidget;
    QGroupBox *groupBox_11;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1303, 757);
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
        groupBox_8 = new QGroupBox(tab_5);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(370, -10, 161, 131));
        lineEdit_8 = new QLineEdit(groupBox_8);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(0, 50, 151, 26));
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
        pushButton_8->setGeometry(QRect(10, 80, 121, 41));
        label_21 = new QLabel(groupBox_8);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(0, 30, 291, 20));
        tableWidget_7 = new QTableWidget(tab_5);
        if (tableWidget_7->columnCount() < 6)
            tableWidget_7->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_7->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget_7->setObjectName("tableWidget_7");
        tableWidget_7->setGeometry(QRect(30, 20, 331, 251));
        tableWidget_7->setRowCount(0);
        tableWidget_7->setColumnCount(6);
        pushButton_17 = new QPushButton(tab_5);
        pushButton_17->setObjectName("pushButton_17");
        pushButton_17->setGeometry(QRect(10, 330, 221, 41));
        pushButton_18 = new QPushButton(tab_5);
        pushButton_18->setObjectName("pushButton_18");
        pushButton_18->setGeometry(QRect(10, 280, 221, 41));
        groupBox_5 = new QGroupBox(tab_5);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(10, 370, 261, 101));
        radioButton_9 = new QRadioButton(groupBox_5);
        radioButton_9->setObjectName("radioButton_9");
        radioButton_9->setGeometry(QRect(10, 30, 110, 24));
        radioButton_10 = new QRadioButton(groupBox_5);
        radioButton_10->setObjectName("radioButton_10");
        radioButton_10->setGeometry(QRect(10, 60, 110, 24));
        groupBox_12 = new QGroupBox(tab_5);
        groupBox_12->setObjectName("groupBox_12");
        groupBox_12->setGeometry(QRect(10, 450, 261, 101));
        radioButton_11 = new QRadioButton(groupBox_12);
        radioButton_11->setObjectName("radioButton_11");
        radioButton_11->setGeometry(QRect(10, 30, 110, 24));
        radioButton_12 = new QRadioButton(groupBox_12);
        radioButton_12->setObjectName("radioButton_12");
        radioButton_12->setGeometry(QRect(10, 60, 110, 24));
        checkBox_5 = new QCheckBox(groupBox_12);
        checkBox_5->setObjectName("checkBox_5");
        checkBox_5->setGeometry(QRect(100, 30, 151, 24));
        checkBox_6 = new QCheckBox(groupBox_12);
        checkBox_6->setObjectName("checkBox_6");
        checkBox_6->setGeometry(QRect(90, 60, 171, 24));
        pushButton_5 = new QPushButton(tab_5);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(60, 550, 121, 41));
        groupBox_7 = new QGroupBox(tab_5);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(370, 120, 211, 131));
        radioButton_16 = new QRadioButton(groupBox_7);
        radioButton_16->setObjectName("radioButton_16");
        radioButton_16->setGeometry(QRect(10, 30, 110, 24));
        radioButton_17 = new QRadioButton(groupBox_7);
        radioButton_17->setObjectName("radioButton_17");
        radioButton_17->setGeometry(QRect(10, 60, 110, 24));
        radioButton_18 = new QRadioButton(groupBox_7);
        radioButton_18->setObjectName("radioButton_18");
        radioButton_18->setGeometry(QRect(10, 90, 110, 24));
        label_6 = new QLabel(groupBox_7);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(0, -10, 241, 41));
        radioButton_19 = new QRadioButton(groupBox_7);
        radioButton_19->setObjectName("radioButton_19");
        radioButton_19->setGeometry(QRect(110, 30, 110, 24));
        radioButton_20 = new QRadioButton(groupBox_7);
        radioButton_20->setObjectName("radioButton_20");
        radioButton_20->setGeometry(QRect(110, 60, 110, 24));
        radioButton_21 = new QRadioButton(groupBox_7);
        radioButton_21->setObjectName("radioButton_21");
        radioButton_21->setGeometry(QRect(110, 90, 110, 24));
        lineEdit_2 = new QLineEdit(tab_5);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(370, 260, 211, 41));
        pushButton_21 = new QPushButton(tab_5);
        pushButton_21->setObjectName("pushButton_21");
        pushButton_21->setGeometry(QRect(390, 300, 91, 41));
        label_44 = new QLabel(tab_5);
        label_44->setObjectName("label_44");
        label_44->setGeometry(QRect(610, 180, 63, 20));
        lineEdit_24 = new QLineEdit(tab_5);
        lineEdit_24->setObjectName("lineEdit_24");
        lineEdit_24->setGeometry(QRect(690, 117, 121, 41));
        label_45 = new QLabel(tab_5);
        label_45->setObjectName("label_45");
        label_45->setGeometry(QRect(610, 80, 63, 20));
        label_46 = new QLabel(tab_5);
        label_46->setObjectName("label_46");
        label_46->setGeometry(QRect(610, 30, 63, 20));
        label_46->setStyleSheet(QString::fromUtf8(""));
        spinBox_3 = new QSpinBox(tab_5);
        spinBox_3->setObjectName("spinBox_3");
        spinBox_3->setGeometry(QRect(690, 270, 121, 41));
        label_47 = new QLabel(tab_5);
        label_47->setObjectName("label_47");
        label_47->setGeometry(QRect(600, 230, 63, 20));
        label_48 = new QLabel(tab_5);
        label_48->setObjectName("label_48");
        label_48->setGeometry(QRect(610, 130, 63, 20));
        label_49 = new QLabel(tab_5);
        label_49->setObjectName("label_49");
        label_49->setGeometry(QRect(600, 280, 63, 20));
        lineEdit_25 = new QLineEdit(tab_5);
        lineEdit_25->setObjectName("lineEdit_25");
        lineEdit_25->setGeometry(QRect(690, 217, 121, 41));
        lineEdit_26 = new QLineEdit(tab_5);
        lineEdit_26->setObjectName("lineEdit_26");
        lineEdit_26->setGeometry(QRect(690, 20, 121, 41));
        dateEdit_6 = new QDateEdit(tab_5);
        dateEdit_6->setObjectName("dateEdit_6");
        dateEdit_6->setGeometry(QRect(690, 170, 121, 41));
        lineEdit_27 = new QLineEdit(tab_5);
        lineEdit_27->setObjectName("lineEdit_27");
        lineEdit_27->setGeometry(QRect(690, 70, 121, 41));
        pushButton_26 = new QPushButton(tab_5);
        pushButton_26->setObjectName("pushButton_26");
        pushButton_26->setGeometry(QRect(650, 320, 111, 41));
        label_17 = new QLabel(tab_5);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(840, 180, 63, 20));
        label_23 = new QLabel(tab_5);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(840, 80, 63, 20));
        lineEdit_28 = new QLineEdit(tab_5);
        lineEdit_28->setObjectName("lineEdit_28");
        lineEdit_28->setGeometry(QRect(930, 70, 111, 41));
        lineEdit_29 = new QLineEdit(tab_5);
        lineEdit_29->setObjectName("lineEdit_29");
        lineEdit_29->setGeometry(QRect(930, 117, 111, 41));
        lineEdit_30 = new QLineEdit(tab_5);
        lineEdit_30->setObjectName("lineEdit_30");
        lineEdit_30->setGeometry(QRect(930, 217, 111, 41));
        lineEdit_31 = new QLineEdit(tab_5);
        lineEdit_31->setObjectName("lineEdit_31");
        lineEdit_31->setGeometry(QRect(930, 20, 111, 41));
        dateEdit_4 = new QDateEdit(tab_5);
        dateEdit_4->setObjectName("dateEdit_4");
        dateEdit_4->setGeometry(QRect(930, 168, 111, 41));
        label_26 = new QLabel(tab_5);
        label_26->setObjectName("label_26");
        label_26->setGeometry(QRect(840, 230, 63, 20));
        label_27 = new QLabel(tab_5);
        label_27->setObjectName("label_27");
        label_27->setGeometry(QRect(840, 30, 91, 20));
        label_28 = new QLabel(tab_5);
        label_28->setObjectName("label_28");
        label_28->setGeometry(QRect(840, 280, 63, 20));
        label_29 = new QLabel(tab_5);
        label_29->setObjectName("label_29");
        label_29->setGeometry(QRect(840, 130, 63, 20));
        spinBox_4 = new QSpinBox(tab_5);
        spinBox_4->setObjectName("spinBox_4");
        spinBox_4->setGeometry(QRect(930, 268, 111, 41));
        pushButton_27 = new QPushButton(tab_5);
        pushButton_27->setObjectName("pushButton_27");
        pushButton_27->setGeometry(QRect(870, 310, 121, 41));
        graphicsView1 = new QGraphicsView(tab_5);
        graphicsView1->setObjectName("graphicsView1");
        graphicsView1->setGeometry(QRect(300, 370, 341, 241));
        graphicsView1_2 = new QGraphicsView(tab_5);
        graphicsView1_2->setObjectName("graphicsView1_2");
        graphicsView1_2->setGeometry(QRect(690, 370, 341, 241));
        label_8 = new QLabel(tab_5);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(290, 600, 391, 41));
        label_9 = new QLabel(tab_5);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(690, 600, 391, 41));
        tabWidget->addTab(tab_5, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        lineEdit_20 = new QLineEdit(tab_4);
        lineEdit_20->setObjectName("lineEdit_20");
        lineEdit_20->setGeometry(QRect(50, 360, 113, 41));
        label_25 = new QLabel(tab_4);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(12, 413, 101, 20));
        pushButton_10 = new QPushButton(tab_4);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(460, 540, 131, 51));
        lineEdit_13 = new QLineEdit(tab_4);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(280, 320, 151, 41));
        pushButton_12 = new QPushButton(tab_4);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(280, 370, 131, 51));
        label_22 = new QLabel(tab_4);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(10, 320, 63, 20));
        tableWidget_2 = new QTableWidget(tab_4);
        if (tableWidget_2->columnCount() < 3)
            tableWidget_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(30, 10, 371, 251));
        tableWidget_2->setRowCount(0);
        tableWidget_2->setColumnCount(3);
        lineEdit_14 = new QLineEdit(tab_4);
        lineEdit_14->setObjectName("lineEdit_14");
        lineEdit_14->setGeometry(QRect(330, 593, 113, 41));
        lineEdit_15 = new QLineEdit(tab_4);
        lineEdit_15->setObjectName("lineEdit_15");
        lineEdit_15->setGeometry(QRect(330, 543, 113, 41));
        label_19 = new QLabel(tab_4);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(10, 360, 63, 20));
        label_24 = new QLabel(tab_4);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(250, 553, 63, 20));
        lineEdit_21 = new QLineEdit(tab_4);
        lineEdit_21->setObjectName("lineEdit_21");
        lineEdit_21->setGeometry(QRect(50, 310, 113, 41));
        label_20 = new QLabel(tab_4);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(180, 503, 131, 20));
        pushButton_19 = new QPushButton(tab_4);
        pushButton_19->setObjectName("pushButton_19");
        pushButton_19->setGeometry(QRect(30, 460, 121, 51));
        label_16 = new QLabel(tab_4);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(190, 613, 121, 20));
        lineEdit_22 = new QLineEdit(tab_4);
        lineEdit_22->setObjectName("lineEdit_22");
        lineEdit_22->setGeometry(QRect(110, 410, 113, 41));
        label_18 = new QLabel(tab_4);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(260, 290, 131, 20));
        lineEdit_23 = new QLineEdit(tab_4);
        lineEdit_23->setObjectName("lineEdit_23");
        lineEdit_23->setGeometry(QRect(330, 490, 111, 41));
        groupBox_6 = new QGroupBox(tab_4);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(650, 390, 261, 121));
        radioButton_13 = new QRadioButton(groupBox_6);
        radioButton_13->setObjectName("radioButton_13");
        radioButton_13->setGeometry(QRect(10, 30, 110, 24));
        radioButton_14 = new QRadioButton(groupBox_6);
        radioButton_14->setObjectName("radioButton_14");
        radioButton_14->setGeometry(QRect(10, 60, 110, 24));
        radioButton_15 = new QRadioButton(groupBox_6);
        radioButton_15->setObjectName("radioButton_15");
        radioButton_15->setGeometry(QRect(10, 90, 110, 24));
        pushButton_20 = new QPushButton(tab_4);
        pushButton_20->setObjectName("pushButton_20");
        pushButton_20->setGeometry(QRect(750, 570, 131, 61));
        lineEdit = new QLineEdit(tab_4);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(660, 520, 251, 41));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(560, 420, 81, 41));
        calendarWidget = new QCalendarWidget(tab_4);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(435, -6, 631, 401));
        tabWidget->addTab(tab_4, QString());
        groupBox_11 = new QGroupBox(centralwidget);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setGeometry(QRect(0, -30, 211, 771));
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
        label_4 = new QLabel(groupBox_11);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(-30, 30, 251, 171));
        label_4->setStyleSheet(QString::fromUtf8("image: url(:/new/prefix1/Downloads/imagetouse.png-removebg-preview.png);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1303, 24));
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
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "supprimer une publication", nullptr));
        lineEdit_8->setText(QString());
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "id a supprimer :", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_7->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_7->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_7->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_7->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_7->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Theme", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_7->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Participants", nullptr));
#if QT_CONFIG(whatsthis)
        pushButton_17->setWhatsThis(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:6pt; font-weight:400;\">kkk</span></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        pushButton_17->setText(QCoreApplication::translate("MainWindow", "tri decoroissont par date", nullptr));
        pushButton_18->setText(QCoreApplication::translate("MainWindow", "tri coroissont par date", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Publications \303\240 exporter", nullptr));
        radioButton_9->setText(QCoreApplication::translate("MainWindow", "Toutes", nullptr));
        radioButton_10->setText(QCoreApplication::translate("MainWindow", "Filtr\303\251es", nullptr));
        groupBox_12->setTitle(QCoreApplication::translate("MainWindow", "Format", nullptr));
        radioButton_11->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        radioButton_12->setText(QCoreApplication::translate("MainWindow", "Excel", nullptr));
        checkBox_5->setText(QCoreApplication::translate("MainWindow", "Inclure statistiques", nullptr));
        checkBox_6->setText(QCoreApplication::translate("MainWindow", "Inclure champs vides", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Exporter", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "Publications \303\240 exporter", nullptr));
        radioButton_16->setText(QCoreApplication::translate("MainWindow", "id", nullptr));
        radioButton_17->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        radioButton_18->setText(QCoreApplication::translate("MainWindow", "lieu", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "filtrage par:", nullptr));
        radioButton_19->setText(QCoreApplication::translate("MainWindow", "date", nullptr));
        radioButton_20->setText(QCoreApplication::translate("MainWindow", "theme", nullptr));
        radioButton_21->setText(QCoreApplication::translate("MainWindow", "participants", nullptr));
        pushButton_21->setText(QCoreApplication::translate("MainWindow", "filtrer", nullptr));
        label_44->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        label_45->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_46->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_47->setText(QCoreApplication::translate("MainWindow", "Theme", nullptr));
        label_48->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        label_49->setText(QCoreApplication::translate("MainWindow", "Participants", nullptr));
        lineEdit_26->setText(QString());
        pushButton_26->setText(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "Theme", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", " ID a modefier", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Participants", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        pushButton_27->setText(QCoreApplication::translate("MainWindow", "modefier", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "number of participants in each conference", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:8pt; font-weight:400;\">number of conference in each day</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainWindow", "Liste des Conf\303\251rences", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "Id Conf\303\251rences", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Suprimer", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Id Conf\303\251rences", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", " ID pour modefier", nullptr));
        pushButton_19->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Id Conf\303\251rences", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "ID pour suprimer", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "Publications \303\240 exporter", nullptr));
        radioButton_13->setText(QCoreApplication::translate("MainWindow", "id", nullptr));
        radioButton_14->setText(QCoreApplication::translate("MainWindow", "nom", nullptr));
        radioButton_15->setText(QCoreApplication::translate("MainWindow", "Id Conf\303\251rences", nullptr));
        pushButton_20->setText(QCoreApplication::translate("MainWindow", "filtrer", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "filtrage par:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "gistion participont et calendrier", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("MainWindow", "Official desktop app", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Publications", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "Reviewers", nullptr));
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Conf\303\251rences", nullptr));
        pushButton_16->setText(QCoreApplication::translate("MainWindow", "Equipements", nullptr));
        label_30->setText(QString());
        label_31->setText(QString());
        label_32->setText(QString());
        label_33->setText(QString());
        label_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUSSAMA_H
