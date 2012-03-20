#ifndef _DHBWCC_MAIN_H
#define _DHBWCC_MAIN_H

#include "diag.h"
#include "resource_manager.h"

/** 
 * \struct cc_options
 * \brief Compiler options for DHBWCC.
 *
 * The global compiler options structure.
 */
typedef struct cc_options
{
  int print_ir;                 /**< Flag indicating if the IR should be printed */
  char *ir_file;                /**< The name of the IR file */

  char *input_file;             /**< The name of the input file */
  char *output_file;            /**< The name of the output file */
} cc_options_t;

extern cc_options_t cc_options; 

#endif
