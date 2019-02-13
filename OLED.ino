#ifdef ENABLEOLED

  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  
  #define SCREEN_WIDTH 128 // OLED display width, in pixels
  #define SCREEN_HEIGHT 64 // OLED display height, in pixels
  
  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  #define OLED_RESET     LED_BUILTIN // Reset pin # (or -1 if sharing Arduino reset pin)
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  
  bool initOLED() {
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
      Serial.println("SSD1306 allocation failed");
      return false;
    }
    return true;
  }

  void drawOLED(String weather) {
    display.clearDisplay();
    if (weather == "Snow") {
        display.drawBitmap(0, 0, snow, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    } else if (weather == "Rain") {
        display.drawBitmap(0, 0, rain, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    } else if (weather == "Mist") {
        display.drawBitmap(0, 0, mist, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    } else {
        display.clearDisplay();
    }
    display.display();
  }

#endif
