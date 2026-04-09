#include "unionfs.h"

extern int unionfs_getattr(const char *, struct stat *, struct fuse_file_info *);
extern int unionfs_readdir(const char *, void *, fuse_fill_dir_t,
                           off_t, struct fuse_file_info *, enum fuse_readdir_flags);
extern int unionfs_open(const char *, struct fuse_file_info *);
extern int unionfs_read(const char *, char *, size_t, off_t, struct fuse_file_info *);
extern int unionfs_write(const char *, const char *, size_t, off_t, struct fuse_file_info *);
extern int unionfs_create(const char *, mode_t, struct fuse_file_info *);
extern int unionfs_unlink(const char *); 

struct fuse_operations unionfs_oper = {
    .getattr = unionfs_getattr,
    .readdir = unionfs_readdir,
    .open    = unionfs_open,
    .read    = unionfs_read,
    .write   = unionfs_write,
    .create  = unionfs_create,
    .unlink  = unionfs_unlink
};