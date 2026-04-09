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

    // shift args so FUSE gets correct arguments
    argv[1] = argv[3];   // mountpoint becomes argv[1]

    return fuse_main(argc - 2, argv, &unionfs_oper, state);
}