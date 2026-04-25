#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Conference
{
    int id;
    QString nom;
    QString lieu;
    QDate dateDebut;
    QString theme;

public:
    Conference();
    Conference(int, QString, QString, QDate, QString);

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getLieu() const { return lieu; }
    QDate getDateDebut() const { return dateDebut; }
    QString getTheme() const { return theme; }

    // Setters
    void setId(int id) { this->id = id; }
    void setNom(QString n) { nom = n; }
    void setLieu(QString l) { lieu = l; }
    void setDateDebut(QDate d) { dateDebut = d; }
    void setTheme(QString t) { theme = t; }

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* rechercher(QString filter);
    
    // Autres
    QSqlQueryModel* trier(QString critere, QString ordre);
};

#endif // CONFERENCE_H
