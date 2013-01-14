#ifndef  ARGV_CMD_ARGS_H
#define ARGV_CMD_ARGS_H

#include <stddef.h>

struct cmd_args {
  const char *programname;
  cmd_option **options;
  const char **values;
  char **errors;
  size_t num_options;
  size_t num_values;
  size_t num_errors;
  size_t pos;
  size_t values_pos;
};

#endif /* ARGV_CMD_ARGS_H */
