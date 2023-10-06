#include "s21_decimal.h"

void s21_sumWithoutEqualDegree(unsigned int *value_1, unsigned int *value_2,
                               unsigned int *result, int size) {  // сложение
  unsigned int mod = 0;
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < 32; j++) {
      unsigned int a = s21_getBit(value_1, i, j);
      unsigned int b = s21_getBit(value_2, i, j);
      result[i] = result[i] | (a + b + mod) % 2 << j;
      mod = (a + b + mod) / 2;
    }
  }
  result[size - 1] =
      result[size - 1] |
      (s21_getDegree(value_1, size) << 16);  // вставляем степень как у вэлью 1
}

int s21_mul_long(s21_decimal_long value_long_1, s21_decimal_long value_long_2,
                 s21_decimal_long *result_long) {
  int res = 0;
  unsigned int degree = 0, sign = 0;
  s21_decimal_long temp = {0};
  degree = s21_getDegree(value_long_1.bits, SIZE_LONG) +
           s21_getDegree(value_long_2.bits, SIZE_LONG);
  sign = s21_getSign(value_long_1.bits, SIZE_LONG) +
         s21_getSign(value_long_2.bits, SIZE_LONG);
  for (int i = 0; i < SIZE_LONG - 1; i++) {
    for (int j = 0; j < 32; j++) {
      char bit = s21_getBit(value_long_2.bits, i,
                            j);  // считываем побитно второе число
      if (bit) {
        memset(result_long, 0,
               sizeof(s21_decimal_long));  // обнуляем десимал результирующий
                                           // для корректной работы
        s21_sumWithoutEqualDegree(value_long_1.bits, temp.bits,
                                  result_long->bits, SIZE_LONG);
        temp = *result_long;
      }
      s21_movingOneBitLeft(value_long_1.bits,
                           SIZE_LONG);  // сдвигаем первое число на 1 бит
    }
  }
  if (degree) {  // если есть степень не нулевая
    result_long->bits[SIZE_LONG - 1] = 0;  // обнуляем инт элемент со степенью,
                                           // так как там содержится информация
    result_long->bits[SIZE_LONG - 1] =
        result_long->bits[SIZE_LONG - 1] |
        (degree) << 16;  // пишем степень в биты старшего элемента
  }
  if (sign % 2) {  // если 1 минус, то число получится отрицательным
    result_long->bits[SIZE_LONG - 1] =
        result_long->bits[SIZE_LONG - 1] | 1u << 31;
  }
  return res;
}

void s21_specialConditions(int res, s21_decimal_long *temp,
                           s21_decimal_long *mod_long) {
  if (res > 5 || (res == 5 && ((mod_long->bits[0] & 1u) ||
                               !s21_zeroDecimal(temp->bits, SIZE_LONG, 0)))) {
    s21_decimal_long one = {0};
    one.bits[0] = 1;
    unsigned int sign =
        s21_getSign(mod_long->bits, SIZE_LONG);  // считываем знак остатка
    memset(temp, 0, sizeof(s21_decimal_long));
    *temp = *mod_long;
    memset(mod_long, 0, sizeof(s21_decimal_long));
    s21_sumWithoutEqualDegree(temp->bits, one.bits, mod_long->bits,
                              SIZE_LONG);  // то добавляем единицу
    mod_long->bits[SIZE_LONG - 1] =
        mod_long->bits[SIZE_LONG - 1] |
        sign << 31;  // устанавливаем считанный знак остатка
  }
}

