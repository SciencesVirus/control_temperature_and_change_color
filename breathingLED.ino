#include "DHT_Async.h"

#define DHT_SENSOR_TYPE DHT_TYPE_22
#define PIN_R 2
#define PIN_G 4
#define PIN_B 15
#define PIN_12 16
#define outputA 23
#define outputB 22
#define relay 19
static const int DHT_SENSOR_PIN = 21;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
volatile int counter = 0;
int aState;
int aLastState;
int speed = 0;
float temperature;
float humidity;
TaskHandle_t breathTaskHandle = NULL;
TaskHandle_t knobEncoderTaskHandle = NULL;
void breathTask(void *parameter);
void knobEncoderTask(void *parameter);
void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_12, OUTPUT);
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(relay, OUTPUT);

  Serial.begin(115200);

  aLastState = digitalRead(outputA);

  xTaskCreatePinnedToCore(
    breathTask,
    "BreathTask",
    10000,
    NULL,
    1,
    &breathTaskHandle,
    0);
  xTaskCreatePinnedToCore(
    knobEncoderTask,
    "KnobEncoderTask",
    10000,
    NULL,
    1,
    &knobEncoderTaskHandle,
    1);  // Assign to the second core (core ID 1)
}

static bool measure_environment(float *temperature, float *humidity) {
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 500ul) {
    if (dht_sensor.measure(temperature, humidity)) {
      measurement_timestamp = millis();
      return (true);
    }
  }

  return (false);
}

void loop() {
  Serial.println(counter);
  analogWrite(PIN_12, 255);
  if (measure_environment(&temperature, &humidity)) {
    Serial.print(temperature, 1);
    Serial.print(" deg.");
  }
  if (temperature < 50) {
    digitalWrite(relay, HIGH);
    Serial.println("heating");
    Serial.println(temperature);
  } else {
    digitalWrite(relay, LOW);
    
  }
  delay(1000);  // Delay to avoid flooding the serial monitor
}
void breathTask(void *parameter) {
  while (1) {
    vTaskDelay(100);  // Adjust delay as needed

    while (1) {
      if (counter == 0) {
        analogWrite(PIN_12, 255);
        blue_breath();
        if (counter != 0) {
          break;
        }
        Serial.println(counter);
        pink_breath();
        if (counter != 0) {
          break;
        }
        Serial.println(counter);
        orange_breath();
        if (counter != 0) {
          break;
        }
        Serial.println(counter);
        purple_breath();
        if (counter != 0) {
          break;
        }
        Serial.println(counter);
      } else if (counter == 1) {
        analogWrite(PIN_12, 255);
        blue_breath();

        Serial.println(counter);
      } else if (counter == 2) {
        analogWrite(PIN_12, 255);
        pink_breath();

        Serial.println(counter);
      } else if (counter == 3) {
        analogWrite(PIN_12, 255);
        orange_breath();

        Serial.println(counter);
      } else if (counter == 4) {
        analogWrite(PIN_12, 255);
        purple_breath();

        Serial.println(counter);
      }
    }
  }
}

void knobEncoderTask(void *parameter) {
  aLastState = digitalRead(outputA);

  while (1) {
    aState = digitalRead(outputA);

    if (aState != aLastState) {
      if (digitalRead(outputB) != aState) {
        counter++;
        if (counter >= 4) {
          counter = 4;
        }
      } else {
        counter--;
        if (counter <= 0) {
          counter = 0;
        }
      }
    }

    aLastState = aState;

    // Add any additional code related to knob_encoder here
  }
}
void purple_breath() {
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, 255);

  for (speed = 0; speed <= 255; speed++) {

    float pwm_valR = map(speed, 0, 255, 255, 0);
    float pwm_valB = map(speed, 0, 255, 255, 0);
    analogWrite(PIN_R, pwm_valR);
    analogWrite(PIN_B, pwm_valB);
    delay(20);
  }
  for (speed = 255; speed >= 0; speed--) {

    float pwm_valR = map(speed, 0, 255, 255, 0);
    float pwm_valB = map(speed, 0, 255, 255, 0);
    analogWrite(PIN_R, pwm_valR);
    analogWrite(PIN_B, pwm_valB);
    delay(5);
  }
}


void orange_breath() {
  analogWrite(PIN_12, 255);
  analogWrite(PIN_B, 255);
  for (speed = 0; speed <= 255; speed++) {

    float pwm_valR = map(speed, 0, 255, 255, 0);
    float pwm_valG = map(speed, 0, 255, 255, 240);
    analogWrite(PIN_R, pwm_valR);
    analogWrite(PIN_G, pwm_valG);
    delay(20);
  }
  for (speed = 255; speed >= 0; speed--) {

    float pwm_valR = map(speed, 0, 255, 255, 0);
    float pwm_valG = map(speed, 0, 255, 255, 240);
    analogWrite(PIN_R, pwm_valR);
    analogWrite(PIN_G, pwm_valG);
    delay(5);
  }
}


void pink_breath() {
  analogWrite(PIN_12, 255);

  for (speed = 0; speed <= 255; speed++) {

    float pwm_valR = map(speed, 0, 255, 255, 0);
    float pwm_valG = map(speed, 0, 255, 255, 240);
    float pwm_valB = map(speed, 0, 255, 255, 240);
    analogWrite(PIN_R, pwm_valR);
    analogWrite(PIN_G, pwm_valG);
    analogWrite(PIN_B, pwm_valB);
    delay(20);
  }
  for (speed = 255; speed >= 0; speed--) {

    float pwm_valR = map(speed, 0, 255, 255, 0);
    float pwm_valG = map(speed, 0, 255, 255, 240);
    float pwm_valB = map(speed, 0, 255, 255, 240);
    analogWrite(PIN_R, pwm_valR);
    analogWrite(PIN_G, pwm_valG);
    analogWrite(PIN_B, pwm_valB);
    delay(5);
  }
}

void blue_breath() {
  analogWrite(PIN_12, 255);
  analogWrite(PIN_G, 255);
  analogWrite(PIN_R, 255);

  for (speed = 0; speed <= 255; speed++) {

    float pwm_valB = map(speed, 0, 255, 255, 0);
    analogWrite(PIN_B, pwm_valB);
    delay(20);
  }
  for (speed = 255; speed >= 0; speed--) {

    float pwm_valB = map(speed, 0, 255, 255, 0);
    analogWrite(PIN_B, pwm_valB);
    delay(5);
  }
}
