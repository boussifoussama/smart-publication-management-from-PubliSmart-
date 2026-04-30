#include "publication.h"
#include <QSqlError>
#include <QDebug>

Publication::Publication()
{
    id = 0;
    titre = "";
    source = "";
    domaine = "";
    date = QDate::currentDate();
    statut = "";
    contenu = "";
}

Publication::Publication(int id, QString titre, QString source, QString domaine, QDate date, QString statut, QString contenu)
{
    this->id = id;
    this->titre = titre;
    this->source = source;
    this->domaine = domaine;
    this->date = date;
    this->statut = statut;
    this->contenu = contenu;
}

bool Publication::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO OUSSAMA.PUBLICATION (IDPUBLICATION, TITRE, SOURCE, DOMAINE, DATEPUBLICATION, STATUT, CONTENU) "
                  "VALUES (:id, :titre, :source, :domaine, :date, :statut, :contenu)");
    query.bindValue(":id", id);
    query.bindValue(":titre", titre);
    query.bindValue(":source", source);
    query.bindValue(":domaine", domaine);
    query.bindValue(":date", date);
    query.bindValue(":statut", statut);
    query.bindValue(":contenu", contenu);

    return query.exec();
}

QSqlQueryModel* Publication::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, DATEPUBLICATION, STATUT, CONTENU FROM OUSSAMA.PUBLICATION ORDER BY IDPUBLICATION");
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Titre");
    model->setHeaderData(2, Qt::Horizontal, "Source");
    model->setHeaderData(3, Qt::Horizontal, "Domaine");
    model->setHeaderData(4, Qt::Horizontal, "Date");
    model->setHeaderData(5, Qt::Horizontal, "Statut");
    model->setHeaderData(6, Qt::Horizontal, "Contenu");

    return model;
}

bool Publication::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM OUSSAMA.PUBLICATION WHERE IDPUBLICATION = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Publication::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE OUSSAMA.PUBLICATION SET TITRE=:titre, SOURCE=:source, DOMAINE=:domaine, DATEPUBLICATION=:date, STATUT=:statut, CONTENU=:contenu "
                  "WHERE IDPUBLICATION=:id");
    query.bindValue(":id", id);
    query.bindValue(":titre", titre);
    query.bindValue(":source", source);
    query.bindValue(":domaine", domaine);
    query.bindValue(":date", date);
    query.bindValue(":statut", statut);
    query.bindValue(":contenu", contenu);

    return query.exec();
}

QSqlQueryModel* Publication::trier(QString critere, QString ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = "SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, DATEPUBLICATION, STATUT, CONTENU FROM OUSSAMA.PUBLICATION ORDER BY " + critere + " " + ordre;
    model->setQuery(queryStr);
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Titre");
    model->setHeaderData(2, Qt::Horizontal, "Source");
    model->setHeaderData(3, Qt::Horizontal, "Domaine");
    model->setHeaderData(4, Qt::Horizontal, "Date");
    model->setHeaderData(5, Qt::Horizontal, "Statut");
    model->setHeaderData(6, Qt::Horizontal, "Contenu");

    return model;
}

QSqlQueryModel* Publication::rechercher(QString titre, QString source, QString domaine, QString statut)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT IDPUBLICATION, TITRE, SOURCE, DOMAINE, DATEPUBLICATION, STATUT, CONTENU FROM OUSSAMA.PUBLICATION WHERE 1=1";
    
    if (!titre.isEmpty())
        sql += " AND UPPER(TITRE) LIKE UPPER('%" + titre + "%')";
    if (!source.isEmpty())
        sql += " AND UPPER(SOURCE) LIKE UPPER('%" + source + "%')";
    if (!domaine.isEmpty() && domaine.toUpper() != "TOUS")
        sql += " AND UPPER(DOMAINE) = UPPER('" + domaine + "')";
    if (!statut.isEmpty())
        sql += " AND UPPER(STATUT) LIKE UPPER('%" + statut + "%')";

    model->setQuery(sql);
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Titre");
    model->setHeaderData(2, Qt::Horizontal, "Source");
    model->setHeaderData(3, Qt::Horizontal, "Domaine");
    model->setHeaderData(4, Qt::Horizontal, "Date");
    model->setHeaderData(5, Qt::Horizontal, "Statut");
    model->setHeaderData(6, Qt::Horizontal, "Contenu");

    return model;
}

