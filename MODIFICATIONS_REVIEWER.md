# 📝 Modifications du module Reviewer

## Problème résolu
Le **nombre d'évaluations**, le **score de fiabilité** et **l'ID de publication** n'apparaissaient pas dans le tableau des reviewers lors de l'exécution, bien qu'ils soient présents dans l'interface de saisie.

## Cause identifiée
La classe `Reviewer` avait bien les attributs `m_nbEvaluations` et `m_scoreFiabilite`, mais ils n'étaient pas inclus dans :
- Les requêtes SQL `SELECT` (`afficher()`, `rechercher()`)
- Les requêtes SQL `INSERT` (`ajouter()`)
- Les requêtes SQL `UPDATE` (`modifier()`)
- L'affichage du tableau dans `loadReviewerTable()`

## Modifications apportées

### 1. Fichier `reviewer.cpp`

#### `afficher()` - Ligne 123-133
Avant :
```sql
SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION FROM OUSSAMA.REVIEWER
```
Après :
```sql
SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION, SCOREFIABILITE, IDPUBLICATION FROM OUSSAMA.REVIEWER
```

#### `rechercher()` - Ligne 146-147
Avant :
```sql
SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION FROM OUSSAMA.REVIEWER
```
Après :
```sql
SELECT IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION, SCOREFIABILITE, IDPUBLICATION FROM OUSSAMA.REVIEWER
```

#### `ajouter()` - Ligne 52-63
Avant :
```sql
INSERT INTO OUSSAMA.REVIEWER (IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION) 
VALUES (:id, :nom, :mail, :specialite, :affiliation, :nbEval)
```
Après :
```sql
INSERT INTO OUSSAMA.REVIEWER (IDREVIEWER, NOM, MAIL, SPECIALITE, AFFILIATION, NBEVALUATION, SCOREFIABILITE) 
VALUES (:id, :nom, :mail, :specialite, :affiliation, :nbEval, :score)
```

#### `modifier()` - Ligne 83-94
Avant :
```sql
UPDATE OUSSAMA.REVIEWER SET NOM = :nom, MAIL = :mail, SPECIALITE = :specialite, 
AFFILIATION = :affiliation, NBEVALUATION = :nbEval WHERE IDREVIEWER = :id
```
Après :
```sql
UPDATE OUSSAMA.REVIEWER SET NOM = :nom, MAIL = :mail, SPECIALITE = :specialite, 
AFFILIATION = :affiliation, NBEVALUATION = :nbEval, SCOREFIABILITE = :score 
WHERE IDREVIEWER = :id
```

### 2. Fichier `smartmarket.cpp`

#### `loadReviewerTable()` - Ligne 2903-2928
Avant :
```cpp
table->setColumnCount(6);
table->setHorizontalHeaderLabels({
    "ID", "Nom", "Email", "Specialite", 
    "Affiliation", "Nb Evaluations"
});
```
Après :
```cpp
table->setColumnCount(8);
table->setHorizontalHeaderLabels({
    "ID", "Nom", "Email", "Specialite", 
    "Affiliation", "Nb Evaluations", "Score Fiabilite", "ID Publication"
});
```

## 📋 Étapes à suivre

### Étape 1 : Exécuter le script SQL
Avant de compiler et exécuter l'application, vous devez ajouter les colonnes manquantes dans la base de données Oracle :

1. Ouvrez Oracle SQL Developer ou votre outil de gestion de base de données
2. Exécutez le fichier `add_reviewer_columns.sql` :
```sql
-- Ajouter la colonne SCOREFIABILITE (score de fiabilité)
ALTER TABLE OUSSAMA.REVIEWER ADD SCOREFIABILITE NUMBER(3,2) DEFAULT 0.0;

-- Ajouter la colonne IDPUBLICATION (ID de la publication assignée)
ALTER TABLE OUSSAMA.REVIEWER ADD IDPUBLICATION NUMBER(10);
```

### Étape 2 : Compiler le projet
```bash
# Dans le dossier du projet
qmake SmartMarket.pro
make
```

### Étape 3 : Exécuter l'application
```bash
./SmartMarket
```

## ✅ Vérification
Après ces modifications :
- Le tableau des reviewers affichera **8 colonnes** au lieu de 6
- Les colonnes **"Score Fiabilite"** et **"ID Publication"** seront visibles
- Lors de l'ajout d'un reviewer, le score de fiabilité sera enregistré dans la base de données
- Lors de la modification d'un reviewer, le score de fiabilité sera mis à jour

## 📊 Structure finale de la table REVIEWER
| Colonne | Type | Description |
|---------|------|-------------|
| IDREVIEWER | NUMBER(10) | ID du reviewer (PK) |
| NOM | VARCHAR2(255) | Nom du reviewer |
| MAIL | VARCHAR2(255) | Email du reviewer |
| SPECIALITE | VARCHAR2(255) | Spécialité |
| AFFILIATION | VARCHAR2(255) | Affiliation |
| NBEVALUATION | NUMBER(10) | Nombre d'évaluations |
| **SCOREFIABILITE** | **NUMBER(3,2)** | **Score de fiabilité (0.00 - 9.99)** |
| **IDPUBLICATION** | **NUMBER(10)** | **ID de la publication assignée (FK)** |