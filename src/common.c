#include "s21_decimal.h"

void s21_enterNumberString(char arr[], int size,
                           int* error) {  // ввод числа в строку
  char ch;
  int i = 0;
  for (i = 0; (ch = getchar()) != '\n' && ch != EOF && !*error;
       i++) {  // считываем в строку первое число
    if (i == size - 1) {  // если число больше сайза - 1, то это уже не децимал
      *error = 1;
      printf("number over decimal\n");
    } else {
      arr[i] = ch;
    }
  }
  arr[i] = '\0';
}

void s21_stringToDecimal(char arr[], s21_decimal* decimal,
                         int* error) {  // перевод строки в децимал
  int index = 0, degree = 0, sign = 0;
  while (arr[index] && !*error) {  // пока есть цифры и нет ошибки
    int num = arr[index];  // извлекаем цифру из асции символа
    if (num > 47 && num < 58) {  // если это цифра
      // дублируем эту цифру в переменную как цифру а не асции код
      unsigned long int number_bits = num - '0';
      // бежим по инт-элементам массива
      for (int i = 0; i < 4 && !*error; i++) {
        // если остаются биты для передачи в самый старший инт
        // элемент, то это число выходит за пределы десимал
        if (i == 3 && number_bits != 0) {
          *error = 1;
          printf("number over decimal\n");
        }
        // корректируем содержимое инт эл-та домнажением содержимого на 10 и
        // добалением считанной цифры (так как мы считываем число в сторону
        // младшей разрядности, то прошлые значения мы делаем старшими по
        // отношению к считанному путем перевода их на более старший разряд)
        number_bits = (unsigned long int)decimal->bits[i] * 10 + number_bits;
        // обновляем содержимое инт элемента из-за добавления нового младшего
        // разряда
        decimal->bits[i] = (unsigned int)number_bits;
        // используем побитовый сдвиг вправо на величину количества бит в инт
        // элементе (4*8 = 32 бита/позиции), Биты, которые в итоге остались,
        // должны быть переданы в более старший инт элемент
        number_bits = number_bits >> sizeof(unsigned int) * BIT;
      }
    } else if (num == 44 || num == 46) {  // если точка или запятая
      if (!degree) {
        degree = index;
      } else {  // если точки уже были
        printf("many point\n");
        *error = 1;
      }
    } else if (!index) {
      if (num == 45) {  // если первым идет знак минус
        sign = 1;
      } else if (num != 43) {
        printf("not right symbol[0] in number\n");
        *error = 1;
      }
    } else {
      printf("not right symbol in number\n");
      *error = 1;
    }
    index++;
  }
  if (degree) {  // если есть степень не нулевая
    // пишем степень в биты старшего элемента
    if (index - 1 - degree < 30) {
      decimal->bits[3] = decimal->bits[3] | (index - 1 - degree) << 16;
    } else {
      printf("very low degree\n");
      *error = 1;
    }
  }
  if (sign) {  // если первый знак минус, меняем первый бит
    decimal->bits[3] = decimal->bits[3] | 1u << 31;
  }
}

void s21_printBinaryNumber(unsigned int* decimal_byte, int size,
                           int error) {  // печать десимал в бинарном виде
  for (int i = size - 1; i >= 0 && !error; --i) {
    for (unsigned int mask = 0x80000000; mask; mask >>= 1)
      printf("%d", !!(decimal_byte[i] & mask));
    printf("\n");
  }
}

unsigned int s21_getDegree(unsigned int* value_byte,
                           int size) {  // получение степени числа
  return (unsigned int)(value_byte[size - 1] & ~(1u << 31)) >> 16;
}

unsigned int s21_getBit(
    unsigned int* value_byte, int num_byte,
    int i) {  // получение бита в определенной позиции в децимале
  return (unsigned int)!!(value_byte[num_byte] & 1u << i);
}

unsigned int s21_getSign(unsigned int* value_byte,
                         int size) {  // получение бита символа знака
  return (unsigned int)!!(value_byte[size - 1] & 1u << 31);
}

unsigned int s21_movingOneBitLeft(
    unsigned int* value_byte,
    int size) {  // смещение децимала влево на 1 бит
  unsigned int temp = 0;
  for (int i = 0; i < size - 1; i++) {
    unsigned int bit = s21_getBit(value_byte, i, 31);
    value_byte[i] = value_byte[i] << 1 | temp;
    temp = bit;
  }
  return temp;
}

void s21_movingOneBitRight(unsigned int* value_byte,
                           int size) {  // смещение децимала влево на 1 бит
  unsigned int temp = 0;
  for (int i = size - 2; i >= 0; i--) {
    unsigned int bit = s21_getBit(value_byte, i, 0);
    value_byte[i] = value_byte[i] >> 1 | (temp << 31);
    temp = bit;
  }
}

