#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result) {  // умножение
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0}, result_long = {0};
  s21_decimalToLong(value_1, &value_long_1);
  s21_decimalToLong(value_2, &value_long_2);
  s21_mul_long(value_long_1, value_long_2, &result_long);
  return s21_longToDecimal(result_long, result);
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_decimal_long result_long = {0}, value_long_1 = {0}, value_long_2 = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1, &value_long_2);
  if (!s21_getSign(value_long_1.bits, SIZE_LONG)) {
    if (!s21_getSign(value_long_2.bits, SIZE_LONG)) {
      s21_sumWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
    } else
      s21_subWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
  } else {
    if (!s21_getSign(value_long_2.bits, SIZE_LONG)) {
      s21_subWithoutEqualDegree(value_long_2.bits, value_long_1.bits,
                                result_long.bits, SIZE_LONG);
    } else {
      s21_sumWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
      result_long.bits[SIZE_LONG - 1] =
          result_long.bits[SIZE_LONG - 1] | 1u << 31;
    }
  }
  res = s21_longToDecimal(result_long, result);
  if (s21_zeroDecimal(result_long.bits, SIZE_LONG, 0))
    result->bits[SIZE_DEC - 1] &= ~(1u << 31);
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  s21_decimal_long value_long_1 = {0}, value_long_2 = {0}, result_long = {0};
  s21_equalityDegree(value_1, value_2, &value_long_1, &value_long_2);
  if (!s21_getSign(value_long_1.bits, SIZE_LONG)) {
    if (!s21_getSign(value_long_2.bits, SIZE_LONG))
      s21_subWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
    else
      s21_sumWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
  } else {
    if (!s21_getSign(value_long_2.bits, SIZE_LONG)) {
      s21_sumWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
      result_long.bits[SIZE_LONG - 1] =
          result_long.bits[SIZE_LONG - 1] | 1u << 31;
    } else {
      s21_subWithoutEqualDegree(value_long_1.bits, value_long_2.bits,
                                result_long.bits, SIZE_LONG);
    }
  }
  res = s21_longToDecimal(result_long, result);
  if (s21_zeroDecimal(result_long.bits, SIZE_LONG, 0))
    result->bits[SIZE_DEC - 1] &= ~(1u << 31);
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, round = 28;  // до какого разряда округлять (не больше 28)
  if (!s21_zeroDecimal(value_2.bits, SIZE_DEC, 0)) {
    s21_decimal div = {0}, mod = {0}, degree29 = {0};
    s21_decimal_long value_long_1 = {0}, value_long_2 = {0}, div_long = {0},
                     mod_long = {0}, temp = {0}, ten = {0};
    int sign = s21_getSign(value_1.bits, SIZE_DEC) +
               s21_getSign(value_2.bits, SIZE_DEC);
    ten.bits[0] = 10;  // используем эти два массива для создания степени
    mod_long.bits[0] = 1;  // десятки в десимале
    for (int i = 0; i < round; i++) {  // создаем степень десятки в десимале
      s21_mul_long(mod_long, ten, &temp);
      mod_long = temp;
      memset(&temp, 0, sizeof(s21_decimal_long));
    }
    ten = mod_long;  // в тен хранится pow(10, round)
    memset(&mod_long, 0, sizeof(s21_decimal_long));
    s21_equalityDegree(value_1, value_2, &value_long_1,
                       &value_long_2);  // преобразуем в лонг формат
    s21_delenie(&value_long_1, value_long_2,
                &div_long);  // находим целую часть от деления
    temp = value_long_1;  // передаем в темп значение остатка от деления
    memset(&value_long_1, 0, sizeof(s21_decimal_long));
    s21_mul_long(temp, ten, &value_long_1);
    s21_absDecimal(&value_long_2);
    s21_delenie(&value_long_1, value_long_2, &mod_long);
    for (int i = 0; i < round; i++) mod_long.bits[SIZE_LONG - 1] += 65536;
    memset(&ten, 0, sizeof(s21_decimal_long));
    ten.bits[0] = 10;
    memset(&temp, 0, sizeof(s21_decimal_long));
    s21_mul_long(value_long_1, ten, &temp);
    memset(&value_long_1, 0, sizeof(s21_decimal_long));
    s21_delenie(&temp, value_long_2, &value_long_1);
    s21_longToDecimal(value_long_1, &degree29);
    s21_from_decimal_to_int(degree29, &res);
    s21_specialConditions(abs(res), &temp, &mod_long);
    res = s21_longToDecimal(div_long, &div);
    if (!res) res = s21_longToDecimal(mod_long, &mod);
    if (!res) res = s21_add(div, mod, result);  // складываем див и мод
    if (sign % 2 && !s21_zeroDecimal(
                        value_1.bits, SIZE_DEC,
                        0)) {  // если 1 минус, то число получится отрицательным
      result->bits[SIZE_DEC - 1] |= 1u << 31;
    }
  } else {
    res = 3;
  }
  return res;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0;
  unsigned int sign = s21_getSign(value_1.bits, SIZE_DEC);
  if (!s21_zeroDecimal(value_2.bits, SIZE_DEC, 0)) {
    s21_decimal_long value_long_1 = {0}, value_long_2 = {0}, result_long = {0};
    s21_equalityDegree(value_1, value_2, &value_long_1,
                       &value_long_2);  // выравниваем степени
    s21_delenie(&value_long_1, value_long_2, &result_long);
    s21_longToDecimal(value_long_1, result);
    if (sign)
      result->bits[SIZE_DEC - 1] = result->bits[SIZE_DEC - 1] | 1u << 31;
    else
      result->bits[SIZE_DEC - 1] = result->bits[SIZE_DEC - 1] & ~(1u << 31);
  } else {
    res = 3;
  }
  return res;
}
