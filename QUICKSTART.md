# ⚡ QUICKSTART - SmartMarket + Ollama (5 minutes)

## 🎯 Objectif
Avoir votre SmartMarket **100% fonctionnel et gratuit** en 5 minutes.

---

## 📋 Checklist Rapide

### ✅ Étape 1: Installer Ollama (2 min)

1. **Télécharger**: https://ollama.ai → Download Windows
2. **Installer**: Double-clic, suivre les instructions
3. **Vérifier** (PowerShell):
   ```powershell
   ollama --version
   ```

### ✅ Étape 2: Télécharger le Modèle (2 min)

```powershell
# 🚀 OPTION RAPIDE: Neural Chat (3GB, très rapide)
ollama pull neural-chat

# OU si vous voulez Mistral (4GB, plus lent)
ollama pull mistral
```

**Pourquoi Neural Chat?**
- ⚡ **3GB seulement** (vs 4GB pour Mistral)
- 🚀 **Plus rapide** à télécharger et utiliser
- ✅ **Parfait pour SmartMarket**

### ✅ Étape 3: Lancer Ollama

Choisissez **UNE** de ces options:

**Option A: Manuel (Recommandé pour débugger)**
```powershell
ollama serve
```
Laissez ce terminal ouvert. Vous verrez:
```
listening on 127.0.0.1:11434
```

**Option B: Automatique (Service Windows)**
Ollama s'exécute automatiquement en arrière-plan (si installé normalement).

### ✅ Étape 4: Lancer SmartMarket

1. **Compiler** (Qt Creator ou qmake)
2. **Exécuter** le projet
3. **Tester**:
   - Onglet "Similarité"
   - Entrez Publication 1 et Publication 2
   - Cliquez "Analyser"
   - **✅ Doit afficher un pourcentage en 5-15 secondes**

---

## 🎮 Testez Rapidement sans SmartMarket

Pour vérifier que Ollama fonctionne:

```powershell
# Dans PowerShell
curl -X POST http://localhost:11434/api/generate -H "Content-Type: application/json" -d "{`"model`": `"mistral`", `"prompt`": `"Hello!`", `"stream`": false}"
```

Vous devriez voir une réponse JSON.

---

## ⚠️ Ça ne marche pas? Dépannage Rapide

| ❌ Problème | ✅ Solution |
|-----------|-----------|
| "Connection refused" | `ollama serve` n'est pas lancé |
| "Model not found" | Lancez `ollama pull mistral` |
| Très lent | Normal, laissez 10-30 sec |
| Erreur réseau | Désactivez le VPN/Proxy |

---

## 📚 Documentation Complète

- **Installation détaillée**: [INSTALLATION_OLLAMA.md](INSTALLATION_OLLAMA.md)
- **Modifications du code**: [MODIFICATIONS_OLLAMA.md](MODIFICATIONS_OLLAMA.md)
- **Ollama officiel**: https://ollama.ai

---

## 🚀 Commandes Essentielles

```powershell
# Lancer Ollama
ollama serve

# Télécharger un modèle
ollama pull mistral
ollama pull neural-chat
ollama pull llama2

# Lister les modèles
ollama list

# Supprimer un modèle
ollama rm mistral

# Testeur interactif
ollama run mistral
```

---

## 💡 Astuce: Modèles Alternatifs

Si Mistral est trop lent ou trop gourmand:

```powershell
# Plus rapide, moins de RAM
ollama pull neural-chat

# Plus intelligent, mais plus lent
ollama pull llama2
```

Puis modifiez [smartmarket.cpp](smartmarket.cpp#L42):
```cpp
static const QString OLLAMA_MODEL = "neural-chat";  // au lieu de "mistral"
```

---

## ✨ État Final

Après ces 5 minutes:

- ✅ Ollama installé et lancé
- ✅ Modèle téléchargé
- ✅ SmartMarket compilé
- ✅ Analyse IA fonctionnelle
- ✅ **Coût: $0 (zéro)**
- ✅ **Données privées**
- ✅ **Hors ligne**

---

**Bonne chance! 🚀**
