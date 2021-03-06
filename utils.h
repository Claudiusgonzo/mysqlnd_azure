#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "php_mysqlnd_azure.h"

extern FILE *logfile;
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

// Azure Log Levels, 1(ERROR), 2(INFO), 3(DEBUG)
#define ALOG_LEVEL_ERR  1
#define ALOG_LEVEL_INFO 2
#define ALOG_LEVEL_DBG  3

// Azure Log Types
#define ALOG_TYPE_STDERR   1
#define ALOG_TYPE_FILE     2


#define OPEN_LOGFILE(filename)                                                               \
  do {                                                                                       \
    if (filename != NULL) {                                                                  \
      logfile = fopen(filename, "a");                                                        \
    }                                                                                        \
  } while (0)

#define CLOSE_LOGFILE()                                                                      \
  do {                                                                                       \
    if (logfile != NULL) {                                                                   \
      fclose(logfile);                                                                       \
    } } while (0)

#define AZURE_LOG(level, format, ...)                                                        \
  do {                                                                                       \
    if (MYSQLND_AZURE_G(logOutput) && level <= MYSQLND_AZURE_G(logLevel)) {                  \
      time_t now = time(NULL);                                                               \
      char timestr[20];                                                                      \
      char *levelstr = (level == ALOG_LEVEL_ERR ? "ERROR" :                                  \
          (level == ALOG_LEVEL_INFO ? "INFO " : "DEBUG"));                                   \
      strftime(timestr, 20, TIME_FORMAT, localtime(&now));                                   \
      if ((MYSQLND_AZURE_G(logOutput) & ALOG_TYPE_FILE) && logfile) {                        \
        fprintf(logfile, "[%s] [MYSQLND_AZURE] [%s] " format "\n", timestr, levelstr,        \
                                ## __VA_ARGS__);                                             \
        fflush(logfile);                                                                     \
      }                                                                                      \
      else if (MYSQLND_AZURE_G(logOutput) & ALOG_TYPE_STDERR) {                              \
        fprintf(stderr, "[%s] [MYSQLND_AZURE] [%s] " format "\n", timestr, levelstr,         \
                                ## __VA_ARGS__);                                             \
        fflush(stderr);                                                                      \
      }                                                                                      \
    }                                                                                        \
} while (0)


#endif // UTILS_H
