1.Overview<br>
Traditional irrigation often leads to significant water wastage and crop damage due to over-watering or under-watering caused by manual monitoring and a lack of real-time environmental data. This project solves these issues by using an ESP32-based IoT framework that automates water delivery based on live soil moisture levels, crop-specific requirements, and real-time weather forecasts via API integration.<br>
2. Key Features<br>
Crop-Specific Logic: The system is programmed with categories (e.g., High Water vs. Low Water requirements), adjusting its moisture threshold automatically based on the selected plant type.<br>
WhatsApp Image 2026-03-13 at 12.38.03 (1)
#Weather-Adaptive Intelligence: By connecting to a Weather API, the system can predict rain. If rain is forecasted, it cancels the scheduled irrigation to save water and prevent soil erosion.<br>

#Cloud Data Synchronization: Uses Firebase as a real-time database to log temperature, humidity, and soil moisture levels, allowing for remote monitoring.<br>
![Image](https://github.com/user-attachments/assets/fbfad5b5-4aaf-4aa1-a064-936cc51762b1)
#Self-Optimizing Output: The ESP32 processes sensor data locally to decide the exact duration the relay (water pump) should stay active, making it a fully autonomous unit.<br>
#Hardware requirements<br>
#ESP32 MicrocontrollerThe brain of the project; handles Wi-Fi, sensors, and logic.<br>
#Soil Moisture SensorMeasures the volumetric water content in the soil.<br>
#DHT11 SensorMonitors ambient Temperature and Humidity.<br>
#Rain Detection SensorProvides instant alerts for precipitation on-site.<br>
#Relay Module (5V/12V)Acts as an electronic switch to turn the Water Pump ON/OFF.<br>
#Jumper Wires & PowerFacilitates connections and provides stable voltage to the circuit<br>

Software & Tech Stack<br>
Firmware: C++ (using the Arduino IDE).<br>

Web Dashboard: HTML and CSS (for the user interface and data visualization).<br>

Database: Firebase Realtime Database (using the Firebase ESP Client library).<br>

External Integration: OpenWeatherMap API<br>

4. System Architecture (Workflow)<br>
Sensing: The sensors collect raw analog and digital data (Moisture, Temp, Humidity).<br>

Processing: The ESP32 connects to Wi-Fi and fetches the local weather forecast via an API.<br>

5. Usage & Applications
Precision Agriculture: Optimizing water usage for large-scale farms with varied crop types.

Urban Gardening: Automated plant care for homeowners and rooftop gardens.

Smart Greenhouses: Maintaining a strict micro-climate for sensitive botanical species.

Decision Making:<br>

If Soil Moisture < Threshold AND Forecast != Rain → Pump ON.<br>

If Soil Moisture > Threshold OR Forecast == Rain → Pump OFF.<br>

Cloud Sync: All parameters are pushed to Firebase in real-time.<br>

Output: The Relay Module triggers the water pump according to the crop category’s specific needs.<br>
![Image](https://github.com/user-attachments/assets/843405df-7770-4068-8953-c3524eeafa7c)
5. Usage & Applications<br>
Precision Agriculture: Optimizing water usage for large-scale farms with varied crop types.<br>

Urban Gardening: Automated plant care for homeowners and rooftop gardens.<br>

Smart Greenhouses: Maintaining a strict micro-climate for sensitive botanical species.<br>
