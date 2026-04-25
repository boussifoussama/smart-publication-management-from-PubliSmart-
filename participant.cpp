#include "participant.h"
#include <QSqlError>

Participant::Participant()
{
    id = 0;
    nom = "";
    idConference = 0;
    uidRfid = "";
}

Participant::Participant(int id, QString nom, int idConference, QString uidRfid)
{
    this->id = id;
    this->nom = nom;
    this->idConference = idConference;
    this->uidRfid = uidRfid;
}

bool Participant::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO OUSSAMA.participant (id, nom, idconference, uid_rfid) "
                  "VALUES (:id, :nom, :idconference, :uid)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idconference", idConference);
    query.bindValue(":uid", uidRfid);

    return query.exec();
}

QSqlQueryModel* Participant::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id, nom, idconference, uid_rfid FROM OUSSAMA.participant ORDER BY id");
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "ID Conference");
    model->setHeaderData(3, Qt::Horizontal, "UID RFID");

    return model;
}

bool Participant::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM OUSSAMA.participant WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Participant::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE OUSSAMA.participant SET nom=:nom, idconference=:idconf, uid_rfid=:uid WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idconf", idConference);
    query.bindValue(":uid", uidRfid);

    return query.exec();
}

QSqlQueryModel* Participant::rechercher(QString filter)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT id, nom, idconference, uid_rfid FROM OUSSAMA.participant WHERE 1=1";
    if (!filter.isEmpty()) {
        sql += " AND (LOWER(nom) LIKE '%" + filter.toLower() + "%' OR LOWER(uid_rfid) LIKE '%" + filter.toLower() + "%')";
    }
    sql += " ORDER BY id";
    model->setQuery(sql);
    return model;
}

