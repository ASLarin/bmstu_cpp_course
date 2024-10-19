#include "int2str.h"
#include <stdbool.h>
#define int_MIN (-2147483647 - 1)
#define int_MAX 2147483647
char* int2str(int number) {
  static char buffer[12];

  bool is_negative = false;
  int i = 0;
  if (number == int_MIN) {
    return "-2147483648";
  }
  if (number == int_MAX) {
    return "2147483647";
  }
  if (number < 0) {
    is_negative = true;
    number = -number;
  }
  if (number == 0) {
    buffer[i++] = '0';
    buffer[i] = '\0';
    return buffer;
  }
  while (number != 0) {
    buffer[i++] = (number % 10) + '0';
    number /= 10;
  }
  if (is_negative) {
    buffer[i++] = '-';
  }
  buffer[i] = '\0';
  for (int j = 0; j < i / 2; j++) {
    char temp = buffer[j];
    buffer[j] = buffer[i - j - 1];
    buffer[i - j - 1] = temp;
  }

  return buffer;
}

