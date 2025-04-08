#include "User_Interface.hpp"

User_Interface::User_Interface() {}

User_Interface::~User_Interface() {}

bool User_Interface::init(TwoWire &I2C_Bus) {
  display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_Bus, OLED_RESET);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }

  return true;
}

void User_Interface::begin() {
  // Clear the buffer
  display.clearDisplay();
  display.display();

  drawIntroPage();

  drawPageBar(false, false, false, false, false, 0, true);
  drawForce(250.0f);
}

void User_Interface::drawForce(float forceInput) {
  static uint8_t fontSize = 2;
  static uint8_t numDigits = 9;

  static int16_t cursorX = ((SCREEN_WIDTH - (numDigits * fontSize * 6)) / 2);
  static int16_t cursorY =
      ((SCREEN_HEIGHT - 20) / 2) + 20 - ((fontSize - 1) * 8);

  display.fillRect(0, 20, SCREEN_WIDTH, (SCREEN_HEIGHT - 20), SSD1306_BLACK);
  display.setTextSize(fontSize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(cursorX, cursorY);

  int16_t force =
      static_cast<int16_t>(round(constrain(forceInput, -20000, 20000)));

  char buf[6];
  sprintf(buf, "%05d", abs(force));

  display.print("F=");  // best accuracy is about 10N, 0.05% FS (20kN)
  display.print(force < 0 ? "-" : "+");
  display.print(buf);
  display.println("N");

  display.display();
}

void User_Interface::drawPageBar(bool cell, bool sd, bool rf, bool armed,
                                 bool ready, float battPer,
                                 bool forceUpdateAll) {
  // Update Cell icon if changed
  if ((cell != m_mainPageStatus.cell) || forceUpdateAll) {
    display.fillRect(0, 0, 16, 16, SSD1306_BLACK);
    drawBitMap(cell ? load_cell_on : load_cell_off, 0, 0);
    m_mainPageStatus.cell = cell;
  }

  // Update SD icon if changed
  if ((sd != m_mainPageStatus.sd) || forceUpdateAll) {
    display.fillRect(16, 0, 16, 16, SSD1306_BLACK);
    drawBitMap(sd ? SD_on : SD_off, 16, 0);
    m_mainPageStatus.sd = sd;
  }

  // Update RF icon if changed
  if ((rf != m_mainPageStatus.rf) || forceUpdateAll) {
    display.fillRect(32, 0, 16, 16, SSD1306_BLACK);
    drawBitMap(rf ? RF_on : RF_off, 32, 0);
    m_mainPageStatus.rf = rf;
  }

  // Update Armed icon if changed
  if ((armed != m_mainPageStatus.armed) || forceUpdateAll) {
    display.fillRect(48, 0, 16, 16, SSD1306_BLACK);
    drawBitMap(armed ? armed_on : armed_off, 48, 0);
    m_mainPageStatus.armed = armed;
  }

  // Update Ready icon if changed
  if ((ready != m_mainPageStatus.ready) || forceUpdateAll) {
    display.fillRect(64, 0, 16, 16, SSD1306_BLACK);
    drawBitMap(ready ? ready_on : ready_off, 64, 0);
    m_mainPageStatus.ready = ready;
  }

  // Update Battery icon if the displayed icon (rounded to nearest 10) is
  // different
  if ((((int)(battPer + 5) / 10) !=
       ((int)(m_mainPageStatus.battPer + 5) / 10)) ||
      forceUpdateAll) {
    // Determine battery icon based on battPer
    Bitmap newBattIcon;
    if (battPer < 10)
      newBattIcon = batt_0;
    else if (battPer < 20)
      newBattIcon = batt_10;
    else if (battPer < 30)
      newBattIcon = batt_20;
    else if (battPer < 40)
      newBattIcon = batt_30;
    else if (battPer < 50)
      newBattIcon = batt_40;
    else if (battPer < 60)
      newBattIcon = batt_50;
    else if (battPer < 70)
      newBattIcon = batt_60;
    else if (battPer < 80)
      newBattIcon = batt_70;
    else if (battPer < 90)
      newBattIcon = batt_80;
    else if (battPer < 100)
      newBattIcon = batt_90;
    else
      newBattIcon = batt_100;

    display.fillRect(80, 0, display.width() - 80, 16, SSD1306_BLACK);
    drawBitMap(newBattIcon, 80, 0);
    m_mainPageStatus.battPer = battPer;
  }

  if (forceUpdateAll) {
    display.fillRect(0, 17, display.width(), 2, SSD1306_BLACK);
    display.fillRect(0, 17, display.width(), 2, SSD1306_WHITE);
  }

  // Finally, update the display with all drawn changes.
  display.display();
}

void User_Interface::drawIntroPage() {
  display.clearDisplay();

  display.drawBitmap((display.width() - logo.width) / 2,
                     (display.height() - logo.height) / 2, logo.bmp, logo.width,
                     logo.height, 1);

  display.display();

  for (int i = 0; i < 6; i++) {
    vTaskDelay(pdMS_TO_TICKS(100));
    display.invertDisplay(true);
    vTaskDelay(pdMS_TO_TICKS(100));
    display.invertDisplay(false);
  }
  vTaskDelay(pdMS_TO_TICKS(1000));
  display.clearDisplay();
  display.display();
}

void User_Interface::drawBitMap(Bitmap data, int16_t x, int16_t y) {
  if (x == -1) {
    x = (display.width() - data.width) / 2;
  }

  if (y == -1) {
    y = (display.height() - data.height) / 2;
  }

  display.drawBitmap(x, y, data.bmp, data.width, data.height, 1);
  display.display();
}

void User_Interface::showSuccess(String msg) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(msg + " Success!");

  display.display();
  vTaskDelay(pdMS_TO_TICKS(500));
  display.clearDisplay();
  display.display();
}

void User_Interface::showError(String msg, uint16_t duration_ms) {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(msg);

  display.display();
  vTaskDelay(pdMS_TO_TICKS(duration_ms));
  display.clearDisplay();
  display.display();
}