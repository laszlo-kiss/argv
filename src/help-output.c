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
  argv_help_print_to(args, stdout);
}

void argv_help_print_to(cmd_args *args, FILE *out) {
  fprintf(out, "Usage: %s", args->programname);
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
    fprintf(out, " %s%s%s%s%s",
      required ? "" : "[ ",
      name, has_value ? " " : "",
      has_value ? option->value_type_description : "",
      required ? " ]" : "");
  }
}

void argv_usage_print_to(cmd_args *args, FILE *out) {
  const char *name;
  unsigned char both;
  unsigned char is_longname;
  char dynamic_format[21] = {};
  cmd_option *option;
  fprintf(out, "Parameters:\n");
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
        fprintf(out, dynamic_format, name);
      } else {
        snprintf(dynamic_format, 20, "  --%%-%is, -%%c", strlen(option->longname));
        fprintf(out, dynamic_format, option->longname, option->shortname);
      }
      fprintf(out, "%s\n", option->description);
    }
  }
}
