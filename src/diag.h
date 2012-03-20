#ifndef _DHBWCC_DIAG_H
#define _DHBWCC_DIAG_H

typedef enum {
  OUT_OF_MEMORY = 0,
  FILE_ACCESS = 1,
  PROCESS_RESOURCES = 2,
  /* add new error codes above */
  NUM_OS_ERRORS
} os_error_t;

extern void fatal_os_error (os_error_t err, int errno, const char *fn, const int line, const char *msg, ...);
#define FATAL_OS_ERROR(err,errno,...) fatal_os_error(err, errno, __FILE__, __LINE__, __VA_ARGS__);

#endif
