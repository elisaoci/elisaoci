#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *start = str;
  int zero_str = 0, zero_flag = 0;
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
      char *p_str = s21_NULL;
      p_str = calloc(1024, sizeof(char));
      switch (*format) { //
        case 'c'://diana
            spec_c(p_str, arg, spec, zero_flag);
            break;
        case 'd'://ksenia
            break;
        case 'i':
            break;
        case 'f'://diana
            spec_f(p_str, args, spec);
            break;
        case 's'://ksenia
          break;
        case 'u':
          break;
      }
      format++;
  }
  *str = '\0';
  va_end(args);
  return (str - start);
}

void spec_f(char *p_str, va_list *args, struct *spec){
    double double_number = 0;
    long double long_number = 0;
    switch (spec->length_l){
        case 'L':
            long_number = va_arg(*args, long double);
            //функция для преобразования в строку
            break;
        case 'l':
        default:
            double_number = va_arg(*args, double);
            //функция для преобразования в строку
            break;
    }
    //функция, обрабатывающая доп штуки строк для чисел с запятой
}

void spec_c(char *p_str, va_list *arg, struct *spec, int *zero_flag){
    if (*zero_flag == 0){
        p_str[0] = va_arg(*args, int);
        if (p_str[0] == 0) {
            spec->zero_simbol++;
            *zero_flag = 1;
        }
        p_str[1] = '\0';
    }
    else {
        spec->zero_simbol++;
        p_str[0] = '\0';
    }
    //функция, обрабаывающая флаги форматирования(выравнивание) s21_string_flags(formats, p_str);
    //функция, обрабатывающая ширины  s21_string_width(formats, p_str);
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