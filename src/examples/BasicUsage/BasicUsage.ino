// BasicUsage example (save this file as UTF-8 WITHOUT BOM)
#include <Arduino.h>            // Include Arduino core library
#include <Wire.h>               // Include Wire library for I2C communication
#include <LiquidCrystal_I2C.h>  // Include the LiquidCrystal_I2C library
#include <LCD_Cyrillic.h>       // Include the Cyrillic support header

LiquidCrystal_I2C lcd(0x27, 16, 2); // adjust address if needed

void setup() {
  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
}

void loop() {
  lcd.clear();
  LCD_Cyrillic::print(lcd, "Привет Мир!", 2, 0); // Print Cyrillic text: X=0, Y=0
  delay(2000);
  
  lcd.clear();
  LCD_Cyrillic::print(lcd, "Как дела?"); // Print Cyrillic text: X=0, Y=0
  delay(2000);

  lcd.clear();
  LCD_Cyrillic::print(lcd, "Хорошо!", 9, 1); // Print Cyrillic text: X=9, Y=1
  delay(2000);

  lcd.clear();
  LCD_Cyrillic::print(lcd, "Теперь встречаем строку номер 2"); // Print long text: X=0, Y=0
  delay(6000);

  lcd.clear();
  LCD_Cyrillic::print(lcd, "Новая строка?\nВозврат каретки!"); // Using newline for line break
  delay(6000);
}