# Arduino Nano-Based RPM & RPH Tachometer System

## 📖 Overview
This project is a compact tachometer system using an Arduino Nano, a TCRT5000 IR sensor for rotation detection, and a 16x2 I2C LCD for display. It measures and displays RPM and RPH in real time, logs total revolutions using EEPROM, and allows reset via a physical button or serial command. It also includes failure detection and is housed in a custom 3D printed case.

## 🚀 Features
- Real-time RPM and RPH display
- EEPROM-based persistent revolution logging
- Manual and serial-based reset options
- Sensor failure detection logic
- Flicker-free LCD updates
- External power supply via buck converter
- Compact 3D printed case with port cutouts

## 🛠️ Hardware Requirements
- Arduino Nano
- TCRT5000 IR Sensor
- 16x2 LCD Display with I2C Module
- Push Button (Reset)
- LM2596 Buck Converter
- Screw Terminal Block (3-pin)
- Micro USB Cable
- Jumper Wires
- 12V DC Power Supply
- 3D Printed Enclosure

## 🔌 Wiring
| Component     | Arduino Nano Pin |
|---------------|------------------|
| IR Sensor OUT | D2               |
| Reset Button  | D4               |
| LCD SDA       | A4               |
| LCD SCL       | A5               |
| LCD VCC/GND   | 5V / GND         |
| Buck Out +    | VIN              |
| Buck Out -    | GND              |

## 💾 EEPROM Logic
- Stores total revolutions every 30 seconds.
- Reloads total on startup.
- Prevents data loss on power failure.

## 🔧 3D Printed Case Prompt
Design a protective case to fit Arduino Nano, 16x2 LCD with I2C, LM2596 buck converter, and screw terminal. Include: IR sensor wire port (3-jumper width), micro USB port (side), 12V input port (back), reset button port. Add internal mounts, wire guides, and ventilation. Use PLA/PETG.

## 💡 Code Summary
- Interrupt on pin 2 for revolution counting
- Debounce filtering in ISR
- RPM = revCount * 60 per second
- RPH = totalRevs / elapsed hours
- EEPROM saves total every 30s
- LCD only updates on value change

## 📄 Code File
The full Arduino `.ino` file is included in this project and contains:
- EEPROM handling
- LCD update logic
- Serial and button reset
- Pulse timeout for sensor failure detection

## 🗂 File Structure
