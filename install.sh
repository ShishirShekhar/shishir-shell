#!/usr/bin/env sh
set -e

REPO="ShishirShekhar/shishir-shell"
BINARY_NAME="sshell"

# Fetch latest release version from GitHub API
echo "Fetching latest release..."
LATEST_TAG=$(curl -s "https://api.github.com/repos/$REPO/releases/latest" | grep -o '"tag_name":"[^"]*"' | head -1 | cut -d'"' -f4)

if [ -z "$LATEST_TAG" ]; then
  echo "Error: Could not fetch latest release from GitHub" >&2
  exit 1
fi

VERSION=${LATEST_TAG#v}

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

echo "Downloading sshell $VERSION for $OS..."
DOWNLOAD_URL="https://github.com/$REPO/releases/download/$LATEST_TAG/$BINARY_NAME-$LATEST_TAG-$PLATFORM.tar.gz"

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
  echo "✓ Downloaded and extracted sshell $VERSION"
  echo ""
  "$BINARY_PATH"
fi
