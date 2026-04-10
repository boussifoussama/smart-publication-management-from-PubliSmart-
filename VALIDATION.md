# ✅ Guide de Validation - SmartMarket + Ollama

**Objectif**: Vérifier que tout fonctionne correctement avant d'utiliser en production.

---

## 🎯 Étapes de Validation

### ✅ Étape 1: Vérifier Ollama (2 minutes)

#### 1.1 Ollama est Installé?
```powershell
ollama --version
```
Expected output:
```
ollama version is 0.X.X
```

✅ **PASS** si version affichée  
❌ **FAIL** → Installer depuis https://ollama.ai

#### 1.2 Modèle Téléchargé?
```powershell
ollama list
```
Expected output:
```
NAME          ID              SIZE      MODIFIED
neural-chat   ...             3.7GB     2 hours ago
mistral       ...             4.1GB     2 hours ago
```

✅ **PASS** si "mistral" (ou autre) dans la liste  
❌ **FAIL** → Lancer `ollama pull mistral`

#### 1.3 Ollama Répond Correctement?
```powershell
# Terminal 1: Lancer Ollama
ollama serve

# Terminal 2: Tester les appels
curl -X POST http://localhost:11434/api/generate ^
  -H "Content-Type: application/json" ^
  -d "{\"model\": \"mistral\", \"prompt\": \"Test\", \"stream\": false}"
```

Expected response:
```json
{
  "response": "Test response from mistral...",
  "done": true
}
```

✅ **PASS** si JSON valide avec "response"  
❌ **FAIL** → Vérifier le format ou relancer Ollama

---

### ✅ Étape 2: Vérifier SmartMarket (5 minutes)

#### 2.1 Code Source Modifié Correctement?
Ouvrez `smartmarket.cpp` et recherchez:

```cpp
static const QString OLLAMA_ENDPOINT = "http://localhost:11434/api/generate";
static const QString OLLAMA_MODEL = "mistral";
```

✅ **PASS** si ces lignes présentes  
❌ **FAIL** → Reverifier les modifications

#### 2.2 Plus d'Anthropic API?
Recherchez dans `smartmarket.cpp`:
```cpp
ANTHROPIC_API_KEY    // Ne doit PAS exister
ANTHROPIC_ENDPOINT   // Ne doit PAS exister
ANTHROPIC_MODEL      // Ne doit PAS exister
```

✅ **PASS** si ces lines n'existent pas  
❌ **FAIL** → Certaines modifications n'ont pas été appliquées

#### 2.3 Compilation Sans Erreurs?
```powershell
# Depuis le dossier du projet
qmake SmartMarket.pro
make
```

Expected output:
```
Building for ...
...
[Linking] smartmarket
Build complete!
```

✅ **PASS** si pas d'erreurs, l'exécutable est créé  
❌ **FAIL** → Vérifier les erreurs affichées

---

### ✅ Étape 3: Test Fonctionnel (10 minutes)

#### 3.1 Lancer SmartMarket

```powershell
# Terminal 1: Ollama
ollama serve

# Terminal 2: SmartMarket compilé
./smartmarket  # ou double-click sur l'EXE
```

✅ **PASS** si l'application démarre  
❌ **FAIL** → Checker les logs, vérifier les dépendances Qt

#### 3.2 Test "Similarité"

1. Allez à l'onglet **"Analyse - Similarité"**
2. Remplissez:
   - Publication 1:
     - Titre: `"Machine Learning Basics"`
     - Contenu: `"Introduction to basic ML concepts"`
   - Publication 2:
     - Titre: `"AI and Deep Learning"`
     - Contenu: `"Advanced neural networks explained"`
3. Cliquez **"Analyser"**

Expected result (après 5-15 sec):
```
Taux de similarite: 45-70%
Statut: Moderement similaires
Justification: Both topics... 
```

✅ **PASS** si:
  - Pourcentage entre 0-100
  - Pas d'erreur réseau
  - Réponse en < 30 secondes

❌ **FAIL** → Voir dépannage plus bas

#### 3.3 Test "Complétude"

1. Allez à l'onglet **"Analyse - Complétude"**
2. Sélectionnez une publication existante
3. Cliquez **"Analyser Completude"**

Expected result (après 5-15 sec):
```
Score: 75/100
Champs manquants: [...]
Qualite contenu: Bon
Recommandations: ...
```

✅ **PASS** si:
  - Score entre 0-100
  - Tableau rempli
  - Pas d'erreur réseau

❌ **FAIL** → Voir dépannage plus bas

#### 3.4 Test avec Données Réelles

Testez avec:
- Publications réelles de votre base
- Différentes longueurs de texte
- Différents domaines

✅ **PASS** si tous les tests fonctionnent

---

