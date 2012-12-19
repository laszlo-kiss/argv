#ifndef ARGV_ARGV_H
#define ARGV_ARGV_H

#include "option.h"

struct cmd_args;
typedef struct cmd_args cmd_args; 

cmd_args *argv_init(void);
void argv_free(cmd_args *args);

void argv_parse(cmd_args *args, int argc, const char **argv);

cmd_option *argv_option_iterate(cmd_args *args);
const char **argv_values(cmd_args *args);

void argv_usage_print(cmd_args *);
void argv_help_print(cmd_args *);


#endif /* ARGV_ARGV_H */
