#ifndef  ARGV_CMD_ARGS_H
#define ARGV_CMD_ARGS_H

#include <stddef.h>

struct cmd_args {
  cmd_option **options;
  const char **values;
  const char *programname;
  size_t num_options;
  size_t num_values;
  size_t pos;
  size_t values_pos;
};

#endif /* ARGV_CMD_ARGS_H */
