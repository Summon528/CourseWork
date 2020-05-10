#include <assert.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *path_canonicalize(const char *path_og, char *buf) {
    char path[PATH_MAX] = {};
    strncpy(path, path_og, PATH_MAX);
    char buf2[PATH_MAX] = {};
    if (path[0] != '/') {
        char cwd[PATH_MAX] = {};
        getcwd(cwd, sizeof(cwd));
        strncpy(buf2, cwd, PATH_MAX);
    }
    char *token;
    token = strtok(path, "/");
    while (token != NULL) {
        if (strcmp(token, ".") == 0 || token[0] == 0) {
            token = strtok(NULL, "/");
            continue;
        }
        int l = strlen(buf2);
        buf2[l] = '/';
        buf2[l + 1] = 0;
        strncpy(buf2 + l + 1, token, PATH_MAX - l - 1);
        token = strtok(NULL, "/");
    }

    int l = strlen(buf2);
    int dotdot = 0;
    for (int i = l - 1; i >= 0; i--) {
        if (buf2[i] != '/') continue;
        if (buf2[i + 1] == '.' && buf2[i + 2] == '.' &&
            (buf2[i + 3] == 0 || buf2[i + 3] == '/')) {
            buf2[i + 1] = 1;
            dotdot++;
        } else if (dotdot != 0) {
            buf2[i + 1] = 1;
            dotdot--;
        }
    }

    if (dotdot != 0) {
        errno = ENOENT;
        return NULL;
    }

    token = strtok(buf2, "/");
    while (token != NULL) {
        if (token[0] == 1) {
            token = strtok(NULL, "/");
            continue;
        }
        int l = strlen(buf);
        buf[l] = '/';
        buf[l + 1] = 0;
        strncpy(buf + l + 1, token, PATH_MAX - l - 1);
        token = strtok(NULL, "/");
    }
    buf[strlen(buf)] = '/';
    buf[strlen(buf) + 1] = 0;
    return buf;
}

static char *basedir = NULL;
static int ttyfd = -1;

void opentty();

void print_bad_fun(const char *fun, const char *exe) {
    char buf[256] = {};
    snprintf(buf, sizeof(buf), "[sandbox] %s(%s): not allowed\n", fun, exe);
    if (ttyfd == -1) opentty();
    write(ttyfd, buf, sizeof(buf));
}

void print_bad_path(const char *fun, const char *path) {
    char buf[256] = {};
    snprintf(buf, sizeof(buf), "[sandbox] %s: access to %s is not allowed\n",
             fun, path);
    if (ttyfd == -1) opentty();
    write(ttyfd, buf, sizeof(buf));
}

void print_bad_path_with_dirfd(const char *fun, const char *path, int dirfd) {
    char buf[256] = {};
    snprintf(buf, sizeof(buf),
             "[sandbox] %s: access to %s when dirfd=%d is not allowed\n", fun,
             path, dirfd);
    if (ttyfd == -1) opentty();
    write(ttyfd, buf, sizeof(buf));
}

bool is_in_basedir(const char *path) {
    if (basedir == NULL) {
        basedir = getenv("BASE_DIR");
        if (basedir == NULL) basedir = "./";
        basedir = realpath(basedir, NULL);
        assert(basedir != NULL);
    }
    int basedir_len = strlen(basedir);
    int path_len = strlen(path);
    if (path_len < basedir_len) {
        errno = EACCES;
        return false;
    }
    for (int i = 0; i < basedir_len; i++) {
        if (path[i] != basedir[i]) {
            errno = EACCES;
            return false;
        }
    }
    return true;
}

