#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <argv/argv.h>
#include <argv/option.h>
#include "cmd-args.h"
#include "cmd-option.h"

#include <stdlib.h>

static void args_init_values(cmd_args *args) {
  static const char *c = "";
  args->values = &c;
}

cmd_args *argv_init(void) {
  cmd_args *args = calloc(1, sizeof(cmd_args));
  assert(args != NULL /* calloc() worked? */);

  args->options = NULL;
  args->num_options = 0;
  args->pos = 0;
  args_init_values(args);

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
    return &args->options[args->pos++];
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

  if(pos < argc) {
    args->values = &argv[pos];
  } else {
    args_init_values(args);
  }

  return ARGV_OK;
}

const char **argv_values(cmd_args *args) {
  return args->values;
}

void argv_usage_print(cmd_args *args) {
  printf("Usage: %s", args->programname);
  cmd_option *option;
  unsigned char required;
  unsigned char has_value;
  const char *name;
  while(NULL != (option = argv_option_iterate(args))) {
    required = argv_option_required(option);
    has_value = argv_option_needs_value(option);
    name = option->longname;
    if(name == NULL) {
      name = &option->shortname;
    }
    printf(" %s%s%s%s%s",
      required ? "" : "[ ",
      name, has_value ? " " : "",
      has_value ? option->value_type_description : "",
      required ? " ]" : "");
  }
}

void argv_help_print(cmd_args *args) {
  const char *name;
  unsigned char both;
  unsigned char is_longname;
  char dynamic_format[21] = {};
  cmd_option *option;
  printf("Parameters:\n");
  while(NULL != (option = argv_option_iterate(args))) {
    name = option->longname;
    is_longname = 1;
    both = 0;
    if(name == NULL) {
      name = &option->shortname;
      is_longname = 0;
    } else if(option->shortname != '\0') {
      both = 1;
    }

    if(option->description != NULL)  {
      if(!both) {
        snprintf(dynamic_format, 20, "  %s%%-%is", is_longname ? "--" : "-", is_longname ? 19 : 20);
        printf(dynamic_format, name);
      } else {
        snprintf(dynamic_format, 20, "  --%%-%is, -%%c", strlen(option->longname));
        printf(dynamic_format, option->longname, option->shortname);
      }
      printf("%s\n", option->description);
    }
  }
}
