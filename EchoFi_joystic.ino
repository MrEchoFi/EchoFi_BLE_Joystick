#include <BleGamepad.h>
#include <U8g2lib.h>
#include <Wire.h>

// ====== CONFIGURABLE AXIS INVERSION ======
// Set to true to flip (invert) that axis’ direction
const bool INV_LX = true;  // Left stick  X
const bool INV_LY = false;  // Left stick  Y
const bool INV_RX = false;  // Right stick X
const bool INV_RY = true;  // Right stick Y

//Invert an axis: change the corresponding INV_… constant at the top:
//const bool INV_LX = true;  // flip left-stick X
//onst bool INV_RY = true;  // flip right-stick Y

// ====== PINS ======
const int JOY1X = 32, JOY1Y = 33, JOY1SW = 25;
const int JOY2X = 34, JOY2Y = 35, JOY2SW = 26;
const int BTN_A = 27, BTN_B = 14;
const int BATT  = 36; // voltage divider midpoint

// ====== OLED ======
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0, /* SCL=*/22, /* SDA=*/21, /* RST=*/ U8X8_PIN_NONE
);

// ====== BLE ======
BleGamepad bleGamepad("EchoFi_BLE_Joystick", "le-777", 100);

// ====== BATTERY CAL ======
const float VDIV   = 2.0;    // voltage divider ratio
const float ADCMAX = 4095.0; // 12-bit ADC max
const float VREF   = 3.3;    // ADC reference voltage

void setup() {
  Serial.begin(115200);

  // Configure buttons with pull-ups
  for (int p : {JOY1SW, JOY2SW, BTN_A, BTN_B})
    pinMode(p, INPUT_PULLUP);

  // Initialize OLED
  u8g2.begin();
  startupAnimation();

  // Initialize BLE Gamepad
  bleGamepad.begin();
}

void loop() {
  // Wait for BLE connection
  if (!bleGamepad.isConnected()) {
    showStatus("Waiting\nfor BLE...");
    delay(200);
    return;
  }

  // --- Read raw analog values ---
  int rawLX = analogRead(JOY1X),
      rawLY = analogRead(JOY1Y),
      rawRX = analogRead(JOY2X),
      rawRY = analogRead(JOY2Y);

  // --- Map to 0–255 ---
  uint8_t lx = map(rawLX, 0, 4095, 0, 255);
  uint8_t ly = map(rawLY, 0, 4095, 0, 255);
  uint8_t rx = map(rawRX, 0, 4095, 0, 255);
  uint8_t ry = map(rawRY, 0, 4095, 0, 255);

  // --- Apply inversion if configured ---
  if (INV_LX) lx = 255 - lx;
  if (INV_LY) ly = 255 - ly;
  if (INV_RX) rx = 255 - rx;
  if (INV_RY) ry = 255 - ry;

  // Send axes over BLE
  bleGamepad.setAxes(lx, ly, rx, ry);

  // Handle buttons (press/release)
  handleButton(1, JOY1SW);
  handleButton(2, JOY2SW);
  handleButton(3, BTN_A);
  handleButton(4, BTN_B);

  // --- Measure battery voltage and % ---
  float volts = analogRead(BATT) / ADCMAX * VREF * VDIV;
  int pct = constrain(int((volts - 3.0) / (4.2 - 3.0) * 100), 0, 100);

  // --- Draw to OLED ---
  u8g2.clearBuffer();
  drawBattery(pct);
  drawJoysticks(lx, ly, rx, ry);
  u8g2.sendBuffer();

  delay(30);  // ~33 Hz refresh
}

// ——— HELPER FUNCTIONS ———

void handleButton(uint8_t btn, int pin) {
  bool pressed = (digitalRead(pin) == LOW);
  if (pressed) bleGamepad.press(btn);
  else         bleGamepad.release(btn);
}

void startupAnimation() {
  for (int x = 0; x < 128; x += 8) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(x, 32, "EchoFi_BLE_Joystick V1  by Mr.EchoFi");
    u8g2.sendBuffer();
    delay(100);
  }
  delay(300);
}

void showStatus(const char* msg) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 30, msg);
  u8g2.sendBuffer();
}

void drawBattery(int pct) {
  int w = map(pct, 0, 100, 0, 30);
  u8g2.drawFrame(0, 0, 34, 12);
  u8g2.drawBox(34, 3, 3, 6);
  u8g2.drawBox(2, 2, w, 8);
  u8g2.setFont(u8g2_font_6x10_tf);
  char buf[6];
  sprintf(buf, "%d%%", pct);
  u8g2.drawStr(40, 10, buf);
}

void drawJoysticks(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry) {
  // Left stick box + crosshair
  u8g2.drawFrame(0, 16, 56, 48);
  int cx1 = map(lx, 0, 255, 1, 54);
  int cy1 = map(ly, 0, 255, 63, 18);
  u8g2.drawLine(cx1 - 5, cy1, cx1 + 5, cy1);
  u8g2.drawLine(cx1, cy1 - 5, cx1, cy1 + 5);

  // Right stick box + crosshair
  u8g2.drawFrame(64, 16, 56, 48);
  int cx2 = map(rx, 0, 255, 65, 118);
  int cy2 = map(ry, 0, 255, 63, 18);
  u8g2.drawLine(cx2 - 5, cy2, cx2 + 5, cy2);
  u8g2.drawLine(cx2, cy2 - 5, cx2, cy2 + 5);
}
