#include <Adafruit_NeoPixel.h>

#define PIN 5         
#define NUMPIXELS 8    

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setColor(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void setup() {
  Serial.begin(115200);
  pixels.begin();
  setColor(0, 0, 0);
  Serial.println("Tomato LED system ready. Type: ripe, unripe, rotten");
}

void loop() {
  if (Serial.available()) {
    String prediction = Serial.readStringUntil('\n');
    prediction.trim();
    prediction.toLowerCase();

    if (prediction == "ripe") {
      setColor(0, 255, 0);       // green
      Serial.println("Prediction: ripe -> GREEN");
    }
    else if (prediction == "unripe") {
      setColor(255, 120, 0);     // amber
      Serial.println("Prediction: unripe -> AMBER");
    }
    else if (prediction == "rotten") {
      setColor(255, 0, 0);       // red
      Serial.println("Prediction: rotten -> RED");
    }
    else {
      setColor(0, 0, 255);       // blue = unknown/error
      Serial.println("Unknown command");
    }
  }
}