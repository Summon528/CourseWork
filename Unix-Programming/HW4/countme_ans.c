#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        assert(ptrace(PTRACE_TRACEME, 0, 0, 0) >= 0);
        execlp("./countme", "./countme", NULL);
        assert(0);
    } else if (pid > 0) {
        int status, cnt = 0;
        assert(wait(&status) >= 0);
        assert(ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL) >= 0);
        while (WIFSTOPPED(status)) {
            cnt++;
            assert(ptrace(PTRACE_SINGLESTEP, pid, 0, 0) >= 0);
            assert(wait(&status) >= 0);
        }
        printf("%d\n", cnt);
    }
}