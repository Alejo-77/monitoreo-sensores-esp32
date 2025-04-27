#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// WiFi
const char* ssid = "Hern@ndez";
const char* password = "Alejandro7";

// MongoDB Atlas (Node.js Server)
const char* serverUrl = "mongodb+srv://ah773080:<db_password>@cluster0.z9gurx2.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0";  

// Sensores
#define LDR_PIN 34     // Pin del LDR (ADC)
#define DHT_PIN 4      // Pin del DHT11/DHT22
#define RCWL_PIN 5     // Pin del RCWL-0516
#define DHT_TYPE DHT11 // Cambia a DHT22 si es necesario

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  pinMode(RCWL_PIN, INPUT);
  dht.begin();
  connectWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Leer sensores
    int luz = analogRead(LDR_PIN);
    float temp = dht.readTemperature();
    float humedad = dht.readHumidity();
    bool movimiento = digitalRead(RCWL_PIN);

    // Enviar datos al servidor
    sendToServer(luz, temp, humedad, movimiento);
  }
  delay(5000); // Espera 5 segundos
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando al WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado. IP: " + WiFi.localIP());
}

void sendToServer(int luz, float temp, float humedad, bool movimiento) {
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  String jsonData = String("{\"luz\":") + luz + 
                   ",\"temp\":" + temp + 
                   ",\"humedad\":" + humedad + 
                   ",\"movimiento\":" + (movimiento ? "true" : "false") + "}";

  int httpCode = http.POST(jsonData);
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Datos enviados al servidor.");
  } else {
    Serial.println("Error: " + http.errorToString(httpCode));
  }
  http.end();
}
