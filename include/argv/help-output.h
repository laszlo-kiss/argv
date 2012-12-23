#ifndef ARGV_HELP_OUTPUT_H
#define ARGV_HELP_OUTPUT_H

#include "struct-names.h"
#include <stdio.h>

void argv_usage_print(cmd_args *);
void argv_help_print(cmd_args *);

void argv_help_print_to(cmd_args *, FILE *);
void argv_usage_print_to(cmd_args *, FILE *);

#endif /* ARGV_HELP_OUTPUT_H */
