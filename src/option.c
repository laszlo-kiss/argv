#include <argv/option.h>
#include <argv/argv.h>
#include "cmd-args.h"

struct cmd_option {
  // parmeter naming
  char shortname;
  char *longname;

  // parameter value
  char *default_value;
  char *value;

  // parameter type
  int flags;

  // parameter info (for --help dialog)
  char *description;
  char *value_type_description;
};

static cmd_option *argv_init_option(cmd_args *args) {
  args->options = realloc(args->options, sizeof(cmd_option) * ++args->num_options);
  assert(args->options != NULL /* realloc() worked ? */);
  cmd_option *option = args->options[args->num_options - 1];

  // initialization
  option->shortname = '\0';
  option->longname = NULL;
  option->default_value = NULL;
  option->value = NULL;
  option->flags = 0x0;
  option->description = NULL;
  option->value_type_description = NULL;

  return option;
}

cmd_option *argv_option_register(cmd_args *args, char shortname, char *longname, int flags) {
  cmd_option *option = argv_init_option(args);
  assert(shortname != NULL || longname != NULL /* either shortname or an longname given */);
  option->shortname = shortname;
  option->longname = longname;
  option->flags = flags;
  return option;
}
