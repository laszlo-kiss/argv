#include <check.h>
#include <argv/argv.h>

cmd_args *args;

static void setup(void) {
  args = argv_init();
}

static void teardown(void) {
  argv_free(args);
}

START_TEST(test_argv_create) {
  fail_unless(args != NULL, "Initialization worked properly");
}
END_TEST

Suite *argv_suite(void) {
  Suite *s = suite_create("argv");
  TCase *tc_core = tcase_create("Core");
  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_argv_create);
  suite_add_tcase(s, tc_core);
  return s;
}

