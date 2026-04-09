#!/bin/bash

cd "$(dirname "$0")/.."

fusermount -u test_env/mnt 2>/dev/null

rm -rf test_env
mkdir -p test_env/lower test_env/upper test_env/mnt

echo "posix" > test_env/lower/test.txt

./mini_unionfs test_env/lower test_env/upper test_env/mnt &
sleep 1

cat test_env/mnt/test.txt
echo "append" >> test_env/mnt/test.txt
rm test_env/mnt/test.txt

echo "POSIX TEST DONE"

fusermount -u test_env/mnt