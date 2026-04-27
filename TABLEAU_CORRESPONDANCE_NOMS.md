# 📋 TABLEAU COMPLET DE CORRESPONDANCE DES NOMS

## 🔐 PAGE LOGIN

### Champs de saisie

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `lineEdit_8` | `lineEditLoginEmail` | QLineEdit | Email de connexion |
| `lineEdit_10` | `lineEditLoginPassword` | QLineEdit | Mot de passe |
| `lineEdit_11` | `lineEditLoginMotDePasseOublie` | QLineEdit | Email (mot de passe oublié) |

### Boutons

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `pushButton_15` | `btnLoginEntrer` | QPushButton | Bouton "Entrer" |
| `pushButton_13` | `btnLoginEnvoyer` | QPushButton | Bouton "Envoyer" |

---

## 📄 PAGE PUBLICATION

### Tableaux

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `tableView` | `tableViewPublications` | QTableView | Tableau principal |
| `tableView_2` | `tableViewPublicationsFiltered` | QTableView | Tableau filtré |
| `tabWidget` | `tabWidgetPublication` | QTabWidget | Widget des onglets |

### Groupe Ajout/Modification (GroupBox avec titre "ajout de publication")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `lineEdit_16` | `lineEditPubTitre` | QLineEdit | Titre |
| `lineEdit_17` | `lineEditPubSource` | QLineEdit | Source |
| `lineEdit_18` | `lineEditPubCreateur` | QLineEdit | Nom créateur |
| `comboBox_5` | `comboBoxPubDomaine` | QComboBox | Domaine |
| `dateEdit_5` | `dateEditPubDate` | QDateEdit | Date de publication |
| `lineEdit_19` | `lineEditPubIdModif` | QLineEdit | ID à modifier |
| `pushButton_17` | `btnAjouterPublication` | QPushButton | Bouton "ajouter" |
| `pushButton_18` | `btnModifierPublication` | QPushButton | Bouton "modifier" |

### Groupe Suppression (GroupBox avec titre "supprimer une publication")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `lineEdit_8` | `lineEditPubSupprimer` | QLineEdit | ID à supprimer |
| `pushButton_8` | `btnSupprimerPublication` | QPushButton | Bouton "Effacer" |

### Groupe Recherche (GroupBox avec titre "Critères de recherche")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `lineEdit_20` | `lineEditRechercheTitre` | QLineEdit | Titre |
| `lineEdit_21` | `lineEditRechercheSource` | QLineEdit | Source |
| `comboBox_2` | `comboBoxRechercheDomaine` | QComboBox | Domaine |
| `lineEdit_23` | `lineEditRechercheScore` | QLineEdit | Score |
| `lineEdit_22` | `lineEditRechercheStatut` | QLineEdit | Statut |
| `pushButton_5` | `btnReinitialiser` | QPushButton | Bouton "Réinitialiser" |
| `pushButton_9` | `btnRechercherPublication` | QPushButton | Bouton "Rechercher" |

### Groupe Export (GroupBox avec titre "export des publications")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `radioButton_5` | `radioButtonExportToutes` | QRadioButton | Radio "Toutes" |
| `radioButton_6` | `radioButtonExportFiltrees` | QRadioButton | Radio "Filtrées" |
| `radioButton_7` | `radioButtonExportPDF` | QRadioButton | Radio "PDF" |
| `radioButton_8` | `radioButtonExportExcel` | QRadioButton | Radio "Excel" |
| `checkBox_3` | `checkBoxIncludeVides` | QCheckBox | Case "Inclure champs vides" |
| `pushButton_19` | `btnExporterPublication` | QPushButton | Bouton "Exporter" |

---

## 📊 ONGLET 2 - ANALYSE

### Groupe Similarité (GroupBox "choisir les publications pour comparer")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `lineEdit_5` | `lineEditSimilariteId1` | QLineEdit | ID publication 1 |
| `lineEdit_6` | `lineEditSimilariteId2` | QLineEdit | ID publication 2 |
| `pushButton_6` | `btnChoisirSimilarite` | QPushButton | Bouton "choisir" |
| `progressBar` | `progressBarSimilarite` | QProgressBar | Barre de progression |

