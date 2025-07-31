#!/bin/bash

get_latest_version() {
    curl -s https://api.github.com/repos/google-deepmind/mujoco/releases/latest \
    | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/'
}

# Use first parameter as version or get latest if not provided
if [ -n "$1" ]; then
    VERSION="$1"
else
    VERSION=$(get_latest_version)
    if [ -z "$VERSION" ]; then
        echo "Failed to fetch latest version from GitHub API."
        exit 1
    fi
    echo "Latest MuJoCo version is $VERSION."
    read -p "Do you want to install MuJoCo $VERSION? (y/n): " CONFIRM
    if [[ "$CONFIRM" != "y" && "$CONFIRM" != "Y" ]]; then
        echo "Installation cancelled."
        exit 0
    fi
fi

# Target directories
INSTALL_DIR="$HOME/mujoco"
TARGET_DIR="$INSTALL_DIR/mujoco-$VERSION"

# Create target directory
mkdir -p "$INSTALL_DIR"
cd "$INSTALL_DIR" || exit 1

# Detect architecture
ARCH="linux-x86_64"  # Adjust for macOS if needed
URL="https://github.com/google-deepmind/mujoco/releases/download/${VERSION}/mujoco-${VERSION}-${ARCH}.tar.gz"

# Download and extract
echo "Downloading MuJoCo $VERSION..."
wget -q --show-progress "$URL" -O "mujoco-${VERSION}.tar.gz" || {
    echo "Download failed! Check version or internet connection."
    exit 1
}

echo "Extracting MuJoCo..."
tar -xvf "mujoco-${VERSION}.tar.gz"
rm "mujoco-${VERSION}.tar.gz"

# Verify extraction
if [ ! -d "$TARGET_DIR" ]; then
    echo "Extraction failed: $TARGET_DIR not found."
    exit 1
fi

# Ask to update environment variables
read -p "Do you want to set MUJOCO_VERSION=$VERSION in ~/.bashrc? (y/n): " ANSWER
if [[ "$ANSWER" == "y" || "$ANSWER" == "Y" ]]; then
    ENV_FILE="$INSTALL_DIR/mujoco_env.sh"

    # Write environment variables to separate file
    cat <<EOF > "$ENV_FILE"
export MUJOCO_VERSION=$VERSION
export MUJOCO_DIR=\$HOME/mujoco/mujoco-\$MUJOCO_VERSION
export LD_LIBRARY_PATH=\$MUJOCO_DIR/lib:\$LD_LIBRARY_PATH
export PATH=\$MUJOCO_DIR/bin:\$PATH
EOF


    # Ensure ~/.bashrc sources this file (add only if not already present)
    if ! grep -q "source $ENV_FILE" ~/.bashrc; then
        echo "" >> ~/.bashrc
        echo "# Source MuJoCo environment variables" >> ~/.bashrc
        echo "source $ENV_FILE" >> ~/.bashrc
    fi

    echo "Environment file created at $ENV_FILE"
fi

source ~/.bashrc
echo "MuJoCo $VERSION installed successfully in $TARGET_DIR"