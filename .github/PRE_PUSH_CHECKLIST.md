# Pre-Push Checklist

Before pushing to GitHub, run these checks locally:

## Quick Check (30 seconds)
```bash
# Test build
make clean && make

# Quick functional test
echo -e "echo test\npwd\nexit" | ./sshell
```

## Full Local CI/CD (1 minute)
```bash
# Run automated local tests
./.github/workflows/local-test.sh
```

## Manual Checks
- [ ] Code compiles without warnings
- [ ] All features work as expected
- [ ] README is up to date
- [ ] No debug code or TODOs left
- [ ] Git status is clean (no untracked important files)

## Then Push
```bash
git add .
git commit -m "Your message"
git push origin main
```

---

**Pro Tip:** Add this to your workflow:
```bash
# Create git pre-push hook (automatic check before push)
cat > .git/hooks/pre-push << 'EOF'
#!/bin/bash
echo "Running pre-push checks..."
make clean && make || exit 1
echo "✓ Build passed, pushing..."
EOF
chmod +x .git/hooks/pre-push
```
