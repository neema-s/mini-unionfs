#include "unionfs.h"
#include "paths.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int unionfs_unlink(const char *path) {
    printf("UNLINK CALLED: %s\n", path);
    char upper[PATH_MAX], lower[PATH_MAX], wh[PATH_MAX];

    get_upper_path(path, upper);
    get_lower_path(path, lower);

    // get filename
    char *name = strrchr(path, '/');
    name = name ? name + 1 : (char*)path;

    snprintf(wh, PATH_MAX, "%s/.wh.%s",
             UNIONFS_DATA->upper_dir, name);

    int upper_exists = (access(upper, F_OK) == 0);
    int lower_exists = (access(lower, F_OK) == 0);

    // case 1: file exists in upper → delete it
    if (upper_exists) {
        if (unlink(upper) != 0)
            return -errno;
    }

    // case 2: file exists in lower → create whiteout
    if (lower_exists) {
        int fd = open(wh, O_CREAT | O_WRONLY | O_TRUNC, 0000);
        if (fd < 0)
            return -errno;
        close(fd);
    }

    if (!upper_exists && !lower_exists)
        return -ENOENT;

    return 0;
}
int unionfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    char upper[PATH_MAX];
    get_upper_path(path, upper);

    int fd = open(upper, O_CREAT | O_WRONLY, mode);
    fi->fh = fd;
    return 0;
}