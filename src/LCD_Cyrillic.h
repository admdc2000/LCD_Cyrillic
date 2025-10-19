// LCD_Cyrillic.h
// Provides API to print Cyrillic (Russian) text to a 16x2 I2C LCD (LiquidCrystal_I2C)

#ifndef LCD_CYRILLIC_H
#define LCD_CYRILIC_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "glyphDB.h"

class LCD_Cyrillic {
public:
    static String prepareForLCD(const String& input, LiquidCrystal_I2C& lcd);
    static void print(LiquidCrystal_I2C& lcd, const String& text, uint8_t startCol = 0, uint8_t startRow = 0);
};

#endif