#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Arduino_ST7789_Fast.h>

// Define TFT pins and screen dimensions
#define TFT_DC    7
#define TFT_RST   8
#define SCR_WD   240
#define SCR_HT   240

// Define soil moisture sensor range
const int dry = 595; // Value for dry sensor
const int wet = 0; // Value for wet sensor

// Define relay pin
#define RELAY_PIN  2

// Initialize the TFT display
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST);

// Variables to store humidity values
int previousHumidity = -1;
int humidity = 0;
int minHumidity = 100;
int maxHumidity = 0;

#define HUMIDITY_X  20
#define HUMIDITY_Y  85

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up ...");

  // Initialize TFT display
  tft.init(SCR_WD, SCR_HT);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(RED);  
  tft.setTextSize(3);
  tft.println(" Humidity");
  tft.setCursor(160, HUMIDITY_Y);
  tft.setTextSize(5);
  tft.setTextColor(WHITE);  
  tft.println("%");

  tft.setCursor(40, 180);
  tft.setTextSize(2);
  tft.setTextColor(CYAN); 
  tft.println("MIN");
  printMinHumiditySymbol();
  printMaxHumiditySymbol();

  tft.setCursor(170, 180);
  tft.setTextSize(2);
  tft.setTextColor(RED); 
  tft.println("MAX");

  // Initialize relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Make sure relay is off at start
}

void loop() {
  delay(1000);
  
  // Read soil moisture sensor value
  int sensorVal = analogRead(A0);

  // Map sensor value to percentage humidity
  previousHumidity = humidity;
  humidity = map(sensorVal, wet, dry, 100, 0);
  
  if(humidity < minHumidity) {
    deleteMinHumidity();
    minHumidity = humidity; 
  }
  if(humidity > maxHumidity) {
    deleteMaxHumidity();
    maxHumidity = humidity; 
  }
  if(previousHumidity != humidity) {
    deletePreviousHumidity();
    printHumidity();
    printMinHumidity();
    printMaxHumidity();
  }

  // Control relay based on humidity level
  if(humidity <= 30) {
    digitalWrite(RELAY_PIN, HIGH); // Turn on relay
  } else {
    digitalWrite(RELAY_PIN, LOW); // Turn off relay
  }

  Serial.print(humidity);
  Serial.println("%");
}

void deletePreviousHumidity() {
  tft.setCursor(HUMIDITY_X, HUMIDITY_Y);
  tft.setTextSize(5);
  tft.setTextColor(BLACK);
  tft.println(previousHumidity);
}

void printHumidity() {
  tft.setCursor(HUMIDITY_X, HUMIDITY_Y);
  tft.setTextSize(5);
  tft.setTextColor(WHITE);
  tft.println(humidity);
}

void printMinHumidity() {
  tft.setCursor(10, 210);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println(minHumidity);
}

void printMaxHumidity() {
  tft.setCursor(150, 210);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.println(maxHumidity);
}

void deleteMaxHumidity() {
  tft.setCursor(150, 210);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.println(maxHumidity); 
}

void deleteMinHumidity() {
  tft.setCursor(10, 210);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.println(minHumidity);
}

void printMinHumiditySymbol() {
  tft.setCursor(70, 210);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("%");
}

void printMaxHumiditySymbol() {
  tft.setCursor(210, 210);
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.println("%");
}
