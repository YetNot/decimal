#include "s21_decimal.h"

int s21_floor(s21_decimal value,
              s21_decimal *result) {  // округление до целого в сторону -inf
  int res = 0, sum_num_mod = 0, degree = 0,
      sign = 0;  // флаг, чтобы понять, число больше половины или нет, нам мод
                 // хранит последнюю цифру дробной части
  degree = s21_getDegree(value.bits, SIZE_DEC);
  sign = s21_getSign(value.bits, SIZE_DEC);
  s21_extractNumGrade(degree, value.bits, SIZE_DEC, &sum_num_mod);
  for (int i = 0; i < SIZE_DEC; i++) result->bits[i] = 0;
  if (sign && sum_num_mod >
                  0) {  // если число отрицательное и есть дробная часть, то
                        // нужно прибавить 1 к числу и сделать его отрицательным
    s21_decimal one = {0};
    one.bits[0] = 1;
    s21_sumWithoutEqualDegree(value.bits, one.bits, result->bits, SIZE_DEC);
    result->bits[SIZE_DEC - 1] = result->bits[SIZE_DEC - 1] | 1u << 31;
  } else {
    *result = value;
  }
  if (s21_zeroDecimal(result->bits, SIZE_DEC, 0))
    result->bits[SIZE_DEC - 1] &= ~(1u << 31);
  return res;
}

int s21_round(s21_decimal value, s21_decimal *result) {  // до ближайшего целого
  int res = 0, sum_num_mod = 0, num_mod = 0, degree = 0,
      sign = 0;  // флаг, чтобы понять, число больше половины или нет, нам мод
                 // хранит последнюю цифру дробной части
  degree = s21_getDegree(value.bits, SIZE_DEC);
  sign = s21_getSign(value.bits, SIZE_DEC);
  num_mod = s21_extractNumGrade(degree, value.bits, SIZE_DEC, &sum_num_mod);
  for (int i = 0; i < SIZE_DEC; i++) result->bits[i] = 0;
  if (num_mod < 5 ||
      (num_mod == 5 && sum_num_mod == 5 &&
       !(value.bits[0] & 1u))) {  // если в десятых число меньше 5 или (там 5 с
                                  // неактивным флагом и число четное)
    *result = value;
  } else {
    s21_decimal one = {0};
    one.bits[0] = 1;
    s21_sumWithoutEqualDegree(value.bits, one.bits, result->bits,
                              SIZE_DEC);  // то добавляем единицу
    if (sign)
      result->bits[SIZE_DEC - 1] = result->bits[SIZE_DEC - 1] | 1u << 31;
  }
  return res;
}

int s21_truncate(
    s21_decimal value,
    s21_decimal *result) {  // возвращает целые цифры, отбрасываются дробные,
                            // включая конечные нули
  int res = 0, empty = 0,
      degree = 0;  // емпти - переменная-заглушка, чтобы можно было использовать
                   // написанную функцию
  degree = s21_getDegree(value.bits, SIZE_DEC);
  for (int i = 0; i < SIZE_DEC; i++) result->bits[i] = 0;
  s21_extractNumGrade(degree, value.bits, SIZE_DEC, &empty);
  *result = value;
  return res;
}

int s21_negate(s21_decimal value, s21_decimal *result) {  // *(-1)
  int res = 0;
  for (int i = 0; i < SIZE_DEC; i++) result->bits[i] = 0;
  if (s21_getSign(value.bits, SIZE_DEC))
    value.bits[SIZE_DEC - 1] = value.bits[SIZE_DEC - 1] & ~(1u << 31);
  else
    value.bits[SIZE_DEC - 1] = value.bits[SIZE_DEC - 1] | 1u << 31;
  *result = value;
  return res;
}
