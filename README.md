# DIY-Camera
In this project I will share with you Esp32-Cam based DIY Camera. It can taking photo after save in SD Card. Also it has photo count

## Video Tutorial 
[![V1](https://img.youtube.com/vi/QXY3eQCvIlQ/0.jpg)](https://youtu.be/QXY3eQCvIlQ)

# 📸 DIY Retro Digital Camera (ESP32-CAM & OLED)

Build your own retro-style digital camera for under $10! This project transforms a simple **ESP32-CAM** into a portable, lo-fi camera inspired by the classic Game Boy Camera aesthetic. It features a grayscale live preview on an OLED screen and saves high-resolution photos directly to a microSD card.

> Don't forget to **Subscribe** to the [Channel](https://www.google.com/search?q=https://www.youtube.com/%40dsnindustries/%3Fsub_confirmation%3D1) and **Like** the video! 
> 
> 

---

## ✨ Features

* **100% DIY & Low Cost:** Built using affordable, off-the-shelf components.
* **Retro Aesthetic:** Monochrome live preview for a classic "Game Boy Camera" look.
* **Photo Storage:** Automatically saves captured images to a microSD card (formatted to FAT32).
* **Persistent Counter:** Remembers your photo count even after a reboot using **EEPROM**.
* **Built-in Flash:** Includes a toggleable flash for high-quality low-light captures.
* **Portable Design:** Integrated LiPo battery support for on-the-go photography.

---

## ⚙️ Hardware Requirements

The project uses **I2C communication** for the display to save precious GPIO pins for the SD card and shutter button.

| Component | Details | Note |
| --- | --- | --- |
| **Development Board** | **ESP32-CAM** (AI-Thinker) | The core microcontroller and camera module. |
| **Display** | 0.96" SSD1306 OLED (I2C) | Provides the monochrome live preview and photo count.|
| **Storage** | MicroSD Card | <br>**Required:** Must be formatted to **FAT32**. |
| **Shutter** | Tactile Pushbutton | Used to trigger the camera and save photos. |
| **Power (Portable)** | LiPo Battery & TP4056 | Includes a charger board and voltage booster (set to 5V). |
| **Case** | 3D Printed Enclosure | Custom design created in Fusion 360. |

---

## 💻 Arduino Firmware Setup

1. **Install Libraries:** Use the Arduino Library Manager to install:
* `Adafruit GFX` & `Adafruit SSD1306` 
* `JPEGDecoder` 
* `TFT_eSPI` (Only if using the optional ST7789 color screen) 

2. **Code Configuration:**
* Choose the "DIY Camera" script from the repository.
* (Optional) Adjust the code to set your flash preference (ON/OFF).

3. **ESP32 Board Settings:**
* Select **AI Thinker ESP32-CAM** as your board.
* Ensure the correct COM port is selected.

4. **Upload Mode:**
* Short circuit **GPIO 0** to **GND** and press **Reset** to enter boot mode.
* Click **Upload** in the Arduino IDE.

---

## ▶️ How to Use

* **Boot Up:** Power the device; the display will show the live grayscale stream and photo count.
* **Aim:** Use the OLED screen as your viewfinder to frame your shot.
* **Shoot:** Press the shutter button. The flash will fire, and the screen will show a "Taking Photo" and "Saved" feedback.
* **Review:** Remove the SD card and plug it into your PC to view your retro-style photos.

---

## 📂 Files & Links

* **3D Models (STL/CAD):** [Available on my Cults3D Page](https://www.google.com/search?q=https://cults3d.com/en/users/DsnDesign) 
* **Custom Design Services:** [Reach out on Fiverr](https://www.google.com/search?q=https://www.fiverr.com/derdacavga) 

## My Mobile Games

* **Maze Escape:** [Google Play Store](https://play.google.com/store/apps/details?id=com.DsnMechanics.MazeEscape)
* **Air Hockey:** [Google Play Store](https://play.google.com/store/apps/details?id=com.DsnMechanics.AirHockey)
* **Click Challenge:** [Google Play Store](https://play.google.com/store/apps/details?id=com.DsNMechanics.ClickChallenge)
* **SkyScrapper:** [Google Play Store](https://play.google.com/store/apps/details?id=com.DsnMechanics.SkyScraper)
* **Flying Triangels:** [Google Play Store](https://play.google.com/store/apps/details?id=com.DsnMechanics.Triangle)
