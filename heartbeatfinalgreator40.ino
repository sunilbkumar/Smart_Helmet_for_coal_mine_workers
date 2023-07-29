#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>

#define REPORTING_PERIOD_MS 1000

// Initialize the MAX30100 sensor
PulseOximeter pox;

// Initialize the 16x2 I2C LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint32_t tsLastReport = 0;

void onBeatDetected()
{
    Serial.println("Beat detected");
}

void setup()
{
    Serial.begin(115200);

    // Initialize the LCD display
    lcd.init();
    lcd.backlight();
    

    // Initialize the MAX30100 sensor
    if (!pox.begin())
    {
        Serial.println("Failed to initialize the sensor");
        while (1);
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    pox.update();

    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        tsLastReport = millis();

        float bpm = pox.getHeartRate();
        float spo2 = pox.getSpO2();

        

        if (bpm > 40)
        {
            lcd.setCursor(0, 0);
            lcd.print("BPM: ");
            lcd.setCursor(5, 0);
            lcd.print(bpm);
            lcd.setCursor(0, 1);
            lcd.print("SpO2: ");
            lcd.setCursor(6, 1);
            lcd.print(spo2);
        }
        else
        {
        lcd.setCursor(0, 0);
        lcd.print("                "); // to Clear the 1st row previous BPM value
        lcd.setCursor(0, 1);
        lcd.print("                "); // to Clear the 2nd row previous SpO2 value
    }
    }
}