void s21_subWithoutEqualDegree(unsigned int *value_1, unsigned int *value_2,
                               unsigned int *result, int size) {  // вычитание
  unsigned int mod = 0;
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < 32; j++) {
      unsigned int a = s21_getBit(value_1, i, j);
      unsigned int b = s21_getBit(value_2, i, j);
      if ((int)(a - mod - b) < 0) {
        a = a + 2 - mod;
        mod = 1;
      } else if (a == 1 && mod == 1) {
        a = 0;
        mod = 0;
      }
      result[i] = result[i] | (a - b) << j;
    }
  }
  if (s21_getSign(value_1, size))  // если вычитаемое было отриц, значит весь
                                   // результат отрицателен
    result[size - 1] = result[size - 1] | 1u << 31;
  if ((mod &&
       result[size - 2] >>
           31)) {  //смотрим переносы в знаковый разряд и из знакового (в
                   //внешний разряд). Если они имеют одинаковый статус (оба есть
                   //или обоих нет), то все нормально переполнения нет
    s21_decimal_long temp = {0};
    for (int i = 0; i < size; i++) temp.bits[i] = result[i];
    s21_invertLongDecimal(&temp);
    for (int i = 0; i < size; i++) result[i] = temp.bits[i];
  }
  result[size - 1] =
      result[size - 1] |
      (s21_getDegree(value_1, size) << 16);  // вставляем степень как у вэлью 1
}

void s21_delenie(s21_decimal_long *value_1, s21_decimal_long value_2,
                 s21_decimal_long *result) {  // деление
  s21_decimal_long temp = {0}, del = {0};
  int sign_result = 0, shift = 0, shift_fact = 0, break_for = 1;
  sign_result = s21_getSign(value_1->bits, SIZE_LONG) +
                s21_getSign(value_2.bits, SIZE_LONG);  // находим знак результат
  shift = s21_searchPositionFirstBit(value_1->bits, SIZE_LONG) -
          s21_searchPositionFirstBit(value_2.bits, SIZE_LONG);
  s21_absDecimal(value_1);  // отсекаем знак
  s21_absDecimal(&value_2);
  del = value_2;  // сохраняем в отдельную переменную наш делитель, что потом
  for (int i = 0; i < shift; i++)  // двигаем делитель на шаг сдвига
    s21_movingOneBitLeft(value_2.bits, SIZE_LONG);
  for (; shift_fact <= shift && break_for; shift_fact++) {
    unsigned int sign = 0;
    s21_subWithoutEqualDegree(value_1->bits, value_2.bits, temp.bits,
                              SIZE_LONG);  // вот эта операция
    if (s21_zeroDecimal(temp.bits, SIZE_LONG, 0))  //если в темпе 0, то знак +
      temp.bits[SIZE_LONG - 1] = temp.bits[SIZE_LONG - 1] & ~(1u << 31);
    sign = (s21_getSign(temp.bits, SIZE_LONG)) ? 0 : 1;
    result->bits[(shift - shift_fact) / 32] =
        result->bits[(shift - shift_fact) / 32] | sign << (shift - shift_fact);
    *value_1 = temp;
    if (!s21_is_less_long(temp, del) && !s21_getSign(temp.bits, SIZE_LONG)) {
      break_for = 0;
    } else {
      if (shift_fact != shift)  // осуществляем сдвиг полученного значения на 1
        s21_movingOneBitLeft(value_1->bits, SIZE_LONG);
    }
    memset(&temp, 0, sizeof(s21_decimal_long));
  }
  shift_fact--;  // так как на последней итерации сдвига не было, то -1
  if (s21_getSign(value_1->bits, SIZE_LONG)) {
    temp = *value_1;
    memset(value_1, 0, sizeof(s21_decimal_long));
    s21_subWithoutEqualDegree(value_2.bits, temp.bits, value_1->bits,
                              SIZE_LONG);
  }
  for (int i = 0; i < shift_fact; i++)
    s21_movingOneBitRight(value_1->bits, SIZE_LONG);
  if (sign_result % 2) {
    result->bits[SIZE_LONG - 1] = result->bits[SIZE_LONG - 1] | 1u << 31;
    value_1->bits[SIZE_LONG - 1] = value_1->bits[SIZE_LONG - 1] | 1u << 31;
  }
}

// int s21_div_v2(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
// {
//   int res = 0;
//   if (!s21_zeroDecimal(value_2.bits, SIZE_DEC, 0)) {
//     s21_decimal_long value_long_1 = {0}, value_long_2 = {0}, result_long =
//     {0}; s21_equalityDegree(value_1, value_2, &value_long_1,
//                        &value_long_2);  // выравниваем степени
//     s21_delenie(&value_long_1, value_long_2, &result_long);
//     res = s21_longToDecimal(result_long, result);
//   } else {
//     res = 3;
//   }
//   return res;
// }
