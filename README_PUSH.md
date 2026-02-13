# Instructions pour pousser ce dossier vers GitHub

1) Vérifier que `git` est installé et configuré (nom/email).

2) Si vous utilisez HTTPS et que Git demande un mot de passe, créez un Personal Access Token (PAT) sur GitHub et utilisez-le comme mot de passe, ou préférez l'authentification par SSH.

3) Commandes recommandées (exécuter depuis la racine du dossier):

```bash
# Rendre le script exécutable (si besoin)
chmod +x push_to_github.sh

# Lancer le script — le premier argument est l'URL du dépôt (optionnel)
./push_to_github.sh

# Exemple avec URL explicite et branche
./push_to_github.sh https://github.com/OWNER/REPO.git main
```

4) Si vous préférez ne pas inclure le token dans l'URL, configurez un credential helper ou utilisez SSH:

```bash
# Configurer SSH (exemple résumé)
ssh-keygen -t ed25519 -C "your_email@example.com"
# Copier la clé publique vers GitHub puis:
git remote set-url origin git@github.com:OWNER/REPO.git
./push_to_github.sh git@github.com:OWNER/REPO.git main
```

Si vous voulez que j'exécute le push depuis cet environnement, fournissez un PAT (je procéderai en toute sécurité). Sinon, lancez les commandes ci-dessus localement.
