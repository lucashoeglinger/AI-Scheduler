#!/usr/bin/env bash
set -euo pipefail

if [ "$EUID" -ne 0 ]; then
  echo "[ERROR] Please run as root (e.g., sudo ./scripts/install.sh)"
  exit 1
fi

echo "==> Setting up system group and queue storage..."
groupadd -f ml-students

QUEUE_DB="/var/log/gpu_queue.txt"
if [ ! -f "$QUEUE_DB" ]; then
    touch "$QUEUE_DB"
fi

chown root:ml-students "$QUEUE_DB"
chmod 660 "$QUEUE_DB"

echo "==> Building and installing queue-wrapper..."
mkdir -p build
cmake -B build
cmake --build build
cmake --install build

echo ""
echo "================================================================="
echo " [SUCCESS] Installation Complete!"
echo "================================================================="
echo "Next steps to enforce SSH isolation:"
echo "1. Append 'config/sshd_config.snippet' to your /etc/ssh/sshd_config"
echo "2. Validate config syntax: sudo sshd -t"
echo "3. Reload SSH: sudo systemctl reload ssh"
echo "================================================================="