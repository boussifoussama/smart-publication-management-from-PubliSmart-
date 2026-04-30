#include "participant.h"
#include <QSqlDatabase>
#include <QSqlError>

namespace {

QString detectFirstWorkingTable(QSqlDatabase &db, const QStringList &candidates)
{
    QSqlQuery q(db);
    for (const QString &tableRef : candidates) {
        if (q.exec("SELECT 1 FROM " + tableRef + " WHERE 1=0"))
            return tableRef;
    }
    return QString();
}

QString resolveParticipantTable(QSqlDatabase &db)
{
    return detectFirstWorkingTable(db, {"OUSSAMA.participant", "participant", "ADAM.participant"});
}

QString resolveConferenceTable(QSqlDatabase &db)
{
    return detectFirstWorkingTable(db, {"OUSSAMA.conference", "conference", "ADAM.conference"});
}

bool columnExists(QSqlDatabase &db, const QString &tableRef, const QString &columnName)
{
    QSqlQuery q(db);
    return q.exec("SELECT " + columnName + " FROM " + tableRef + " WHERE 1=0");
}

}

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
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return false;
    }

    const QString participantTable = resolveParticipantTable(db);
    const QString conferenceTable = resolveConferenceTable(db);
    if (participantTable.isEmpty()) {
        lastError = "Table participant introuvable (schemas OUSSAMA/ADAM et schema courant).";
        return false;
    }

    if (idConference > 0 && !conferenceTable.isEmpty()) {
        QSqlQuery checkConf(db);
        checkConf.prepare("SELECT COUNT(*) FROM " + conferenceTable + " WHERE IDCONFERENCE = :idconf");
        checkConf.bindValue(":idconf", idConference);
        if (!checkConf.exec() || !checkConf.next() || checkConf.value(0).toInt() == 0) {
            lastError = "ID conference invalide ! Cet ID n'existe pas dans la table conference.";
            return false;
        }
    }

    const bool hasUidColumn = columnExists(db, participantTable, "uid_rfid");

    if (hasUidColumn && !uidRfid.trimmed().isEmpty()) {
        QSqlQuery checkUid(db);
        checkUid.prepare("SELECT COUNT(*) FROM " + participantTable + " WHERE UPPER(TRIM(uid_rfid)) = :uid");
        checkUid.bindValue(":uid", uidRfid.trimmed().toUpper());
        if (!checkUid.exec() || !checkUid.next()) {
            lastError = "Impossible de verifier l'UID RFID avant insertion: " + checkUid.lastError().text();
            return false;
        }
        if (checkUid.value(0).toInt() > 0) {
            lastError = "Cet UID RFID existe deja dans la base. Utilisez un autre badge ou modifiez le participant existant.";
            return false;
        }
    }

    QSqlQuery query(db);
    QString sql = "INSERT INTO " + participantTable + " (id, nom, idconference";
    if (hasUidColumn)
        sql += ", uid_rfid";
    sql += ") VALUES (:id, :nom, :idconference";
    if (hasUidColumn)
        sql += ", :uid";
    sql += ")";

    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idconference", idConference);
    if (hasUidColumn)
        query.bindValue(":uid", uidRfid);

    if (!query.exec()) {
        lastError = "Erreur INSERT participant: " + query.lastError().text();
        return false;
    }
    lastError.clear();
    return true;
}

QSqlQueryModel* Participant::afficher()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return nullptr;
    }

    const QString participantTable = resolveParticipantTable(db);
    if (participantTable.isEmpty()) {
        lastError = "Table participant introuvable.";
        return nullptr;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    const bool hasUidColumn = columnExists(db, participantTable, "uid_rfid");
    const QString uidExpr = hasUidColumn ? "uid_rfid" : "NULL AS uid_rfid";
    model->setQuery("SELECT id, nom, idconference, " + uidExpr + " FROM " + participantTable + " ORDER BY id", db);

    if (model->lastError().isValid()) {
        lastError = "Erreur SELECT participant: " + model->lastError().text();
        delete model;
        return nullptr;
    }
    
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "ID Conference");
    model->setHeaderData(3, Qt::Horizontal, "UID RFID");

    return model;
}

bool Participant::supprimer(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return false;
    }

    const QString participantTable = resolveParticipantTable(db);
    if (participantTable.isEmpty()) {
        lastError = "Table participant introuvable.";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM " + participantTable + " WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        lastError = "Erreur DELETE participant: " + query.lastError().text();
        return false;
    }
    lastError.clear();
    return true;
}

bool Participant::modifier()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return false;
    }

    const QString participantTable = resolveParticipantTable(db);
    if (participantTable.isEmpty()) {
        lastError = "Table participant introuvable.";
        return false;
    }

    const bool hasUidColumn = columnExists(db, participantTable, "uid_rfid");
    QSqlQuery query(db);
    QString sql = "UPDATE " + participantTable + " SET nom=:nom, idconference=:idconf";
    if (hasUidColumn)
        sql += ", uid_rfid=:uid";
    sql += " WHERE id=:id";

    query.prepare(sql);
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idconf", idConference);
    if (hasUidColumn)
        query.bindValue(":uid", uidRfid);

    if (!query.exec()) {
        lastError = "Erreur UPDATE participant: " + query.lastError().text();
        return false;
    }
    lastError.clear();
    return true;
}

QSqlQueryModel* Participant::rechercher(QString filter)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return nullptr;
    }

    const QString participantTable = resolveParticipantTable(db);
    if (participantTable.isEmpty()) {
        lastError = "Table participant introuvable.";
        return nullptr;
    }

    const bool hasUidColumn = columnExists(db, participantTable, "uid_rfid");
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT id, nom, idconference, " + QString(hasUidColumn ? "uid_rfid" : "NULL AS uid_rfid") +
                  " FROM " + participantTable + " WHERE 1=1";
    if (!filter.isEmpty()) {
        sql += " AND (LOWER(nom) LIKE '%" + filter.toLower() + "%' OR LOWER(uid_rfid) LIKE '%" + filter.toLower() + "%')";
    }
    sql += " ORDER BY id";

    model->setQuery(sql, db);
    if (model->lastError().isValid()) {
        lastError = "Erreur SEARCH participant: " + model->lastError().text();
        delete model;
        return nullptr;
    }
    return model;
}

