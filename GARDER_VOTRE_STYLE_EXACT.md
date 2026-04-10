# 🎨 COMMENT GARDER EXACTEMENT VOTRE STYLE ORIGINAL

## ⚠️ PROBLÈME IDENTIFIÉ

Vous avez raison ! J'ai modifié certains éléments de style. Voici comment garder **EXACTEMENT** votre style original.

---

## ✅ SOLUTION : Utiliser Qt Designer pour combiner les interfaces

### Étape 1 : Ouvrir VOS fichiers originaux

1. **Gardez vos 2 projets originaux** :
   - Projet Login (avec votre interface de login)
   - Projet Publication (avec votre interface de publication)

2. **Ne supprimez RIEN** de vos projets originaux

---

### Étape 2 : Créer le projet SmartMarket intégré

1. **Créer un nouveau projet Qt** :
   ```
   Fichier → Nouveau projet → Application Qt Widgets
   Nom : SmartMarket
   ```

2. **Copier les fichiers que je vous ai fournis** :
   - `smartmarket.h` (gardez-le, il contient les slots)
   - `smartmarket.cpp` (gardez-le, il contient la logique)
   - `main.cpp` (gardez-le)
   - `SmartMarket.pro` (gardez-le, il a Qt Charts)

---

### Étape 3 : Créer l'interface dans Qt Designer (IMPORTANT)

#### 3.1 Ouvrir smartmarket.ui dans Qt Designer

#### 3.2 Ajouter le StackedWidget

1. **Supprimer** tous les widgets du `centralwidget`
2. **Ajouter** un `QStackedWidget` nommé `stackedWidgetMain`
3. **Redimensionner** pour remplir toute la fenêtre (1298 x 730)

#### 3.3 Créer Page 0 - Login

1. **Ajouter une page** au StackedWidget (clic droit → "Insérer une page")
2. **Ouvrir votre fichier login original** dans un AUTRE Qt Designer
3. **Sélectionner TOUS les widgets** de votre page login (Ctrl+A)
4. **Copier** (Ctrl+C)
5. **Retourner** dans smartmarket.ui
6. **Coller** dans la Page 0 du StackedWidget (Ctrl+V)

#### 3.4 Renommer les objets de la page Login

**TRÈS IMPORTANT** : Renommer selon ces noms (pour que le code fonctionne) :

| Ancien nom (votre projet) | Nouveau nom (requis) |
|---------------------------|----------------------|
| `lineEdit_8` (email) | `lineEditLoginEmail` |
| `lineEdit_10` (password) | `lineEditLoginPassword` |
| `lineEdit_11` (mot de passe oublié) | `lineEditLoginMotDePasseOublie` |
| `pushButton_15` (Entrer) | `btnLoginEntrer` |
| `pushButton_13` (Envoyer) | `btnLoginEnvoyer` |

**Comment renommer** :
- Clic droit sur le widget → "Changer objectName..."
- Ou via l'inspecteur de propriétés (colonne de droite)

#### 3.5 Créer Page 1 - Publication

1. **Ajouter une autre page** au StackedWidget
2. **Ouvrir votre fichier publication original** dans un AUTRE Qt Designer
3. **Sélectionner TOUS les widgets** (Ctrl+A)
4. **Copier** (Ctrl+C)
5. **Retourner** dans smartmarket.ui
6. **Coller** dans la Page 1 du StackedWidget (Ctrl+V)

#### 3.6 Renommer les objets de la page Publication

**Objets principaux à renommer** :

| Ancien nom | Nouveau nom |
|------------|-------------|
| `tableView` | `tableViewPublications` |
| `tableView_2` | `tableViewPublicationsFiltered` |
| `lineEdit_16` (titre) | `lineEditPubTitre` |
| `lineEdit_17` (source) | `lineEditPubSource` |
| `lineEdit_18` (créateur) | `lineEditPubCreateur` |
| `comboBox_5` (domaine) | `comboBoxPubDomaine` |
| `dateEdit_5` (date) | `dateEditPubDate` |
| `lineEdit_19` (ID modif) | `lineEditPubIdModif` |
| `lineEdit_8` (ID supprimer) | `lineEditPubSupprimer` |
| `pushButton_17` (Ajouter) | `btnAjouterPublication` |
| `pushButton_18` (Modifier) | `btnModifierPublication` |
| `pushButton_8` (Effacer) | `btnSupprimerPublication` |
| `pushButton_9` (Rechercher) | `btnRechercherPublication` |
| `pushButton_5` (Réinitialiser) | `btnReinitialiser` |
| `pushButton_19` (Exporter) | `btnExporterPublication` |

