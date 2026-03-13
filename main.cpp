#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Pins (Sensor on 32, Buzzer on 18)
const int SOIL_SENSOR_PIN = 32; 
const int BUZZER_PIN = 18; 

// Hotspot
const char* ssid = "SmartFarm";
const char* password = "12345678";

WebServer server(80);

// Global Variables
int moistureValue = 0;
String selectedCrop = "Wheat";
String soilType = "Black";
int cropAge = 30;
String finalAdvice = "Checking...";
String voiceText = "Sichai ki zaroorat nahi hai";

void updateLogic() {
  // 1. Buzzer Logic (Strict 15% Rule)
  if (moistureValue < 15) {
    digitalWrite(BUZZER_PIN, HIGH);
    voiceText = "Mitti bahut sookhi hai, kripya turant sichai karein.";
    finalAdvice = "ALERT: Turant pani dein! Nami 15% se kam hai.";
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    
    // 2. Decision Logic (Based on Crop, Soil, Weather)
    // Weather: 2 din baad baarish hone wali hai
    if (moistureValue > 35) {
      finalAdvice = "Nami kaafi hai aur baarish bhi hone wali hai, pani na dein.";
      voiceText = "Abhi pani dene ki koi zaroorat nahi hai.";
    } else if (moistureValue <= 35 && moistureValue >= 15) {
      finalAdvice = "Nami kam ho rahi hai, lekin 2 din baad baarish hai. Halki sichai kar sakte hain.";
      voiceText = "Baarish hone wali hai, isliye halki sichai kaafi hogi.";
    }
  }
}

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  
  // AJAX for LIVE Tracking
  html += "<script>setInterval(function() {";
  html += "  fetch('/data').then(r => r.json()).then(d => {";
  html += "    document.getElementById('p').innerHTML = d.m + '%';";
  html += "    document.getElementById('f').style.width = d.m + '%';";
  html += "    document.getElementById('adv').innerHTML = d.adv;";
  html += "    window.vText = d.v;";
  html += "    if(d.m < 15) document.getElementById('adv').style.color = 'red';";
  html += "    else document.getElementById('adv').style.color = 'green';";
  html += "  });";
  html += "}, 500);";
  html += "function speak(){ var msg = new SpeechSynthesisUtterance(window.vText); msg.lang='hi-IN'; window.speechSynthesis.speak(msg); }</script>";

  html += "<style>body{font-family:Arial; background:#f4f7f6; text-align:center; padding:10px;}";
  html += ".card{background:white; padding:20px; border-radius:20px; max-width:600px; margin:auto; box-shadow:0 4px 15px rgba(0,0,0,0.1);}";
  html += ".battery{width:90%; height:55px; border:4px solid #333; margin:20px auto; border-radius:12px; position:relative; background:#eee; overflow:hidden;}";
  html += ".fill{height:100%; width:0%; background:linear-gradient(90deg, #3498db, #2ecc71); transition:0.4s;}";
  html += "select, input{padding:12px; width:95%; margin:8px 0; border-radius:8px; border:1px solid #ddd; font-size:16px;}";
  html += ".btn{padding:15px; width:100%; background:#f39c12; color:white; border-radius:10px; border:none; font-weight:bold; cursor:pointer; font-size:18px;}</style></head><body>";

  html += "<div class='card'><h2>SmartFarm Dashboard</h2>";
  html += "<b>Live Humidity Level:</b><div class='battery'><div id='f' class='fill'></div></div>";
  html += "<h1 id='p'>0%</h1>";
  
  html += "<div style='background:#e3f2fd; padding:15px; border-radius:10px; margin-bottom:15px;'>";
  html += "<b>Expert Advice:</b><br><span id='adv' style='font-weight:bold; font-size:18px;'>Loading...</span></div>";
  
  html += "<button class='btn' onclick='speak()'>📢 Salah Sunein (Voice)</button>";

  html += "<hr><form action='/setup' style='text-align:left;'>";
  html += "Fasal (Crop): <select name='c'><option value='Wheat'>Gehu (Wheat)</option><option value='Rice'>Dhan (Rice)</option></select>";
  html += "Mitti (Soil): <select name='s'><option value='Black'>Kali Mitti</option><option value='Sandy'>Retili Mitti</option></select>";
  html += "Umr (Days): <input type='number' name='a' value='" + String(cropAge) + "'>";
  html += "Weather: <input type='text' value='2 din baad baarish hone wali hai' readonly style='background:#f9f9f9;'>";
  html += "<input type='submit' value='Settings Update Karein' style='background:#2ecc71; color:white; border:none; padding:15px; width:100%; border-radius:10px; cursor:pointer; font-weight:bold; margin-top:10px;'>";
  html += "</form></div></body></html>";
  
  server.send(200, "text/html", html);
}

void handleData() {
  updateLogic();
  String j = "{\"m\":" + String(moistureValue) + ", \"adv\":\"" + finalAdvice + "\", \"v\":\"" + voiceText + "\"}";
  server.send(200, "application/json", j);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/setup", [](){ 
    selectedCrop = server.arg("c"); 
    soilType = server.arg("s"); 
    cropAge = server.arg("a").toInt();
    server.sendHeader("Location", "/"); server.send(303); 
  });
  server.begin();
}

void loop() {
  server.handleClient();
  int raw = analogRead(SOIL_SENSOR_PIN);
  
  // Final Calibration (Hawa mein 0%, Paani mein 100%)
  moistureValue = map(raw, 4095, 1200, 0, 100); 
  if(moistureValue < 0) moistureValue = 0;
  if(moistureValue > 100) moistureValue = 100;
  
  delay(100); 
}
