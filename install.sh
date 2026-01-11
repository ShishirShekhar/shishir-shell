#!/usr/bin/env sh
set -e

VERSION="1.0.1"
REPO="ShishirShekhar/shishir-shell"
BINARY_NAME="sshell"

# Detect OS
OS=$(uname -s)
ARCH=$(uname -m)

case "$OS" in
  Darwin)
    PLATFORM="darwin-amd64"
    ;;
  Linux)
    PLATFORM="linux-amd64"
    ;;
  *)
    echo "Error: Unsupported OS: $OS" >&2
    exit 1
    ;;
esac

# Create temp directory
TMPDIR=$(mktemp -d)
trap "rm -rf $TMPDIR" EXIT

echo "Downloading sshell v$VERSION for $OS ($ARCH)..."
DOWNLOAD_URL="https://github.com/$REPO/releases/download/v$VERSION/$BINARY_NAME-v$VERSION-$PLATFORM.tar.gz"

# Download and extract
if ! curl -fsSL "$DOWNLOAD_URL" | tar -xzf - -C "$TMPDIR"; then
  echo "Error: Failed to download or extract binary" >&2
  exit 1
fi

BINARY_PATH="$TMPDIR/$BINARY_NAME"

if [ ! -f "$BINARY_PATH" ]; then
  echo "Error: Binary not found after extraction" >&2
  exit 1
fi

chmod +x "$BINARY_PATH"

# Handle options
if [ "$1" = "--global" ] || [ "$1" = "-g" ]; then
  INSTALL_DIR="${2:-/usr/local/bin}"
  mkdir -p "$INSTALL_DIR"
  
  if [ -w "$INSTALL_DIR" ]; then
    cp "$BINARY_PATH" "$INSTALL_DIR/$BINARY_NAME"
    echo "✓ Installed to $INSTALL_DIR/$BINARY_NAME"
    echo "Run: sshell"
  else
    echo "Need elevated permissions to write to $INSTALL_DIR"
    if command -v sudo >/dev/null 2>&1; then
      sudo cp "$BINARY_PATH" "$INSTALL_DIR/$BINARY_NAME"
      echo "✓ Installed to $INSTALL_DIR/$BINARY_NAME"
      echo "Run: sshell"
    else
      echo "Error: sudo not available" >&2
      exit 1
    fi
  fi
else
  # Run directly from temp
  echo "✓ Downloaded and extracted sshell v$VERSION"
  echo ""
  "$BINARY_PATH"
fi
