#include <Adafruit_SSD1306.h>

#include "display.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displaySetup()
{
    Serial.print("Starting Display ... ");
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    Serial.println("Done");

    display.clearDisplay();
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(APPNAME);
    display.display();
}

void displayLoop()
{
    static unsigned long msTick = millis();
    if (millis() - msTick >= 500) // 500ms refresh rate
    {
        msTick = millis();

        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("i2c display");
        // display.println(ct_current, 2);
        display.display();
        display.display();
    }
}
