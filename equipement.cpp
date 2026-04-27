#include "equipement.h"
#include <QSqlError>
#include <QDebug>

// ================================================================
// CONSTRUCTORS
// ================================================================
Equipement::Equipement()
    : idEquipement(0), nom(""), type(""), etat("Bon"),
      quantite(1), dateAchat(QDate::currentDate()), prix(0.0), idConference(0)
{}

Equipement::Equipement(int idEquipement, QString nom, QString type, QString etat,
                       int quantite, QDate dateAchat, double prix, int idConference)
    : idEquipement(idEquipement), nom(nom), type(type), etat(etat),
      quantite(quantite), dateAchat(dateAchat), prix(prix), idConference(idConference)
{}

// ================================================================
// CRUD
// ================================================================

bool Equipement::ajouter()
{
    QSqlQuery q;
    // Column order matches real Oracle schema:
    // IDEQUIPEMENT, NOM, TYPE, ETAT, QUANTITE, DATEACHAT, PRIX, IDCONFERENCE
    q.prepare(
        "INSERT INTO OUSSAMA.equipement "
        "(idequipement, nom, type, etat, quantite, dateachat, prix, idconference) "
        "VALUES (:id, :nom, :type, :etat, :quantite, :dateachat, :prix, :idconf)"
    );
    q.bindValue(":id",        idEquipement);
    q.bindValue(":nom",       nom);
    q.bindValue(":type",      type);
    q.bindValue(":etat",      etat);
    q.bindValue(":quantite",  quantite);
    q.bindValue(":dateachat", dateAchat.isValid() ? QVariant(dateAchat) : QVariant(QVariant::Date));
    q.bindValue(":prix",      prix);
    // idConference = 0 means NULL (no conference assigned)
    q.bindValue(":idconf",    idConference > 0 ? QVariant(idConference) : QVariant(QVariant::Int));

    if (!q.exec()) {
        qDebug() << "[Equipement::ajouter]" << q.lastError().text();
        return false;
    }
    return true;
}

bool Equipement::modifier()
{
    QSqlQuery q;
    q.prepare(
        "UPDATE OUSSAMA.equipement "
        "SET nom=:nom, type=:type, etat=:etat, quantite=:quantite, "
        "    dateachat=:dateachat, prix=:prix, idconference=:idconf "
        "WHERE idequipement=:id"
    );
    q.bindValue(":nom",       nom);
    q.bindValue(":type",      type);
    q.bindValue(":etat",      etat);
    q.bindValue(":quantite",  quantite);
    q.bindValue(":dateachat", dateAchat.isValid() ? QVariant(dateAchat) : QVariant(QVariant::Date));
    q.bindValue(":prix",      prix);
    q.bindValue(":idconf",    idConference > 0 ? QVariant(idConference) : QVariant(QVariant::Int));
    q.bindValue(":id",        idEquipement);

    if (!q.exec()) {
        qDebug() << "[Equipement::modifier]" << q.lastError().text();
        return false;
    }
    return true;
}

bool Equipement::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM OUSSAMA.equipement WHERE idequipement=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "[Equipement::supprimer]" << q.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Equipement::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT idequipement, nom, type, etat, quantite, dateachat, prix, idconference "
        "FROM OUSSAMA.equipement ORDER BY idequipement"
    );
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Type");
    model->setHeaderData(3, Qt::Horizontal, "État");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Date Achat");
    model->setHeaderData(6, Qt::Horizontal, "Prix (TND)");
    model->setHeaderData(7, Qt::Horizontal, "ID Conférence");
    return model;
}

// ================================================================
// SEARCH / SORT
// ================================================================

QSqlQueryModel* Equipement::rechercher(const QString &nom, const QString &type)
{
    bool filterNom  = !nom.isEmpty();
    bool filterType = !type.isEmpty() && type != "Tous";

    QString sql =
        "SELECT idequipement, nom, type, etat, quantite, "
        "       dateachat, prix, idconference "
        "FROM OUSSAMA.equipement WHERE 1=1";

    if (filterNom)  sql += " AND LOWER(nom) LIKE :nom";
    if (filterType) sql += " AND LOWER(type) = :type";
    sql += " ORDER BY nom";

    QSqlQuery q;
    q.prepare(sql);
    if (filterNom)  q.bindValue(":nom",  "%" + nom.toLower() + "%");
    if (filterType) q.bindValue(":type", type.toLower());
    q.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(q));
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Type");
    model->setHeaderData(3, Qt::Horizontal, "État");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Date Achat");
    model->setHeaderData(6, Qt::Horizontal, "Prix (TND)");
    model->setHeaderData(7, Qt::Horizontal, "ID Conférence");
    return model;
}

