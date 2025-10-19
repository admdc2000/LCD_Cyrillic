# LCD_Cyrillic

**LCD_Cyrillic** is an Arduino library for displaying Cyrillic (Russian) characters on a 16x2 LCD (HD44780) via an I2C backpack (LiquidCrystal_I2C).

## What it does
- Provides functions to display Russian (Cyrillic) text on 16x2 LCD using a limited set of custom characters.
- Maps Cyrillic characters either to visually-similar ASCII characters when possible (e.g. 'А' -> 'A')
  or to custom LCD glyph slots (user-defined characters) for characters that don't have ASCII equivalents.
- Automatically manages the custom-character slots (up to 8) at runtime.

## Files provided
- `src/LCD_Cyrillic.h` — public header with API and comments.
- `src/LCD_Cyrillic.cpp` — implementation, UTF-8 handling and glyph management.
- `src/glyphDB.h` — bitmap definitions for the custom glyphs used for Cyrillic letters.
- `src/examples/BasicUsage/BasicUsage.ino` — example Arduino sketch showing how to print Russian text (saved as UTF-8 without BOM).

## Important: UTF-8 without BOM
When working with Arduino and modern editors, **save your .ino/.cpp/.h files as UTF-8 without BOM (no byte order mark)**.
- In **VS Code**: `File` → `Save with Encoding` → `UTF-8`.
- In **Arduino IDE 1.x**: use a modern version or ensure encoding is UTF-8 without BOM.
Using BOM may insert an invisible character before the first file bytes and break parsing on some toolchains.

## How to use
1. Install `LiquidCrystal_I2C` library (from Library Manager).
2. Copy this library folder to your Arduino `libraries/` folder or install as a library.
3. Open the example `src/examples/BasicUsage/BasicUsage.ino` in VS Code / Arduino IDE.
4. Make sure the `.ino` file is saved as UTF-8 without BOM.
5. Upload to your Arduino (e.g. Uno).

## Example snippet
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LCD_Cyrillic.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  // Print Russian: "Привет мир" ("Hello world")
  LCD_Cyrillic::print(lcd, "Привет мир", 0, 0);
}
void loop() {}
```

## Developer notes & improvements performed
- Added clear instructions about `UTF-8 without BOM`.
- Example demonstrates printing Cyrillic strings encoded in UTF-8.
- Clean the screen before use if it already has Russian letters on it, otherwise they may be unexpectedly changed.

## Encoding checklist (to avoid BOM)
- In VS Code bottom-right you can see file encoding. Ensure it says `UTF-8`.
- If it says `UTF-8 with BOM`, click it → `Save with encoding` → choose `UTF-8`.

## Notes about microcontroller and UTF-8
- The library expects UTF-8 encoded input. The code decodes UTF-8 sequences and maps Unicode code points (U+0400..U+04FF).
- AVR (e.g. Arduino Uno) stores string literals in flash (PROGMEM). When using non-ASCII characters in the source,
  saving the file as UTF-8 without BOM ensures the bytes in flash represent the intended UTF-8 sequences.
