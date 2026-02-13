#include "smartmarket.h"
#include "ui_smartmarket.h"

#include <QHeaderView>
#include <QStandardItem>
#include <QVBoxLayout>
#include <QMessageBox>

// Qt Charts
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

SmartMarket::SmartMarket(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SmartMarket)
    , publicationModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    
    // Masquer le mot de passe
    ui->lineEdit_10->setEchoMode(QLineEdit::Password);
    
    // Initialiser le tableau des publications
    initPublicationTable();
    
    // Créer les graphiques
    createCharts();
    
    // Démarrer sur la page Login (page 0)
    ui->stackedWidgetMain->setCurrentIndex(0);
}

SmartMarket::~SmartMarket()
{
    delete ui;
}

void SmartMarket::initPublicationTable()
{
    // ================== TABLEAU DES PUBLICATIONS ==================
    publicationModel->setRowCount(4);
    publicationModel->setColumnCount(5);

    publicationModel->setHeaderData(0, Qt::Horizontal, "Domaine");
    publicationModel->setHeaderData(1, Qt::Horizontal, "Titre");
    publicationModel->setHeaderData(2, Qt::Horizontal, "Source");
    publicationModel->setHeaderData(3, Qt::Horizontal, "Date");
    publicationModel->setHeaderData(4, Qt::Horizontal, "Statut");

    publicationModel->setItem(0, 0, new QStandardItem("SVT"));
    publicationModel->setItem(0, 1, new QStandardItem("Molécules"));
    publicationModel->setItem(0, 2, new QStandardItem("Journal"));
    publicationModel->setItem(0, 3, new QStandardItem("01/11/2000"));
    publicationModel->setItem(0, 4, new QStandardItem("Évaluée"));

    publicationModel->setItem(1, 0, new QStandardItem("Informatique"));
    publicationModel->setItem(1, 1, new QStandardItem("IA"));
    publicationModel->setItem(1, 2, new QStandardItem("Magazine"));
    publicationModel->setItem(1, 3, new QStandardItem("01/09/2009"));
    publicationModel->setItem(1, 4, new QStandardItem("Non évaluée"));

    publicationModel->setItem(2, 0, new QStandardItem("Informatique"));
    publicationModel->setItem(2, 1, new QStandardItem("Deep Learning"));
    publicationModel->setItem(2, 2, new QStandardItem("Magazine of Science"));
    publicationModel->setItem(2, 3, new QStandardItem("06/09/2005"));
    publicationModel->setItem(2, 4, new QStandardItem("Rejetée"));

    publicationModel->setItem(3, 0, new QStandardItem("Physique"));
    publicationModel->setItem(3, 1, new QStandardItem("Atomes"));
    publicationModel->setItem(3, 2, new QStandardItem("Journal Parisien"));
    publicationModel->setItem(3, 3, new QStandardItem("06/07/2008"));
    publicationModel->setItem(3, 4, new QStandardItem("Rejetée"));

    ui->tableView->setModel(publicationModel);
    ui->tableView_2->setModel(publicationModel);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void SmartMarket::createCharts()
{
    // ================== PIE CHART - RÉPARTITION PAR STATUT ==================
    int eval = 0, nonEval = 0, rejet = 0;
    for (int row = 0; row < publicationModel->rowCount(); ++row) {
        QString statut = publicationModel->item(row, 4)->text();
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
    pieLayout->setContentsMargins(0, 0, 0, 0);
    pieLayout->addWidget(pieChartView);

    // ================== BAR CHART - PUBLICATIONS PAR DOMAINE ==================
    QStringList domaines = {"SVT", "Informatique", "Physique", "Math", "Chimie", "Biologie"};
    QMap<QString, int> domainCounts;
    for (const QString &d : domaines) domainCounts[d] = 0;

    for (int row = 0; row < publicationModel->rowCount(); ++row) {
        QString domaine = publicationModel->item(row, 0)->text();
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

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(domaines);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(true);
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *barLayout = new QVBoxLayout(ui->domainChartWidget);
    barLayout->setContentsMargins(0, 0, 0, 0);
    barLayout->addWidget(barChartView);
}

// ============================================================
// NAVIGATION - MODULE LOGIN
// ============================================================

void SmartMarket::on_pushButton_15_clicked()
{
    QString email = ui->lineEdit_8->text().trimmed();
    QString password = ui->lineEdit_10->text().trimmed();

    if(email.compare("Selim.ASCHI@esprit.tn", Qt::CaseInsensitive) == 0
        && password == "selim")
    {
        QMessageBox::information(this, "Succès", "Connexion réussie ! Bienvenue dans SmartMarket.");
        
        // 🔥 Navigation vers la page Publication (page 1)
        ui->stackedWidgetMain->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Email ou mot de passe incorrect !");
    }
}

void SmartMarket::on_pushButton_13_clicked()
{
    QString email = ui->lineEdit_11->text().trimmed();
    
    if(email.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer votre adresse email.");
        return;
    }
    
    QMessageBox::information(this, "Email envoyé", 
        "Un email de réinitialisation a été envoyé à : " + email);
    
    ui->lineEdit_11->clear();
}

// ============================================================
// MODULE PUBLICATION - CRUD
// ============================================================

void SmartMarket::on_pushButton_17_clicked()
{
    QString titre = ui->lineEdit_16->text().trimmed();
    QString source = ui->lineEdit_17->text().trimmed();
    QString createur = ui->lineEdit_18->text().trimmed();
    QString domaine = ui->comboBox_5->currentText();
    QString date = ui->dateEdit_5->date().toString("dd/MM/yyyy");
    QString idModif = ui->lineEdit_19->text().trimmed();
    
    if(titre.isEmpty() || source.isEmpty() || createur.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    if(idModif.isEmpty())
    {
        int newRow = publicationModel->rowCount();
        publicationModel->insertRow(newRow);
        
        publicationModel->setItem(newRow, 0, new QStandardItem(domaine));
        publicationModel->setItem(newRow, 1, new QStandardItem(titre));
        publicationModel->setItem(newRow, 2, new QStandardItem(source));
        publicationModel->setItem(newRow, 3, new QStandardItem(date));
        publicationModel->setItem(newRow, 4, new QStandardItem("Non évaluée"));
        
        QMessageBox::information(this, "Succès", "Publication ajoutée avec succès !");
        
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
    }
}

void SmartMarket::on_pushButton_18_clicked()
{
    QString titre = ui->lineEdit_16->text().trimmed();
    QString source = ui->lineEdit_17->text().trimmed();
    QString createur = ui->lineEdit_18->text().trimmed();
    QString domaine = ui->comboBox_5->currentText();
    QString date = ui->dateEdit_5->date().toString("dd/MM/yyyy");
    QString idModif = ui->lineEdit_19->text().trimmed();
    
    if(idModif.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID de la publication à modifier.");
        return;
    }
    
    int rowToModify = idModif.toInt() - 1;
    
    if(rowToModify >= 0 && rowToModify < publicationModel->rowCount())
    {
        publicationModel->setItem(rowToModify, 0, new QStandardItem(domaine));
        publicationModel->setItem(rowToModify, 1, new QStandardItem(titre));
        publicationModel->setItem(rowToModify, 2, new QStandardItem(source));
        publicationModel->setItem(rowToModify, 3, new QStandardItem(date));
        
        QMessageBox::information(this, "Succès", "Publication modifiée avec succès !");
        
        ui->lineEdit_16->clear();
        ui->lineEdit_17->clear();
        ui->lineEdit_18->clear();
        ui->lineEdit_19->clear();
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "ID de publication introuvable.");
    }
}

void SmartMarket::on_pushButton_8_clicked()
{
    QString idSupprimer = ui->lineEdit_8->text().trimmed();
    
    if(idSupprimer.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID de la publication à supprimer.");
        return;
    }
    
    int rowToDelete = idSupprimer.toInt() - 1;
    
    if(rowToDelete >= 0 && rowToDelete < publicationModel->rowCount())
    {
        publicationModel->removeRow(rowToDelete);
        QMessageBox::information(this, "Succès", "Publication supprimée avec succès !");
        ui->lineEdit_8->clear();
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "ID de publication introuvable.");
    }
}

void SmartMarket::on_pushButton_9_clicked()
{
    QString titre = ui->lineEdit_20->text().trimmed();
    QString source = ui->lineEdit_21->text().trimmed();
    QString domaine = ui->comboBox_2->currentText();
    QString score = ui->lineEdit_23->text().trimmed();
    QString statut = ui->lineEdit_22->text().trimmed();
    
    QMessageBox::information(this, "Recherche", 
        "Recherche effectuée avec les critères :\n"
        "Titre: " + titre + "\n"
        "Source: " + source + "\n"
        "Domaine: " + domaine + "\n"
        "Score: " + score + "\n"
        "Statut: " + statut);
}

void SmartMarket::on_pushButton_5_clicked()
{
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_23->clear();
    ui->lineEdit_22->clear();
    ui->comboBox_2->setCurrentIndex(0);
    
    QMessageBox::information(this, "Réinitialisation", "Critères de recherche réinitialisés.");
}

void SmartMarket::on_pushButton_19_clicked()
{
    bool toutes = ui->radioButton_5->isChecked();
    bool pdf = ui->radioButton_7->isChecked();
    bool excel = ui->radioButton_8->isChecked();
    bool includeVides = ui->checkBox_3->isChecked();
    
    QString format = pdf ? "PDF" : "Excel";
    QString scope = toutes ? "toutes les publications" : "les publications filtrées";
    
    QMessageBox::information(this, "Export", 
        "Export en " + format + " de " + scope + ".\n"
        "Inclure champs vides : " + (includeVides ? "Oui" : "Non"));
}

// ============================================================
// MODULE PUBLICATION - ANALYSE
// ============================================================

void SmartMarket::on_pushButton_6_clicked()
{
    QString id1 = ui->lineEdit_5->text().trimmed();
    QString id2 = ui->lineEdit_6->text().trimmed();
    
    if(id1.isEmpty() || id2.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer les deux IDs de publication.");
        return;
    }
    
    int similarite = 75;
    
    QMessageBox::information(this, "Détection de similarité", 
        "Comparaison des publications " + id1 + " et " + id2 + ".\n"
        "Taux de similarité : " + QString::number(similarite) + "%");
    
    ui->progressBar->setValue(similarite);
}

void SmartMarket::on_pushButton_7_clicked()
{
    QString id = ui->lineEdit_7->text().trimmed();
    
    if(id.isEmpty())
    {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID de la publication.");
        return;
    }
    
    int completude = 85;
    
    QMessageBox::information(this, "Vérification de complétude", 
        "Publication " + id + " vérifiée.\n"
        "Complétude : " + QString::number(completude) + "%");
    
    ui->progressBar_2->setValue(completude);
}

// ============================================================
// SIDEBAR - NAVIGATION ENTRE MODULES
// ============================================================

void SmartMarket::on_pushButton_13_sidebar_clicked()
{
    ui->stackedWidgetMain->setCurrentIndex(1);
}

void SmartMarket::on_pushButton_14_clicked()
{
    QMessageBox::information(this, "Module Reviewers", 
        "Le module Reviewers sera intégré prochainement.");
}

void SmartMarket::on_pushButton_15_sidebar_clicked()
{
    QMessageBox::information(this, "Module Conférences", 
        "Le module Conférences sera intégré prochainement.");
}

void SmartMarket::on_pushButton_16_clicked()
{
    QMessageBox::information(this, "Module Équipements", 
        "Le module Équipements sera intégré prochainement.");
}
