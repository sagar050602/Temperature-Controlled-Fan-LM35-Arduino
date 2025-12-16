

#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define LCD_ADDR 0x27    
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);


const uint8_t PIN_LM35 = A0;
const uint8_t PIN_ENA  = 11; 
const uint8_t PIN_IN1  = 9;
const uint8_t PIN_IN2  = 8;


const float TEMP_MIN = 25.0;   
const float TEMP_MAX = 50.0;  
const float DISPLAY_DELTA = 1.0; 
const unsigned long SENSOR_INTERVAL_MS = 200;  
const unsigned long LCD_REFRESH_MS     = 5000; 
const unsigned long SOFT_RAMP_MS       = 800;  


const uint8_t ADC_SAMPLES = 8;  


#define SERIAL_DEBUG 1


unsigned long lastSensorMs = 0;
unsigned long lastLcdRefreshMs = 0;
float lastDisplayedTemp = -200.0;

int currentPwm = 0;
int targetPwm = 0;
int rampStartPwm = 0;
unsigned long rampStartMs = 0;

void setup() {
  #if SERIAL_DEBUG
    Serial.begin(115200);
    Serial.println(F("Temp-Fan Boot"));
  #endif

  lcd.init();
  lcd.backlight();

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);

  analogWrite(PIN_ENA, 0); 

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TempCtrl Fan");
  lcd.setCursor(0,1);
  lcd.print("Init...");
  delay(200);
}

float readTempC_avg() {
  unsigned long acc = 0;
  for (uint8_t i = 0; i < ADC_SAMPLES; ++i) {
    acc += analogRead(PIN_LM35);
    delay(3); 
  }
  float avg = (float)acc / ADC_SAMPLES;
  float voltage = avg * (5.0 / 1023.0);
  float tempC = voltage * 100.0;        
  return tempC;
}

int tempToPwm(float t) {
 
  float norm = (t - TEMP_MIN) / (TEMP_MAX - TEMP_MIN);
  if (norm <= 0.0) return 0;
  if (norm >= 1.0) return 255;
  int pwm = (int)(norm * 255.0 + 0.5);
  return pwm;
}

void startRamp(int newPwm) {
  if (newPwm == targetPwm) return;
  rampStartPwm = currentPwm;
  targetPwm = newPwm;
  rampStartMs = millis();
}

void updateRamp() {
  if (currentPwm == targetPwm) return;
  unsigned long now = millis();
  unsigned long elapsed = now - rampStartMs;
  if (elapsed >= SOFT_RAMP_MS) {
    currentPwm = targetPwm;
  } else {
    float frac = (float)elapsed / (float)SOFT_RAMP_MS;
    currentPwm = rampStartPwm + (int)((targetPwm - rampStartPwm) * frac + 0.5);
  }
  analogWrite(PIN_ENA, currentPwm);
}

void updateLCD(float tempC, int pwmVal) {
  
  char buf[16];
  lcd.setCursor(0,0);
  lcd.print("TempCtrl Fan    "); 
  lcd.setCursor(0,1);
  dtostrf(tempC, 4, 1, buf); 
  lcd.print("T:");
  lcd.print(buf);
  lcd.print("C ");
  lcd.print("P:");
  if (pwmVal < 10) lcd.print("  ");
  else if (pwmVal < 100) lcd.print(" ");
  lcd.print(pwmVal);
  lcd.print(" ");
}

void loop() {
  unsigned long now = millis();


  if (now - lastSensorMs >= SENSOR_INTERVAL_MS) {
    lastSensorMs = now;
    float t = readTempC_avg();
    #if SERIAL_DEBUG
      Serial.print("Temp: "); Serial.print(t,2);
      Serial.print(" C -> ");
    #endif

    int pwm = tempToPwm(t);
    startRamp(pwm); 

  
    if (fabs(t - lastDisplayedTemp) >= DISPLAY_DELTA) {
      lastDisplayedTemp = t;
      updateLCD(t, pwm);
      lastLcdRefreshMs = now;
    }

    #if SERIAL_DEBUG
      Serial.print("PWM Target: "); Serial.println(pwm);
    #endif
  }

  
  if (now - lastLcdRefreshMs >= LCD_REFRESH_MS) {
  
    float quickT = (analogRead(PIN_LM35) * (5.0 / 1023.0)) * 100.0;
    int quickP = tempToPwm(quickT);
    updateLCD(quickT, quickP);
    lastLcdRefreshMs = now;
    lastDisplayedTemp = quickT;
  }

  
  updateRamp();


  delay(8);
}

