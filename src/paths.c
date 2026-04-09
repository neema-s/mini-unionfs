#include "unionfs.h"
#include <errno.h>
#include "paths.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>

void get_upper_path(const char *path, char *buf) {
    snprintf(buf, PATH_MAX, "%s%s", UNIONFS_DATA->upper_dir, path);
}

void get_lower_path(const char *path, char *buf) {
    snprintf(buf, PATH_MAX, "%s%s", UNIONFS_DATA->lower_dir, path);
}

void get_whiteout_path(const char *path, char *buf) {
    char temp[PATH_MAX];
    strcpy(temp, path);

    char *dir = dirname(temp);

    char temp2[PATH_MAX];
    strcpy(temp2, path);
    char *base = basename(temp2);

    snprintf(buf, PATH_MAX, "%s%s/.wh.%s",
             UNIONFS_DATA->upper_dir,
             dir,
             base);
}
int resolve_path(const char *path, char *resolved, int *layer) {

    char upper[PATH_MAX], lower[PATH_MAX];

    get_upper_path(path, upper);
    get_lower_path(path, lower);

    // 🔴 SIMPLE WHITEOUT CHECK (MATCHES unlink)
    char *name = strrchr(path, '/');
    name = name ? name + 1 : (char*)path;

    char wh[PATH_MAX];
    snprintf(wh, PATH_MAX, "%s/.wh.%s",
             UNIONFS_DATA->upper_dir, name);

    if (access(wh, F_OK) == 0)
        return -ENOENT;

    if (access(upper, F_OK) == 0) {
        strcpy(resolved, upper);
        *layer = LAYER_UPPER;
        return 0;
    }

    if (access(lower, F_OK) == 0) {
        strcpy(resolved, lower);
        *layer = LAYER_LOWER;
        return 0;
    }

    return -ENOENT;
}
int mkdir_p(const char *path, mode_t mode) {
    char tmp[PATH_MAX];
    snprintf(tmp, sizeof(tmp), "%s", path);

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, mode);
            *p = '/';
        }
    }
    return mkdir(tmp, mode);
}