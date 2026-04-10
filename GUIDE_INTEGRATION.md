# 🎯 PROJET SMARTMARKET - GUIDE D'INTÉGRATION COMPLET

## 📁 Structure du Projet

```
SmartMarket/
├── SmartMarket.pro          # Fichier de configuration Qt
├── main.cpp                 # Point d'entrée de l'application
├── smartmarket.h            # En-tête de la classe principale
├── smartmarket.cpp          # Implémentation avec toute la logique
├── smartmarket.ui           # Interface graphique (StackedWidget)
├── resources.qrc            # Fichier de ressources (images)
└── images/                  # Dossier contenant vos images
```

## 🚀 ÉTAPES D'INTÉGRATION

### Étape 1: Créer le projet dans Qt Creator

1. **Fichier → Nouveau projet → Application Qt Widgets**
2. Nommer: `SmartMarket`
3. Kit: Desktop Qt 6.x.x MinGW 64-bit

### Étape 2: Remplacer les fichiers générés

1. **Supprimer** les fichiers `mainwindow.h`, `mainwindow.cpp`, `mainwindow.ui`
2. **Créer** les fichiers `smartmarket.h`, `smartmarket.cpp`, `smartmarket.ui`
3. **Copier** le contenu que je vous ai fourni dans chaque fichier

### Étape 3: Modifier main.cpp

Remplacer `MainWindow` par `SmartMarket`:

```cpp
#include "smartmarket.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartMarket w;  // ← Important: SmartMarket au lieu de MainWindow
    w.show();
    return a.exec();
}
```

### Étape 4: Ajouter vos images

1. Créer un dossier `images/` dans le projet
2. Y placer vos images:
   - `imagetouse.png-removebg-preview.png` (logo)
   - `menuimage.png` (image login)
   - `publication icon.png`
   - `reviewer icon.png`
   - `conference icon.png`
   - `equipement icon.png`

### Étape 5: Créer le fichier resources.qrc

**Méthode 1: Via Qt Creator**
1. Clic droit sur le projet → Ajouter nouveau → Qt → Qt Resource File
2. Nommer: `resources.qrc`
3. Clic droit sur `resources.qrc` → Ouvrir dans l'éditeur
4. Ajouter un préfixe: `/new/prefix1`
5. Ajouter toutes vos images

**Méthode 2: Manuellement**
Créer `resources.qrc`:
```xml
<RCC>
    <qresource prefix="/new/prefix1">
        <file>images/imagetouse.png-removebg-preview.png</file>
        <file>images/menuimage.png</file>
        <file>Downloads/publication icon.png</file>
        <file>Downloads/reviewer icon.png</file>
        <file>Downloads/conference icon.png</file>
        <file>Downloads/equipement icon.png</file>
    </qresource>
</RCC>
```

### Étape 6: Compiler et tester

1. **Build → Nettoyer tout**
2. **Build → Reconstruire tout**
3. **Exécuter** (Ctrl+R)

## 🎨 STRUCTURE DE L'INTERFACE (smartmarket.ui)

L'interface utilise un **StackedWidget principal** avec 2 pages:

### Page 0: Module LOGIN
- **Gauche (fond bleu marine):**
  - Logo SmartMarket
  - Formulaire de connexion (email + mot de passe)
  - Bouton "Entrer" → navigation vers Page 1
  - Section "Mot de passe oublié"
  
- **Droite (fond blanc):**
  - Image de bienvenue

### Page 1: Module PUBLICATION
- **Sidebar gauche:**
  - Logo
  - Boutons de navigation: Publications, Reviewers, Conférences, Équipements
  
- **Zone principale (TabWidget avec 2 onglets):**

  **Onglet 1: Liste des publications**
  - Tableau des publications
  - Groupe "Supprimer une publication"
  - Groupe "Ajout/Modification de publication"
  - Groupe "Critères de recherche"
  - Groupe "Export des publications"
  
  **Onglet 2: Détection de similarité et complétude**
  - Groupe "Choisir les publications pour comparer"
  - Tableau de similarité
  - Groupe "Vérifier la complétude"
  - Graphiques (Pie Chart + Bar Chart)

## 🔧 MODIFICATION DU FICHIER .UI

Puisque le fichier .ui est très long, voici comment le créer étape par étape:

### Méthode recommandée: Utiliser Qt Designer

