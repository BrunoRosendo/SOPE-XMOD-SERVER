#ifndef LOGS_H_
#define LOGS_H_

// System includes
#include <sys/stat.h>

// C libraries includes
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>

// Constants and structs
#define LOGFILE "LOG_FILENAME"
#define START_TIME "XMOD_START_TIME"
#define FIRST_PID "XMOD_FIRST_PID"

typedef struct logInfo {
    bool hasLogFile;
    FILE *logfile;
    long startTime;
} logInfo;

logInfo logs;

extern char *canonicPath;

/**
 * Sets the log file
 */
void setLogFile();

/**
 * Sets the log start time
 */
void setLogStart();

/**
 * Closes the log file
 */
void closeLogFile();

/**
 * Logs a line
 */
void logAction(char*action, char *info);

/** 
 * Logs process creation
 */
void logProcessCreation();

/**
 *  Logs process termination
 */
void logExit(int exitStatus);

/**
 * Logs a received signal
 */ 
void logSignalReceived(int signal);

/**
 * Logs a sent signal
 */
void logSignalSent(int signal, int pid);

/**
 * Logs a change in permission
 */
void logChangePerms(char*path, mode_t newPerm, mode_t oldMode);

#endif  // LOGS_H_
