#ifndef PUBLICATION_H
#define PUBLICATION_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Publication
{
    int id;
    QString titre;
    QString source;
    QString domaine;
    QDate date;
    QString statut;
    QString contenu;

public:
    // Constructeurs
    Publication();
    Publication(int, QString, QString, QString, QDate, QString, QString);

    // Getters
    int getId() const { return id; }
    QString getTitre() const { return titre; }
    QString getSource() const { return source; }
    QString getDomaine() const { return domaine; }
    QDate getDate() const { return date; }
    QString getStatut() const { return statut; }
    QString getContenu() const { return contenu; }

    // Setters
    void setId(int id) { this->id = id; }
    void setTitre(QString t) { titre = t; }
    void setSource(QString s) { source = s; }
    void setDomaine(QString d) { domaine = d; }
    void setDate(QDate d) { date = d; }
    void setStatut(QString s) { statut = s; }
    void setContenu(QString c) { contenu = c; }

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();

    // Autres fonctionnalités métier
    QSqlQueryModel* trier(QString critere, QString ordre);
    QSqlQueryModel* rechercher(QString titre, QString source, QString domaine, QString statut);
};

#endif // PUBLICATION_H
