#include <check.h>
#include <argv/argv.h>
#include <argv/help-output.h>
#include "test-argv.h"

cmd_args *args;
cmd_option *verbose_option;
cmd_option *file_option;
cmd_option *output_option;

static void setup(void) {
  args = argv_init();
  verbose_option = argv_option_register(args, 'v', NULL, ARGV_OPTION_FLAG);
  file_option = argv_option_register(args, 0, "file", ARGV_OPTION_VALUE | ARGV_OPTION_REQUIRED);
  output_option = argv_option_register(args, 'o', "output", ARGV_OPTION_VALUE);
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

START_TEST(test_argv_parse_partially_argv_ok) {
  const char *str_args[] = {
    "-v",
    "-f",
    "file.txt",
  };
  fail_unless(ARGV_OK == argv_parse_partially(args, "testprogram", 3, str_args), "argv_parse_partially parses correct arguements correctly");
} END_TEST

START_TEST(test_argv_parse_partially_values) {
  const char *str_args[] = {
    "value1",
    "value2",
  };
  size_t num_values;
  const char **values;
  fail_unless(ARGV_OK == argv_parse_partially(args, "testprogram", 0, str_args), "argv_parse_partially parses values correct when none given");
  values = argv_values(args, &num_values);
  ck_assert(values == NULL);
  ck_assert(num_values == 0);

  fail_unless(ARGV_OK == argv_parse_partially(args, "testprogram", 1, str_args), "argv_parse_partially parses values correct when one given");
  values = argv_values(args, &num_values);
  ck_assert(values != NULL);
  ck_assert(strcmp(values[0], "value1") == 0);
  ck_assert(num_values == 1);

  fail_unless(ARGV_OK == argv_parse_partially(args, "testprogram", 2, str_args), "argv_parse_partially parses values correct when two given");
  values = argv_values(args, &num_values);
  ck_assert(values != NULL);
  ck_assert(strcmp(values[0], "value1") == 0);
  ck_assert(strcmp(values[1], "value2") == 0);
  ck_assert(num_values == 2);
} END_TEST

START_TEST(test_argv_parse_partially_unexpected_token) {
  const char *str_args[] = {
    "-v",
    "-f",
    "file.txt",
    "random-token",
    "-o",
    "value"
  };
  fail_unless(ARGV_UNEXPECTED_TOKEN == argv_parse_partially(args, "testprogram", 6, str_args), "argv_parse_partially returns UNEXPECTED_TOKEN");
  fail_unless(ARGV_UNEXPECTED_TOKEN == argv_parse_partially(args, "testprogram", 5, str_args), "argv_parse_partially returns UNEXPECTED_TOKEN");
} END_TEST

START_TEST(test_argv_usage_print) {
  argv_option_set_description(verbose_option, "be more verbose");
  argv_option_set_description(file_option, "name of the input file");
  argv_option_set_description(output_option, "name of the output file");
  argv_parse_partially(args, "program", 0, NULL);
  argv_help_print(args);
} END_TEST

TCase *argv_tcase(void) {
  TCase *tc = tcase_create("argv");
  tcase_add_checked_fixture(tc, setup, teardown);
  tcase_add_test(tc, test_argv_init);
  tcase_add_test(tc, test_argv_option_iterate);
  tcase_add_test(tc, test_argv_parse_partially_argv_ok);
  tcase_add_test(tc, test_argv_parse_partially_values);
  tcase_add_test(tc, test_argv_parse_partially_unexpected_token);
  tcase_add_test(tc, test_argv_usage_print);
  return tc;
}