#define HOOK_begin(name, R, r, ...)                                   \
    static R (*old_##name)(const char *target, ##__VA_ARGS__) = NULL; \
    R name(const char *target, ##__VA_ARGS__) {                       \
        if (old_##name == NULL) {                                     \
            void *handle = dlopen("libc.so.6", RTLD_LAZY);            \
            assert(handle != NULL);                                   \
            old_##name = dlsym(handle, #name);                        \
            assert(old_##name != NULL);                               \
        }                                                             \
        char rpath[PATH_MAX] = {};                                    \
        if (path_canonicalize(target, rpath) == NULL) return r;

#define HOOK_mid(name, ...)                       \
    if (is_in_basedir(rpath)) {                   \
        return old_##name(target, ##__VA_ARGS__); \
    };

#define HOOK_end(r)                   \
    print_bad_path(__func__, target); \
    return r;                         \
    }

#define HOOK_2(name)                                                           \
    static int (*old_##name)(const char *oldpath, const char *newpath) = NULL; \
    int name(const char *oldpath, const char *newpath) {                       \
        if (old_##name == NULL) {                                              \
            void *handle = dlopen("libc.so.6", RTLD_LAZY);                     \
            assert(handle != NULL);                                            \
            old_##name = dlsym(handle, #name);                                 \
            assert(old_##name != NULL);                                        \
        }                                                                      \
        char rpath1[PATH_MAX] = {}, rpath2[PATH_MAX] = {};                     \
        if (path_canonicalize(oldpath, rpath1) == NULL) return -1;             \
        if (path_canonicalize(newpath, rpath2) == NULL) return -1;             \
        bool bad = false;                                                      \
        if (!is_in_basedir(rpath1)) {                                          \
            print_bad_path(__func__, oldpath);                                 \
            bad = true;                                                        \
        }                                                                      \
        if (!is_in_basedir(rpath2)) {                                          \
            print_bad_path(__func__, newpath);                                 \
            bad = true;                                                        \
        }                                                                      \
        if (bad) return -1;                                                    \
        return old_##name(oldpath, newpath);                                   \
    }

#define HOOK_OPEN(name)                                                    \
    static int (*old_##name)(const char *pathname, int flags, ...) = NULL; \
    int name(const char *pathname, int flags, ...) {                       \
        if (old_##name == NULL) {                                          \
            void *handle = dlopen("libc.so.6", RTLD_LAZY);                 \
            assert(handle != NULL);                                        \
            old_##name = dlsym(handle, "open");                            \
            assert(old_##name != NULL);                                    \
        }                                                                  \
        char rpath[PATH_MAX] = {};                                         \
        if (path_canonicalize(pathname, rpath) == NULL) return -1;         \
        if (!is_in_basedir(rpath)) {                                       \
            print_bad_path(__func__, pathname);                            \
            return -1;                                                     \
        }                                                                  \
                                                                           \
        if (flags & O_CREAT) {                                             \
            va_list args;                                                  \
            va_start(args, flags);                                         \
            mode_t mode = va_arg(args, mode_t);                            \
            va_end(args);                                                  \
            return old_##name(pathname, flags, mode);                      \
        }                                                                  \
        return old_##name(pathname, flags);                                \
    }

#if defined __cplusplus
extern "C" {
#endif

int execl(const char *pathname, const char *arg, ...) {
    print_bad_fun(__func__, pathname);
    errno = EACCES;
    return -1;
};
int execlp(const char *file, const char *arg, ...) {
    print_bad_fun(__func__, file);
    errno = EACCES;
    return -1;
};
int execle(const char *pathname, const char *arg, ...) {
    print_bad_fun(__func__, pathname);
    errno = EACCES;
    return -1;
};
int execv(const char *pathname, char *const argv[]) {
    print_bad_fun(__func__, pathname);
    errno = EACCES;
    return -1;
};
int execve(const char *pathname, char *const argv[], char *const envp[]) {
    print_bad_fun(__func__, pathname);
    errno = EACCES;
    return -1;
};
int execvp(const char *file, char *const argv[]) {
    print_bad_fun(__func__, file);
    errno = EACCES;
    return -1;
};
int system(const char *command) {
    print_bad_fun(__func__, command);
    errno = EACCES;
    return -1;
};

static int (*old___xstat)(int ver, const char *path,
                          struct stat *stat_buf) = NULL;
int __xstat(int ver, const char *path, struct stat *stat_buf) {
    if (old___xstat == NULL) {
        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        assert(handle != NULL);
        old___xstat = dlsym(handle, "__xstat");
        assert(old___xstat != NULL);
    }
    char rpath[PATH_MAX] = {};
    if (path_canonicalize(path, rpath) == NULL) return -1;
    if (is_in_basedir(rpath)) return old___xstat(ver, path, stat_buf);

    print_bad_path(__func__, path);
    return -1;
}

static int (*old___xstat64)(int ver, const char *path,
                            struct stat *stat_buf) = NULL;
int __xstat64(int ver, const char *path, struct stat *stat_buf) {
    if (old___xstat64 == NULL) {
        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        assert(handle != NULL);
        old___xstat64 = dlsym(handle, "__xstat64");
        assert(old___xstat64 != NULL);
    }
    char rpath[PATH_MAX] = {};
    if (path_canonicalize(path, rpath) == NULL) return -1;
    if (is_in_basedir(rpath)) return old___xstat64(ver, path, stat_buf);

    print_bad_path(__func__, path);
    return -1;
}

// clang-format off

HOOK_begin(chdir, int, -1) HOOK_mid(chdir) HOOK_end(-1)

HOOK_begin(chmod, int, -1, mode_t mode) HOOK_mid(chmod, mode) HOOK_end(-1)

HOOK_begin(chown, int, -1, uid_t owner, gid_t group) HOOK_mid(chown, owner, group) HOOK_end(-1)

HOOK_begin(creat, int, -1, mode_t mode) HOOK_mid(creat, mode) HOOK_end(-1)

HOOK_begin(creat64, int, -1, mode_t mode) HOOK_mid(creat64, mode) HOOK_end(-1)

HOOK_begin(fopen, FILE *, NULL, const char *mode) HOOK_mid(fopen, mode) HOOK_end(NULL)

HOOK_begin(fopen64, FILE *, NULL, const char *mode) HOOK_mid(fopen64, mode) HOOK_end(NULL)

HOOK_begin(mkdir, int, -1, mode_t mode) HOOK_mid(mkdir, mode) HOOK_end(-1)

HOOK_begin(opendir, DIR *, NULL) HOOK_mid(opendir) HOOK_end(NULL)

HOOK_begin(readlink, ssize_t, -1, char *buf, size_t bufsiz) HOOK_mid(readlink, buf, bufsiz) HOOK_end(-1)

HOOK_begin(remove, int, -1) HOOK_mid(remove) HOOK_end(-1)

HOOK_begin(rmdir, int, -1) HOOK_mid(rmdir) HOOK_end(-1)

HOOK_begin(symlink, int, -1, const char *linkpath) HOOK_mid(symlink, linkpath) HOOK_end(-1)

HOOK_begin(unlink, int, -1) HOOK_mid(unlink) HOOK_end(-1)

HOOK_2(rename)

HOOK_2(link)

HOOK_OPEN(open)

HOOK_OPEN(open64)

// clang-format on
#define HOOK_OPENAT(name)                                                      \
    static int (*old_##name)(int dirfd, const char *pathname, int flags,       \
                             ...) = NULL;                                      \
    int name(int dirfd, const char *pathname, int flags, ...) {                \
        if (old_##name == NULL) {                                              \
            void *handle = dlopen("libc.so.6", RTLD_LAZY);                     \
            assert(handle != NULL);                                            \
            old_##name = dlsym(handle, "openat");                              \
            assert(old_##name != NULL);                                        \
        }                                                                      \
                                                                               \
        char mod_path[PATH_MAX] = {};                                          \
        if (dirfd != AT_FDCWD && pathname[0] == '.') {                         \
            struct stat s;                                                     \
            if (fstatat(dirfd, ".", &s, 0) == -1) {                            \
                return -1;                                                     \
            }                                                                  \
            char proc_path[PATH_MAX] = {};                                     \
            snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", dirfd); \
            if (old_readlink == NULL) {                                        \
                void *handle = dlopen("libc.so.6", RTLD_LAZY);                 \
                assert(handle != NULL);                                        \
                old_readlink = dlsym(handle, "readlink");                      \
                assert(old_readlink != NULL);                                  \
            }                                                                  \
            if (old_readlink(proc_path, mod_path, sizeof(mod_path)) == -1) {   \
                return -1;                                                     \
            }                                                                  \
            mod_path[strlen(mod_path)] = '/';                                  \
        }                                                                      \
                                                                               \
        strncpy(mod_path + strlen(mod_path), pathname,                         \
                sizeof(mod_path) - strlen(mod_path));                          \
                                                                               \
        char rpath[PATH_MAX] = {};                                             \
                                                                               \
        if (path_canonicalize(mod_path, rpath) == NULL) return -1;             \
        if (!is_in_basedir(rpath)) {                                           \
            print_bad_path_with_dirfd(__func__, pathname, dirfd);              \
            return -1;                                                         \
        }                                                                      \
                                                                               \
        if (flags & O_CREAT) {                                                 \
            va_list args;                                                      \
            va_start(args, flags);                                             \
            mode_t mode = va_arg(args, mode_t);                                \
            va_end(args);                                                      \
            return old_##name(dirfd, pathname, flags, mode);                   \
        }                                                                      \
        return old_##name(dirfd, pathname, flags);                             \
    }

    // clang-format off
HOOK_OPENAT(openat)

HOOK_OPENAT(openat64)

// clang-format on
#if defined __cplusplus
}
#endif

void opentty() {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    assert(handle != NULL);
    old_open = dlsym(handle, "open");
    assert(old_open != NULL);
    ttyfd = old_open("/dev/tty", O_RDWR);
    assert(ttyfd >= 0);
}
