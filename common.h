#pragma once

/**
  Макросы для печати значения других макросов на этапе компиляции:
  Использовать так:
  #pragma message "OLED_DISPLAY_WIDTH ==> " XSTR1(OLED_DISPLAY_WIDTH)
  #pragma message DESCRIBE_MACRO(QMK_KEYBOARD_H)
*/
#define DESCRIBE_MACRO__(x) #x
#define DESCRIBE_MACRO_VALUE(x) DESCRIBE_MACRO__(x)

#define DESCRIBE_MACRO(x) #x " ==> " DESCRIBE_MACRO__(x)
