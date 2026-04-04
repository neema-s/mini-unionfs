# Mini UnionFS (FUSE-based)

## Overview

Mini UnionFS is a user-space filesystem built using FUSE that combines two directories:

* Lower directory (read-only base layer)
* Upper directory (writable layer)

The mounted directory presents a unified view of both layers.

---

## Features

* Read files from lower and upper layers
* Copy-on-Write (CoW)

  * Modifying a file from lower copies it to upper
* File creation in upper layer
* File deletion using whiteouts (`.wh.*`)
* Merged directory view (no duplicates)
* Hides deleted lower-layer files

---

## Project Structure

```
.
├── include/        # Header files
├── src/            # Source code
├── test_env/       # Test directories (lower, upper, mount)
├── tests/          # Test scripts
├── Makefile
└── README.md
```

---

## Requirements

* Linux / WSL (Ubuntu recommended)
* FUSE3 installed

Install dependencies:

```
sudo apt update
sudo apt install fuse3 libfuse3-dev
```

---

## Build Instructions

```
make clean
make
```

This generates the executable:

```
mini_unionfs
```

---

## How to Run

### Step 1: Prepare directories

```
mkdir -p test_env/lower test_env/upper test_env/mnt
```

Add sample files:

```
echo "base_only_content" > test_env/lower/base.txt
echo "to_be_deleted" > test_env/lower/delete_me.txt
```

---

### Step 2: Mount the filesystem

```
./mini_unionfs test_env/lower test_env/upper test_env/mnt
```

Keep this terminal running.

---

### Step 3: Use the filesystem (in a new terminal)

```
cd ~/mini_unionfs
```

#### Read file

```
cat test_env/mnt/base.txt
```

#### Modify file (Copy-on-Write)

```
echo "modified_content" >> test_env/mnt/base.txt
```

#### Delete file (Whiteout)

```
rm test_env/mnt/delete_me.txt
```

#### List files

```
ls test_env/mnt
```

Expected:

```
base.txt
```

---

### Step 4: Unmount

```
fusermount -u test_env/mnt
```

---

## Running Tests

Make scripts executable:

```
chmod +x tests/*.sh
```

Run tests:

```
bash tests/test_cow.sh
bash tests/test_whiteout.sh
bash tests/test_unionfs.sh
bash tests/test_posix.sh
```

---

## Key Concepts

### Copy-on-Write

* Files from lower are copied to upper before modification

### Whiteouts

* Deleting a file creates:

```
.wh.filename
```

* This hides the file from the lower layer

### Directory Merge

* Combines upper + lower contents
* Removes duplicates
* Respects whiteouts

---

## Notes

* Always run inside WSL/Linux (not directly on Windows paths)
* If mount fails:

```
fusermount -u test_env/mnt
chmod 777 test_env/mnt
```

---