### Groupe Complétude (GroupBox "choisir la publication pour vérifier sa completude")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `lineEdit_7` | `lineEditCompletudId` | QLineEdit | ID publication |
| `pushButton_7` | `btnChoisirCompletude` | QPushButton | Bouton "choisir" |
| `progressBar_2` | `progressBarCompletude` | QProgressBar | Barre de progression |

### Widgets pour graphiques

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `chartWidget` | `chartWidgetStatut` | QWidget | Widget pour Pie Chart |
| `domainChartWidget` | `domainChartWidget` | QWidget | Widget pour Bar Chart |

---

## 🎨 SIDEBAR (GroupBox avec objectName "groupBox_11")

| Votre nom original | Nouveau nom REQUIS | Widget | Description |
|-------------------|-------------------|---------|-------------|
| `pushButton_13` | `btnSidebarPublications` | QPushButton | Bouton "Publications" |
| `pushButton_14` | `btnSidebarReviewers` | QPushButton | Bouton "Reviewers" |
| `pushButton_15` | `btnSidebarConferences` | QPushButton | Bouton "Conférences" |
| `pushButton_16` | `btnSidebarEquipements` | QPushButton | Bouton "Equipements" |

**IMPORTANT** : Le GroupBox de la sidebar DOIT s'appeler `groupBox_11` pour que les styles CSS s'appliquent !

---

## 🖼️ IMAGES (Labels avec images)

### Page Login

| Votre nom original | Description | Image |
|-------------------|-------------|-------|
| `label_29` | Logo SmartMarket | `imagetouse.png-removebg-preview.png` |
| `label_5` | Image droite | `menuimage.png` |

### Page Publication - Sidebar

| Votre nom original | Description | Image |
|-------------------|-------------|-------|
| `label_29` | Logo | `imagetouse.png-removebg-preview.png` |
| `label_30` | Icône Publications | `publication icon.png` |
| `label_31` | Icône Reviewers | `reviewer icon.png` |
| `label_32` | Icône Conférences | `conference icon.png` |
| `label_33` | Icône Équipements | `equipement icon.png` |

**Vous pouvez garder ces noms de labels, ils n'ont pas besoin d'être renommés.**

---

## ✅ COMMENT UTILISER CE TABLEAU

### Méthode 1 : Dans Qt Designer

1. Ouvrir `smartmarket.ui` dans Qt Designer
2. Sélectionner un widget
3. Dans l'inspecteur de propriétés (colonne de droite), chercher `objectName`
4. Changer selon le tableau ci-dessus

### Méthode 2 : Dans le XML

1. Ouvrir `smartmarket.ui` avec un éditeur de texte
2. Chercher `<widget class="..." name="ANCIEN_NOM">`
3. Remplacer par `<widget class="..." name="NOUVEAU_NOM">`

### Exemple de remplacement XML

**Avant :**
```xml
<widget class="QLineEdit" name="lineEdit_8">
```

**Après :**
```xml
<widget class="QLineEdit" name="lineEditLoginEmail">
```

---

## 🔍 VÉRIFICATION RAPIDE

Après renommage, vérifiez ces éléments clés :

### Page Login
- [ ] `lineEditLoginEmail` existe
- [ ] `lineEditLoginPassword` existe
- [ ] `btnLoginEntrer` existe
- [ ] `btnLoginEnvoyer` existe

### Page Publication - Principaux
- [ ] `tableViewPublications` existe
- [ ] `btnAjouterPublication` existe
- [ ] `btnSupprimerPublication` existe
- [ ] `btnRechercherPublication` existe

### Sidebar
- [ ] `groupBox_11` (GroupBox de la sidebar)
- [ ] `btnSidebarPublications` existe

### Graphiques
- [ ] `chartWidgetStatut` existe
- [ ] `domainChartWidget` existe

---

## 💡 ASTUCE

Si vous avez beaucoup de widgets à renommer, utilisez la fonction "Rechercher et remplacer" de votre éditeur de texte :

1. Ouvrir `smartmarket.ui` avec Notepad++ ou VSCode
2. Ctrl+H (Rechercher et remplacer)
3. Chercher : `name="lineEdit_8"`
4. Remplacer par : `name="lineEditLoginEmail"`
5. Remplacer tout

**Répétez pour chaque ligne du tableau !**

---

**Avec ce tableau, vous pouvez garder 100% de votre style original ! 🎨**
