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
  args->errors = NULL;
  argv_clear_errors(args);

  return args;
}

void argv_free(cmd_args *args) {
  if(args != NULL) {
    cmd_option *option;
    argv_option_iterate_reset(args);
    while(NULL != (option = argv_option_iterate(args))) {
      free(option);
    }
    free(args->options);
    argv_clear_errors(args);
    free(args);
  }
}

cmd_option *argv_option_iterate(cmd_args *args) {
  cmd_option *option = NULL;
  if(args->pos < args->num_options) {
    option = args->options[args->pos++];
  }
  return option;
}

void argv_option_iterate_reset(cmd_args *args) {
  args->pos = 0;
}

static unsigned char is_parameter(const char *value, int length) {
  return length > 1 && value[0] == '-' ? 1 : 0;
}

static unsigned char is_single_short_parameter(const char *value, int length) {
  return length == 2 && value[0] == '-' && value[1] != '-';
}

static unsigned char is_long_parameter(const char *value, int length) {
  return length > 2 && value[0] == '-' && value[1] == '-' ? 1 : 0;
}

static int is_long_parameter_with_value(const char *value, int length) {
  if(!is_long_parameter(value, length)) return 0;

  int pos = 2;
  unsigned char contains = 0;


  while(pos < length && !contains) {
    if(value[pos] == '=') {
      contains = 1;
    }
  }

  return contains ? pos : 0;
}

static unsigned char is_end_of_parameters(const char *value, int length) {
  return length == 2 && value[0] == '-' && value[1] == '-' ? 1 : 0;
}

static cmd_option *find_option(cmd_args *args, const char *current int length, unsigned is_shortname) {
  cmd_option *option;
  argv_option_iterate_reset(args);
  while(NULL != (option = argv_option_iterate(args))) {
    if((is_shortname
        && (option->shortname != '\0' && current[0] == option->shortname))
        || (option->longname != NULL && strncmp(current + 2, option->longname, length) == 0)) {
      return option;
    }
  }
  return NULL;
}

static cmd_option *find_option_by_longname(cmd_args *args, const char *current int length) {
  return find_option(args, current, length, 0);
}

static cmd_option *find_option_by_shortname(cmd_args *args, const char *current int length) {
  return find_option(args, current, length, 1);
}

static char parse_argv_tokens(cmd_args *args, int new_argc, const char **new_argv, cmd_option **parsed_option, const char **parsed_value) {
  static const char **argv = NULL;
  static int argc = 0;
  static int position = 0;
  static int valueposition = 0;
  static int subposition = 0;

  if(new_argc > 0 && new_argv != NULL && new_argv != argv) {
    argv = new_argv;
    argc = new_argc;
    position = 0;
    subposition = 0;
    valueposition = 0;
  }

  if(position >= argc) return -1;

  const char *current = argv[position];
  int length = strlen(current);
  int pos;
  cmd_option *option;

  if(is_parameter(current, length)) {
    if(is_long_parameter(current, length)) {
      if((pos = is_long_parameter_with_value(current, length)) && valueposition) {
        *parsed_value = current + valueposition + 1;
        valueposition = 0;
        position++;
      } else {
        valueposition = pos;
        option = find_option_by_longname(args, current, length - pos);
        // no parameter found
        if(option == NULL) return -1;
        *parsed_option = option;
        return 0;
      }
    } else {
      subposition++;
      if(valueposition)  {
        *parsed_value = current + subposition;
        valueposition = 0;
        position++;
      } else {
        option = find_option_by_shortname(args, current + subposition, 1);
        if(option == NULL) {
          return -1;
        } else {
          if(argv_option_needs_value(option)) {
            valueposition = 1;
          }
        }
        if(subposition + 1 >= length) {
          position++;
          valueposition = 0;
        }
        *parsed_option = option;
        return 0;
      }
    }
  } else {
    *parsed_value = current;
    position++;
    return 1;
  }
}


int argv_parse(cmd_args *args, int argc, const char **argv) {
  if(argc <= 0) {
    return ARGV_EMPTY;
  }

  return argv_parse_partially(args, argv[0], argc - 1, argv + 1);
}

int argv_parse_partially(cmd_args *args, const char *programname, int argc, const char **argv) {
  cmd_option *option;  
  int pos, expect_no_more_parameters = -1;
  unsigned char expect_value = 0;
  unsigned char is_param;
  cmd_option *current_option = NULL;
  argv_option_iterate_reset(args);

  args->programname = programname;

  for(pos = 0; pos < argc; pos++) {
    current = argv[pos];
    is_param = is_parameter(current, length);

    if(!is_param && !expect_value && expect_no_more_parameters < 0) {
      expect_no_more_parameters = pos;
    }

    if(is_param && expect_value) {
      return ARGV_VALUE_EXPECTED;
    }

    if(!is_param && expect_value) {
      current_option->value = current;
      expect_value = 0;
    }

    if(is_param && !expect_value) {
      if(expect_no_more_parameters > 0) {
        return ARGV_UNEXPECTED_TOKEN;
      }
      current_option = NULL;
      if(is_end_of_parameters(current, length)) {
        expect_no_more_parameters = pos + 1;
        break;
      }
      argv_option_iterate_reset(args);
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
          break;
        }
      }
      if(current_option == NULL) {
        return ARGV_UNDEFINED_PARAMETER;
      } else {
        expect_value = argv_option_needs_value(current_option);
      }
    }
  }

  if(expect_no_more_parameters >= 0 && expect_no_more_parameters < argc) {
    args->values = &argv[expect_no_more_parameters];
    args->num_values = argc - expect_no_more_parameters;
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

char **argv_errors(cmd_args *args) {
  return args->errors;
}

void argv_clear_errors(cmd_args *args) {
  free(args->errors);
  args->errors = NULL;
  args->num_errors;
}

void argv_add_error(cmd_args *args, char *error) {
  args->errors = realloc(args->errors, sizeof(char *) * ++args->num_errors);
  args->errors[args->num_errors - 1] = error;
}
