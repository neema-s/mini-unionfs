#!/bin/bash

# Unmount if already mounted
fusermount -u test_env/mnt 2>/dev/null

# Create directories if not exist
mkdir -p test_env/lower test_env/upper test_env/mnt

# Sample files (optional)
echo "base_only_content" > test_env/lower/base.txt
echo "to_be_deleted" > test_env/lower/delete_me.txt

# Run filesystem
./mini_unionfs test_env/lower test_env/upper test_env/mnt