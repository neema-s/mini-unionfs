#!/bin/bash

cd "$(dirname "$0")/.."

fusermount -u test_env/mnt 2>/dev/null

rm -rf test_env
mkdir -p test_env/lower test_env/upper test_env/mnt

echo "delete me" > test_env/lower/delete.txt

./mini_unionfs test_env/lower test_env/upper test_env/mnt &
sleep 1

rm test_env/mnt/delete.txt

if [ ! -f test_env/mnt/delete.txt ] && \
   [ -f test_env/upper/.wh.delete.txt ]; then
    echo "WHITEOUT TEST PASS"
else
    echo "WHITEOUT TEST FAIL"
fi

fusermount -u test_env/mnt