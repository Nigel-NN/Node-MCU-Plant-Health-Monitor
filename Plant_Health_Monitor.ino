#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Nigel";
const char* password = "ikrni93715";

ESP8266WebServer server(80);  // Create a webserver object that listens for HTTP request on port 80

const int MOISTURE_SENSOR_PIN = A0;
const int LDR_SENSOR_PIN = D0;
const int DHT_PIN = D2;  // Change this to the pin you've connected the DHT sensor to
#define DHT_TYPE DHT11  // Change this to DHT22 if you're using a DHT22 sensor
const int LED_PIN = D1;  // Change this to the pin you've connected the LED to

DHT dht(DHT_PIN, DHT_TYPE);

// Define the initial moisture and light level thresholds
int moistureThresholdLow = 300;
int moistureThresholdHigh = 700;
int lightThresholdLow = 300;
int lightThresholdHigh = 700;

void setup(void){
  Serial.begin(9600);
  WiFi.begin(ssid, password);  // Connect to the network
  Serial.println("");

  // Wait for the Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());   // Send the IP address of the ESP8266 to the computer

  server.on("/", HTTP_GET, handleRoot);  // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/setThresholds", HTTP_POST, setThresholds);  // Call the 'setThresholds' function when a client sends a POST request to URI "/setThresholds"
  
  server.begin();  // Actually start the server
  Serial.println("HTTP server started");

  dht.begin();

  pinMode(LED_PIN, OUTPUT);  // Set the LED pin as output
}

void loop(void){
  server.handleClient();  // Handle a new client request

  // Blink the LED in a heartbeat pattern
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(1700);
}

String interpretMoisture(int moisture) {
  if (moisture < moistureThresholdLow) {
    return "Underwatered";
  } else if (moisture > moistureThresholdHigh) {
    return "Overwatered";
  } else {
    return "Good";
  }
}

String interpretLight(int light) {
  if (light < lightThresholdLow) {
    return "Too dark";
  } else if (light > lightThresholdHigh) {
    return "Too bright";
  } else {
    return "Perfect";
  }
}

void handleRoot() {
  int moisture = analogRead(MOISTURE_SENSOR_PIN);
  int light = analogRead(LDR_SENSOR_PIN);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  String html = "<html>\
<head>\
  <title>Plant Health Monitor</title>\
  <style>\
    body { font-family: Arial, Helvetica, sans-serif; background-color: #f0f0f0; }\
    h1 { color: #333; }\
    p { color: #666; }\
  </style>\
  <meta http-equiv='refresh' content='5'>\
  <script>\
    function setThresholds() {\
      var moistureLow = document.getElementById('moistureLow').value;\
      var moistureHigh = document.getElementById('moistureHigh').value;\
      var lightLow = document.getElementById('lightLow').value;\
      var lightHigh = document.getElementById('lightHigh').value;\
      var xhr = new XMLHttpRequest();\
      xhr.open('POST', '/setThresholds', true);\
      xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');\
      xhr.send('moistureLow=' + moistureLow + '&moistureHigh=' + moistureHigh + '&lightLow=' + lightLow + '&lightHigh=' + lightHigh);\
    }\
  </script>\
</head>\
<body>\
  <h1>Plant Health Monitor</h1>\
  <p>Moisture: " + interpretMoisture(moisture) + "</p>\
  <p>Light: " + interpretLight(light) + "</p>\
  <p>Humidity: " + String(humidity) + "%</p>\
  <p>Temperature: " + String(temperature) + "°C</p>\
  <p>\
    <label for='moistureLow'>Moisture Low Threshold:</label>\
    <input type='range' id='moistureLow' name='moistureLow' min='0' max='1023' value='" + String(moistureThresholdLow) + "' onchange='setThresholds()'>\
  </p>\
  <p>\
    <label for='moistureHigh'>Moisture High Threshold:</label>\
    <input type='range' id='moistureHigh' name='moistureHigh' min='0' max='1023' value='" + String(moistureThresholdHigh) + "' onchange='setThresholds()'>\
  </p>\
  <p>\
    <label for='lightLow'>Light Low Threshold:</label>\
    <input type='range' id='lightLow' name='lightLow' min='0' max='1023' value='" + String(lightThresholdLow) + "' onchange='setThresholds()'>\
  </p>\
  <p>\
    <label for='lightHigh'>Light High Threshold:</label>\
    <input type='range' id='lightHigh' name='lightHigh' min='0' max='1023' value='" + String(lightThresholdHigh) + "' onchange='setThresholds()'>\
  </p>\
</body>\
</html>";

  server.send(200, "text/html", html);
}

void setThresholds() {
  if (server.hasArg("moistureLow") && server.hasArg("moistureHigh") && server.hasArg("lightLow") && server.hasArg("lightHigh")) {
    moistureThresholdLow = server.arg("moistureLow").toInt();
    moistureThresholdHigh = server.arg("moistureHigh").toInt();
    lightThresholdLow = server.arg("lightLow").toInt();
    lightThresholdHigh = server.arg("lightHigh").toInt();
  }
  server.send(200, "text/plain", "Thresholds updated");
}
