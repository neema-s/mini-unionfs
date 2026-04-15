#include "unionfs.h"
#include "paths.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>
#include <stdio.h>

int copy_file(const char *src, const char *dst) {
    int in = open(src, O_RDONLY);
    if (in < 0) return -errno;

    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        close(in);
        return -errno;
    }

    char buf[4096];
    int n;

    while ((n = read(in, buf, sizeof(buf))) > 0) {
        if (write(out, buf, n) != n) {
            close(in);
            close(out);
            return -EIO;
        }
    }

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
            char *dir = dirname(temp);
            mkdir_p(dir, 0755);

            if (access(upper, F_OK) != 0) {
                int ret = copy_file(resolved, upper);
                if (ret != 0) return ret;
            }

            strcpy(resolved, upper);
            layer = LAYER_UPPER;
        }
    }
    
    int fd = open(resolved, fi->flags);
    if (fd < 0) return -errno;

    fi->fh = fd;
    return 0;
}

int unionfs_read(const char *path, char *buf, size_t size,
                        off_t offset, struct fuse_file_info *fi) {
    return pread(fi->fh, buf, size, offset);
}

// int unionfs_write(const char *path, const char *buf, size_t size,
//                   off_t offset, struct fuse_file_info *fi) {

//     char resolved[PATH_MAX], upper[PATH_MAX];
//     int layer;
//                     printf("WRITE: %s\n", path);
//     int res = resolve_path(path, resolved, &layer);
//     if (res != 0) return res;

//     // Apply COW if needed
//     if (layer == LAYER_LOWER) {
//         get_upper_path(path, upper);

//         char temp[PATH_MAX];
//         strcpy(temp, upper);
//         char *dir = dirname(temp);
//         mkdir_p(dir, 0755);

//         if (access(upper, F_OK) != 0) {
//             int ret = copy_file(resolved, upper);
//             if (ret != 0) return ret;
//         }

//         strcpy(resolved, upper);
//     }

//     int fd = open(resolved, O_WRONLY);
//     if (fd < 0) return -errno;

//     int ret = pwrite(fd, buf, size, offset);
//     close(fd);

//     return ret;
// }

int unionfs_write(const char *path, const char *buf, size_t size,
                  off_t offset, struct fuse_file_info *fi) {
    int ret = pwrite(fi->fh, buf, size, offset);
    return (ret < 0) ? -errno : ret;
}

int unionfs_release(const char *path, struct fuse_file_info *fi) {
    if (fi->fh >= 0) {
        int ret = close(fi->fh);
        if (ret < 0) return -errno;
    }
    return 0;
}