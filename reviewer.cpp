#include "reviewer.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Reviewer::Reviewer()
    : m_id(0), m_nom(""), m_email(""), m_specialite(""), 
    m_affiliation(""), m_nbEvaluations(0), 
    m_scoreFiabilite(0.0), m_idPublication(0), m_lastError("")
{
}

Reviewer::Reviewer(int id, const QString &nom, const QString &email,
                   const QString &specialite, const QString &affiliation,
                   int nbEvaluations, double scoreFiabilite, int idPublication)
    : m_id(id), m_nom(nom), m_email(email), m_specialite(specialite),
      m_affiliation(affiliation), m_nbEvaluations(nbEvaluations),
      m_scoreFiabilite(scoreFiabilite), m_idPublication(idPublication), m_lastError("")
{
}

// Getters
int Reviewer::getId() const { return m_id; }
QString Reviewer::getNom() const { return m_nom; }
QString Reviewer::getEmail() const { return m_email; }
QString Reviewer::getSpecialite() const { return m_specialite; }
QString Reviewer::getAffiliation() const { return m_affiliation; }
int Reviewer::getNbEvaluations() const { return m_nbEvaluations; }
double Reviewer::getScoreFiabilite() const { return m_scoreFiabilite; }
int Reviewer::getIdPublication() const { return m_idPublication; }

// Setters
void Reviewer::setId(int id) { m_id = id; }
void Reviewer::setNom(const QString &nom) { m_nom = nom; }
void Reviewer::setEmail(const QString &email) { m_email = email; }
void Reviewer::setSpecialite(const QString &specialite) { m_specialite = specialite; }
void Reviewer::setAffiliation(const QString &affiliation) { m_affiliation = affiliation; }
void Reviewer::setNbEvaluations(int nb) { m_nbEvaluations = nb; }
void Reviewer::setScoreFiabilite(double score) { m_scoreFiabilite = score; }
void Reviewer::setIdPublication(int id) { m_idPublication = id; }

// Database operations
bool Reviewer::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return false;
    }

    QSqlQuery q(db);
    // Validate that ID PUBLICATION exists in PUBLICATION table if provided
    if (m_idPublication > 0) {
        QSqlQuery checkPub(db);
        checkPub.prepare("SELECT COUNT(*) FROM ADAM.PUBLICATION WHERE IDPUBLICATION = :idPub");
        checkPub.bindValue(":idPub", m_idPublication);
        if (!checkPub.exec() || !checkPub.next() || checkPub.value(0).toInt() == 0) {
            m_lastError = "ID Publication invalide ! Cet ID n'existe pas dans la table PUBLICATION.";
            return false;
        }
    }

    q.prepare("INSERT INTO ADAM.REVIEWER "
              "(IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, "
              "NBEVALUATION, SCOREFIABILITE, IDPUBLICATION) "
              "VALUES (:id, :nom, :mail, :specialite, :affiliation, "
              ":nbEval, :score, :idPub)");
    
    q.bindValue(":id", m_id);
    q.bindValue(":nom", m_nom);
    q.bindValue(":mail", m_email);
    q.bindValue(":specialite", m_specialite);
    q.bindValue(":affiliation", m_affiliation);
    q.bindValue(":nbEval", m_nbEvaluations);
    q.bindValue(":score", m_scoreFiabilite);
    q.bindValue(":idPub", m_idPublication > 0 ? QVariant(m_idPublication) : QVariant());

    if (!q.exec())
    {
        m_lastError = "Erreur INSERT: " + q.lastError().text();
        return false;
    }
    return true;
}

