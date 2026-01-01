#!/bin/bash
# quick_commit.sh - Quick commit to both WORKSHOP and PUBLIC repos
# Usage: ./quick_commit.sh "commit message"

set -e

WORKSHOP_DIR="/home/pardus/projeler/MLP/MELP-GCC-WORKSHOP"
PUBLIC_DIR="/home/pardus/projeler/MLP/MELP-GCC"

# Check if commit message provided
if [ -z "$1" ]; then
    echo "❌ Error: Commit message required"
    echo "Usage: ./quick_commit.sh \"your commit message\""
    exit 1
fi

COMMIT_MSG="$1"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║  MELP-GCC Two-Stage Git Workflow                          ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Stage 1: WORKSHOP (Private)
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "📦 Stage 1: WORKSHOP (Private)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
cd "$WORKSHOP_DIR"

echo "📝 Adding files..."
git add -A

echo "💾 Committing: $COMMIT_MSG"
git commit -m "$COMMIT_MSG" || {
    echo "⚠️  No changes to commit (WORKSHOP)"
}

echo "🚀 Pushing to private repo..."
git push origin main || {
    echo "⚠️  Push failed or already up to date (WORKSHOP)"
}

echo "✅ WORKSHOP sync complete!"
echo ""

# Ask if user wants to sync to public
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "📦 Stage 2: PUBLIC (Optional)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
read -p "Sync to PUBLIC repo? (y/n): " -n 1 -r
echo ""

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "🔄 Running sync_to_public.sh..."
    "$WORKSHOP_DIR/sync_to_public.sh"
    
    echo ""
    echo "📍 Committing to public repo..."
    cd "$PUBLIC_DIR"
    
    git add -A
    git commit -m "sync: $COMMIT_MSG" || {
        echo "⚠️  No changes to commit (PUBLIC)"
        exit 0
    }
    
    echo "🚀 Pushing to public repo..."
    git push origin main || {
        echo "⚠️  Push failed or already up to date (PUBLIC)"
    }
    
    echo "✅ PUBLIC sync complete!"
else
    echo "⏸️  Skipped public sync"
fi

echo ""
echo "╔════════════════════════════════════════════════════════════╗"
echo "║  ✅ Workflow Complete!                                     ║"
echo "╚════════════════════════════════════════════════════════════╝"
