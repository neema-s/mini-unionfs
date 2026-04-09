#include "unionfs.h"
#include <errno.h>
#include <sys/stat.h>
#include "paths.h"
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int unionfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    char resolved[PATH_MAX];
    int layer;

    int res = resolve_path(path, resolved, &layer);
    if (res != 0) return res;

    if (lstat(resolved, stbuf) == -1)
        return -errno;

    return 0;
}

int unionfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info *fi,
                    enum fuse_readdir_flags flags) {

    (void)offset;
    (void)fi;
    (void)flags;

    char upper[PATH_MAX], lower[PATH_MAX];

    get_upper_path(path, upper);
    get_lower_path(path, lower);

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    DIR *dp;
    struct dirent *de;

    // Track seen files (avoid duplicates)
    char seen[256][256];
    int seen_count = 0;

    // -------- UPPER --------
    dp = opendir(upper);
    if (dp) {
        while ((de = readdir(dp)) != NULL) {

            if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                continue;

            // skip whiteout files
            if (strncmp(de->d_name, ".wh.", 4) == 0)
                continue;

            strcpy(seen[seen_count++], de->d_name);
            filler(buf, de->d_name, NULL, 0, 0);
        }
        closedir(dp);
    }

    // -------- LOWER --------
dp = opendir(lower);
if (dp) {
    while ((de = readdir(dp)) != NULL) {

        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;

        int skip = 0;

        // skip duplicates
        for (int i = 0; i < seen_count; i++) {
            if (strcmp(seen[i], de->d_name) == 0) {
                skip = 1;
                break;
            }
        }

        // whiteout check
        char wh[PATH_MAX];
        snprintf(wh, PATH_MAX, "%s%s/.wh.%s",
                 UNIONFS_DATA->upper_dir, path, de->d_name);

        if (access(wh, F_OK) == 0) {
            skip = 1;
        }

        if (!skip) {
            filler(buf, de->d_name, NULL, 0, 0);
        }
    }
    closedir(dp);
}
                    }
                