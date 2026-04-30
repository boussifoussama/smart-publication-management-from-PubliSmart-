#ifndef REVIEWER_H
#define REVIEWER_H

#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Reviewer
{
public:
    Reviewer();
    Reviewer(int id, const QString &nom, const QString &email, 
             const QString &specialite, const QString &affiliation,
             int nbEvaluations, double scoreFiabilite, int idPublication = 0);

    // Getters
    int getId() const;
    QString getNom() const;
    QString getEmail() const;
    QString getSpecialite() const;
    QString getAffiliation() const;
    int getNbEvaluations() const;
    double getScoreFiabilite() const;
    int getIdPublication() const;

    // Setters
    void setId(int id);
    void setNom(const QString &nom);
    void setEmail(const QString &email);
    void setSpecialite(const QString &specialite);
    void setAffiliation(const QString &affiliation);
    void setNbEvaluations(int nb);
    void setScoreFiabilite(double score);
    void setIdPublication(int id);

    // Database operations
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString &filter);
    QSqlQueryModel* rechercher(const QString &idFilter,
                               const QString &nomFilter,
                               const QString &specialiteFilter);
    QString getLastError() const;

    // ── Métier #4 : Rappels Deadlines ──────────────────────────────
    // Retourne tous les reviewers dont la deadline est dépassée
    QSqlQueryModel* getReviewersEnRetard();
    // Retourne tous les reviewers avec deadline dans les prochains `joursAvant` jours
    QSqlQueryModel* getReviewersProchesDeadline(int joursAvant = 7);
    // Définit/met à jour la deadline d'un reviewer
    bool setDeadline(int idReviewer, const QDate& deadline);
    // Supprime la deadline d'un reviewer (le libère)
    bool supprimerDeadline(int idReviewer);
    // Vérifie si la colonne DEADLINE_EVALUATION existe dans OUSSAMA.REVIEWER
    static bool ensureDeadlineColumn();

private:
    int m_id;
    QString m_nom;
    QString m_email;
    QString m_specialite;
    QString m_affiliation;
    int m_nbEvaluations;
    double m_scoreFiabilite;
    int m_idPublication;
    QString m_lastError;
};

#endif // REVIEWER_H