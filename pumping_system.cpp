#include <Firebase_ESP_Client.h>
#include <WiFi.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "REALME"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCU-Bdb3pb_iKEZS9JkqAZ5-jiUoW1Rshk" 

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://irrigation-4991d-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;



unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

// Motor driver pins
#define MOTOR_IN1 27
#define MOTOR_IN2 26
#define MOTOR_ENA 14

int category;
int moisture;
int rainValue;
int soilTemperature;

void setup() {
  Serial.begin(115200);
  
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  } Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize motor control pins
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);
} 

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)) {
  sendDataPrevMillis = millis();
     
    if (Firebase.RTDB.getInt(&fbdo, "cropCategory")){
    if (fbdo.dataType() == "int"){ 
    category =  fbdo.intData();   
    Serial.println("cropCategory:"); 
    Serial.println(category); 
     }
    }

    if (Firebase.RTDB.getInt(&fbdo, "Temp")){
    if (fbdo.dataType() == "int"){ 
    soilTemperature =  fbdo.intData();
    Serial.println("soilTemperature:"); 
    Serial.println(soilTemperature); 
     }
    }
    
    if (Firebase.RTDB.getInt(&fbdo, "Moisture")){
    if (fbdo.dataType() == "int"){ 
    moisture =  fbdo.intData();
    Serial.println("soilMoistureValue:"); 
    Serial.println(moisture); 
     }
    }


    if (Firebase.RTDB.getInt(&fbdo, "Rain")){
    if (fbdo.dataType() == "int"){ 
    rainValue =  fbdo.intData();  
    Serial.println("rainValue:"); 
    Serial.println(rainValue); 
      }
    }

    int speed = 0;
  
  if (category == 1) {                   // Low water requirement
    if (moisture < 1700) speed = 70;          // Low Moisture
    else if (moisture >= 1700 && moisture < 2100) speed = 60;     // Medium Moisture
    else if (moisture >= 2100) speed = 50;     // Medium Moisture
  } 
  
  else if (category == 2) {             // Medium water requirement
    if (moisture < 1700) speed = 85;          // Low Moisture
    else if (moisture >= 1700 && moisture < 2100) speed = 65;     // Medium Moisture
    else if (moisture >= 2100) speed = 50;     // Medium Moisture

  } 
  
  else if (category == 3) {             // High water requirement
    if (moisture < 1700) speed = 100;          // Low Moisture
    else if (moisture >= 1700 && moisture < 2100) speed = 75;     // Medium Moisture
    else if (moisture >= 2100) speed = 50;     // Medium Moisture
  
  }

  if (rainValue > 150) {  // Rain detected
    speed=0;
  }
  Serial.print("Speed: ");
  Serial.println(speed);
  
  analogWrite(MOTOR_ENA, speed);
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  delay(10000);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, 0);
  
  delay(2000);  // Delay for stability
}
}
