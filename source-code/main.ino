#define BLYNK_TEMPLATE_ID "TMPL6h3OrB3zU"
#define BLYNK_TEMPLATE_NAME "Flood gate"
#define BLYNK_AUTH_TOKEN "ip7FeQwiBZLgA2V21HioiNA01GVsY_PW"

#define ULTRASONIC_ECHO_PIN 17
#define ULTRASONIC_TRIG_PIN 5

#define RAINFALL_SENSOR_PIN 15

#define WATER_SENSOR_PIN 4

#define BUZZER_PIN 22
#define LED_PIN 21

#define MOTOR_OPEN_PIN 19
#define MOTOR_REVERSE_PIN 18

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// network credentials
char ssid[] = "Sohaila's A55";
char pass[] = "am55sikiak12";
char auth[] = BLYNK_AUTH_TOKEN;

// variable to store ultrasonic sensor readings
long duration, distance;

void setup() {
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  pinMode(RAINFALL_SENSOR_PIN, INPUT);

  pinMode(WATER_SENSOR_PIN, INPUT);
  digitalWrite(WATER_SENSOR_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Buzzer OFF by default

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED OFF by default

  pinMode(MOTOR_OPEN_PIN, OUTPUT);
  pinMode(MOTOR_REVERSE_PIN, OUTPUT);
  // motor off by defualt
  digitalWrite(MOTOR_OPEN_PIN, LOW); 
  digitalWrite(MOTOR_REVERSE_PIN, LOW);
}

void loop() {
  Blynk.run();
  ultrasonic();
  detectRain();
  detectWater();
}

//Ultrasonic function to calculte the distance and send to blynk
void ultrasonic() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = duration / 58.2;
  String disp = String(distance);

  Blynk.virtualWrite(V2, distance); 
  delay(1000);
}

void detectRain(){
  // LOW if water detected, HIGH if not
  int rainDetected = digitalRead(RAINFALL_SENSOR_PIN);

  if (rainDetected == LOW) {
    Blynk.virtualWrite(V1, 1); 
    Serial.print("rain detected\n");
  } else {
    Blynk.virtualWrite(V1, 0);
    Serial.print("no rain\n"); 
  }

  delay(1000);
}

void detectWater(){
  int waterLevel = digitalRead(WATER_SENSOR_PIN);

  Serial.print("Water Sensor Value: ");
  Serial.println(waterLevel); 

  if (waterLevel == HIGH) {
    Blynk.virtualWrite(V4, 1);
    activateWarning();
  } else {
    Blynk.virtualWrite(V4, 0);
  }

  delay(500);
}

void activateWarning() {
  Blynk.virtualWrite(V0, 1);
  Blynk.virtualWrite(V5, 1);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
}

// Functions to control the relay from the Blynk app 

// Buzzer functionality
BLYNK_WRITE(V0) { 
  if (param.asInt() == HIGH)  { 
    digitalWrite(BUZZER_PIN, HIGH); 
  } else {
    digitalWrite(BUZZER_PIN, LOW); 
  } 
}

// LED functionality
BLYNK_WRITE(V5) {
  if(param.asInt() == HIGH) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

// open gate functionality
BLYNK_WRITE(V3) {
  if(param.asInt() == HIGH) {
    digitalWrite(MOTOR_OPEN_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_OPEN_PIN, LOW);
  }
}

// close gate functionality 
BLYNK_WRITE(V6) {
   if(param.asInt() == HIGH) {
    digitalWrite(MOTOR_REVERSE_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_REVERSE_PIN, LOW);
  }
}