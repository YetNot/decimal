#ifndef S21_DECIMAL_H_
#define S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_STR 32
#define BIT 8
#define SIZE_DEC 4
#define SIZE_LONG 7

typedef struct decimal {
  unsigned int bits[SIZE_DEC];
} s21_decimal;

typedef struct long_decimal {
  unsigned int bits[SIZE_LONG];
} s21_decimal_long;

typedef union special {
  float a;
  unsigned int b;
} s21_special;

int s21_zeroDecimal(unsigned int *decimal_byte, int size,
                    int shift);  // проверка на то, что массив пустой
int s21_digitExtracting(unsigned int *value_byte,
                        int size);  // извлечение цифры из десимала
void s21_decimalToString(s21_decimal decimal,
                         char res[SIZE_STR]);  // перевод из десима в строку
void s21_enterNumberString(char arr[], int size,
                           int *error);  // ввод числа в строку
void s21_stringToDecimal(char arr[], s21_decimal *decimal,
                         int *error);  // перевод строки в децимал
void s21_printBinaryNumber(unsigned int *value_byte, int amount,
                           int error);  // печать десимал в бинарном виде
unsigned int s21_getDegree(unsigned int *value_byte,
                           int size);  // получение степени числа
unsigned int s21_movingOneBitLeft(
    unsigned int *value_byte,
    int size);  // смещение децимала влево на 1 бит
void s21_movingOneBitRight(unsigned int *value_byte,
                           int size);  // смещение децимала вправо на 1 бит
void s21_equalityDegree(
    s21_decimal value_1, s21_decimal value_2, s21_decimal_long *value_long_1,
    s21_decimal_long *value_long_2);  // приведение чисел к одной степени //
void s21_changeDegree(
    s21_decimal_long *value,
    int difference_degree);  // изменение степени для выравнивания
unsigned int s21_getBit(
    unsigned int *value_byte, int num_byte,
    int i);  // получение бита в определенной позиции в децимале
unsigned int s21_getSign(unsigned int *value_byte,
                         int size);  // получение знака у десимала
void s21_sumWithoutEqualDegree(unsigned int *value_1, unsigned int *value_2,
                               unsigned int *result, int size);
void s21_subWithoutEqualDegree(unsigned int *value_1, unsigned int *value_2,
                               unsigned int *result, int size);
unsigned int s21_searchPositionFirstBit(
    unsigned int *value_byte,
    int size);  // нахождение позиции самого старшего бита
void s21_invertLongDecimal(s21_decimal_long *value);  // инвертирование
int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // сложение
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // умножение
int s21_mul_long(s21_decimal_long value_long_1, s21_decimal_long value_long_2,
                 s21_decimal_long *result_long);
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // разность
void s21_delenie(s21_decimal_long *value_1, s21_decimal_long value_2,
                 s21_decimal_long *result);  // деление
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // деление стандартное
int s21_mod(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // остаток от деления
void s21_v1LessV2(s21_decimal_long value_1, s21_decimal_long value_2,
                  int *end_for,
                  int *res);  // первое меньше второго
void s21_v1GreaterV2(s21_decimal_long value_1, s21_decimal_long value_2,
                     int *end_for, int *res);  //первое больше второго
int s21_is_less(s21_decimal value_1, s21_decimal value_2);  // меньше
int s21_is_less_or_equal(s21_decimal value_1,
                         s21_decimal value_2);  // меньше/равно
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);  // больше
int s21_is_greater_or_equal(s21_decimal value_1,
                            s21_decimal value_2);  // больше или равно
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);      // равны
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);  // не равны
void s21_absDecimal(s21_decimal_long *value);  // модуль десимал
int s21_extractNumGrade(
    int degree, unsigned int *byte, int size,
    int *sum_num_mod);  // извлечение низшего разряда и понижение степени
int s21_floor(s21_decimal value,
              s21_decimal *result);  // округление до целого в сторону -inf
int s21_round(s21_decimal value, s21_decimal *result);  // до ближайшего целого
int s21_truncate(s21_decimal value,
                 s21_decimal *result);  // возвращает целые цифры, отбрасываются
                                        // дробные, включая конечные нули
int s21_negate(s21_decimal value, s21_decimal *result);  // *(-1)
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_int_to_decimal(int src, s21_decimal *value);
int s21_from_decimal_to_int(s21_decimal src, int *integer);
void s21_decimalToLong(s21_decimal value, s21_decimal_long *value_long);
int s21_is_less_long(s21_decimal_long value_1, s21_decimal_long value_2);
int s21_longToDecimal(s21_decimal_long value_long, s21_decimal *value);
void s21_specialConditions(int res, s21_decimal_long *temp,
                           s21_decimal_long *mod_long);

#endif  // S21_DECIMAL_H_
