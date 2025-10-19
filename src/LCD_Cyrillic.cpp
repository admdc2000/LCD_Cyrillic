// LCD_Cyrillic.cpp
// This file decodes UTF-8 sequences, maps Cyrillic Unicode code points to
// either ASCII approximations or custom LCD glyph slots, and prints to the LCD.

#include "LCD_Cyrillic.h"
#include <Wire.h>

static String mapCyrToAscii(uint16_t cp) {
    switch (cp) {
        case 0x0410: return "A"; // А
        case 0x0412: return "B"; // В
        case 0x0415: return "E"; // Е
        case 0x0401: return "E"; // Ё
        case 0x041A: return "K"; // К
        case 0x041C: return "M"; // М
        case 0x041D: return "H"; // Н
        case 0x041E: return "O"; // О
        case 0x0420: return "P"; // Р
        case 0x0421: return "C"; // С
        case 0x0422: return "T"; // Т
        case 0x0425: return "X"; // Х
        case 0x0417: return "3"; // З
        case 0x042C: return "b"; // Ь
        default: return "";
    }
}

static const uint8_t* findGlyphBitmap(uint16_t cp) {
    for (size_t i = 0; i < GLYPH_DB_SIZE; ++i) {
        if (glyphDB[i].code == cp) return glyphDB[i].data;
    }
    return nullptr;
}

static int readUtf8Codepoint(const String &s, size_t &i) {
    if (i >= s.length()) return -1;
    uint8_t b1 = static_cast<uint8_t>(s[i]);
    if (b1 < 0x80) {
        i++;
        return b1;
    }
    if (i + 1 >= s.length()) {
        i++;
        return -1;
    }
    uint8_t b2 = static_cast<uint8_t>(s[i + 1]);
    if (b1 == 0xD0) {
        i += 2;
        return 0x0400 + (b2 - 0x80);
    } else if (b1 == 0xD1) {
        i += 2;
        return 0x0440 + (b2 - 0x80);
    } else {
        i++;
        return -1;
    }
}

static uint16_t toUpperCyrillic(uint16_t cp) {
    if (cp >= 0x0430 && cp <= 0x044F) return cp - 0x20;
    if (cp == 0x0451) return 0x0401;
    return cp;
}

// ----------------- Public Function -----------------
String LCD_Cyrillic::prepareForLCD(const String &input, LiquidCrystal_I2C &lcd) {
    constexpr size_t MAX_CH = 256;
    uint16_t ubuf[MAX_CH];
    size_t ucount = 0;

    // Step 1: UTF-8 decode → uppercase Unicode
    for (size_t i = 0; i < input.length(); ) {
        int cp = readUtf8Codepoint(input, i);
        if (cp < 0) continue;

        if (cp < 128) {
            if (cp >= 'a' && cp <= 'z') cp = cp - 'a' + 'A';
            ubuf[ucount++] = static_cast<uint16_t>(cp);
        } else {
            cp = toUpperCyrillic(static_cast<uint16_t>(cp));
            ubuf[ucount++] = static_cast<uint16_t>(cp);
        }
        if (ucount >= MAX_CH) break;
    }

    // Step 2: collect unique custom glyphs (no ASCII fallback)
    uint16_t customList[8];
    uint8_t customCount = 0;

    for (size_t i = 0; i < ucount; ++i) {
        uint16_t cp = ubuf[i];
        if (cp < 0x0400) continue;

        if (!mapCyrToAscii(cp).isEmpty()) continue;

        const uint8_t* bm = findGlyphBitmap(cp);
        if (!bm) continue;

        bool found = false;
        for (uint8_t j = 0; j < customCount; ++j) {
            if (customList[j] == cp) {
                found = true;
                break;
            }
        }
        if (!found && customCount < 8) {
            customList[customCount++] = cp;
        }
    }

    // Step 3: load glyphs into CGRAM
    for (uint8_t slot = 0; slot < customCount; ++slot) {
        const uint8_t* bm = findGlyphBitmap(customList[slot]);
        if (bm) {
            lcd.createChar(slot, const_cast<uint8_t*>(bm));
            delay(5);
        }
    }

    // Step 4: build output string
    String out;
    for (size_t i = 0; i < ucount; ++i) {
        uint16_t cp = ubuf[i];
        if (cp < 0x0400) {
            out += static_cast<char>(cp);
            continue;
        }

        String ascii = mapCyrToAscii(cp);
        if (!ascii.isEmpty()) {
            out += ascii;
            continue;
        }

        int8_t slot = -1;
        for (uint8_t j = 0; j < customCount; ++j) {
            if (customList[j] == cp) {
                slot = j;
                break;
            }
        }

        if (slot >= 0) {
            out += static_cast<char>(slot);
        } else {
            out += '?';
        }
    }

    return out;
}

void LCD_Cyrillic::print(LiquidCrystal_I2C& lcd, const String& text, uint8_t startCol, uint8_t startRow) {
    String prepared = prepareForLCD(text, lcd);
    uint8_t col = startCol;
    uint8_t row = startRow;

    lcd.setCursor(col, row);

    for (size_t i = 0; i < prepared.length(); ++i) {
        char c = prepared[i];

        if (c == '\n') {
            row++;
            col = 0;
            if (row >= 2) break;
            lcd.setCursor(col, row);
        } else {
            lcd.write(static_cast<uint8_t>(c));
            col++;
            // Auto-wrap when moving past the end of the line
            if (col >= 16) {
                row++;
                col = 0;
                if (row >= 2) break;
                lcd.setCursor(col, row);
            }
        }
    }
}