#include <argv/option.h>
#include <argv/argv.h>
#include "cmd-args.h"
#include "cmd-option.h"

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

cmd_option *argv_option_register(cmd_args *args, char shortname, const char *longname, int flags) {
  cmd_option *option = argv_init_option(args);
  assert(shortname != NULL || longname != NULL /* either shortname or an longname given */);
  option->shortname = shortname;
  option->longname = longname;
  option->flags = flags;
  return option;
}

void argv_option_set_default_value(cmd_option *option, const char *default_value) {
  option->default_value = default_value;
}

void argv_option_set_description(cmd_option *option, const char *description) {
  option->description = description;
}

void argv_option_set_value_type_description(cmd_option *option, const char *value_type_description) {
  option->value_type_description = value_type_description;
}

unsigned char argv_option_given(cmd_option *option) {
  return value == NULL ? 0 : 1;
}

const char *argv_option_value(cmd_option *option) {
  return option->value;
}
