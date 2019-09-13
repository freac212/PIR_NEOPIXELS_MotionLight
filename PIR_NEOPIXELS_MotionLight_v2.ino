#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN        6
#define NUMPIXELS 30
#define PIR_PIN 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int LIGHTUP_DELAY = 100; // in milliseconds
int FADEOUT_DELAY_MULTIPLIER = 0.03; // Between 0.02 and 0.05 seems to be a good range.
int FADEIN_DELAY_MULTIPLIER = .8; // 2 seconds is too high, and 0.5 is kinda too low.
int pirVal = 0;
long previousTime = 0;
long interval = 30000;
bool delayIsActive = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Starting Device...");
  pixels.begin();
  pixels.show();
}


void loop() {
  pirVal = digitalRead(PIR_PIN);

  Serial.print("Pir val: ");
  Serial.println(pirVal);
  Serial.println();
  
  // If movement is detected, wait *interval* before checking again.
  if(pirVal > 0 && !delayIsActive){
      Serial.println("===========================");
      Serial.println("Movement Detected!");
      Serial.println("Fading Neopixels IN...");
      fadeIn();
      Serial.println("===========================");
      delayIsActive = true;
      previousTime = millis();
  } else if (pirVal > 0 && delayIsActive){
      // If there's still movement, reset timer..
      Serial.println("===========================");
      Serial.println("More movement detected!");
      Serial.println("Reseting timer...");
      Serial.println("===========================");
      previousTime = millis();
  }
  
  unsigned long currentTime = millis();
  if (currentTime - previousTime > interval && delayIsActive)
  {
    previousTime = currentTime;
    delayIsActive = false;
    fadeOut();
    Serial.println("===========================");
    Serial.println("Drawing BLANK...");
    Serial.println("===========================");
  }
}

void drawWhite(){
  pixels.fill(pixels.Color(255, 255, 255), 0, pixels.numPixels());
  pixels.show();
}

void fadeOut(){
  for(int colour = 255; colour > 0; colour--){ 
    pixels.fill(pixels.Color(colour, colour, colour), 0, pixels.numPixels());
    pixels.show();
    delay(colour * FADEOUT_DELAY_MULTIPLIER);
  }
  pixels.clear();
  pixels.show();
}

void fadeIn(){
  for(int colour = 1; colour <= 255; colour++){ 
    pixels.fill(pixels.Color(colour, colour, colour), 0, pixels.numPixels());
    pixels.show();
    delay(colour * FADEIN_DELAY_MULTIPLIER);
  }
}

void drawBlank(){
    pixels.clear();
    pixels.show();
}
