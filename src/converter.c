#include "s21_decimal.h"

/*int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
    int degree = 0;
    double sum = 1;
    unsigned int floatpoint = *((unsigned int *)&src);
    unsigned int mantissa = floatpoint & 0x7FFFFF;
    unsigned int sign = !!(floatpoint & 1u << 31);
    int exponenta = ((floatpoint & ~(1u << 31)) >> 23) - 127;
    memset(dst, 0, sizeof(s21_decimal));
    if (exponenta >= 96 || (src < 1e-28 && src > -1e-28)) {
      error = 1;
    } else {
      unsigned long long int number_bits = 0;
      for (int i = 22; i >= 0; i--) {
        sum += !!(mantissa & 1 << i) * pow(2, i - 23);
      }
      while (fmod(sum, 1)) {
        sum *= 10;
        degree++;
      }  // sum - аналог мантиссы для десимала
      number_bits = (unsigned long long int)
          sum;  // дублируем эту цифру в переменную как цифру а не асции код
      for (int i = 0; i < SIZE_DEC - 1;
           i++) {  // бежим по инт-элементам массива
        dst->bits[i] = (unsigned int)number_bits;
        number_bits = number_bits >> sizeof(unsigned int) * BIT;
      }
      dst->bits[SIZE_DEC - 1] =
          dst->bits[SIZE_DEC - 1] |
          degree << 16;  // записываем степень в 4 инт элемент
      dst->bits[SIZE_DEC - 1] =
          dst->bits[SIZE_DEC - 1] | sign << 31;  // записываем знак
      if (exponenta > 0) {
        for (int i = 0; i < exponenta; i++) {
          unsigned int bit = s21_movingOneBitLeft(dst->bits, SIZE_DEC);
          if (bit) {
            int empty = 0;
            s21_movingOneBitRight(dst->bits, SIZE_DEC);
            dst->bits[SIZE_DEC - 2] = dst->bits[SIZE_DEC - 2] | 1u << 31;
            s21_extractNumGrade(1, dst->bits, SIZE_DEC, &empty);
            s21_movingOneBitLeft(dst->bits, SIZE_DEC);
          }
        }
      } else if (exponenta < 0) {
        for (int i = 0; i < -exponenta; i++) {
          s21_movingOneBitRight(dst->bits, SIZE_DEC);
        }
      }
    }
  return error;
}*/
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0, degree = 6;
  double sum = 1;
  unsigned int floatpoint = *((unsigned int *)&src);
  unsigned int mantissa = floatpoint & 0x7FFFFF;
  unsigned int sign = !!(floatpoint & 1u << 31);
  int exponenta = ((floatpoint & ~(1u << 31)) >> 23) - 127;
  memset(dst, 0, sizeof(s21_decimal));
  for (int i = 0; i < SIZE_DEC; i++) {
    dst->bits[i] = 0;
  }
  if (exponenta >= 96 || (src < 1e-28 && src > 0)) {
    error = 1;
  } else {
    unsigned int number_bits = 0;
    for (int i = 22; i >= 0; i--) {
      sum += !!(mantissa & 1 << i) * pow(2, i - 23);
    }
    sum *= pow(2, exponenta);
    if (src >= pow(10, 7)) degree = 0;
    if (src < 1) degree = 28;
    sum *= pow(10, degree);
    while (sum >= pow(10, 7)) {
      sum /= 10;
      degree--;
    }
    sum = round(sum);
    number_bits = (unsigned int)sum;
    if (number_bits) {
      while (!(number_bits % 10) && (number_bits / 10)) {
        number_bits /= 10;
        degree--;
      }
      dst->bits[0] = number_bits;
      if (degree >= 0) {  // записываем степень в 4 инт элемент
        dst->bits[SIZE_DEC - 1] |= degree << 16;
      } else {
        s21_decimal ten = {0}, result = {0};
        ten.bits[0] = 10;
        while (degree) {
          s21_mul(*dst, ten, &result);
          degree++;
          *dst = result;
          memset(&result, 0, sizeof(s21_decimal));
        }
      }
    }
    dst->bits[SIZE_DEC - 1] |= sign << 31;  // записываем знак
  }
  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  s21_special our_float = {0};
  our_float.b = 0;
  int res = 0;
  unsigned int exponenta = 0, mantissa = 0, firstbit = 0, sign = 0, degree = 0;
  while (!s21_zeroDecimal(src.bits, SIZE_DEC, 1)) {
    s21_movingOneBitRight(src.bits, SIZE_DEC);
    exponenta++;
  }
  while (src.bits[0] >= pow(2, 24)) {
    s21_movingOneBitRight(src.bits, SIZE_DEC);
    exponenta++;
  }
  mantissa = src.bits[0];
  sign = s21_getSign(src.bits, SIZE_DEC);
  degree = s21_getDegree(src.bits, SIZE_DEC);
  firstbit = s21_searchPositionFirstBit(src.bits, SIZE_DEC);
  mantissa -= pow(2, firstbit);  // удаляем старшую единицу
  while (src.bits[0] > 1) {
    s21_movingOneBitRight(src.bits, SIZE_DEC);
    exponenta++;
  }
  exponenta += 127;
  if ((int)mantissa >= 0) {
    our_float.b = our_float.b | mantissa
                                    << (23 - firstbit);  // перенесли мантиссу
    our_float.b = our_float.b | exponenta << 23;  // перенесли экпоненту
    our_float.b = our_float.b | sign << 31;  // перенесли знак
    *dst = our_float.a / pow(10, degree);
  } else {
    *dst = 0;
  }
  return res;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  unsigned int sign = 0;
  memset(dst, 0, sizeof(s21_decimal));
  for (int i = 0; i < SIZE_DEC; i++) {
    dst->bits[i] = 0;
  }
  if (src < 0) {
    sign = 1;
    src *= (-1);
  }
  dst->bits[0] = src;
  dst->bits[SIZE_DEC - 1] = dst->bits[SIZE_DEC - 1] | sign << 31;
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int sign = s21_getSign(src.bits, SIZE_DEC);
  int error = 0;
  s21_truncate(src, &src);
  if (s21_zeroDecimal(src.bits, SIZE_DEC, 1)) {
    *dst = src.bits[0];
    if (sign) {
      *dst *= -1;
    }
  } else {
    *dst = -2147483648;
    error = 1;
  }
  return error;
}
