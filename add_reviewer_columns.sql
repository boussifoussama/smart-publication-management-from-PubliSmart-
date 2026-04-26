-- Script SQL pour ajouter les colonnes manquantes dans la table REVIEWER
-- Exécuter ce script dans Oracle SQL Developer ou via la ligne de commande

-- Ajouter la colonne SCOREFIABILITE (score de fiabilité)
ALTER TABLE ADAM.REVIEWER ADD SCOREFIABILITE NUMBER(3,2) DEFAULT 0.0;

-- Ajouter la colonne IDPUBLICATION (ID de la publication assignée)
ALTER TABLE ADAM.REVIEWER ADD IDPUBLICATION NUMBER(10);

-- Mettre à jour les enregistrements existants avec des valeurs par défaut
UPDATE ADAM.REVIEWER SET SCOREFIABILITE = 0.0 WHERE SCOREFIABILITE IS NULL;
UPDATE ADAM.REVIEWER SET IDPUBLICATION = NULL WHERE IDPUBLICATION IS NULL;

-- Vérifier que les colonnes ont été ajoutées
DESCRIBE ADAM.REVIEWER;

-- Afficher le contenu de la table pour vérification
SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION, SCOREFIABILITE, IDPUBLICATION 
FROM ADAM.REVIEWER;