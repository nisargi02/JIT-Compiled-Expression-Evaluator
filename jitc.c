/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * jitc.c
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dlfcn.h>
#include "system.h"
#include "jitc.h"

struct jitc {
    void *handle;
};

int jitc_compile(const char *input, const char *output) {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        const char *args[7];
        args[0] = "/usr/bin/gcc";
        args[1] = "-fpic";
        args[2] = "-shared";
        args[3] = input;
        args[4] = "-o";
        args[5] = output;
        args[6] = NULL;

        execv("/usr/bin/gcc",(char *const *)args);
        perror("execv");
        exit(EXIT_FAILURE);
    } else if (child_pid > 0){
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                TRACE("Compilation failed");
                return exit_status;
            }
        } else {
            TRACE("Child process did not exit normally");
            return -1;
        }
    } else {
    	TRACE("Fork Creation Failed.");
        return -1;
    }
    return 0;
}

struct jitc *jitc_open(const char *pathname) {
    struct jitc *jitc = (struct jitc *)malloc(sizeof(struct jitc));
    
    if (!jitc) {
        TRACE("Failed to allocate memory for struct jitc.");
        return NULL;
    }

    jitc -> handle = dlopen(pathname, RTLD_LAZY);
    if (!jitc -> handle) {
        TRACE(dlerror());
        FREE(jitc);
        return NULL;
    }

    return jitc;
}

void jitc_close(struct jitc *jitc) {
    if(!dlclose(jitc -> handle)){
        printf("Sucess");
    }
    else{
        printf("Fail");
    }
    free(jitc);
}

long jitc_lookup(struct jitc *jitc, const char *function_name) {
    long *function_address = dlsym(jitc -> handle, function_name);

    if (!function_address) {
        TRACE(dlerror());
        return 0;
    }
    return (long)function_address;
}
