CC=gcc
CFLAGS=-Wall -Wextra -O2 -D_FILE_OFFSET_BITS=64 -Iinclude `pkg-config fuse3 --cflags`
LIBS=`pkg-config fuse3 --libs`

SRC=src/main.c src/paths.c src/ops_read.c src/ops_rw.c src/ops_write.c src/unionfs_ops.c
all:
	$(CC) $(CFLAGS) -o mini_unionfs $(SRC) $(LIBS)

clean:
	rm -f mini_unionfs

test:
	bash tests/test_unionfs.sh