// BasicUsage example (save this file as UTF-8 WITHOUT BOM)
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LCD_Cyrillic.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // adjust address if needed

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.clear();
  LCD_Cyrillic::print(lcd, "Привет, Мир!\nФУФАЙКА Ы =)", 0, 0);
}

void loop() {}