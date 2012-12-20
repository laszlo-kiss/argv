#include <check.h>
#include <argv/argv.h>
#include "test-option.h"

cmd_args *args;

static void setup(void) {
  args = argv_init();;
}

static void teardown(void) {
  argv_free(args);
}

START_TEST(test_argv_option_register) {
  cmd_option *awesome = argv_option_register(args, 'a', "awesome", ARGV_OPTION_VALUE);
  cmd_option *required = argv_option_register(args, 'r', NULL, ARGV_OPTION_REQUIRED);

  ck_assert(awesome != NULL);
  ck_assert(required != NULL);

  ck_assert(argv_option_value(awesome) == NULL);
  ck_assert(argv_option_given(required) == 0);

  fail_unless(awesome == argv_option_iterate(args), "internal parameter order is correct");
  fail_unless(required == argv_option_iterate(args), "internal parameter order is correct");
  fail_unless(NULL == argv_option_iterate(args), "internal parameter order is correct");
}
END_TEST

START_TEST(test_argv_option_needs_value) {
  cmd_option *awesome = argv_option_register(args, 'a', "awesome", ARGV_OPTION_VALUE);
  cmd_option *required = argv_option_register(args, 'r', NULL, ARGV_OPTION_FLAG);

  ck_assert(argv_option_needs_value(awesome) == 1);
  ck_assert(argv_option_needs_value(required) == 0);
}
END_TEST

START_TEST(test_argv_option_required) {
  cmd_option *awesome = argv_option_register(args, 'a', "awesome", ARGV_OPTION_REQUIRED);
  cmd_option *required = argv_option_register(args, 'r', NULL, ARGV_OPTION_FLAG);

  ck_assert(argv_option_required(awesome) == 1);
  ck_assert(argv_option_required(required) == 0);
}
END_TEST

TCase *option_tcase(void) {
  TCase *tc = tcase_create("option");
  tcase_add_checked_fixture(tc, setup, teardown);
  tcase_add_test(tc, test_argv_option_register);
  tcase_add_test(tc, test_argv_option_needs_value);
  tcase_add_test(tc, test_argv_option_required);
  return tc;
}

