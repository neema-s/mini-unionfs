#!/bin/bash

cd "$(dirname "$0")/.."

fusermount -u test_env/mnt 2>/dev/null

rm -rf test_env
mkdir -p test_env/lower test_env/upper test_env/mnt

echo "base" > test_env/lower/base.txt

./mini_unionfs test_env/lower test_env/upper test_env/mnt &
sleep 1

# Read test
grep -q "base" test_env/mnt/base.txt || echo "READ FAIL"

# Write test
echo "mod" >> test_env/mnt/base.txt

# Delete test
echo "temp" > test_env/lower/delete.txt
rm test_env/mnt/delete.txt

echo "UNIONFS TEST DONE"

fusermount -u test_env/mnt