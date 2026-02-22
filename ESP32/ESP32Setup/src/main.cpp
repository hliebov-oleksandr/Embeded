#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENCODER_BUTTON D6
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_DC    D3   // D3 GPIO0
#define OLED_CS    D8  // D8 GPIO15
#define OLED_RESET D4   // D4 GPIO2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);
bool isButtonPressed(int pin);
void displayInfo(const char* inEndInfo);

void setup() {
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  displayInfo("");
}

void displayInfo(const char* inEndInfo) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("FM Radio");
  display.setTextSize(2);
  display.println("Station: 101.2 MHz");

  display.display();
}

bool isButtonPressed(int pin) {
  return digitalRead(pin) == LOW ? delay(5), digitalRead(pin) == LOW : false;
}

void loop() {

  if (isButtonPressed(ENCODER_BUTTON)) {
    Serial.println("Button Pressed");
    displayInfo("Button is Printed");
  }
}