QSqlQueryModel* Equipement::trierParNom(const QString &ordre)
{
    const QString safeOrdre = (ordre.toUpper() == "DESC") ? "DESC" : "ASC";

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT idequipement, nom, type, etat, quantite, dateachat, prix, idconference "
        "FROM OUSSAMA.equipement ORDER BY nom " + safeOrdre
    );
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Type");
    model->setHeaderData(3, Qt::Horizontal, "État");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");
    model->setHeaderData(5, Qt::Horizontal, "Date Achat");
    model->setHeaderData(6, Qt::Horizontal, "Prix (TND)");
    model->setHeaderData(7, Qt::Horizontal, "ID Conférence");
    return model;
}

// ================================================================
// STATISTICS / CALCULATIONS
// ================================================================

double Equipement::coutTotalParConference(int idConference)
{
    QSqlQuery q;
    q.prepare(
        "SELECT SUM(prix * quantite) FROM OUSSAMA.equipement WHERE idconference=:id"
    );
    q.bindValue(":id", idConference);
    if (q.exec() && q.next())
        return q.value(0).toDouble();
    return 0.0;
}

QMap<QString,double> Equipement::coutParType()
{
    QMap<QString,double> result;
    QSqlQuery q;
    q.exec(
        "SELECT type, SUM(prix * quantite) FROM OUSSAMA.equipement GROUP BY type ORDER BY type"
    );
    while (q.next())
        result[q.value(0).toString()] = q.value(1).toDouble();
    return result;
}

double Equipement::valeurTotaleStock()
{
    QSqlQuery q;
    q.exec("SELECT SUM(prix * quantite) FROM OUSSAMA.equipement");
    if (q.next())
        return q.value(0).toDouble();
    return 0.0;
}
// ── Statut automatique basé sur quantite ────────────────────
QString Equipement::getStatutStock() const
{
    if (quantite == 0)  return "Rupture";
    if (quantite <= 5)  return "Faible stock";
    return "Disponible";
}

// ── Équipement le plus cher (requête DB) ────────────────────
QPair<QString,double> Equipement::equipementLePlusCher()
{
    QSqlQuery q;
    q.exec(
        "SELECT nom, prix FROM OUSSAMA.equipement "
        "ORDER BY prix DESC FETCH FIRST 1 ROWS ONLY"
        );
    if (q.next())
        return {q.value(0).toString(), q.value(1).toDouble()};
    return {"—", 0.0};
}

// ── Nombre d'équipements par état physique ──────────────────
QMap<QString,int> Equipement::statistiquesParEtat()
{
    QMap<QString,int> result;
    QSqlQuery q;
    q.exec(
        "SELECT etat, COUNT(*) FROM OUSSAMA.equipement "
        "GROUP BY etat ORDER BY etat"
        );
    while (q.next())
        result[q.value(0).toString()] = q.value(1).toInt();
    return result;
}

// ── Liste des équipements en alerte (quantite <= 5) ─────────
QList<Equipement> Equipement::equipementsEnAlerte()
{
    QList<Equipement> liste;
    QSqlQuery q;
    q.exec(
        "SELECT idequipement, nom, type, etat, quantite, "
        "       dateachat, prix, idconference "
        "FROM OUSSAMA.equipement "
        "WHERE quantite <= 5 ORDER BY quantite ASC"
        );
    while (q.next()) {
        liste.append(Equipement(
            q.value(0).toInt(),
            q.value(1).toString(),
            q.value(2).toString(),
            q.value(3).toString(),
            q.value(4).toInt(),
            q.value(5).toDate(),
            q.value(6).toDouble(),
            q.value(7).toInt()
            ));
    }
    return liste;
}
// ════════════════════════════════════════════════════════════
//  ARDUINO — Décrémente quantité d'un équipement
// ════════════════════════════════════════════════════════════
bool Equipement::decrementerQuantite(int idEquipement)
{
    // Vérifier que l'équipement existe et a du stock
    QSqlQuery check;
    check.prepare(
        "SELECT quantite FROM OUSSAMA.equipement "
        "WHERE idequipement = :id"
        );
    check.bindValue(":id", idEquipement);

    if (!check.exec() || !check.next()) {
        qDebug() << "[Arduino] Équipement ID" << idEquipement << "introuvable.";
        return false;
    }

    int qteActuelle = check.value(0).toInt();
    if (qteActuelle <= 0) {
        qDebug() << "[Arduino] Équipement ID" << idEquipement << "déjà en rupture.";
        return false;
    }

    // Décrémenter
    QSqlQuery update;
    update.prepare(
        "UPDATE OUSSAMA.equipement "
        "SET quantite = quantite - 1 "
        "WHERE idequipement = :id AND quantite > 0"
        );
    update.bindValue(":id", idEquipement);

    if (!update.exec()) {
        qDebug() << "[Arduino] Erreur UPDATE :" << update.lastError().text();
        return false;
    }

    qDebug() << "[Arduino] Quantité décrémentée pour ID" << idEquipement;
    return true;
}
