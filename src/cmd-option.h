#ifndef  ARGV_CMD_OPTION_H
#define ARGV_CMD_OPTION_H

struct cmd_option {
  // parmeter naming
  char shortname;
  const char *longname;

  // parameter value
  const char *default_value;
  const char *value;

  // parameter type
  int flags;

  // parameter info (for --help dialog)
  const char *description;
  const char *value_type_description;
};

#endif /* ARGV_CMD_OPTION_H */