1. **Ouvrir smartmarket.ui dans Qt Designer**
2. **Créer la structure de base:**
   - Ajouter un `QStackedWidget` nommé `stackedWidgetMain`
   - Lui donner toute la taille de la fenêtre centrale

3. **Créer Page 0 (Login):**
   - Ajouter Page 0 au StackedWidget
   - Créer deux GroupBox:
     - `groupBoxLoginLeft` (fond bleu) à gauche
     - `groupBoxLoginRight` (fond blanc) à droite
   - Ajouter les widgets de login dans `groupBoxLoginLeft`:
     - `lineEditLoginEmail` (QLineEdit)
     - `lineEditLoginPassword` (QLineEdit avec echoMode=Password)
     - `btnLoginEntrer` (QPushButton)
     - `lineEditLoginMotDePasseOublie` (QLineEdit)
     - `btnLoginEnvoyer` (QPushButton)

4. **Créer Page 1 (Publication):**
   - Ajouter Page 1 au StackedWidget
   - Créer la sidebar (`groupBox_11`) à gauche avec:
     - `btnSidebarPublications`
     - `btnSidebarReviewers`
     - `btnSidebarConferences`
     - `btnSidebarEquipements`
   - Ajouter un `QTabWidget` à droite
   - Dans l'onglet 1, ajouter tous les groupes et widgets pour les publications
   - Dans l'onglet 2, ajouter les widgets pour l'analyse

5. **Configurer les noms d'objets** (très important!):
   Tous les widgets doivent avoir les noms exacts utilisés dans le code:
   - `lineEditLoginEmail`
   - `lineEditLoginPassword`
   - `btnLoginEntrer`
   - etc. (voir smartmarket.h pour la liste complète)

6. **Connecter les slots:**
   - Clic droit sur chaque bouton → "Go to slot..." → `clicked()`
   - Qt Creator créera automatiquement les slots

## ⚙️ FICHIER .PRO

Votre `SmartMarket.pro` doit contenir:

```qmake
QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    smartmarket.cpp

HEADERS += \
    smartmarket.h

FORMS += \
    smartmarket.ui

RESOURCES += \
    resources.qrc
```

**Important:** La ligne `QT += charts` est ESSENTIELLE pour les graphiques!

## 🔐 IDENTIFIANTS DE TEST

**Login:**
- Email: `oussama@esprit.tn` (insensible à la casse)
- Mot de passe: `oussama`

## 📝 CHECKLIST AVANT COMPILATION

- [ ] Tous les fichiers créés (h, cpp, ui, pro, qrc)
- [ ] Images ajoutées au dossier `images/`
- [ ] `resources.qrc` créé et configuré
- [ ] Noms d'objets dans .ui correspondent au code
- [ ] `QT += charts` dans le fichier .pro
- [ ] Nettoyer + Reconstruire le projet

## ❓ RÉSOLUTION DES PROBLÈMES COURANTS

### Erreur: "undefined reference to vtable"
→ **Solution:** Build → Nettoyer tout, puis Reconstruire

### Erreur: "QChart not found"
→ **Solution:** Ajouter `QT += charts` dans le .pro

### Images ne s'affichent pas
→ **Solution:** Vérifier que les chemins dans le .ui correspondent à ceux du .qrc

### Slots non trouvés
→ **Solution:** Vérifier que les noms de boutons dans le .ui correspondent exactement aux slots dans le .h

## 🎉 RÉSULTAT ATTENDU

✅ **Page Login:**
- Design professionnel bleu marine
- Formulaire de connexion fonctionnel
- Navigation vers Publications après connexion réussie

✅ **Page Publication:**
- Sidebar avec navigation (autres modules affichent un message "à venir")
- 2 onglets: Liste + Analyse
- Tableaux fonctionnels avec données de test
- Graphiques (Pie Chart statut + Bar Chart domaines)
- Tous les boutons CRUD connectés

✅ **Navigation:**
- Login → Publication (via bouton Entrer)
- Sidebar → autres modules (messages info)

## 📞 SUPPORT

Si vous rencontrez des problèmes, vérifiez:
1. Tous les noms d'objets correspondent
2. Les slots sont bien déclarés dans le .h
3. Le .pro contient `QT += charts`
4. Le .qrc est bien inclus dans le .pro

---

**Bonne intégration! 🚀**