void s21_decimalToLong(s21_decimal value, s21_decimal_long* value_long) {
  for (int i = 0; i < SIZE_DEC - 1; i++) {
    value_long->bits[i] = value.bits[i];
  }
  value_long->bits[SIZE_LONG - 1] = value.bits[SIZE_DEC - 1];
}

void s21_changeDegree(s21_decimal_long* value, int difference_degree) {
  s21_decimal_long temp = *value, result = {0};
  unsigned int sign = s21_getSign(value->bits, SIZE_LONG);
  for (int i = 0; i < abs(difference_degree); i++) {
    for (int j = 0; j < 9;
         j++) {  // 9 операций, т,к в первой мы уже складываем 2 числа
      s21_sumWithoutEqualDegree(temp.bits, value->bits, result.bits, SIZE_LONG);
      temp = result;
      memset(&result, 0, sizeof(s21_decimal_long));
    }
    temp.bits[SIZE_LONG - 1] +=
        65536;  // добавляем единичку в биты степеней (2^16)
    *value = temp;
  }
  value->bits[SIZE_LONG - 1] =
      value->bits[SIZE_LONG - 1] |
      sign << 31;  // установка знака числа, какой он был
}

void s21_equalityDegree(
    s21_decimal value_1, s21_decimal value_2, s21_decimal_long* value_long_1,
    s21_decimal_long* value_long_2) {  // приведение чисел к одной степени
  int difference_degree = 0;
  s21_decimal_long temp1 = {0}, temp2 = {0};
  s21_decimalToLong(value_1, &temp1);
  s21_decimalToLong(value_2, &temp2);
  difference_degree = s21_getDegree(temp1.bits, SIZE_LONG) -
                      s21_getDegree(temp2.bits, SIZE_LONG);
  if (difference_degree > 0) {
    s21_changeDegree(&temp2, difference_degree);
  } else if (difference_degree < 0) {
    s21_changeDegree(&temp1, difference_degree);
  }
  *value_long_1 = temp1;
  *value_long_2 = temp2;
}

int s21_longToDecimal(s21_decimal_long value_long, s21_decimal* value) {
  int res = 0, num_mod = 0, sum_num_mod = 0;
  unsigned int degree = s21_getDegree(value_long.bits, SIZE_LONG);
  unsigned int sign = s21_getSign(value_long.bits, SIZE_LONG);
  while (degree > 28 || (!s21_zeroDecimal(value_long.bits, SIZE_LONG,
                                          SIZE_DEC - 1) &&
                         degree > 0)) {  // пока степень не будет меньше 29 или
    // высшие биты не опустеют
    num_mod = s21_extractNumGrade(1, value_long.bits, SIZE_LONG, &sum_num_mod);
    degree--;
  }
  if (num_mod > 5 || (num_mod == 5 && sum_num_mod == 5 &&
                      (value_long.bits[0] &
                       1u))) {  // если последнее извлеченное число больше 5 или
                                // (там 5 с активным флагом и число нечетное)
    s21_decimal_long one = {0}, temp = value_long;
    one.bits[0] = 1;
    memset(&value_long, 0, sizeof(s21_decimal_long));
    s21_sumWithoutEqualDegree(temp.bits, one.bits, value_long.bits,
                              SIZE_LONG);  // то добавляем единицу
    if (sign)
      value_long.bits[SIZE_LONG - 1] =
          value_long.bits[SIZE_LONG - 1] | 1u << 31;
  }
  if (s21_zeroDecimal(value_long.bits, SIZE_LONG, SIZE_DEC - 1)) {
    for (int i = 0; i < SIZE_DEC - 1; i++) {
      value->bits[i] = value_long.bits[i];
    }
    value->bits[SIZE_DEC - 1] = value_long.bits[SIZE_LONG - 1];
  } else {
    if (!sign) {  // если знак минус
      res = 1;
    } else {  // если знак плюс
      res = 2;
    }
  }
  return res;
}

