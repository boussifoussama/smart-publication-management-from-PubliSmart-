#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMap>

class Equipement
{
    int     idEquipement;
    QString nom;
    QString type;       // "Audio", "Video", "Informatique", "Mobilier", "Autre"
    QString etat;       // "Bon", "Moyen", "Mauvais"
    int     quantite;
    QDate   dateAchat;
    double  prix;
    int     idConference;

public:
    // ── Constructors ───────────────────────────────────────────
    Equipement();
    Equipement(int idEquipement, QString nom, QString type, QString etat,
               int quantite, QDate dateAchat, double prix, int idConference);

    // ── Getters ────────────────────────────────────────────────
    int     getIdEquipement()  const { return idEquipement; }
    QString getNom()           const { return nom; }
    QString getType()          const { return type; }
    QString getEtat()          const { return etat; }
    int     getQuantite()      const { return quantite; }
    QDate   getDateAchat()     const { return dateAchat; }
    double  getPrix()          const { return prix; }
    int     getIdConference()  const { return idConference; }

    // ── Setters ────────────────────────────────────────────────
    void setIdEquipement(int id)   { idEquipement = id; }
    void setNom(QString n)         { nom = n; }
    void setType(QString t)        { type = t; }
    void setEtat(QString e)        { etat = e; }
    void setQuantite(int q)        { quantite = q; }
    void setDateAchat(QDate d)     { dateAchat = d; }
    void setPrix(double p)         { prix = p; }
    void setIdConference(int id)   { idConference = id; }

    // ── CRUD ───────────────────────────────────────────────────
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();

    // ── Search / Sort ──────────────────────────────────────────
    QSqlQueryModel* rechercher(const QString &nom, const QString &type);
    QSqlQueryModel* trierParNom(const QString &ordre = "ASC");

    // ── Statistics / Calculations ──────────────────────────────
    double               coutTotalParConference(int idConference);
    QMap<QString,double> coutParType();
    double               valeurTotaleStock();
    QString               getStatutStock() const;
    QPair<QString,double> equipementLePlusCher();
    QMap<QString,int>     statistiquesParEtat();
    QList<Equipement>     equipementsEnAlerte();
    // ── Arduino ────────────────────────────────────────────────
    bool decrementerQuantite(int idEquipement);
};

#endif // EQUIPEMENT_H