## ⚠️ Dépannage

### Problème: "Connection refused"

**Cause**: Ollama n'est pas lancé

**Solutions**:
```powershell
# Vérifier Ollama
ollama serve

# OU si service Windows
Get-Process | findstr ollama

# OU redémarrer le service
Restart-Service ollama
```

### Problème: "Model not found"

**Cause**: Modèle non téléchargé

**Solutions**:
```powershell
ollama pull mistral

# OU vérifier les modèles
ollama list
```

### Problème: "Erreur JSON" ou "Erreur reseau"

**Cause Possible 1**: Format réponse invalide
```powershell
# Tester directement
curl -X POST http://localhost:11434/api/generate ^
  -H "Content-Type: application/json" ^
  -d "{\"model\": \"mistral\", \"prompt\": \"Hi\", \"stream\": false}"

# Doit retourner: {"response": "...", "done": true}
```

**Cause Possible 2**: Endpoint incorrect
```cpp
// Vérifier dans smartmarket.cpp:
static const QString OLLAMA_ENDPOINT = "http://localhost:11434/api/generate";
```

**Cause Possible 3**: Firewall/Proxy
```powershell
# Désactiver VPN/Proxy temporairement
# Relancer le test
```

### Problème: Compilation Échoue

**Solution 1**: Nettoyer et recompiler
```powershell
# Supprimer fichiers build
Remove-Item build -Force -Recurse
Remove-Item debug -Force -Recurse
Remove-Item release -Force -Recurse

# Recompiler
qmake SmartMarket.pro
make clean
make
```

**Solution 2**: Vérifier les dépendances Qt
```powershell
# Les headers doivent être présentes
Get-ChildItem "C:\Qt\6.7\msvc2022_64\include" | grep -i network
```

### Problème: Très Lent (30+ secondes)

**Cause**: Modèle trop gros ou PC trop faible

**Solutions**:
```cpp
// Utiliser modèle plus rapide
static const QString OLLAMA_MODEL = "neural-chat";
```

```powershell
# Ou remplacer le modèle
ollama pull neural-chat
```

### Problème: Utilisation RAM Élevée

**Cause**: Normal pendant le traitement

**Vérifier**:
- Mistral: ~4 GB
- Neural Chat: ~3 GB
- Llama 2: ~7 GB

**Solution**: Utiliser modèle plus léger

---

## 📊 Checkliste Complète

### Avant de Lancer SmartMarket

- [ ] Ollama installé (`ollama --version` fonctionne)
- [ ] Modèle téléchargé (`ollama list` affiche mistral)
- [ ] Ollama lancé (`ollama serve` affiche "listening on 127.0.0.1:11434")
- [ ] Code modifié correctement (OLLAMA_* constants présents)
- [ ] Aucune erreur de compilation

### Après Lancement

- [ ] SmartMarket démarre sans crash
- [ ] Interface complète affichée
- [ ] Test Similarité: Score 0-100 ✅
- [ ] Test Complétude: Score 0-100 ✅
- [ ] Temps réponse < 30 secondes
- [ ] Aucune erreur "Connection refused"
- [ ] Aucune erreur "Model not found"

---

## 🎯 Tests Avancés (Optionnel)

### Test 1: Différents Modèles
```powershell
ollama pull neural-chat
ollama pull llama2

# Modifier smartmarket.cpp et recompiler
# Comparer les résultats
```

### Test 2: Performances
Mesurer le temps de réponse pour:
- Textes courts (< 100 chars)
- Textes moyens (100-500 chars)
- Textes longs (500+ chars)

### Test 3: Charge (Multiple Requêtes)
Lancer plusieurs analyses simultanément:
- SmartMarket 1: Similarité
- SmartMarket 2: Complétude
- Mesurer si Ollama gère bien les requêtes concurrentes

### Test 4: Base de Données
- Importer 100+ publications
- Analyser plusieurs à la fois
- Vérifier pas de fuite mémoire

---

## ✅ Résultat Final

Si tous les tests passent:

```
✅ Ollama fonctionne correctement
✅ SmartMarket communique avec Ollama
✅ Analyse Similarité opérationnelle
✅ Analyse Complétude opérationnelle
✅ Performance acceptable
✅ SYSTÈME PRÊT POUR PRODUCTION
```

---

## 📞 Impossible de Résoudre?

1. Check [INSTALLATION_OLLAMA.md](INSTALLATION_OLLAMA.md)
2. Check [MODIFICATIONS_OLLAMA.md](MODIFICATIONS_OLLAMA.md)
3. Consulter: https://github.com/ollama/ollama/issues
4. Désactiver VPN/Proxy et retry

---

**Date**: 26 Mars 2026  
**Version**: 1.0  
**Statut**: ✅ Guide de validation complet
