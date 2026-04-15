# #!/bin/bash

# cd "$(dirname "$0")/.."

# fusermount -u test_env/mnt 2>/dev/null

# rm -rf test_env
# mkdir -p test_env/lower test_env/upper test_env/mnt

# echo "hello" > test_env/lower/file.txt

# ./mini_unionfs test_env/lower test_env/upper test_env/mnt &
# sleep 1

# echo "newdata" >> test_env/mnt/file.txt

# if grep -q "newdata" test_env/upper/file.txt && \
#    ! grep -q "newdata" test_env/lower/file.txt; then
#     echo "COW TEST PASS"
# else
#     echo "COW TEST FAIL"
# fi

# fusermount -u test_env/mnt