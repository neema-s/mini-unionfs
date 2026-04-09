#include "unionfs.h"
#include "paths.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>

int copy_file(const char *src, const char *dst) {
    int in = open(src, O_RDONLY);
    int out = open(dst, O_WRONLY | O_CREAT, 0644);

    char buf[4096];
    int n;

    while ((n = read(in, buf, sizeof(buf))) > 0)
        write(out, buf, n);

    close(in);
    close(out);
    return 0;
}

int unionfs_open(const char *path, struct fuse_file_info *fi) {
    char resolved[PATH_MAX], upper[PATH_MAX];
    int layer;

    int res = resolve_path(path, resolved, &layer);
    if (res != 0) return res;

    if ((fi->flags & O_WRONLY) || (fi->flags & O_RDWR)) {
        if (layer == LAYER_LOWER) {
            get_upper_path(path, upper);
            char temp[PATH_MAX];
            strcpy(temp, upper);
            mkdir_p(dirname(temp), 0755);
            copy_file(resolved, upper);
            strcpy(resolved, upper);
        }
    }

    int fd = open(resolved, fi->flags);
    fi->fh = fd;
    return 0;
}

int unionfs_read(const char *path, char *buf, size_t size,
                        off_t offset, struct fuse_file_info *fi) {
    return pread(fi->fh, buf, size, offset);
}

int unionfs_write(const char *path, const char *buf, size_t size,
                         off_t offset, struct fuse_file_info *fi) {
    return pwrite(fi->fh, buf, size, offset);
}