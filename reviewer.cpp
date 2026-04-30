#include "reviewer.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

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

bool columnExists(QSqlDatabase &db, const QString &tableRef, const QString &columnName)
{
    QSqlQuery q(db);
    return q.exec("SELECT " + columnName + " FROM " + tableRef + " WHERE 1=0");
}

QString resolveReviewerTable(QSqlDatabase &db)
{
    return detectFirstWorkingTable(db, {
        "OUSSAMA.REVIEWER",
        "REVIEWER",
        "ADAM.REVIEWER"
    });
}

QString resolvePublicationTable(QSqlDatabase &db)
{
    return detectFirstWorkingTable(db, {
        "OUSSAMA.PUBLICATION",
        "PUBLICATION",
        "ADAM.PUBLICATION"
    });
}

QString resolveFirstExistingColumn(QSqlDatabase &db, const QString &tableRef, const QStringList &candidates)
{
    for (const QString &col : candidates) {
        if (columnExists(db, tableRef, col))
            return col;
    }
    return QString();
}

}

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

    const QString reviewerTable = resolveReviewerTable(db);
    if (reviewerTable.isEmpty()) {
        m_lastError = "Table REVIEWER introuvable (schema OUSSAMA/ADAM ou schema courant).";
        return false;
    }

    const QString publicationTable = resolvePublicationTable(db);
    const QString scoreColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                           {"SCOREFIABILITE", "SCORE_FIABILITE", "SCOREFIABILITE1"});
    const QString idPublicationColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                                   {"IDPUBLICATION", "ID_PUBLICATION"});

    QSqlQuery q(db);
    // Validate that ID PUBLICATION exists in PUBLICATION table if provided
    if (m_idPublication > 0 && !publicationTable.isEmpty() && !idPublicationColumn.isEmpty()) {
        QSqlQuery checkPub(db);
        checkPub.prepare("SELECT COUNT(*) FROM " + publicationTable + " WHERE IDPUBLICATION = :idPub");
        checkPub.bindValue(":idPub", m_idPublication);
        if (!checkPub.exec() || !checkPub.next() || checkPub.value(0).toInt() == 0) {
            m_lastError = "ID Publication invalide ! Cet ID n'existe pas dans la table PUBLICATION.";
            return false;
        }
    }

    QStringList columns = {"IDREVIEWER", "NOM", "MAIL", "SPECIALITE", "AFFILIATION", "NBEVALUATION"};
    QStringList values = {":id", ":nom", ":mail", ":specialite", ":affiliation", ":nbEval"};

    if (!scoreColumn.isEmpty()) {
        columns << scoreColumn;
        values << ":score";
    }
    if (!idPublicationColumn.isEmpty()) {
        columns << idPublicationColumn;
        values << ":idPub";
    }

    q.prepare("INSERT INTO " + reviewerTable + " (" + columns.join(", ") + ") "
              "VALUES (" + values.join(", ") + ")");
    
    q.bindValue(":id", m_id);
    q.bindValue(":nom", m_nom);
    q.bindValue(":mail", m_email);
    q.bindValue(":specialite", m_specialite);
    q.bindValue(":affiliation", m_affiliation);
    q.bindValue(":nbEval", m_nbEvaluations);
    if (!scoreColumn.isEmpty())
        q.bindValue(":score", m_scoreFiabilite);
    if (!idPublicationColumn.isEmpty())
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

    const QString reviewerTable = resolveReviewerTable(db);
    if (reviewerTable.isEmpty()) {
        m_lastError = "Table REVIEWER introuvable (schema OUSSAMA/ADAM ou schema courant).";
        return false;
    }

    const QString publicationTable = resolvePublicationTable(db);
    const QString scoreColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                           {"SCOREFIABILITE", "SCORE_FIABILITE", "SCOREFIABILITE1"});
    const QString idPublicationColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                                   {"IDPUBLICATION", "ID_PUBLICATION"});

    QSqlQuery q(db);
    // Validate that ID PUBLICATION exists in PUBLICATION table if provided
    if (m_idPublication > 0 && !publicationTable.isEmpty() && !idPublicationColumn.isEmpty()) {
        QSqlQuery checkPub(db);
        checkPub.prepare("SELECT COUNT(*) FROM " + publicationTable + " WHERE IDPUBLICATION = :idPub");
        checkPub.bindValue(":idPub", m_idPublication);
        if (!checkPub.exec() || !checkPub.next() || checkPub.value(0).toInt() == 0) {
            m_lastError = "ID Publication invalide ! Cet ID n'existe pas dans la table PUBLICATION.";
            return false;
        }
    }

    QStringList setClauses = {
        "NOM = :nom",
        "MAIL = :mail",
        "SPECIALITE = :specialite",
        "AFFILIATION = :affiliation",
        "NBEVALUATION = :nbEval"
    };
    if (!scoreColumn.isEmpty())
        setClauses << (scoreColumn + " = :score");
    if (!idPublicationColumn.isEmpty())
        setClauses << (idPublicationColumn + " = :idPub");

    q.prepare("UPDATE " + reviewerTable + " SET " + setClauses.join(", ") + " WHERE IDREVIEWER = :id");
    
    q.bindValue(":id", m_id);
    q.bindValue(":nom", m_nom);
    q.bindValue(":mail", m_email);
    q.bindValue(":specialite", m_specialite);
    q.bindValue(":affiliation", m_affiliation);
    q.bindValue(":nbEval", m_nbEvaluations);
    if (!scoreColumn.isEmpty())
        q.bindValue(":score", m_scoreFiabilite);
    if (!idPublicationColumn.isEmpty())
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

    const QString reviewerTable = resolveReviewerTable(db);
    if (reviewerTable.isEmpty()) {
        m_lastError = "Table REVIEWER introuvable (schema OUSSAMA/ADAM ou schema courant).";
        return false;
    }

    QSqlQuery q(db);
    q.prepare("DELETE FROM " + reviewerTable + " WHERE IDREVIEWER = :id");
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
    if (!db.isOpen() && !db.open()) {
        m_lastError = "Connexion a la base de donnees echouee: " + db.lastError().text();
        return nullptr;
    }

    const QString reviewerTable = resolveReviewerTable(db);
    if (reviewerTable.isEmpty()) {
        m_lastError = "Table REVIEWER introuvable (schema OUSSAMA/ADAM ou schema courant).";
        return nullptr;
    }

    const QString scoreColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                           {"SCOREFIABILITE", "SCORE_FIABILITE", "SCOREFIABILITE1"});
    const QString idPublicationColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                                   {"IDPUBLICATION", "ID_PUBLICATION"});

    const QString scoreExpr = scoreColumn.isEmpty() ? "0 AS SCOREFIABILITE"
                                                     : scoreColumn + " AS SCOREFIABILITE";
    const QString idPubExpr = idPublicationColumn.isEmpty() ? "NULL AS IDPUBLICATION"
                                                             : idPublicationColumn + " AS IDPUBLICATION";

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, "
                    "NBEVALUATION, " + scoreExpr + ", " + idPubExpr + " FROM " + reviewerTable, db);

    if (model->lastError().isValid()) {
        m_lastError = "Erreur SELECT: " + model->lastError().text();
        delete model;
        return nullptr;
    }

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

    const QString reviewerTable = resolveReviewerTable(db);
    if (reviewerTable.isEmpty()) {
        m_lastError = "Table REVIEWER introuvable (schema OUSSAMA/ADAM ou schema courant).";
        return nullptr;
    }

    const QString scoreColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                           {"SCOREFIABILITE", "SCORE_FIABILITE", "SCOREFIABILITE1"});
    const QString idPublicationColumn = resolveFirstExistingColumn(db, reviewerTable,
                                                                   {"IDPUBLICATION", "ID_PUBLICATION"});

    const QString scoreExpr = scoreColumn.isEmpty() ? "0 AS SCOREFIABILITE"
                                                     : scoreColumn + " AS SCOREFIABILITE";
    const QString idPubExpr = idPublicationColumn.isEmpty() ? "NULL AS IDPUBLICATION"
                                                             : idPublicationColumn + " AS IDPUBLICATION";

    const QString idSearch = idFilter.trimmed();
    const QString nomSearch = nomFilter.trimmed();
    const QString specialiteSearch = specialiteFilter.trimmed();

    QSqlQuery q(db);
    QString sql = "SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, "
                  "NBEVALUATION, " + scoreExpr + ", " + idPubExpr + " "
                  "FROM " + reviewerTable + " WHERE 1=1";

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
        if (!alter.exec("ALTER TABLE OUSSAMA.REVIEWER ADD DEADLINE_EVALUATION DATE"))
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
        "FROM OUSSAMA.REVIEWER "
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
        "FROM OUSSAMA.REVIEWER "
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
    q.prepare("UPDATE OUSSAMA.REVIEWER SET DEADLINE_EVALUATION = :dl "
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
    q.prepare("UPDATE OUSSAMA.REVIEWER SET DEADLINE_EVALUATION = NULL "
              "WHERE IDREVIEWER = :id");
    q.bindValue(":id", idReviewer);

    if (!q.exec())
    {
        m_lastError = "Erreur supprimerDeadline: " + q.lastError().text();
        return false;
    }
    return true;
}
