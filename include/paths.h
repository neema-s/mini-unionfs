#ifndef PATHS_H
#define PATHS_H

#include <limits.h>

enum {
    LAYER_LOWER = 0,
    LAYER_UPPER = 1
};

int resolve_path(const char *path, char *resolved, int *layer);

void get_upper_path(const char *path, char *buf);
void get_lower_path(const char *path, char *buf);
void get_whiteout_path(const char *path, char *buf);

int mkdir_p(const char *path, mode_t mode);

#endif