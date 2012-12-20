#include <check.h>
#include <argv/argv.h>
#include "test-option.h"

static void setup(void) {
}

static void teardown(void) {
}

START_TEST(test_option_init) {
}
END_TEST

TCase *option_tcase(void) {
  TCase *tc = tcase_create("Core");
  tcase_add_checked_fixture(tc, setup, teardown);
  tcase_add_test(tc, test_option_init);
  return tc;
}

