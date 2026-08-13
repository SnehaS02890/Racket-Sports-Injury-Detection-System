#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int MPU = 0x68;

#define LED_PIN 18
#define BUZZER_PIN 19

LiquidCrystal_I2C lcd(0x27, 16, 2);

int threshold = 5000;
bool warningState = false;

void setup() {

  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Wire.begin(21,22);

  // Wake MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("System Ready");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAFE");
  lcd.setCursor(0,1);
  lcd.print("Good Position");
}

void loop() {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(MPU, 6, true);

  int16_t AcX = Wire.read()<<8 | Wire.read();
  int16_t AcY = Wire.read()<<8 | Wire.read();
  int16_t AcZ = Wire.read()<<8 | Wire.read();

  Serial.print("X=");
  Serial.print(AcX);
  Serial.print(" Y=");
  Serial.print(AcY);
  Serial.print(" Z=");
  Serial.println(AcZ);

  bool warning =
      abs(AcX) > threshold ||
      abs(AcY) > threshold;

  if (warning && !warningState) {

    warningState = true;

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WARNING!");
    lcd.setCursor(0,1);
    lcd.print("Bad Position");
  }

  if (!warning && warningState) {

    warningState = false;

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SAFE");
    lcd.setCursor(0,1);
    lcd.print("Good Position");
  }

  delay(200);
}