**Objets de recherche** :

| Ancien nom | Nouveau nom |
|------------|-------------|
| `lineEdit_20` (titre) | `lineEditRechercheTitre` |
| `lineEdit_21` (source) | `lineEditRechercheSource` |
| `comboBox_2` (domaine) | `comboBoxRechercheDomaine` |
| `lineEdit_23` (score) | `lineEditRechercheScore` |
| `lineEdit_22` (statut) | `lineEditRechercheStatut` |

**Objets d'export** :

| Ancien nom | Nouveau nom |
|------------|-------------|
| `radioButton_5` | `radioButtonExportToutes` |
| `radioButton_6` | `radioButtonExportFiltrees` |
| `radioButton_7` | `radioButtonExportPDF` |
| `radioButton_8` | `radioButtonExportExcel` |
| `checkBox_3` | `checkBoxIncludeVides` |

**Objets d'analyse (onglet 2)** :

| Ancien nom | Nouveau nom |
|------------|-------------|
| `lineEdit_5` (ID pub 1) | `lineEditSimilariteId1` |
| `lineEdit_6` (ID pub 2) | `lineEditSimilariteId2` |
| `pushButton_6` (Choisir similarité) | `btnChoisirSimilarite` |
| `lineEdit_7` (ID complétude) | `lineEditCompletudId` |
| `pushButton_7` (Choisir complétude) | `btnChoisirCompletude` |
| `progressBar` | `progressBarSimilarite` |
| `progressBar_2` | `progressBarCompletude` |
| `chartWidget` | `chartWidgetStatut` |
| `domainChartWidget` | `domainChartWidget` |

**Sidebar** :

| Ancien nom | Nouveau nom |
|------------|-------------|
| `pushButton_13` | `btnSidebarPublications` |
| `pushButton_14` | `btnSidebarReviewers` |
| `pushButton_15` | `btnSidebarConferences` |
| `pushButton_16` | `btnSidebarEquipements` |

---

### Étape 4 : Sauvegarder et compiler

1. **Sauvegarder** smartmarket.ui
2. **Fermer** Qt Designer
3. **Ouvrir** le projet dans Qt Creator
4. **Build → Nettoyer tout**
5. **Build → Reconstruire tout**
6. **Exécuter**

---

## 🎯 RÉSULTAT

✅ **100% de votre style original conservé** :
- Mêmes couleurs
- Mêmes positions de widgets
- Mêmes images
- Mêmes boutons
- Même design exact

✅ **Navigation fonctionnelle** :
- Login → Publication après connexion
- Tous les boutons connectés

---

## 📝 ALTERNATIVE : Modifier directement le XML

Si vous préférez modifier le XML directement :

1. **Ouvrir** `smartmarket.ui` avec un éditeur de texte
2. **Copier** tout le contenu entre `<widget class="QWidget" name="centralwidget">` et `</widget>` de votre fichier login original
3. **Coller** dans une page du StackedWidget
4. **Faire pareil** pour la publication
5. **Renommer** les objectName selon le tableau ci-dessus

---

## ⚠️ NOTES IMPORTANTES

1. **Ne modifiez PAS** smartmarket.h et smartmarket.cpp
2. **Ne modifiez PAS** les noms de slots dans le code
3. **Modifiez UNIQUEMENT** les objectName dans le fichier .ui
4. **Gardez** resources.qrc avec vos images

---

## 🔧 DÉPANNAGE

### Les images ne s'affichent pas
→ Vérifier le fichier `resources.qrc` et les chemins d'images

### Les boutons ne répondent pas
→ Vérifier que les objectName correspondent exactement

### Erreur de compilation
→ Nettoyer et reconstruire

---

**En suivant cette méthode, vous gardez 100% votre style original ! 🎨**
