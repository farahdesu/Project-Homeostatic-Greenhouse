#include <SimpleDHT.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ================== DHT22 setup ==================
int pinDHT22 = 2;
SimpleDHT22 dht22(pinDHT22);

// ================== Raindrop sensor ==================
#define RAIN_SENSOR A0
#define LED_RAIN 13       // LED for rain sensor
#define BUZZER_PIN 11     // Buzzer for rain detection
int rainThreshold = 500;  // tune

// ================== Soil moisture ==================
#define SOIL_PIN A1
#define SOIL_WET 210
#define SOIL_DRY 510
int soilThreshold = 650;  // tune based on calibration

// ================== LDR sensor ==================
#define LDR_PIN A2
#define LED_LDR 12        // LED for LDR control
int ldrValue = 0;

// ================== Button ==================
const int BUTTON = 3;
int BUTTONstate = 0;
int lastButtonState = 0;
bool readingActive = false;

// ================== Relays ==================
#define RELAY_FAN 7   // active LOW
#define RELAY_PUMP 8  // active LOW

// ================== LCD ==================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================== Servo ==================
Servo myServo;
const int SERVO_PIN = 9;
const bool SERVO_IS_CONTINUOUS = true;
int SERVO_STOP_US      = 1500;
int SERVO_FULL_CCW_US  = 1000;
int SERVO_FULL_CW_US   = 2000; // For positional servos
int SERVO_DRY_ANGLE    = 0;
int SERVO_WET_ANGLE    = 90;
int SERVO_RETURN_ANGLE = 90;

// ================== UV Sensor ==================
#define UV_SENSOR_PIN A3

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_RAIN, OUTPUT);    // LED for rain sensor
  pinMode(LED_LDR, OUTPUT);     // LED for LDR control
  pinMode(BUZZER_PIN, OUTPUT);  // Buzzer pin for rain detection
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);

  digitalWrite(RELAY_FAN, HIGH);   // OFF
  digitalWrite(RELAY_PUMP, HIGH);  // OFF

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();  // Ensure backlight fully on

  myServo.attach(SERVO_PIN);
  if (SERVO_IS_CONTINUOUS) {
    myServo.writeMicroseconds(SERVO_STOP_US);
  } else {
    myServo.write(SERVO_WET_ANGLE);
  }

  // Initial message
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
}

void loop() {
  // === Button handling: toggle system ===
  BUTTONstate = digitalRead(BUTTON);
  if (BUTTONstate == LOW && lastButtonState == HIGH) {
    readingActive = !readingActive;
    lcd.clear();
    lcd.backlight(); // Restore full brightness

    if (readingActive) {
      lcd.setCursor(0, 0);
      lcd.print("Starting Smart");
      lcd.setCursor(0, 1);
      lcd.print("Greenhouse");
      Serial.println("Smart Greenhouse Started");
      delay(500); // brief pause before showing readings
    } else {
      lcd.setCursor(0, 0);
      lcd.print("System Stopped");
      Serial.println("Smart Greenhouse Stopped");
    }

    delay(200); // debounce
  }
  lastButtonState = BUTTONstate;

  if (readingActive) {
    Serial.println("=================================");

    // ===== DHT22: temperature & humidity =====
    float temperature = 0;
    float humidity = 0;
    int err = SimpleDHTErrSuccess;

    if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT22 failed, err=");
      Serial.print(SimpleDHTErrCode(err));
      Serial.print(",");
      Serial.println(SimpleDHTErrDuration(err));
    } else {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.println(" %RH");
    }

    // ===== Fan control =====
    if (temperature > 31.50) {
      digitalWrite(RELAY_FAN, LOW);  // ON
      Serial.println("Fan ON");
    } else {
      digitalWrite(RELAY_FAN, HIGH); // OFF
      Serial.println("Fan OFF");
    }

    // ===== Raindrop sensor =====
    int rainValue = analogRead(RAIN_SENSOR);
    Serial.print("Raindrop Sensor Value: ");
    Serial.println(rainValue);

    if (rainValue < rainThreshold) {
      digitalWrite(LED_RAIN, HIGH);   // LED ON
      digitalWrite(BUZZER_PIN, HIGH); // Buzzer ON
    } else {
      digitalWrite(LED_RAIN, LOW);    // LED OFF
      digitalWrite(BUZZER_PIN, LOW);  // Buzzer OFF
    }

    // ===== Soil moisture & pump =====
    int moistureValue = analogRead(SOIL_PIN);
    Serial.print("Soil Moisture Value: ");
    Serial.println(moistureValue);

    if (moistureValue > soilThreshold) {
      digitalWrite(RELAY_PUMP, LOW);   // ON
      Serial.println("Water pump ON");
    } else {
      digitalWrite(RELAY_PUMP, HIGH);  // OFF
      Serial.println("Water pump OFF");
    }

    // ===== Servo control based on soil =====
    if (moistureValue > soilThreshold) {
      if (SERVO_IS_CONTINUOUS) {
        myServo.writeMicroseconds(SERVO_FULL_CW_US);
        delay(2000);
        myServo.writeMicroseconds(SERVO_STOP_US);
      } else {
        myServo.write(SERVO_DRY_ANGLE);
        delay(2000);
        myServo.write(SERVO_RETURN_ANGLE);
      }
      Serial.println("Servo action performed due to dry soil");
    } else {
      if (SERVO_IS_CONTINUOUS) {
        myServo.writeMicroseconds(SERVO_STOP_US);
      } else {
        myServo.write(SERVO_WET_ANGLE);
      }
    }

    // ===== UV Sensor =====
    int uvValue = analogRead(UV_SENSOR_PIN);
    float uvVoltage = uvValue * (5.0 / 1023.0);
    float uvIndex = uvVoltage / 0.1;

    // ===== LDR Sensor =====
    ldrValue = analogRead(LDR_PIN);
    float ldrVoltage = ldrValue * (5.0 / 1023.0);

    // LED control based on LDR value
    if (ldrValue > 700) {
      digitalWrite(LED_LDR, HIGH);  // Turn on LDR LED
    } else {
      digitalWrite(LED_LDR, LOW);   // Turn off LDR LED
    }

    Serial.print("UV Voltage: ");
    Serial.print(uvVoltage, 2);
    Serial.print(" V, UV Index: ");
    Serial.print(uvIndex, 1);
    Serial.print(" | LDR Value: ");
    Serial.print(ldrValue);
    Serial.print(", Voltage: ");
    Serial.println(ldrVoltage, 2);

    // ===== LCD Display =====
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("UV:");
    lcd.print(uvVoltage, 2);
    lcd.setCursor(10, 0);
    lcd.print("LDR:");
    lcd.print(ldrValue);

    lcd.setCursor(0, 1);
    lcd.print("UVIdx:");
    lcd.print(uvIndex, 1);
    lcd.setCursor(10, 1);
    lcd.print("Volt:");
    lcd.print(ldrVoltage, 2);

    delay(1000); // main loop delay

  } else {
    // Safe state when system is stopped
    digitalWrite(LED_RAIN, LOW);
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_PUMP, HIGH);

    if (SERVO_IS_CONTINUOUS) {
      myServo.writeMicroseconds(SERVO_STOP_US);
    } else {
      myServo.write(SERVO_RETURN_ANGLE);
    }
  }
}
