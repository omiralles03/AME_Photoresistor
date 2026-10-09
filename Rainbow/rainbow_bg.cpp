#include "rainbow_bg.h"
#include <cmath>

void hsvToRgb(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
    float c = v * s;[cite: 6]
    float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));[cite: 6]
    float m = v - c;[cite: 6]

    float rPrime = 0.0f, gPrime = 0.0f, bPrime = 0.0f;[cite: 6]

    if (h >= 0.0f && h < 60.0f) {[cite: 6]
        rPrime = c; gPrime = x; bPrime = 0.0f;[cite: 6]
    } else if (h >= 60.0f && h < 120.0f) {[cite: 6]
        rPrime = x; gPrime = c; bPrime = 0.0f;[cite: 6]
    } else if (h >= 120.0f && h < 180.0f) {[cite: 6]
        rPrime = 0.0f; gPrime = c; bPrime = x;[cite: 6]
    } else if (h >= 180.0f && h < 240.0f) {[cite: 6]
        rPrime = 0.0f; gPrime = x; bPrime = c;[cite: 6]
    } else if (h >= 240.0f && h < 300.0f) {[cite: 6]
        rPrime = x; gPrime = 0.0f; bPrime = c;[cite: 6]
    } else {[cite: 6]
        rPrime = c; gPrime = 0.0f; bPrime = x;[cite: 6]
    }

    r = static_cast<uint8_t>((rPrime + m) * 255.0f);[cite: 6]
    g = static_cast<uint8_t>((gPrime + m) * 255.0f);[cite: 6]
    b = static_cast<uint8_t>((bPrime + m) * 255.0f);[cite: 6]
}

void updateRainbowNonBlocking(Grove_LCD_RGB_Backlight &lcd, float stepDegrees, std::chrono::milliseconds interval) {
    static Timer timer;[cite: 6]
    static bool started = false;[cite: 6]
    static float hue = 0.0f;[cite: 6]

    if (!started) {[cite: 6]
        timer.start();[cite: 6]
        started = true;[cite: 6]
    }

    if (timer.elapsed_time() >= interval) {[cite: 6]
        timer.reset();[cite: 6]

        uint8_t r = 0, g = 0, b = 0;[cite: 6]
        hsvToRgb(hue, 1.0f, 1.0f, r, g, b);[cite: 6]
        
        // Apply to the Grove LCD instead of the generic forward declaration
        lcd.setRGB(static_cast<char>(r), static_cast<char>(g), static_cast<char>(b));

        hue += stepDegrees;[cite: 6]
        if (hue >= 360.0f) {[cite: 6]
            hue -= 360.0f;[cite: 6]
        }
    }
}