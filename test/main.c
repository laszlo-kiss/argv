#include <stdlib.h>
#include <check.h>
#include "test-argv.h"
#include "test-option.h"

Suite *argv_suite() {
  Suite *s = suite_create("argv");
  suite_add_tcase(s, argv_tcase());
  suite_add_tcase(s, option_tcase());
  return s;
}

int main(void) {
  int number_failed;
  Suite *s = argv_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
