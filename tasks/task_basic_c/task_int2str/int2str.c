#include "int2str.h"
#include <stdbool.h>
#define int_MIN (-2147483647 - 1)
#define int_MAX 2147483647
char* int2str(int number) {
  static char buffer[12];
  long int h = number;

  bool is_negative = false;
  int i = 0;

  if (h < 0) {
    is_negative = true;
    h = -h;
  }
  if (h == 0) {
    buffer[i++] = '0';
    buffer[i] = '\0';
    return buffer;
  }
  while (h != 0) {
    buffer[i++] = (h % 10) + '0';
    h /= 10;
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

