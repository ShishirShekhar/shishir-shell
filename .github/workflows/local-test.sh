#!/bin/bash
# Local test script to simulate GitHub Actions
# Run this BEFORE pushing: ./github/workflows/local-test.sh

set -e  # Exit on any error

echo "=== Local CI/CD Test ==="
echo ""

echo "✓ Step 1: Clean build"
make clean

echo ""
echo "✓ Step 2: Build with strict flags"
make

echo ""
echo "✓ Step 3: Run basic tests"
printf "echo test\npwd\nhelp\nexit\n" | ./sshell > /dev/null

echo ""
echo "✓ Step 4: Check for common issues"
echo "  - Checking for TODO/FIXME..."
if grep -r "TODO\|FIXME" src/*.c src/*.h 2>/dev/null; then
    echo "    Warning: Found TODO/FIXME (not blocking)"
else
    echo "    ✓ No TODO/FIXME found"
fi

echo ""
echo "  - Checking for memory leaks indicators..."
if grep -r "malloc\|calloc" src/*.c | grep -v "free"; then
    echo "    ℹ Memory allocations found (manual review needed)"
else
    echo "    ✓ No obvious leak patterns"
fi

echo ""
echo "==========================================="
echo "✅ All local checks passed!"
echo "Safe to push to GitHub"
echo "==========================================="
