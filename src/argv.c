#include <argv/argv.h>
#include "cmd-args.h"

#include <stdlib.h>

cmd_args *argv_init(void) {
  cmd_args *args = calloc(1, sizeof(cmd_args));
  assert(args != NULL /* calloc() worked? */);

  args->options = NULL;

  return args;
}

void argv_free(cmd_args *args) {
  if(args != NULL) {
    free(args);
  }
}
