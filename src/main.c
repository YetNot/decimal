#include "s21_decimal.h"

void mathFunc(s21_decimal decimal1, s21_decimal decimal2) {
  s21_decimal result = {0};
  int res_int = 0;
  char res[SIZE_STR];  // 29 цифр + знак + запятая + конец строки
  printf("---decimal_1:---\n");
  s21_printBinaryNumber(decimal1.bits, SIZE_DEC, 0);
  printf("\n---decimal_2:---\n");
  s21_printBinaryNumber(decimal2.bits, SIZE_DEC, 0);
  res_int = s21_add(decimal1, decimal2, &result);
  printf("\n---SUM---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - +inf, 2 - -inf)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_mul(decimal1, decimal2, &result);
  printf("\n---MUL---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - +inf, 2 - -inf)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_sub(decimal1, decimal2, &result);
  printf("\n---SUB---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - +inf, 2 - -inf)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_div(decimal1, decimal2, &result);
  printf("\n---DIV---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - +inf, 2 - -inf, 3 - /0)\n", res,
         res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_mod(decimal1, decimal2, &result);
  printf("\n---MOD---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - +inf, 2 - -inf, 3 - /0)\n", res,
         res_int);
}

void equalFunc(s21_decimal decimal1, s21_decimal decimal2) {
  printf("\ns21_is_less:%d (1 - TRUE, 0 - FALSE)\n",
         s21_is_less(decimal1, decimal2));
  printf("s21_is_less_or_equal:%d (1 - TRUE, 0 - FALSE)\n",
         s21_is_less_or_equal(decimal1, decimal2));
  printf("s21_is_greater:%d (1 - TRUE, 0 - FALSE)\n",
         s21_is_greater(decimal1, decimal2));
  printf("s21_is_greater_or_equal:%d (1 - TRUE, 0 - FALSE)\n",
         s21_is_greater_or_equal(decimal1, decimal2));
  printf("s21_is_equal:%d (1 - TRUE, 0 - FALSE)\n",
         s21_is_equal(decimal1, decimal2));
  printf("s21_is_not_equal:%d (1 - TRUE, 0 - FALSE)\n",
         s21_is_not_equal(decimal1, decimal2));
}

void roundFunc(s21_decimal decimal1) {
  int res_int = 0;
  s21_decimal result = {0};
  //    decimal1.bits[0] = UINT_MAX;
  //    decimal1.bits[1] = UINT_MAX;
  //    decimal1.bits[2] = 0;
  //    decimal1.bits[3] = 0;
  char res[SIZE_STR];  // 29 цифр + знак + запятая + конец строки
                       //    char num[SIZE_STR];
                       //    for (int deg = 20; deg < 28; deg ++) {
                       //        decimal1.bits[3] |= deg << 16;
                       //        decimal1.bits[3] |= 1u << 31;
                       //        for (int j = 2; j < 3; j++) {
  //            for (unsigned int i = 0; i < UINT_MAX; i += 1000000) {
  //                decimal1.bits[j] = i;
  //                s21_decimalToString(decimal1, num);
  //                s21_floor(decimal1, &result);
  //                s21_decimalToString(result, res);
  //                printf("\nnum: %s\nres: %s\n", num, res);
  //                memset(&result, 0, sizeof(s21_decimal));
  //            }
  //        }
  //        decimal1.bits[3] = 0;
  //    }
  res_int = s21_floor(decimal1, &result);
  printf("\n---FLOOR---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - FALSE)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_round(decimal1, &result);
  printf("\n---ROUND---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - FALSE)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_truncate(decimal1, &result);
  printf("\n---TRUNC---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - FALSE)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_negate(decimal1, &result);
  printf("\n---NEGATIVE---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  s21_decimalToString(result, res);
  printf("\n%s\noutput:%d (0 - OK, 1 - FALSE)\n", res, res_int);
  memset(&result, 0, sizeof(s21_decimal));
}

void converterFunc() {
  float a = 0, res_float = 0;
  s21_decimal result = {0}, ex = {0}, ex_int = {0};
  int int_ex = 800, res_inter = 0;
  int res_int = 0;
  char res[SIZE_STR];  // 29 цифр + знак + запятая + конец строки
  scanf("%f", &a);
  res_int = s21_from_float_to_decimal(a, &result);
  printf("\n---FLOAT to DEC---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  printf("float: in memory %.30f\n", a);
  s21_decimalToString(result, res);
  printf("out:%d  decimal = %s\n", res_int, res);
  ex = result;
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_from_decimal_to_float(ex, &res_float);
  printf("\n---DEC to FLOAT---\n");
  s21_printBinaryNumber(ex.bits, SIZE_DEC, 0);
  s21_decimalToString(ex, res);
  printf("float: in decimal %s\n", res);
  printf("out:%d  float      %.30f\n", res_int, res_float);
  memset(&result, 0, sizeof(s21_decimal));
  scanf("%d", &int_ex);
  res_int = s21_from_int_to_decimal(int_ex, &result);
  printf("\n---INT to DEC---\n");
  s21_printBinaryNumber(result.bits, SIZE_DEC, 0);
  printf("int: in memory %d\n", int_ex);
  s21_decimalToString(result, res);
  printf("out:%d  decimal %s\n", res_int, res);
  ex_int = result;
  memset(&result, 0, sizeof(s21_decimal));
  res_int = s21_from_decimal_to_int(ex_int, &res_inter);
  printf("\n---DEC to INT---\n");
  s21_printBinaryNumber(ex_int.bits, SIZE_DEC, 0);
  s21_decimalToString(ex_int, res);
  printf("int:   in decimal %s\n", res);
  printf("out:%d  int        %d\n", res_int, res_inter);
  memset(&result, 0, sizeof(s21_decimal));
}

int main() {
  int error1 = 0, error2 = 0;  // флаг ошибки, счетчик
  s21_decimal decimal1 = {0}, decimal2 = {0};  // вводим два числа децимал
  char num1[SIZE_STR], num2[SIZE_STR];
  s21_enterNumberString(num1, SIZE_STR, &error1);
  s21_enterNumberString(num2, SIZE_STR, &error2);
  if (!error1 && !error2) {
    s21_stringToDecimal(num1, &decimal1, &error1);
    s21_stringToDecimal(num2, &decimal2, &error2);
  }
  if (!error1 && !error2) {
    mathFunc(decimal1, decimal2);
    equalFunc(decimal1, decimal2);
    roundFunc(decimal1);
  }
  converterFunc();
  return 0;
}