bool Reviewer::modifier()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return false;
    }

    QSqlQuery q(db);
    // Validate that ID PUBLICATION exists in PUBLICATION table if provided
    if (m_idPublication > 0) {
        QSqlQuery checkPub(db);
        checkPub.prepare("SELECT COUNT(*) FROM ADAM.PUBLICATION WHERE IDPUBLICATION = :idPub");
        checkPub.bindValue(":idPub", m_idPublication);
        if (!checkPub.exec() || !checkPub.next() || checkPub.value(0).toInt() == 0) {
            m_lastError = "ID Publication invalide ! Cet ID n'existe pas dans la table PUBLICATION.";
            return false;
        }
    }

    q.prepare("UPDATE ADAM.REVIEWER SET "
              "NOM = :nom, MAIL = :mail, SPECIALITE = :specialite, "
              "AFFILIATION = :affiliation, NBEVALUATION = :nbEval, "
              "SCOREFIABILITE = :score, IDPUBLICATION = :idPub "
              "WHERE IDREVIEWER = :id");
    
    q.bindValue(":id", m_id);
    q.bindValue(":nom", m_nom);
    q.bindValue(":mail", m_email);
    q.bindValue(":specialite", m_specialite);
    q.bindValue(":affiliation", m_affiliation);
    q.bindValue(":nbEval", m_nbEvaluations);
    q.bindValue(":score", m_scoreFiabilite);
    q.bindValue(":idPub", m_idPublication > 0 ? QVariant(m_idPublication) : QVariant());

    if (!q.exec())
    {
        m_lastError = "Erreur UPDATE: " + q.lastError().text();
        return false;
    }
    return true;
}

bool Reviewer::supprimer(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return false;
    }

    QSqlQuery q(db);
    q.prepare("DELETE FROM ADAM.REVIEWER WHERE IDREVIEWER = :id");
    q.bindValue(":id", id);
    if (!q.exec())
    {
        m_lastError = "Erreur DELETE: " + q.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Reviewer::afficher()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
        return nullptr;

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, "
                    "NBEVALUATION, SCOREFIABILITE, IDPUBLICATION FROM ADAM.REVIEWER", db);
    return model;
}

QSqlQueryModel* Reviewer::rechercher(const QString &filter)
{
    return rechercher(filter, filter, filter);
}

QSqlQueryModel* Reviewer::rechercher(const QString &idFilter,
                                     const QString &nomFilter,
                                     const QString &specialiteFilter)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return nullptr;
    }

    const QString idSearch = idFilter.trimmed();
    const QString nomSearch = nomFilter.trimmed();
    const QString specialiteSearch = specialiteFilter.trimmed();

    QSqlQuery q(db);
    QString sql = "SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, "
                  "NBEVALUATION, SCOREFIABILITE, IDPUBLICATION "
                  "FROM ADAM.REVIEWER WHERE 1=1";

    if (!idSearch.isEmpty())
        sql += " AND TO_CHAR(IDREVIEWER) LIKE :idFilter";
    if (!nomSearch.isEmpty())
        sql += " AND LOWER(NOM) LIKE :nomFilter";
    if (!specialiteSearch.isEmpty())
        sql += " AND LOWER(SPECIALITE) LIKE :specialiteFilter";

    sql += " ORDER BY IDREVIEWER";

    q.prepare(sql);

    if (!idSearch.isEmpty())
        q.bindValue(":idFilter", "%" + idSearch + "%");
    if (!nomSearch.isEmpty())
        q.bindValue(":nomFilter", "%" + nomSearch.toLower() + "%");
    if (!specialiteSearch.isEmpty())
        q.bindValue(":specialiteFilter", "%" + specialiteSearch.toLower() + "%");

    if (!q.exec())
    {
        m_lastError = "Erreur SEARCH: " + q.lastError().text();
        return nullptr;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(q);
    if (model->lastError().isValid())
    {
        m_lastError = "Erreur SEARCH: " + model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

QString Reviewer::getLastError() const
{
    return m_lastError;
}

// ================================================================
// METIER #4 : RAPPELS DEADLINES
// ================================================================

bool Reviewer::ensureDeadlineColumn()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
        return false;

    // Vérifie si la colonne existe déjà (Oracle: USER_TAB_COLUMNS)
    QSqlQuery check(db);
    check.prepare("SELECT COUNT(*) FROM USER_TAB_COLUMNS "
                  "WHERE TABLE_NAME = 'REVIEWER' AND COLUMN_NAME = 'DEADLINE_EVALUATION'");
    if (check.exec() && check.next() && check.value(0).toInt() == 0)
    {
        // La colonne n'existe pas → on la crée
        QSqlQuery alter(db);
        if (!alter.exec("ALTER TABLE ADAM.REVIEWER ADD DEADLINE_EVALUATION DATE"))
        {
            qDebug() << "[Deadline] Impossible d'ajouter la colonne:"
                     << alter.lastError().text();
            return false;
        }
        qDebug() << "[Deadline] Colonne DEADLINE_EVALUATION créée avec succès.";
    }
    return true;
}

// Retourne les reviewers dont la deadline est déjà dépassée
QSqlQueryModel* Reviewer::getReviewersEnRetard()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion BDD échouée.";
        return nullptr;
    }

    ensureDeadlineColumn();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, "
        "       IDPUBLICATION, DEADLINE_EVALUATION, "
        "       (SYSDATE - DEADLINE_EVALUATION) AS JOURS_RETARD "
        "FROM ADAM.REVIEWER "
        "WHERE DEADLINE_EVALUATION IS NOT NULL "
        "  AND DEADLINE_EVALUATION < SYSDATE "
        "  AND IDPUBLICATION IS NOT NULL "
        "ORDER BY DEADLINE_EVALUATION ASC",
        db
    );
    return model;
}

