#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {  // меньше
  int res = 0;
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1,
                     &value_long_2);  // выравниваем степени
  if (s21_zeroDecimal(value_1.bits, SIZE_DEC, 0) &&
      s21_zeroDecimal(value_2.bits, SIZE_DEC, 0))
    res = 0;
  else
    res = !s21_is_less_long(value_long_1, value_long_2);
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {  // больше
  int res = 0, end_for = 1;  // break cycle
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1,
                     &value_long_2);  // выравниваем степени
  if (s21_getSign(value_long_1.bits, SIZE_LONG) <
      s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 1;
  } else if (s21_getSign(value_long_1.bits, SIZE_LONG) >
             s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 0;
  } else if (!s21_getSign(value_long_1.bits, SIZE_LONG)) {
    s21_v1LessV2(value_long_1, value_long_2, &end_for, &res);
  } else if (s21_getSign(value_long_1.bits, SIZE_LONG)) {
    s21_v1GreaterV2(value_long_1, value_long_2, &end_for, &res);
  }
  if (s21_zeroDecimal(value_1.bits, SIZE_DEC, 0) &&
      s21_zeroDecimal(value_2.bits, SIZE_DEC, 0))
    res = 0;
  return !res;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = 0, end_for = 1;  // break cycle
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1,
                     &value_long_2);  // выравниваем степени
  if (s21_getSign(value_long_1.bits, SIZE_LONG) >
      s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 1;
  } else if (s21_getSign(value_long_1.bits, SIZE_LONG) <
             s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 0;
  } else if (!s21_getSign(value_long_1.bits, SIZE_LONG)) {
    s21_v1GreaterV2(value_long_1, value_long_2, &end_for, &res);
    if (end_for)  // если переменная не изменилась, то это значит что они равны,
                  // а это фолс
      res = 1;
  } else if (s21_getSign(value_long_1.bits, SIZE_LONG)) {
    s21_v1LessV2(value_long_1, value_long_2, &end_for, &res);
    if (end_for)  // если переменная не изменилась, то это значит что они равны,
                  // а это фолс
      res = 1;
  }
  if (s21_zeroDecimal(value_1.bits, SIZE_DEC, 0) &&
      s21_zeroDecimal(value_2.bits, SIZE_DEC, 0))
    res = 1;
  return !res;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0, end_for = 1;  // break cycle
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1,
                     &value_long_2);  // выравниваем степени
  if (s21_getSign(value_long_1.bits, SIZE_LONG) >
      s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 1;
  } else if (s21_getSign(value_long_1.bits, SIZE_LONG) <
             s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 0;
  } else if (!s21_getSign(value_long_1.bits, SIZE_LONG)) {
    s21_v1GreaterV2(value_long_1, value_long_2, &end_for, &res);
  } else if (s21_getSign(value_long_1.bits, SIZE_LONG)) {
    s21_v1LessV2(value_long_1, value_long_2, &end_for, &res);
  }
  if (s21_zeroDecimal(value_1.bits, SIZE_DEC, 0) &&
      s21_zeroDecimal(value_2.bits, SIZE_DEC, 0))
    res = 0;
  return !res;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1,
                     &value_long_2);  // выравниваем степени
  if (s21_getSign(value_long_1.bits, SIZE_LONG) !=
      s21_getSign(value_long_2.bits, SIZE_LONG))
    res = 1;
  for (int i = 0; i < SIZE_LONG - 1 && !res; i++) {
    for (int j = 0; j < 32 && !res; j++) {
      if (s21_getBit(value_long_1.bits, i, j) !=
          s21_getBit(value_long_2.bits, i, j))
        res = 1;
    }
  }
  if (s21_zeroDecimal(value_1.bits, SIZE_DEC, 0) &&
      s21_zeroDecimal(value_2.bits, SIZE_DEC, 0))
    res = 0;
  return !res;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0, end_for = 1;  // break cycle;
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1,
                     &value_long_2);  // выравниваем степени
  if (s21_getSign(value_long_1.bits, SIZE_LONG) !=
      s21_getSign(value_long_2.bits, SIZE_LONG)) {
    res = 0;
  } else {
    for (int i = 0; i < SIZE_LONG - 1 && end_for; i++) {
      for (int j = 0; j < 32 && end_for; j++) {
        if (s21_getBit(value_long_1.bits, i, j) !=
            s21_getBit(value_long_2.bits, i, j))
          end_for = 0;
      }
    }
    if (end_for) res = 1;
  }
  if (s21_zeroDecimal(value_1.bits, SIZE_DEC, 0) &&
      s21_zeroDecimal(value_2.bits, SIZE_DEC, 0))
    res = 1;
  return !res;
}
