#define FUSE_USE_VERSION 31

#include <fuse3/fuse.h>
#include <limits.h>

struct unionfs_state {
    char lower_dir[PATH_MAX];
    char upper_dir[PATH_MAX];
};

#define UNIONFS_DATA ((struct unionfs_state *) fuse_get_context()->private_data)

extern struct fuse_operations unionfs_oper;