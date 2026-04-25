# 🎯 SMARTMARKET - PROJET INTÉGRÉ

## 📋 DESCRIPTION

**SmartMarket** est un système de gestion intégré développé en Qt/C++ qui combine :
- ✅ **Module Login** : Authentification sécurisée
- ✅ **Module Publication** : Gestion complète des publications scientifiques avec analyse

---

## 🗂️ STRUCTURE DU PROJET

```
SmartMarket/
├── SmartMarket.pro          ← Fichier de configuration Qt
├── main.cpp                 ← Point d'entrée
├── smartmarket.h            ← Déclarations de la classe
├── smartmarket.cpp          ← Implémentation complète
├── smartmarket.ui           ← Interface graphique
├── resources.qrc            ← Fichier de ressources
├── README.md                ← Ce fichier
├── GUIDE_INTEGRATION.md     ← Guide détaillé
└── images/                  ← Vos images (à créer)
    ├── imagetouse.png-removebg-preview.png
    ├── menuimage.png
    ├── publication_icon.png
    ├── reviewer_icon.png
    ├── conference_icon.png
    └── equipement_icon.png
```

---

## 🚀 INSTALLATION RAPIDE

### Prérequis
- Qt Creator 6.x ou supérieur
- Qt 6.x avec module **Qt Charts**
- Compilateur C++17 (MinGW, MSVC, ou GCC)

### Étapes

1. **Ouvrir le projet dans Qt Creator**
   ```
   Fichier → Ouvrir un fichier ou projet → SmartMarket.pro
   ```

2. **Ajouter vos images**
   - Créer le dossier `images/`
   - Y placer toutes vos images

3. **Configurer le .qrc**
   - Vérifier que `resources.qrc` pointe vers vos images
   - Adapter les chemins si nécessaire

4. **Compiler**
   ```
   Build → Nettoyer tout
   Build → Reconstruire tout
   ```

5. **Exécuter**
   ```
   Appuyer sur Ctrl+R ou le bouton ▶️
   ```

---

## 🔐 IDENTIFIANTS DE TEST

**Pour se connecter :**
- Email : `selim@esprit.tn` (insensible à la casse)
- Mot de passe : `selim`

---

## 📱 FONCTIONNALITÉS

### Module LOGIN (Page 0)
- ✅ Connexion avec validation
- ✅ Mot de passe masqué
- ✅ Fonctionnalité "Mot de passe oublié"
- ✅ Design moderne bleu marine

### Module PUBLICATION (Page 1)

**Onglet 1 : Liste des publications**
- ✅ Affichage des publications dans un tableau
- ✅ Ajout de nouvelles publications
- ✅ Modification de publications existantes
- ✅ Suppression de publications
- ✅ Recherche multicritères
- ✅ Export PDF/Excel

**Onglet 2 : Analyse**
- ✅ Détection de similarité entre 2 publications
- ✅ Vérification de complétude
- ✅ **Pie Chart** : Répartition par statut
- ✅ **Bar Chart** : Publications par domaine

### Sidebar
- ✅ Navigation vers Publications (actif)
- ⚠️ Reviewers (à venir)
- ⚠️ Conférences (à venir)
- ⚠️ Équipements (à venir)

---

## 🎨 CAPTURES D'ÉCRAN

### Page Login
```
┌─────────────────────────────────────────────────┐
│  [LOGO]              │                          │
│  SmartMarket         │      Bienvenue           │
│                      │                          │
│  Email: [_________]  │   Gestion intelligente   │
│  Mot de passe: [__]  │   de la recherche        │
│  [  Entrer  ]        │   scientifique           │
│                      │                          │
│  Mot de passe oublié?│                          │
│  Email: [_________]  │                          │
│  [  Envoyer  ]       │                          │
└─────────────────────────────────────────────────┘
```

### Page Publication
```
┌──────┬────────────────────────────────────────────┐
│      │  [Liste]  [Analyse]                       │
│ Logo │                                            │
│      │  Tableau des publications                 │
│ 📄 Publications  │  [Domaine] [Titre] [Source]   │
│                 │                                │
│ 👤 Reviewers    │  [Supprimer] [Ajouter/Modifier]│
│                 │                                │
│ 🎤 Conférences  │  [Recherche] [Export]          │
│                 │                                │
│ 🔧 Équipements  │  📊 Graphiques                 │
└──────┴────────────────────────────────────────────┘
```

---

## 🔧 PERSONNALISATION

### Modifier les couleurs
Dans `smartmarket.ui` ou via CSS dans le `.styleSheet`:
```css
/* Couleur principale */
background-color: #071526;  /* Bleu marine foncé */

/* Couleur secondaire */
background-color: #1E40AF;  /* Bleu */
```

### Ajouter un nouveau module
1. Ajouter une page au `stackedWidgetMain`
2. Créer les widgets dans Qt Designer
3. Ajouter les slots dans `smartmarket.h`
4. Implémenter dans `smartmarket.cpp`

---

## ❓ RÉSOLUTION DES PROBLÈMES

### ❌ Erreur : "undefined reference to vtable"
**Solution :** 
```
Build → Nettoyer tout
Build → Reconstruire tout
```

### ❌ Erreur : "QChart not found"
**Solution :** Ajouter dans `SmartMarket.pro` :
```qmake
QT += charts
```

### ❌ Les images ne s'affichent pas
**Solution :**
1. Vérifier que le dossier `images/` existe
2. Vérifier que `resources.qrc` est correct
3. Nettoyer et reconstruire

### ❌ Les slots ne se connectent pas
**Solution :**
1. Vérifier les noms d'objets dans le `.ui`
2. S'assurer que les slots sont bien déclarés dans le `.h`
3. Utiliser la convention `on_nomBouton_clicked()`

---

## 📚 FICHIERS IMPORTANTS

### SmartMarket.pro
Fichier de configuration Qt - **NE PAS OUBLIER** `QT += charts`

### smartmarket.h
Contient toutes les déclarations de slots pour :
- Navigation (Login → Publication)
- CRUD Publications
- Analyse (Similarité, Complétude)
- Navigation Sidebar

### smartmarket.cpp
Implémentation complète avec :
- Configuration initiale
- Création des tableaux
- Génération des graphiques (Pie + Bar Chart)
- Gestion des événements

### smartmarket.ui
Interface graphique avec :
- `stackedWidgetMain` (2 pages)
  - Page 0 : Login
  - Page 1 : Publication
- Tous les widgets nommés correctement

---

## 📞 SUPPORT

Pour toute question ou problème :
1. Consultez le fichier `GUIDE_INTEGRATION.md`
2. Vérifiez la CHECKLIST dans ce README
3. Assurez-vous que tous les fichiers sont présents

---

## ✅ CHECKLIST AVANT DE SOUMETTRE

- [ ] Projet compile sans erreurs
- [ ] Login fonctionne (email + mot de passe)
- [ ] Navigation Login → Publication fonctionne
- [ ] Tableaux affichent les données
- [ ] Graphiques s'affichent correctement
- [ ] Boutons CRUD répondent (messages de confirmation)
- [ ] Sidebar fonctionne
- [ ] Pas de warnings critiques

---

## 📄 LICENCE

Projet académique - Esprit 2A AU 2025-2026

---

## 👥 CONTRIBUTEURS

- **Module Login** : Votre nom
- **Module Publication** : Votre nom
- **Intégration** : Selon le guide du professeur

---

**🎉 Bon travail et bonne chance ! 🚀**

