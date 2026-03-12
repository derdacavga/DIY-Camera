#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TJpg_Decoder.h>
#include <EEPROM.h>
 
#define USE_FLASH true  // Set to true to enable flash, false to disable
#define FLASH_PIN 4   
#define EEPROM_SIZE 4 
 
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define BUTTON_PIN 3  

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int photoCount = 0;
 
bool oled_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      uint16_t color = bitmap[j * w + i];
 
      uint8_t r = (color & 0xF800) >> 8;
      uint8_t g = (color & 0x07E0) >> 3;
      uint8_t b = (color & 0x001F) << 3;
 
      uint8_t gray = (r * 77 + g * 150 + b * 29) >> 8;
 
      if (gray > 100) {
        display.drawPixel(x + i, y + j, SSD1306_WHITE);
      } else {
        display.drawPixel(x + i, y + j, SSD1306_BLACK);
      }
    }
  }
  return 1;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
 
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW);
 
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, photoCount);
  
  if (photoCount < 0 || photoCount > 99999) {
    photoCount = 0;
  }
 
  Wire.begin(13, 1); // SDA=13, SCL=TX
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1)
      ;  
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("System Booting...");
  display.print("Saved Photos: ");
  display.println(photoCount);  
  display.display();
  delay(1500);  
 
  if (!SD_MMC.begin("/sdcard", true)) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("SD CARD ERROR!");
    display.display();
    while (1)
      ;
  }
 
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  if (esp_camera_init(&config) != ESP_OK) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.println("CAMERA ERROR!");
    display.display();
    while (1)
      ;
  }
 
  sensor_t* s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_HQVGA);

  TJpgDec.setJpgScale(2);
  TJpgDec.setCallback(oled_output);

  display.clearDisplay();
}

void takePhoto() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 30);
  display.println("CAPTURING...");
  display.display();

  if (USE_FLASH) {
    digitalWrite(FLASH_PIN, HIGH);
    delay(800);
  }

  sensor_t* s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_UXGA);
  delay(500);

  camera_fb_t* fb = esp_camera_fb_get();
  if (fb) esp_camera_fb_return(fb);
  fb = esp_camera_fb_get();
  if (fb) esp_camera_fb_return(fb);

  fb = esp_camera_fb_get();

  if (USE_FLASH) {
    digitalWrite(FLASH_PIN, LOW);
  }

  if (!fb) {
    display.clearDisplay();
    display.setCursor(10, 30);
    display.println("CAPTURE FAILED!");
    display.display();
  } else {

    photoCount++;
    String path = "/PHOTO_" + String(photoCount) + ".jpg";

    fs::FS& fs = SD_MMC;
    File file = fs.open(path.c_str(), FILE_WRITE);
    if (file) {
      file.write(fb->buf, fb->len);
      file.close();
 
      EEPROM.put(0, photoCount);
      EEPROM.commit();  

      display.clearDisplay();
      display.setCursor(20, 30);
      display.println("SAVED!");
      display.setCursor(20, 45);
      display.print("Count: ");
      display.println(photoCount);
      display.display();
    } else {
      display.clearDisplay();
      display.setCursor(10, 30);
      display.println("SD WRITE ERROR!");
 
      photoCount--;

      display.display();
    }
    esp_camera_fb_return(fb);
  }

  delay(1500);
  s->set_framesize(s, FRAMESIZE_HQVGA);
}

void loop() { 
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50);
    if (digitalRead(BUTTON_PIN) == LOW) {
      takePhoto();
      while (digitalRead(BUTTON_PIN) == LOW)
        ;
    }
  }
 
  camera_fb_t* fb = esp_camera_fb_get();
  if (fb) {
    display.clearDisplay();
 
    TJpgDec.drawJpg(4, -12, (const uint8_t*)fb->buf, fb->len);
 
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setCursor(0, 0);
    display.print("[");
    display.print(photoCount);
    display.print("]");
 
    display.display();

    esp_camera_fb_return(fb);
  }
}