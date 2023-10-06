#include "s21_decimal.h"

void s21_v1LessV2(s21_decimal_long value_1, s21_decimal_long value_2,
                  int* end_for, int* res) {
  for (int i = SIZE_LONG - 2; i >= 0 && *end_for && !*res; i--) {
    for (int j = 31; j >= 0 && *end_for && !*res; j--) {
      if (s21_getBit(value_1.bits, i, j) <
          s21_getBit(
              value_2.bits, i,
              j))  // если числа положительные, то как только старший разряд
        // второго выше, то число первое меньше
        *end_for = 0;
      else if (s21_getBit(value_1.bits, i, j) >
               s21_getBit(value_2.bits, i,
                          j))  // если старший разряд второго меньше, то число
                               // первое больше (рез = 1)
        *res = 1;
    }
  }
}

void s21_v1GreaterV2(s21_decimal_long value_1, s21_decimal_long value_2,
                     int* end_for, int* res) {
  for (int i = SIZE_LONG - 2; i >= 0 && *end_for && !*res; i--) {
    for (int j = 31; j >= 0 && *end_for && !*res; j--) {
      if (s21_getBit(value_1.bits, i, j) >
          s21_getBit(
              value_2.bits, i,
              j))  // если числа положительные, то как только старший разряд
        // второго выше, то число первое меньше
        *end_for = 0;
      else if (s21_getBit(value_1.bits, i, j) <
               s21_getBit(value_2.bits, i,
                          j))  // если старший разряд второго меньше, то число
                               // первое больше (рез = 1)
        *res = 1;
    }
  }
}

int s21_is_less_long(
    s21_decimal_long value_1,
    s21_decimal_long value_2) {  // заглушка для использования в функции деления
  int res = 0, end_for = 1;      // break cycle
  if (s21_getSign(value_1.bits, SIZE_LONG) <
      s21_getSign(value_2.bits, SIZE_LONG)) {
    res = 1;
  } else if (s21_getSign(value_1.bits, SIZE_LONG) >
             s21_getSign(value_2.bits, SIZE_LONG)) {
    res = 0;
  } else if (!s21_getSign(value_1.bits, SIZE_LONG)) {
    s21_v1LessV2(value_1, value_2, &end_for, &res);
    if (end_for)  // если переменная не изменилась, то это значит что они равны,
                  // а это фолс
      res = 1;
  } else if (s21_getSign(value_1.bits, SIZE_LONG)) {
    s21_v1GreaterV2(value_1, value_2, &end_for, &res);
    if (end_for)  // если переменная не изменилась, то это значит что они равны,
                  // а это фолс
      res = 1;
  }
  return res;
}
