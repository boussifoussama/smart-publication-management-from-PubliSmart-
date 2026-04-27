#include "conference.h"
#include <QSqlError>

Conference::Conference()
{
    id = 0;
    nom = "";
    lieu = "";
    dateDebut = QDate::currentDate();
    theme = "";
}

Conference::Conference(int id, QString nom, QString lieu, QDate dateDebut, QString theme)
{
    this->id = id;
    this->nom = nom;
    this->lieu = lieu;
    this->dateDebut = dateDebut;
    this->theme = theme;
}

bool Conference::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO OUSSAMA.conference (idconference, nom, lieu, datedebut, theme) "
                  "VALUES (:id, :nom, :lieu, :date, :theme)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":date", dateDebut);
    query.bindValue(":theme", theme);

    return query.exec();
}

QSqlQueryModel* Conference::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                  "(SELECT COUNT(*) FROM OUSSAMA.participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM OUSSAMA.conference c ORDER BY c.idconference";
    model->setQuery(sql);
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Lieu");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Theme");
    model->setHeaderData(5, Qt::Horizontal, "Participants");

    return model;
}

bool Conference::supprimer(int id)
{
    QSqlQuery query;
    // Note: participant deletion is handled in SmartMarket or can be moved here
    // To match the original code logic, we handle FK in SmartMarket or CASCADE if defined
    query.prepare("DELETE FROM OUSSAMA.conference WHERE idconference = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Conference::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE OUSSAMA.conference "
                  "SET nom = :nom, lieu = :lieu, datedebut = :date, theme = :theme "
                  "WHERE idconference = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":lieu", lieu);
    query.bindValue(":date", dateDebut);
    query.bindValue(":theme", theme);
    query.bindValue(":id", id);

    return query.exec();
}

QSqlQueryModel* Conference::trier(QString critere, QString ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                  "(SELECT COUNT(*) FROM OUSSAMA.participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM OUSSAMA.conference c ORDER BY " + critere + " " + ordre;
    model->setQuery(sql);
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Lieu");
    model->setHeaderData(3, Qt::Horizontal, "Date");
    model->setHeaderData(4, Qt::Horizontal, "Theme");
    model->setHeaderData(5, Qt::Horizontal, "Participants");

    return model;
}

QSqlQueryModel* Conference::rechercher(QString filter)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT c.idconference, c.nom, c.lieu, c.datedebut, c.theme, "
                  "(SELECT COUNT(*) FROM OUSSAMA.participant p WHERE p.idconference = c.idconference) AS nombreparticipants "
                  "FROM OUSSAMA.conference c";
    if (!filter.isEmpty()) {
        sql += " WHERE LOWER(c.nom) LIKE '%" + filter.toLower() + "%' OR LOWER(c.lieu) LIKE '%" + filter.toLower() + "%' OR LOWER(c.theme) LIKE '%" + filter.toLower() + "%'";
    }
    sql += " ORDER BY c.idconference";
    model->setQuery(sql);
    return model;
}

