<div align="center">

  <img src="https://github.com/MrEchoFi/MrEchoFi/raw/4274f537dec313ac7dde4403fe0fae24259beade/Mr.EchoFi-New-Logo-with-ASCII.jpg" alt="logo" width="200" height="auto" />
  <h1>EchoFi_BLE_Joystick</h1>
   
  <p>
   Esp32 based joystick
  </p>


  📫 How to reach me **tanjibisham777@gmail.com & tanjibisham888@gmail.com**
# videos:
https://github.com/user-attachments/assets/2c57e91c-9de5-4f7d-a130-1614504734f5

LOOK-

https://github.com/user-attachments/assets/5e54977b-e49f-47de-897e-68a440d08927


![EchoFi_BLE_Joystick](https://github.com/user-attachments/assets/f3604810-efd3-488c-8952-867db6db4ae4)

# Components: Esp32 V1.3, two PS2 joystick module, SSD1306 1.3inch OLED display, two push buttons, jumper wires, divider [optional].
Note: Battery Voltage Divider (for “battery-life”)
      4.2 V LiPo, use two 100 kΩ in series:
      
      BAT+ ── R1 ──┬── R2 ── GND
             │
         GPIO 36 (ADC1_CH0)
This divides the voltage by 2 so that 0–4.2 V → 0–2.1 V, safe for the ESP32’s ADC.


# Connections:
| Signal         | Module      | ESP32 V1.3 Pin             |
| -------------- | ----------- | -------------------------- |
|   3V3          | All VCC     | 3V3                        |
|   GND          | All GND     | GND                        |
|   Joy1 VRx     | Joystick 1  | GPIO 32 (ADC1\_CH4)        |
| **Joy1 VRy**   | Joystick 1  | GPIO 33 (ADC1\_CH5)        |
| **Joy1 SW**    | Joystick 1  | GPIO 25                    |
| **Joy2 VRx**   | Joystick 2  | GPIO 34 (ADC1\_CH6)        |
| **Joy2 VRy**   | Joystick 2  | GPIO 35 (ADC1\_CH7)        |
| **Joy2 SW**    | Joystick 2  | GPIO 26                    |
| **Btn A**      | Push-Button | GPIO 27                    |
| **Btn B**      | Push-Button | GPIO 14                    |
| **Batt sense** | Divider     | GPIO 36 (ADC1\_CH0)        |
| **OLED SDA**   | SSD1306 I²C | GPIO 21                    |
| **OLED SCL**   | SSD1306 I²C | GPIO 22                    |
| **OLED RST**   | SSD1306 I²C | *–1* (use `U8X8_PIN_NONE`) |
