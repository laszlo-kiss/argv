#ifndef ARGV_ARGV_H
#define ARGV_ARGV_H

#include "option.h"
#include "struct-names.h"


/**
 * get an cmd_args object to make definitions for which parameters are
 * supported
 *
 * @return newly allocated cmd_args object (needs to be free'd using argv_free())
 */
cmd_args *argv_init(void);

/**
 * free all the memory used by the cmd_args object
 * and its options
 *
 * @params cmd_args object to free, may be NULL
 */ 
void argv_free(cmd_args *args);

/**
 * error codes for argv_parse()
 *
 * ARGV_OK no problems
 * ARGV_EMPTY no arguments given, no programname was extracted
 * ARGV_UNEXPECTED_TOKEN an parameter was expected but a token appeared
 * ARGV_VALUE_EXPECTED an parameter appeared, but an value was expected
 * ARGV_UNDEFINED_PARAMETER an parameter not previously defined by argv_option_register() appeared
 * ARGV_REQUIRED_PARAMETER_MISSING an required parameter is not given
 */
typedef enum {
  ARGV_OK = 0,
  ARGV_EMPTY,
  ARGV_UNEXPECTED_TOKEN,
  ARGV_VALUE_EXPECTED,
  ARGV_UNDEFINED_PARAMETER
} argv_parse_error;

/**
 * parses the argc and argv arguments given to your main() function
 * extracts the programname (argv[0]) for later use in argv_usage_print()
 *
 * if you only want to parse a part of the arguments given to a program
 * use the argv_parse_partially() function
 *
 * @param args the cmd_args object to use
 * @param argc the number of parameters in argv
 * @param argv the command line parameters
 * @return value other than 0 on failure. see error codes above.
 * @see argv_parse_partially
 */
int argv_parse(cmd_args *args, int argc, const char **argv);

/**
 * parses command line arguments
 *
 * @param args the cmd_args object to use
 * @param programname the programname (only used in argv_usage_print())
 * @param argc the number of parameters in argv
 * @param argv (some of) the command line parameters
 * @return value other than 0 on failure. see error codes above.
 * @see argv_parse()
 */
int argv_parse_partially(cmd_args *args, const char *programname, int argc, const char **argv);

/**
 * iterate over all options added using argv_option_register()
 *
 * @param args cmd_args object to use
 * @return the next cmd_option object, or NULL if end of list reached
 */
cmd_option *argv_option_iterate(cmd_args *args);

/**
 * move the internal pointer of argv_option_iterate()
 * back to the first option
 *
 * @param args cmd_args object to use
 */
void argv_option_iterate_reset(cmd_args *args);

/**
 * returns the remaining values which are not directly
 * a value of a parameter
 *
 * @param args cmd_args object to use
 * @param size the number of remaining values
 * @return the remaining values
 */
const char **argv_values(cmd_args *args, size_t *size);

/**
 * iterate over the remaining values which are not directly 
 * a value of a parameter
 *
 * @param args cmd_args object to use
 * @return the next remaining value or MULL
 */
const char *argv_values_iterate(cmd_args *args);

/**
 * resets the internal pointer of argv_values_iterate()
 * so you can iterate it multiple times
 *
 * @param args cmd_args object to use
 */
void argv_values_iterate_reset(cmd_args *args);

#endif /* ARGV_ARGV_H */
