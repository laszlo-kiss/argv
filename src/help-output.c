#include <stdio.h>
#include <string.h>
#include <argv/help-output.h>
#include <argv/argv.h>
#include "cmd-args.h"
#include "cmd-option.h"


void argv_help_print(cmd_args *args) {
  argv_help_print_to(args, stdout);
}

void argv_usage_print(cmd_args *args) {
  argv_usage_print_to(args, stdout);
}

void argv_usage_print_to(cmd_args *args, FILE *out) {
  fprintf(out, "Usage: %s", args->programname);
  cmd_option *option;
  unsigned char required;
  unsigned char has_value;
  const char *name;
  char shortname_buffer[2] = {};
  char dash_buffer[3] = {'-'};
  argv_option_iterate_reset(args);
  while(NULL != (option = argv_option_iterate(args))) {
    required = argv_option_required(option);
    has_value = argv_option_needs_value(option);
    name = option->longname;
    dash_buffer[1] = '-';
    if(name == NULL) {
      dash_buffer[1] = '\0';
      shortname_buffer[0] = option->shortname;
      name = shortname_buffer;
    }

    fputc(' ', out);
    if(!required) {
      fprintf(out, "[ ");
    }
    fprintf(out, "%s%s", dash_buffer, name);

    if(has_value) {
      fputc(' ', out);
      fprintf(out, "%s", option->value_type_description == NULL ? "value" : option->value_type_description);
    }

    if(!required) {
      fprintf(out, " ]");
    }
  }
}

void argv_help_print_to(cmd_args *args, FILE *out) {
  const char *name;
  unsigned char both;
  unsigned char is_longname;
  char dynamic_format[21] = {};
  cmd_option *option;
  fprintf(out, "Parameters:\n");
  argv_option_iterate_reset(args);
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
        snprintf(dynamic_format, 20, "  %s%%-%is ", is_longname ? "--" : "-", is_longname ? 28 : 29);
        fprintf(out, dynamic_format, name);
      } else {
        snprintf(dynamic_format, 20, "  --%%-%is, -%%c %%-%is", strlen(option->longname), 30 - 6 - strlen(option->longname));
        fprintf(out, dynamic_format, option->longname, option->shortname, "");
      }
      fprintf(out, "%s\n", option->description);
    }
  }
}
