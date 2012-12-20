#ifndef ARGV_OPTIONS_H
#define ARGV_OPTIONS_H

#include "option-flags.h"
#include "struct-names.h"

cmd_option *argv_option_register(cmd_args *args, char shortname, const char *longname, int flags);

void argv_option_set_default_value(cmd_option *option, const char *default_value);
void argv_option_set_description(cmd_option *option, const char *description);
void argv_option_set_value_type_description(cmd_option *option, const char *value_type_description);

unsigned char argv_option_given(cmd_option *option);
const char *argv_option_value(cmd_option *option);

unsigned char argv_option_needs_value(cmd_option *option);
unsigned char argv_option_required(cmd_option *option);

#endif /* ARGV_OPTIONS_H */
