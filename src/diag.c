#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diag.h"
#include "resource_manager.h"

const char *os_err_msgs[] = {
  [OUT_OF_MEMORY] = "out of memory",
  [FILE_ACCESS] = "file access"
};

/**
 * \brief Print a fatal OS error message 
 * \param err The error code (corresponding to os_err_msgs array  
 * \param errno The errno number (e.g. for failed system calls). '0' should be
                passed if no errno code exists (e.g. for ENOMEM)
 * \param fn The file where this error occurred
 * \param line The line number of the file where this error occurred
 * \param msg The error msg plus a variadic list (printf-like) of arguments
 */
void fatal_os_error (os_error_t err, int errno, const char *fn, const int line, const char *msg, ...) {
  if (err >= NUM_OS_ERRORS) return;

  fprintf(stderr, "Fatal OS Error (%s) in %s:%d -- ", ((errno != 0) ? os_err_msgs[err] : strerror(errno)), fn, line);
  
  va_list list;
  va_start(list, msg);
  vfprintf(stderr, msg, list);
  va_end(list);
}
