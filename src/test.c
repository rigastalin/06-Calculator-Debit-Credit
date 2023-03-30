#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "calculator.h"

START_TEST(test_1) {
    char *string = "5+3-2*1/6+sin(4)-cos(6)+log(100)-ln(10)+tan(8)";
    double result = -1.152602;
    double res = 0;
    int err = 0;
    char *output = (char *)malloc(600 *sizeof(char));
    parser(string, &err, output);
    if (err == 0)
        res = polishNotation(output, &err);
    ck_assert_double_eq_tol(res, result, 6);
    ck_assert_int_eq(err, 0);
    free(output);
}
END_TEST

START_TEST(test_2) {
    char *string = "777+666-999+tan(34567)";
    double result = 444.056091;
    double res = 0;
    int err = 0;
    char *output = (char *)malloc(600 *sizeof(char));
    parser(string, &err, output);
    if (err == 0)
        res = polishNotation(output, &err);
    ck_assert_double_eq_tol(res, result, 6);
    ck_assert_int_eq(err, 0);
    free(output);
}
END_TEST

START_TEST(test_3) {
    char *string = "3^12";
    double result = 531441;
    double res = 0;
    int err = 0;
    char *output = (char *)malloc(600 *sizeof(char));
    parser(string, &err, output);
    if (err == 0)
        res = polishNotation(output, &err);
    ck_assert_double_eq_tol(res, result, 6);
    ck_assert_int_eq(err, 0);
    free(output);
}
END_TEST

START_TEST(test_4) {
    char *string = "cos(789484)";
    double result = -0.194116;
    double res = 0;
    int err = 0;
    char *output = (char *)malloc(600 *sizeof(char));
    parser(string, &err, output);
    if (err == 0)
        res = polishNotation(output, &err);
    ck_assert_double_eq_tol(res, result, 6);
    ck_assert_int_eq(err, 0);
    free(output);
}
END_TEST

START_TEST(test_5) {
    char *string = "log(100500)";
    double result = 5.002166;
    double res = 0;
    int err = 0;
    char *output = (char *)malloc(600 *sizeof(char));
    parser(string, &err, output);
    if (err == 0)
        res = polishNotation(output, &err);
    ck_assert_double_eq_tol(res, result, 6);
    ck_assert_int_eq(err, 0);
    free(output);
}
END_TEST

START_TEST(test_6) {
    char *string = "(4+456)-(2343-666)*((45*3)-(55-666))";
    double result = -1250582;
    double res = 0;
    int err = 0;
    char *output = (char *)malloc(600 *sizeof(char));
    parser(string, &err, output);
    if (err == 0)
        res = polishNotation(output, &err);
    ck_assert_double_eq_tol(res, result, 6);
    ck_assert_int_eq(err, 0);
    free(output);
}
END_TEST

int main(void) {
    Suite* s1 = suite_create("Core");
    TCase* tc1_1 = tcase_create("calculator");
    SRunner* sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_1);
    tcase_add_test(tc1_1, test_2);
    tcase_add_test(tc1_1, test_3);
    tcase_add_test(tc1_1, test_4);
    tcase_add_test(tc1_1, test_5);
    tcase_add_test(tc1_1, test_6);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
