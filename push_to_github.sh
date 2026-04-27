#!/usr/bin/env bash
set -euo pipefail

# Usage: ./push_to_github.sh [remote_repo_url] [branch]
# Default remote_repo_url: https://github.com/boussifoussama/smart-publication-management-from-PubliSmart-.git
# Default branch: main

REMOTE_URL=${1:-https://github.com/boussifoussama/smart-publication-management-from-PubliSmart-.git}
BRANCH=${2:-main}

if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "No git repository found — initializing a new repository."
  git init
fi

echo "Adding .gitignore (if present) and staging files..."
git add .gitignore 2>/dev/null || true
git add -A

if git diff --cached --quiet; then
  echo "No changes to commit."
else
  git commit -m "Initial import from local SmartMarket" || true
fi

echo "Setting remote to: $REMOTE_URL"
git remote remove origin >/dev/null 2>&1 || true
git remote add origin "$REMOTE_URL"

echo "Ensuring branch is '$BRANCH' and pushing to origin..."
git branch -M "$BRANCH"
git push -u origin "$BRANCH"

echo "Push complete. If the push failed due to authentication, use a GitHub PAT or set up SSH keys."
