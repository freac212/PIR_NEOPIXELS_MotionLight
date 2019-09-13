#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN        6
#define NUMPIXELS 30
#define PIR_PIN 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int LIGHTUP_DELAY = 100; // in milliseconds
int FADEOUT_DELAY = 10;
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
      Serial.println("Drawing WHITE...");
      drawWhite();
      Serial.println("===========================");
      delayIsActive = true;
      previousTime = millis();
  } else if (pirVal > 0 && delayIsActive){
      // If there's still movement, reset timer..
      previousTime = millis();
  }
  
  unsigned long currentTime = millis();
  if (currentTime - previousTime > interval && delayIsActive)
  {
    previousTime = currentTime;
    delayIsActive = false;
    drawBlank();
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
    for(int i; i<NUMPIXELS; i++){ //                  R,  G,  B
      pixels.setPixelColor(i, pixels.Color(colour,colour,colour)); 
      pixels.show();
    }
  }
}

void drawBlank(){
    pixels.clear();
    pixels.show();
}