void s21_invertLongDecimal(s21_decimal_long* value) {
  s21_decimal_long one = {0}, temp = {0};
  one.bits[0] = 1;
  for (int i = 0; i < SIZE_LONG - 1; i++)
    value->bits[i] = ~value->bits[i];  // инвертируем
  temp = *value;
  memset(value, 0, sizeof(s21_decimal_long));
  s21_sumWithoutEqualDegree(temp.bits, one.bits, value->bits,
                            SIZE_LONG);  // и добавляем единицу
  if (!s21_getSign(temp.bits, SIZE_LONG)) {  // меняем знак числа
    value->bits[SIZE_LONG - 1] =
        value->bits[SIZE_LONG - 1] |
        1u << 31;  // ставим единицу в бит знака (говорим, что число
                   // отрицательное)
  } else {
    value->bits[SIZE_LONG - 1] =
        value->bits[SIZE_LONG - 1] &
        ~(1u
          << 31);  // ставим ноль в бит знака (говорим, что число положительное)
  }
}

unsigned int s21_searchPositionFirstBit(
    unsigned int* value_byte,
    int size) {  // нахождение позиции самого старшего бита
  unsigned int res = 0;
  for (int i = 0; i < 32 * (size - 1); i++) {
    if (value_byte[i / 32] & 1u << i % 32) res = i;
  }
  return res;
}

int s21_zeroDecimal(unsigned int* decimal_byte, int size,
                    int shift) {  // проверка на то, что массив пустой
  int result = 1;
  for (int i = 0 + shift; i < size - 1 && result; i++)
    if (decimal_byte[i]) result = 0;
  return result;  // 1 - все массивы равны 0, 0 - в противоположном случае
}

int s21_digitExtracting(unsigned int* decimal_byte, int size) {
  unsigned long long int number_bits =
      0;  // создаем буффер в 2 раза больший, чем инт элемент массива
  for (int i = size - 2; i >= 0; i--) {
    // к содержимому буфера, смещенному на 32 бита (изначально нулевому),
    // добавляем содержимое iго инт-элемента
    number_bits = (number_bits << sizeof(unsigned int) * 8) + decimal_byte[i];
    // перезаписываем инт элемент с учетом того, что младший разряд выводится на
    // печать в строку
    decimal_byte[i] = (unsigned int)(number_bits / 10);
    // эта цифра выведется на печать, если это была последняя итерация цикла.
    // Если это не так, то цифра спускается в более младший инт элемент, тем
    // самым для следующей итерации буфера становится равным этой цифрею
    number_bits %= 10;
  }
  return (int)number_bits;
}

void s21_decimalToString(s21_decimal decimal, char res[SIZE_STR]) {
  int i, count = 0, degree = 0;
  res[0] = '0';  // изначально в первый элемент записываем 0
  while (!s21_zeroDecimal(decimal.bits, SIZE_DEC,
                          0)) {  // пока инт элементы не опустеют
    // извлекаем по цифре из массива и записываем его символом
    res[count++] = '0' + s21_digitExtracting(decimal.bits, SIZE_DEC);
  }
  degree = s21_getDegree(decimal.bits, SIZE_DEC);  // получение степени числа
  if (count) {  // если происходила какая либо запись в строку
    res[count--] = '\0';  // в конце рисуем символ конца строки
    // разворачиваем строку, так как в нее мы записывали
    // числа, начиная с младшего разряда
    for (i = 0; i < count; i++, count--) {
      char t = res[count];
      res[count] = res[i];
      res[i] = t;
    }
  } else {
    res[1] = '\0';
  }
  if (degree) {
    int point = strlen(res) - degree, j = 0;
    i = strlen(res);
    if (point < 1) {
      for (int j = i; j >= 0; j--)  // освобождаем место под 0 в начале строки,
                                    // которых будет 1 - поинт
        res[j - point + 1] = res[j];
      for (j = 0; j <= -point; j++) res[j] = '0';
    }
    for (i += j; i >= point + j; i--) res[i + 1] = res[i];
    res[point + j] = '.';
  }
  if (s21_getSign(decimal.bits, SIZE_DEC)) {
    i = strlen(res);
    for (; i >= 0; i--) res[i + 1] = res[i];
    res[0] = '-';
  }
}

void s21_absDecimal(s21_decimal_long* value) {
  if (s21_getSign(value->bits, SIZE_LONG))
    value->bits[SIZE_LONG - 1] = value->bits[SIZE_LONG - 1] & ~(1u << 31);
}

int s21_extractNumGrade(int degree, unsigned int* byte, int size,
                        int* sum_num_mod) {
  int num_mod = 0;
  while (degree > 0) {
    num_mod = s21_digitExtracting(
        byte, size);  // извлекаем самый низший разряд в числе (в случае наличия
                      // степени эти разряды будут относится к дробной части)
    degree--;
    byte[size - 1] -= 65536;  // понижаем также саму степень
    *sum_num_mod +=
        num_mod;  // считаем сумму извлеченных цифр, если она была больше 5 при
                  // последней цифре 5, то значит раньше были цифры
  }
  return num_mod;
}
