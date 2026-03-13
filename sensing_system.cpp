#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

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

// Sensor pins

#define SOIL_MOISTURE_PIN 35
#define RAIN_SENSOR_PIN 34


void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);

  

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

  analogReadResolution(12); // Ensure 12-bit resolution (0 to 4095)

}

void loop() {
  // Read sensor values
  delay(500); 
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  int rainValue = analogRead(RAIN_SENSOR_PIN);
  float temperature = dht.readTemperature();


if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0)) {
 sendDataPrevMillis = millis();

  Firebase.RTDB.setFloat(&fbdo, "Temp", temperature);
  Serial.print("Temp: "); 
    Serial.println(temperature); 
  Firebase.RTDB.setInt(&fbdo, "Moisture", soilMoistureValue);
  Serial.print("soilMoistureValue: "); 
    Serial.println(soilMoistureValue); 
  Firebase.RTDB.setInt(&fbdo, "Rain", rainValue);
  Serial.print("rainValue: "); 
    Serial.println(rainValue); 

  delay(1000);  // Delay for stability
  
  } 
}
