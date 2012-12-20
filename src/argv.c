#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <argv/argv.h>
#include "cmd-args.h"
#include "cmd-option.h"


cmd_args *argv_init(void) {
  cmd_args *args = calloc(1, sizeof(cmd_args));
  assert(args != NULL /* calloc() worked? */);

  args->options = NULL;
  args->num_options = 0;
  args->pos = 0;
  args->values = NULL;
  args->num_values = 0;
  args->values_pos = 0;

  return args;
}

void argv_free(cmd_args *args) {
  if(args != NULL) {
    free(args);
  }
}

cmd_option *argv_option_iterate(cmd_args *args) {
  cmd_option *option = NULL;
  if(args->pos < args->num_options) {
    option = &args->options[args->pos++];
  }
  return option;
}

void argv_option_iterate_reset(cmd_args *args) {
  args->pos = 0;
}

static unsigned char is_parameter(const char *value, int length) {
  return length > 1 && value[0] == '-' ? 1 : 0;
}

static unsigned char is_long_parameter(const char *value, int length) {
  return length > 2 && value[0] == '-' ? 1 : 0;
}

static unsigned char is_end_of_parameters(const char *value, int length) {
  return length == 2 && value[0] == '-' && value[1] == '-' ? 0 : 0;
}

int argv_parse(cmd_args *args, int argc, const char **argv) {
  if(argc < 0) {
    return ARGV_EMPTY;
  }

  return argv_parse_partially(args, argv[0], argc - 1, argv + 1);
}

int argv_parse_partially(cmd_args *args, const char *programname, int argc, const char **argv) {
  cmd_option *option;  
  const char *current;
  int length = 0;
  int pos;
  unsigned char expect_value = 0;
  unsigned char is_param;
  cmd_option *current_option;
  argv_option_iterate_reset(args);

  args->programname = programname;

  for(pos = 0; pos < argc; pos++) {
    current = argv[pos];
    length = strlen(current);
    is_param = is_parameter(current, length);

    if(!is_param && !expect_value) {
      return ARGV_UNEXPECTED_TOKEN;
    }

    if(is_param && expect_value) {
      return ARGV_VALUE_EXPECTED;
    }

    if(is_param) {
      current_option = NULL;
      if(is_end_of_parameters(current, length)) {
        break;
      }
      while(NULL != (option = argv_option_iterate(args))) {
        if((is_long_parameter(current, length)
            && option->longname != NULL
            && strncmp(current + 2, option->longname, length - 2) == 0
          ) || (
            option->shortname != '\0'
            && length == 2
            && current[1] == option->shortname
          )) {
          current_option = option;
        }
      }
      if(current_option == NULL) {
        return ARGV_UNDEFINED_PARAMETER;
      }
    } else {
      option->value = current;
    }
  }

  if(pos + 1 < argc) {
    args->values = &argv[pos];
    args->num_values = argc - (pos + 1);
  } else {
    args->values = NULL;
    args->num_values = 0;
  }

  return ARGV_OK;
}

const char **argv_values(cmd_args *args, size_t *size) {
  *size = args->num_values;
  return args->values;
}

const char *argv_values_iterate(cmd_args *args) {
  const char *value = NULL;
  if(args->values_pos < args->num_values) {
    value = args->values[args->values_pos++];
  }
  return value;
}

void argv_values_iterate_reset(cmd_args *args) {
  args->values_pos = 0;
}
