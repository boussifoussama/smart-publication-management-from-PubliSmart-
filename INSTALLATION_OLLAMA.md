# 🚀 Installation Ollama - Guide Complet

## ✨ Qu'est-ce que Ollama ?

Ollama vous permet de **lancer des modèles IA puissants sur votre ordinateur**, gratuitement et à 100% hors ligne. Aucun compte API, aucun crédit, aucun frais.

---

## 📋 Prérequis

- **Windows 10/11** (ou Mac/Linux)
- **RAM**: Au minimum 8 GB (16 GB recommandé)
- **Espace disque**: 5-20 GB selon le modèle choisi
- **Connexion Internet**: Uniquement pour le téléchargement initial

---

## 🎯 Installation Étape par Étape

### 1️⃣ **Télécharger Ollama**

- Allez sur: **https://ollama.ai**
- Cliquez sur **"Download"** pour Windows
- Installez comme une application normale

### 2️⃣ **Vérifier l'Installation**

Ouvrez **PowerShell** ou **Invite de commandes** et tapez:

```powershell
ollama --version
```

Si vous voyez un numéro de version, ✅ c'est bon!

---

## 📥 Télécharger un Modèle

Les modèles recommandés pour SmartMarket:

### **Option 1: Neural Chat 7B** (RECOMMANDÉ - Le plus rapide)
```powershell
ollama pull neural-chat
```
- ⚡ **Très rapide** (5-10 secondes par réponse)
- 💾 **3GB de RAM**
- ✅ **Bon rapport qualité/vitesse**

### **Option 2: Mistral 7B** (Alternative)
```powershell
ollama pull mistral
```
- ⚡⚡ **Rapide** (10-15 secondes par réponse)
- 💾 **4.1 GB de RAM**
- ✅ **Très bonne qualité**

### **Option 3: Llama 2** (Plus puissant)
```powershell
ollama pull llama2
```
- 🧠 **Plus intelligent, mais plus lent**
- 💾 **7 GB de RAM**
- ⏱️ **15-30 secondes par réponse**

> **Conseil**: Commencez avec **Mistral**, c'est le meilleur équilibre!

---

## ▶️ Lancer Ollama

### **Méthode 1: Terminal**

```powershell
ollama serve
```

La console affichera:
```
listening on 127.0.0.1:11434
```

✅ Ollama est maintenant **prêt!** Laissez ce terminal ouvert.

### **Méthode 2: Service Windows (Automatique)**

Si vous avez installé Ollama normalement, il s'exécute déjà en arrière-plan!

Vérifiez dans **Gestionnaire des tâches**:
- Onglet "Processus"
- Cherchez "ollama"

---

## 🚀 Lancer SmartMarket

Maintenant que Ollama tourne:

1. **Ouvrez SmartMarket**
2. **Testez la fonctionnalité "Similarité"**:
   - Entrez deux publications
   - Cliquez sur "Analyser"
   - L'IA Ollama traitera votre demande!

---

## ⚠️ Dépannage

### **Erreur: "Connection refused"**
```
👉 Ollama n'est pas lancé!
   Ouvrez PowerShell et tapez: ollama serve
```

### **Erreur: "Model not found"**
```
👉 Le modèle n'est pas téléchargé!
   Tapez: ollama pull mistral
```

### **Lenteur / Utilisation RAM élevée**
```
👉 Le modèle est en cours de traitement, c'est normal!
   Attendez 10-30 secondes que ça finisse.
   
   Pour économiser la RAM:
   - Utilisez "neural-chat" au lieu de "mistral"
   - Réduisez la taille du contexte dans le code
```

### **Impossible de télécharger le modèle**
```
👉 Problème de connexion Internet ou disque plein
   - Vérifiez votre connexion
   - Libérez 10+ GB d'espace disque
   - Relancez: ollama pull mistral
```

---

## 🎮 Tester Ollama Directement

Pour vérifier que tout fonctionne avant de lancer SmartMarket:

```powershell
# Terminal 1: Lancer Ollama
ollama serve

# Terminal 2: Tester une requête
curl -X POST http://localhost:11434/api/generate -H "Content-Type: application/json" -d "{\"model\": \"mistral\", \"prompt\": \"Hello!\", \"stream\": false}"
```

Vous devriez voir une réponse JSON avec le texte généré.

---

## 📊 Configuration Avancée

### **Changer le Modèle utilisé**

Modifiez cette ligne dans `smartmarket.cpp` (ligne ~42):

```cpp
// Actuellement: Mistral
static const QString OLLAMA_MODEL = "mistral";

// Vers: Neural Chat
static const QString OLLAMA_MODEL = "neural-chat";

// Ou: Llama 2
static const QString OLLAMA_MODEL = "llama2";
```

Puis récomplez le projet.

### **Utiliser un Ollama Distant**

Si Ollama tourne sur une **autre machine**:

```cpp
static const QString OLLAMA_ENDPOINT = "http://192.168.1.100:11434/api/generate";
```

Remplacez `192.168.1.100` par l'IP réelle.

---

## ✅ Checklist Final

- [ ] Ollama installé ✅
- [ ] Modèle téléchargé (`ollama pull mistral`)
- [ ] Ollama lancé (`ollama serve`)
- [ ] SmartMarket compilé
- [ ] Test Similarité: **OK** ✅
- [ ] Test Complétude: **OK** ✅

---

## 🎉 Félicitations!

Votre **SmartMarket est maintenant 100% opérationnel avec une IA gratuite!**

Pas de frais d'API, pas de limites de crédit, purement **gratuit et hors ligne**.

---

## 📞 Besoin d'Aide?

- **Ollama Docs**: https://github.com/ollama/ollama
- **Modèles disponibles**: https://ollama.ai/library
- **Problèmes connus**: https://github.com/ollama/ollama/issues
