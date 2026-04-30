#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Participant
{
    int id;
    QString nom;
    int idConference;
    QString uidRfid;
    QString lastError;

public:
    Participant();
    Participant(int, QString, int, QString);

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    int getIdConference() const { return idConference; }
    QString getUidRfid() const { return uidRfid; }

    // Setters
    void setId(int id) { this->id = id; }
    void setNom(QString n) { nom = n; }
    void setIdConference(int idC) { idConference = idC; }
    void setUidRfid(QString uid) { uidRfid = uid; }

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* rechercher(QString);
    QString getLastError() const { return lastError; }
};

#endif // PARTICIPANT_H
