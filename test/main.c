#include <stdlib.h>
#include <check.h>
#include "check-argv.h"

int main(void) {
  int number_failed;
  Suite *s = argv_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