// Retourne les reviewers dont la deadline approche dans les N prochains jours
QSqlQueryModel* Reviewer::getReviewersProchesDeadline(int joursAvant)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion BDD échouée.";
        return nullptr;
    }

    ensureDeadlineColumn();

    QSqlQuery q(db);
    q.prepare(
        "SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, "
        "       IDPUBLICATION, DEADLINE_EVALUATION, "
        "       (DEADLINE_EVALUATION - SYSDATE) AS JOURS_RESTANTS "
        "FROM ADAM.REVIEWER "
        "WHERE DEADLINE_EVALUATION IS NOT NULL "
        "  AND DEADLINE_EVALUATION >= SYSDATE "
        "  AND DEADLINE_EVALUATION <= SYSDATE + :jours "
        "  AND IDPUBLICATION IS NOT NULL "
        "ORDER BY DEADLINE_EVALUATION ASC"
    );
    q.bindValue(":jours", joursAvant);
    q.exec();

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(q);
    return model;
}

// Définit ou met à jour la deadline d'un reviewer
bool Reviewer::setDeadline(int idReviewer, const QDate &deadline)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion BDD échouée.";
        return false;
    }

    ensureDeadlineColumn();

    QSqlQuery q(db);
    q.prepare("UPDATE ADAM.REVIEWER SET DEADLINE_EVALUATION = :dl "
              "WHERE IDREVIEWER = :id");
    q.bindValue(":dl", deadline);
    q.bindValue(":id", idReviewer);

    if (!q.exec())
    {
        m_lastError = "Erreur setDeadline: " + q.lastError().text();
        return false;
    }
    return true;
}

// Supprime la deadline d'un reviewer (NULL)
bool Reviewer::supprimerDeadline(int idReviewer)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open())
    {
        m_lastError = "Connexion BDD échouée.";
        return false;
    }

    QSqlQuery q(db);
    q.prepare("UPDATE ADAM.REVIEWER SET DEADLINE_EVALUATION = NULL "
              "WHERE IDREVIEWER = :id");
    q.bindValue(":id", idReviewer);

    if (!q.exec())
    {
        m_lastError = "Erreur supprimerDeadline: " + q.lastError().text();
        return false;
    }
    return true;
}
