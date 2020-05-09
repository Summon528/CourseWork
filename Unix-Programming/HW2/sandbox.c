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

#define IS_SLASH(s) (s == '/')

char *path_canonicalize(const char *path, char *name) {
    if (path[0] == '.') {
        char cwd[PATH_MAX] = {};
        getcwd(cwd, sizeof(cwd));
        cwd[strlen(cwd)] = '/';
        strncpy(name, cwd, PATH_MAX);
    }
    strncpy(name + strlen(name), path, PATH_MAX - strlen(name));
    char *next;
    int l, w, first_dot;

    for (next = name; *next && (*next != '.'); next++) {
    }

    l = w = first_dot = next - name;
    while (name[l] != '\0') {
        if (name[l] == '.' && (name[l + 1] == '/') &&
            (l == 0 || (name[l - 1] == '/')))
            l += 2;
        else
            name[w++] = name[l++];
    }

    if (w == 1 && name[0] == '.')
        w--;
    else if (w > 1 && name[w - 1] == '.' && (name[w - 2] == '/'))
        w--;
    name[w] = '\0';

    l = first_dot;

    while (name[l] != '\0') {
        if (name[l] == '.' && name[l + 1] == '.' && (name[l + 2] == '/') &&
            (l == 0 || (name[l - 1] == '/'))) {
            int m = l + 3, n;

            l = l - 2;
            if (l >= 0) {
                while (l >= 0 && !(name[l] == '/')) l--;
                l++;
            } else
                l = 0;
            n = l;
            while ((name[n] = name[m])) (++n, ++m);
        } else
            ++l;
    }

    if (l == 2 && name[0] == '.' && name[1] == '.')
        name[0] = '\0';
    else if (l > 2 && name[l - 1] == '.' && name[l - 2] == '.' &&
             (name[l - 3] == '/')) {
        l = l - 4;
        if (l >= 0) {
            while (l >= 0 && !(name[l] == '/')) l--;
            l++;
        } else
            l = 0;
        name[l] = '\0';
    }

    return name;
}

static char *basedir = "/";
static int ttyfd = -1;

void print_bad_fun(const char *fun, const char *exe) {
    if (ttyfd == -1) return;
    char buf[256] = {};
    snprintf(buf, sizeof(buf), "[sandbox] %s(%s): not allowed\n", fun, exe);
    write(ttyfd, buf, sizeof(buf));
}

void print_bad_path(const char *fun, const char *path) {
    if (ttyfd == -1) return;
    char buf[256] = {};
    snprintf(buf, sizeof(buf), "[sandbox] %s: access to %s is not allowed\n",
             fun, path);
    write(ttyfd, buf, sizeof(buf));
}

bool is_in_basedir(const char *path) {
    int basedir_len = strlen(basedir);
    int path_len = strlen(path);
    if (path_len < basedir_len) return false;
    for (int i = 0; i < basedir_len; i++) {
        if (path[i] != basedir[i]) return false;
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
        path_canonicalize(target, rpath);

#define HOOK_mid(name, ...)                       \
    if (is_in_basedir(rpath)) {                   \
        return old_##name(target, ##__VA_ARGS__); \
    };

#define HOOK_end(r)                  \
    print_bad_path(__func__, rpath); \
    return r;                        \
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
        path_canonicalize(oldpath, rpath1);                                    \
        path_canonicalize(newpath, rpath2);                                    \
        bool bad = false;                                                      \
        if (!is_in_basedir(rpath1)) {                                          \
            print_bad_path(__func__, rpath1);                                  \
            bad = true;                                                        \
        }                                                                      \
        if (!is_in_basedir(rpath2)) {                                          \
            print_bad_path(__func__, rpath2);                                  \
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
        path_canonicalize(pathname, rpath);                                \
        if (!is_in_basedir(rpath)) {                                       \
            print_bad_path(__func__, rpath);                               \
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
    return -1;
};
int execlp(const char *file, const char *arg, ...) {
    print_bad_fun(__func__, file);
    return -1;
};
int execle(const char *pathname, const char *arg, ...) {
    print_bad_fun(__func__, pathname);
    return -1;
};
int execv(const char *pathname, char *const argv[]) {
    print_bad_fun(__func__, pathname);
    return -1;
};
int execve(const char *pathname, char *const argv[], char *const envp[]) {
    print_bad_fun(__func__, pathname);
    return -1;
};
int execvp(const char *file, char *const argv[]) {
    print_bad_fun(__func__, file);
    return -1;
};
int system(const char *command) {
    print_bad_fun(__func__, command);
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
    path_canonicalize(path, rpath);
    if (is_in_basedir(rpath)) return old___xstat(ver, path, stat_buf);

    print_bad_path(__func__, rpath);
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
    path_canonicalize(path, rpath);
    if (is_in_basedir(rpath)) return old___xstat64(ver, path, stat_buf);

    print_bad_path(__func__, rpath);
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
        path_canonicalize(mod_path, rpath);                                    \
        if (!is_in_basedir(rpath)) {                                           \
            print_bad_path(__func__, rpath);                                   \
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

__attribute__((constructor)) void init() {
    void *handle = dlopen("libc.so.6", RTLD_LAZY);
    assert(handle != NULL);
    old_open = dlsym(handle, "open");
    assert(old_open != NULL);
    ttyfd = old_open("/dev/tty", O_RDWR);
    assert(ttyfd >= 0);
    basedir = getenv("BASE_DIR");
    if (basedir == NULL) basedir = "./";
    basedir = realpath(basedir, NULL);
    assert(basedir != NULL);
}
