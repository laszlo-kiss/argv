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


START_TEST(test_find_option_by_longname) {
  fail_unless(file_option == argv_find_option_by_longname(args, "file", strlen("file")));
  fail_unless(output_option == argv_find_option_by_longname(args, "output", strlen("output")));
} END_TEST

START_TEST(test_find_option_by_shortname) {
  fail_unless(verbose_option == argv_find_option_by_shortname(args, 'v'));
} END_TEST

START_TEST(test_parse_argv_tokens) {
  const char *tokens[] = {
    "-v",
    "this is a sample value",
    "--output",
    "--file",
    "stopa rouge regime"
  };
  const int argc = 4;
  cmd_option *option;
  const char *value = NULL;

  /* a short option -v */
  fail_unless(0 == argv_parse_tokens(args, argc, tokens, &option, &value));
  fail_unless(verbose_option == option);
  fail_unless(value == NULL);

  /* an value */
  option = NULL;
  fail_unless(1 == argv_parse_tokens(args, argc, tokens, &option, &value));
  fail_unless(NULL == option);
  fail_unless(value != NULL && strcmp(value, "this is a sample value") == 0);

  /* a long parameter */
  value = NULL;
  fail_unless(0 == argv_parse_tokens(args, argc, tokens, &option, &value));
  fail_unless(output_option == option);
  fail_unless(value == NULL);

  /* a long parameter with value */
  option = NULL;
  int ret = argv_parse_tokens(args, argc, tokens, &option, &value);
  printf("RET: %i\n", ret);
  fail_unless(0 == ret);
  fail_unless(file_option == option);
  fail_unless(value == NULL);

  /* the value of the long parameter */
  value = NULL;
  fail_unless(1 == argv_parse_tokens(args, argc, tokens, &option, &value));
  fail_unless(NULL == option);
  fail_unless(value != NULL && strcmp(value, "file.txt") == 0);

} END_TEST

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
  tcase_add_test(tc, test_argv_parse_partially_unexpected_token);
  tcase_add_test(tc, test_find_option_by_shortname);
  tcase_add_test(tc, test_find_option_by_longname);
  tcase_add_test(tc, test_parse_argv_tokens);
  //tcase_add_test(tc, test_argv_parse_partially_values);
  //tcase_add_test(tc, test_argv_usage_print);
  return tc;
}

