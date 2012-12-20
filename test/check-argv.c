#include <check.h>
#include <argv/argv.h>

cmd_args *args;

static void setup(void) {
  args = argv_init();
  argv_option_register(args, 'v', "verbose", ARGV_OPTION_FLAG);
  argv_option_register(args, 'f', "file", ARGV_OPTION_VALUE | ARGV_OPTION_REQUIRED);
  argv_option_register(args, 'o', "output", ARGV_OPTION_VALUE);
}

static void teardown(void) {
  argv_free(args);
}

START_TEST(test_argv_init) {
  fail_unless(args != NULL, "Initialization worked properly");
}
END_TEST


START_TEST(test_argv_option_iterate) {
  cmd_option *option;
  cmd_option *option1;

  option1 = option = argv_option_iterate(args);
  fail_unless(option != NULL, "first iteration returns valid option");

  option = argv_option_iterate(args);
  fail_unless(option != NULL, "second iteration returns valid option");

  option = argv_option_iterate(args);
  fail_unless(option != NULL, "thrird iteration returns valid option");

  option = argv_option_iterate(args);
  fail_unless(option == NULL, "fourth iteration returns NULL");

  option = argv_option_iterate(args);
  fail_unless(option == NULL, "fifth iteration also returns NULL");

  argv_option_iterate_reset(args);
  option = argv_option_iterate(args);
  fail_unless(option == option1, "resetting iterator works properly and returns option in same order");
}
END_TEST

Suite *argv_suite(void) {
  Suite *s = suite_create("argv");
  TCase *tc_core = tcase_create("Core");
  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_argv_init);
  tcase_add_test(tc_core, test_argv_option_iterate);
  suite_add_tcase(s, tc_core);
  return s;
}

