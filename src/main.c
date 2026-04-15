#include "unionfs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern struct fuse_operations unionfs_oper;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <lower> <upper> <mount>\n", argv[0]);
        return 1;
    }

    struct unionfs_state *state = malloc(sizeof(struct unionfs_state));

    realpath(argv[1], state->lower_dir);
    realpath(argv[2], state->upper_dir);

    // prepare FUSE args: program + mountpoint
    char *fuse_argv[2];
    fuse_argv[0] = argv[0];
    fuse_argv[1] = argv[3];

    return fuse_main(2, fuse_argv, &unionfs_oper, state);
}