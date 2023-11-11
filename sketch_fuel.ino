#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define trigPin 2
#define echoPin 3

LiquidCrystal_I2C lcd(0x27, 16, 2); 

float distance = 0;
float lastDistance = 0;
float currentDistance = 0;
float fuelVolume = 0;
float timeLeft = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;
}

void calculateVolLeft() {
  currentDistance = distance;

  // ความสูงของถังน้ำมัน 9 cm, หา volume ของน้ำมันที่เหลืออยู่
  fuelVolume = 26.5 * 9.5 * (9.0 - currentDistance); // กว้าง x ยาว x สูง
  fuelVolume = round(fuelVolume * 10.0) / 10.0; // ปรับเป็นทศนิยม 1 ตำแหน่ง

  float distanceChange = currentDistance - lastDistance;

  // เชคดูว่ามีการเปลี่ยนแปลงระยะทางหรือไม่ ถ้าไม่มีก็ใช้ค่าเดิม
  timeLeft = (distanceChange > 0) ? (fuelVolume / distanceChange) : timeLeft; 
  timeLeft = round(timeLeft * 10.0) / 10.0; // ปรับเป็นทศนิยม 1 ตำแหน่ง

  if (fuelVolume < 100) {
    timeLeft = 0;
  }

  lastDistance = distance;
}

void loop() {
  getDistance();
  delay(1);
  calculateVolLeft();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fuel: ");
  lcd.setCursor(6, 0);
  lcd.print(fuelVolume);

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.setCursor(6, 1);

  float timeMinute = round(timeLeft / 60);

  lcd.print(timeMinute);
  lcd.print("m");


  // 5 นาที
  delay(300000);
}
