#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int hallSensor = A0;

const float sensitivity = 0.0025; // Sensitivity of Sensor in V/G
const float offsetVoltage = 2.55; // Offset Value of Sensor

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    pinMode(hallSensor, INPUT);
    Serial.begin(9600);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    display.display();
    delay(2000); // Startup Delay
    display.clearDisplay();
    display.display();
}

void loop() {
    float analogValue;
    float voltage;
    float magneticFieldGauss;
    float magneticFieldMicrotesla;
    float temperature;

    analogValue = analogRead(hallSensor); // Conversion to Voltage
    voltage = analogValue * (5.0 / 1023.0);

    magneticFieldGauss = (voltage - offsetVoltage) / sensitivity; // Gauss

    magneticFieldMicrotesla = magneticFieldGauss * 100; // microTesla

    temperature = abs(59086 - magneticFieldMicrotesla) / 498.65; // Temperature Formula Derived from Linear Regression

    display.clearDisplay(); // Clear
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    int16_t x1, y1;
    uint16_t width, height;

    // Magnetic Field Value
    String magneticFieldText = "B: " + String(magneticFieldMicrotesla, 2) + " uT";
    display.getTextBounds(magneticFieldText, 0, 0, &x1, &y1, &width, &height);
    display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT / 2) - height);
    display.print(magneticFieldText);

    // Temperature Value
    if (temperature >= 0 && temperature <= 95) {
        String temperatureText = "T: " + String(temperature, 2) + " " + (char)248 + "C";
        display.getTextBounds(temperatureText, 0, 0, &x1, &y1, &width, &height);
        display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT / 2) + 10);
        display.print(temperatureText);
    } else {
        String undefinedText = "Out of Range!";
        display.getTextBounds(undefinedText, 0, 0, &x1, &y1, &width, &height);
        display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT / 2) + 10);
        display.print(undefinedText);
    }

    display.display();
    delay(2000);
}
