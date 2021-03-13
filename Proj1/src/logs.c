#include "logs.h"

void setLogFile(char **envp, struct logInfo *log) {
    for (char **env = envp; *env != 0; env++)     {
        char *thisEnv = *env;
        if (strstr(thisEnv, LOGFILE) != 0) {
            const char delim[2] = "=";
            strtok(thisEnv, delim);
            char *filename = strtok(NULL, delim);
            log->logfile = fopen(filename, "w");
            log->hasLogFile = true;
            return;
        }
    }
    log->hasLogFile = false;
}

void closeLogFile(struct logInfo *log) {
    if (log->hasLogFile) {
        fclose(log->logfile);
    }
}

void setLogStart(struct logInfo *log) {
    log->startTime = clock();
}

void logAction(struct logInfo *log, char *action, char *info) {
    if (!log->hasLogFile) {
        return;
    }
    pid_t pid = getpid();
    clock_t now = clock();
    fprintf(
        log->logfile,
        "%ld ; %d ; %s ; %s\n",
        now - log->startTime,
        pid,
        action,
        info);
}

void logProcessCreation(struct logInfo *log, int argc, char *argv[]) {
    char *args = argv[0];
    for (int i = 1; i < argc; i++) {
        strcat(args, " ");
        strcat(args, argv[i]);
    }
    logAction(log, "PROC_CREAT", args);
}

void logExit(struct logInfo *log, int exitStatus) {
    char exit[20];
    snprintf(exit, sizeof(exit), "%d", exitStatus);
    logAction(log, "PROC_EXIT", exit);
}

void logSignalReceived(struct logInfo *log, int signal) {
    char *sigName = strdup(sys_siglist[signal]);
    if (!sigName) {
        return;
    }
    char *oldPointer = sigName;
    while (*sigName) {
        *sigName = toupper(*sigName);
        sigName++;
    }

    logAction(log, "SIGNAL_RECV", sigName);
    free(oldPointer);
}

void logSignalSent(struct logInfo *log, int signal, int pid) {
    char *sigName = strdup(sys_siglist[signal]);
    if (!sigName) {
        return;
    }
    char *oldPointer = sigName;
    while (*sigName) {
        *sigName = toupper(*sigName);
        sigName++;
    }

    char sigNamePid[100];
    snprintf(sigNamePid, sizeof(sigNamePid), "%s : %d", sigName, pid);
    logAction(log, "SIGNAL_SENT", sigNamePid);
    free(oldPointer);
}

void logChangePerms(struct logInfo *log, char *path, mode_t oldPerm,
                    mode_t newPerm) {
    char perms[100];
    snprintf(perms, sizeof(perms), " : %o : %o", oldPerm, newPerm);
    strcat(path, perms);
    logAction(log, "FILE_MODF", path);
}
