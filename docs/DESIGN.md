# Mini-UnionFS Design Document

## 1. Data Structures

We use a global structure:

struct mini_unionfs_state:
- lower_dir → read-only base layer
- upper_dir → writable layer
- mount_dir → mount point

## 2. Path Resolution Strategy

Resolution priority:

1. Check whiteout file in upper_dir
   Format: upper_dir/.wh.<filename>

2. Check upper_dir
   If exists → use it

3. Check lower_dir
   If exists → use it

4. Else → return ENOENT

## 3. Whiteout Convention

Whiteout files are used to represent deletions.

Example:
Deleting file.txt creates:
upper_dir/.wh.file.txt

This hides file.txt from lower_dir

# Mini-UnionFS Design Document

## 1. Data Structures

We use a global structure:

struct mini_unionfs_state:
- lower_dir → read-only base layer
- upper_dir → writable layer
- mount_dir → mount point

## 2. Path Resolution Strategy

Resolution priority:

1. Check whiteout file in upper_dir
   Format: upper_dir/.wh.<filename>

2. Check upper_dir
   If exists → use it

3. Check lower_dir
   If exists → use it

4. Else → return ENOENT

## 3. Whiteout Convention

Whiteout files are used to represent deletions.

Example:
Deleting file.txt creates:
upper_dir/.wh.file.txt

This hides file.txt from lower_dir

## FUSE Operation Mapping

FUSE uses a struct called fuse_operations to map filesystem calls to user-defined functions.

Examples:
- getattr → retrieves file metadata
- readdir → lists directory contents
- open → opens file
- read → reads file content
- write → writes to file
- unlink → deletes file

Each of these operations will internally call resolve_path() to determine whether the file exists in upper_dir or lower_dir.

## FUSE Operation Mapping

FUSE uses a struct called fuse_operations to map filesystem calls to user-defined functions.

Examples:
- getattr → retrieves file metadata
- readdir → lists directory contents
- open → opens file
- read → reads file content
- write → writes to file
- unlink → deletes file

Each of these operations will internally call resolve_path() to determine whether the file exists in upper_dir or lower_dir.

## FUSE Operation Mapping

FUSE uses a struct called fuse_operations to map filesystem calls to user-defined functions.

Examples:
- getattr → retrieves file metadata
- readdir → lists directory contents
- open → opens file
- read → reads file content
- write → writes to file
- unlink → deletes file

Each of these operations will internally call resolve_path() to determine whether the file exists in upper_dir or lower_dir.
## resolve_path() Pseudocode

function resolve_path(path):
    if whiteout exists:
        return ENOENT

    if file exists in upper_dir:
        return upper path

    if file exists in lower_dir:
        return lower path

    return ENOENT