#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *start = str;
  while (*format) {
    if (*format != '%') {
      *str++ = *format;
    } else {
      format++;
      spec specific = {0};
      // format = parser_flags(format, &specific);
      // format = parser_width(format, args, &specific.width);
      // format = parser_accuracy(format, args, &specific);
      if (*format == 'h' || *format == 'l') {
        switch (*format) {
          case 'h':
            specific.length_h = 1;
            format++;
            break;
          case 'l':
            specific.length_l = 1;
            format++;
            break;
        }
      }
      format++;
      // спецификаторы
      switch (*format) { //
        case 'c'://diana
          break;
        case 'd'://ksenia
          break;
        case 'i':
          break;
        case 'f'://diana
          break;
        case 's'://ksenia
          break;
        case 'u':
          break;
      }
      format++;
      /*if (*format == 'd') {
        int value = va_arg(args, int);  // Извлекаем число
        char temp[50];
        sprintf(temp, "%d", value);  // Преобразуем число в строку

        int len = strlen(temp);
        if (value < 0) len--;  // Учитываем знак для отрицательных чисел
        int zero_padding =
            (specific.accuracy > len) ? specific.accuracy - len : 0;

        // Если число отрицательное, добавляем место для знака
        if (value < 0 && specific.accuracy > len) zero_padding++;

        // Выводим пробелы слева, если ширина больше
        if (specific.width > len + zero_padding && !specific.minus) {
          int padding = specific.width - len - zero_padding;
          while (padding-- > 0) {
            *str++ = ' ';
          }
        }

        // Копируем знак для отрицательных чисел
        if (value < 0) {
          *str++ = '-';
          strcpy(temp, temp + 1);  // Убираем знак из строки
        }

        // Выводим ведущие нули для точности
        while (zero_padding-- > 0) {
          *str++ = '0';
        }

        // Копируем само число
        strcpy(str, temp);
        str += strlen(temp);

        // Выводим пробелы справа, если выравнивание по левому краю
        if (specific.width > len + zero_padding && specific.minus) {
          int padding = specific.width - len - zero_padding;
          while (padding-- > 0) {
            *str++ = ' ';
          }
        }

        format++;*/
    }  //
  }
  *str = '\0';
  va_end(args);
  return (str - start);
}

const char *parser_flags(const char *format, spec *specific) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '#' ||
         *format == '0') {
    switch (*format) {
      case '+':
        specific->plus = 1;
        break;
      case '-':
        specific->minus = 1;
        break;
      case ' ':
        specific->space = 1;
        break;
    }
    format++;
  }
  specific->space =
      (specific->space &&
       !specific->plus);  // не может быть плюс и пробел одновременно
  return format;
}

const char *parser_width(const char *format, va_list args, int *width) {
  if (*format == '*') {  // есть два варианта ширины, если не звездочка, то другое
    *width = va_arg(args, int);
    format++;
  } else {  // перевод символа в число
    *width = 0;
    while (*format >= '0' && *format <= '9') {
      *width *= 10;
      *width += *format - '0';
      format++;
    }
  }
  return format;
}
const char *parser_accuracy(const char *format, va_list args, spec *specific) {
  if (*format == '.') {
    specific->point = 1;  //???
    specific->zero = 0;
    format++;
    parser_width(format, args, &specific->accuracy);
  } else {
    specific->accuracy = -1;  // не указана точность
  }
  return format;
}
void spec_d(char *str, spec *specific, va_list args) {
  long int n = 0;
  if (specific->length_h == 1) 
    n = (short)va_arg(args, short); //получить аргументы из списка
  else if (specific->length_l == 1)
    n = (long)va_arg(args, long);
  else 
    n = (int)va_arg(args, int);
}
int main() {
  char buffer[100];
  s21_sprintf(buffer, "%*d", 4, 42);
  printf("Result: '%s'\n", buffer);
}