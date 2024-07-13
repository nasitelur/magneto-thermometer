#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int hallSensor = A0; 

const float sensitivity = 0.0025; // Sensitivity of Sensor in V/G
const float offsetVoltage = 2.55; // Offset Value of Sensor

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    pinMode(hallSensor, INPUT);

    lcd.begin();
    lcd.backlight();
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

    lcd.clear();

    // Magnetic Field Value
    lcd.setCursor(0, 0);
    lcd.print("B: ");
    lcd.print(magneticFieldMicrotesla, 2);
    lcd.print(" uT");

    // Temperature Value
    if (temperature >= 0 && temperature <= 95) {
        lcd.setCursor(0, 1);
        lcd.print("T: ");
        lcd.print(temperature, 2);
        lcd.print(" ");
        lcd.print((char)223); // Degree Symbol
        lcd.print("C");
    } else {
        lcd.setCursor(0, 1);
        lcd.print("Out of Range!");
    }

    delay(2000);
}
