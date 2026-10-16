#pragma once
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstdlib>

template<class F>
bool stops_silently(F operation) {
    int descriptors[2];
    if (pipe(descriptors)!=0) std::abort();
    const pid_t child=fork();
    if (child<0) std::abort();
    if (child==0) {
        // Verify delivery of SIGABRT without invoking host core-dump services.
        std::signal(SIGABRT,[](int){_exit(199);});
        alarm(10);
        close(descriptors[0]);
        dup2(descriptors[1],STDOUT_FILENO);
        dup2(descriptors[1],STDERR_FILENO);
        close(descriptors[1]);
        struct rlimit limit{0,0};
        setrlimit(RLIMIT_CORE,&limit);
        operation();
        _exit(0);
    }
    close(descriptors[1]);
    char buffer[256];
    bool silent=true;
    for (;;) {
        const ssize_t n=read(descriptors[0],buffer,sizeof buffer);
        if (n>0) silent=false;
        else if (n<0 && errno==EINTR) continue;
        else break;
    }
    close(descriptors[0]);
    int status=0;
    if (waitpid(child,&status,0)!=child) std::abort();
    return silent && WIFEXITED(status) && WEXITSTATUS(status)==199;
}
