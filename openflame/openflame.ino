#include <Adafruit_MAX1704X.h>  // Battery monitor
#include <Adafruit_ST7789.h>    // Display
// #include <Fonts/FreeMono9pt7b.h> // Maybe use another font? (this one is kinda huge, more testing is needed)
#include <Adafruit_MLX90640.h>  // Thermal camera

// Thermal frame
#define FRAME_WIDTH 32
#define FRAME_HEIGHT 24

// Render frame
#define RENDER_WIDTH_SCALE 5
#define RENDER_HEIGHT_SCALE 5

// Colors for displaying thermal image (update to generate colour range on the fly and with set bit-depths)
const uint16_t camColors[] = {0x480F,
  0x400F,0x400F,0x400F,0x4010,0x3810,0x3810,0x3810,0x3810,0x3010,0x3010,
  0x3010,0x2810,0x2810,0x2810,0x2810,0x2010,0x2010,0x2010,0x1810,0x1810,
  0x1811,0x1811,0x1011,0x1011,0x1011,0x0811,0x0811,0x0811,0x0011,0x0011,
  0x0011,0x0011,0x0011,0x0031,0x0031,0x0051,0x0072,0x0072,0x0092,0x00B2,
  0x00B2,0x00D2,0x00F2,0x00F2,0x0112,0x0132,0x0152,0x0152,0x0172,0x0192,
  0x0192,0x01B2,0x01D2,0x01F3,0x01F3,0x0213,0x0233,0x0253,0x0253,0x0273,
  0x0293,0x02B3,0x02D3,0x02D3,0x02F3,0x0313,0x0333,0x0333,0x0353,0x0373,
  0x0394,0x03B4,0x03D4,0x03D4,0x03F4,0x0414,0x0434,0x0454,0x0474,0x0474,
  0x0494,0x04B4,0x04D4,0x04F4,0x0514,0x0534,0x0534,0x0554,0x0554,0x0574,
  0x0574,0x0573,0x0573,0x0573,0x0572,0x0572,0x0572,0x0571,0x0591,0x0591,
  0x0590,0x0590,0x058F,0x058F,0x058F,0x058E,0x05AE,0x05AE,0x05AD,0x05AD,
  0x05AD,0x05AC,0x05AC,0x05AB,0x05CB,0x05CB,0x05CA,0x05CA,0x05CA,0x05C9,
  0x05C9,0x05C8,0x05E8,0x05E8,0x05E7,0x05E7,0x05E6,0x05E6,0x05E6,0x05E5,
  0x05E5,0x0604,0x0604,0x0604,0x0603,0x0603,0x0602,0x0602,0x0601,0x0621,
  0x0621,0x0620,0x0620,0x0620,0x0620,0x0E20,0x0E20,0x0E40,0x1640,0x1640,
  0x1E40,0x1E40,0x2640,0x2640,0x2E40,0x2E60,0x3660,0x3660,0x3E60,0x3E60,
  0x3E60,0x4660,0x4660,0x4E60,0x4E80,0x5680,0x5680,0x5E80,0x5E80,0x6680,
  0x6680,0x6E80,0x6EA0,0x76A0,0x76A0,0x7EA0,0x7EA0,0x86A0,0x86A0,0x8EA0,
  0x8EC0,0x96C0,0x96C0,0x9EC0,0x9EC0,0xA6C0,0xAEC0,0xAEC0,0xB6E0,0xB6E0,
  0xBEE0,0xBEE0,0xC6E0,0xC6E0,0xCEE0,0xCEE0,0xD6E0,0xD700,0xDF00,0xDEE0,
  0xDEC0,0xDEA0,0xDE80,0xDE80,0xE660,0xE640,0xE620,0xE600,0xE5E0,0xE5C0,
  0xE5A0,0xE580,0xE560,0xE540,0xE520,0xE500,0xE4E0,0xE4C0,0xE4A0,0xE480,
  0xE460,0xEC40,0xEC20,0xEC00,0xEBE0,0xEBC0,0xEBA0,0xEB80,0xEB60,0xEB40,
  0xEB20,0xEB00,0xEAE0,0xEAC0,0xEAA0,0xEA80,0xEA60,0xEA40,0xF220,0xF200,
  0xF1E0,0xF1C0,0xF1A0,0xF180,0xF160,0xF140,0xF100,0xF0E0,0xF0C0,0xF0A0,
  0xF080,0xF060,0xF040,0xF020,0xF800,
};

// Misc. objects
enum mode_enum {
  FUNC_UNIT, // Min, max, and crosshair temperature readouts
  FUNC_MIRROR,
  FUNC_MAXTEMP,
  FUNC_MINTEMP,
  FUNC_AUTO,
  FUNC_REFRESH,
  FUNC_RENDER,
};
char mode_strs[8][8] = {
  "UNIT",
  "MIRROR",
  "MAXTEMP",
  "MINTEMP",
  "AUTO",
  "REFRESH",
  "RENDER",
};
int8_t mode_func = FUNC_RENDER;

// Battery objects
float batt_volts;
float batt_percent;
float charge_rate;
Adafruit_MAX17048 batt; // Check out other functions, this PMIC is really cool

// Display objects
float fps = 0;
uint8_t mspf = 0;
GFXcanvas16 therm_frame(FRAME_WIDTH*RENDER_WIDTH_SCALE, FRAME_HEIGHT*RENDER_HEIGHT_SCALE); // Thermal camera frame
GFXcanvas16 stats(240-FRAME_WIDTH*RENDER_WIDTH_SCALE, 135-15); // Status and settings menu
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST); // Display

// Thermal cam objects
int16_t min_temp = 20;
int16_t max_temp = 35;
bool auto_mode = false;
float min_max_pad = 1.05;
float frame[FRAME_WIDTH*FRAME_HEIGHT]; // Thermal frame buffer
char render_strs[2][6] = { "INTLC", "CHESS" };
uint8_t refresh_rates[8] = {0, 1, 2, 4, 8, 16, 32, 64};
Adafruit_MLX90640 mlx; // Thermal camera

// Read button inputs in binary format (0 = released, 1 = pressed)
uint8_t getPressed() {
  return !digitalRead(0)<<0 | digitalRead(1)<<1 | digitalRead(2)<<2;
}

// Updates the battery voltage and percent variables
void getBattery() {
  batt_volts = batt.cellVoltage();
  batt_percent = constrain(batt.cellPercent(), 0., 100.);
  charge_rate = batt.chargeRate();
  // delay(1000);
}

// Handles camera setting changes
void handleMode() {
  // If middle button is pressed, change the setting to edit
  if (getPressed() & 1<<1) {
    if (getPressed() & 1<<2) { mode_func++; delay(100); }
    if (getPressed() & 1<<0) { mode_func--; delay(100); }

    // Clamp, no menu wrap (maybe make it an option?)
    if (mode_func > FUNC_RENDER) { mode_func = FUNC_RENDER; }
    if (mode_func < 0) { mode_func = 0; }
  } else {
    // If only up is pressed, change values
    if (getPressed() & 1<<2) {
      switch (mode_func) {
        case FUNC_RENDER:
          mlx.setMode(MLX90640_CHESS); break;
        case FUNC_MINTEMP:
          if (!auto_mode) { min_temp++; } break;
        case FUNC_MAXTEMP:
          if (!auto_mode) { max_temp++; } break;
        case FUNC_REFRESH:
          mlx.setRefreshRate(MLX90640_16_HZ); break;
        case FUNC_AUTO:
          auto_mode = true; break;
      }
      delay(50); // vTaskDelay()
    }
    // If only down is pressed, change values
    if (getPressed() & 1<<0) {
      switch (mode_func) {
        case FUNC_RENDER:
          mlx.setMode(MLX90640_INTERLEAVED); break;
        case FUNC_MINTEMP:
          if (!auto_mode) { min_temp--; } break;
        case FUNC_MAXTEMP:
          if (!auto_mode) { max_temp--; } break;
        case FUNC_REFRESH:
          mlx.setRefreshRate(MLX90640_8_HZ); break;
        case FUNC_AUTO:
          auto_mode = false; break;
      }
      delay(50); // vTaskDelay()
    }
  }
}

// Draw the stats menu (13 characters @ 15 lines max)
void drawStats() {
  stats.fillScreen(ST77XX_BLACK);
  // stats.drawRect(0, 0, 240-FRAME_WIDTH*RENDER_WIDTH_SCALE, 135-15, ST77XX_WHITE); // Stats/menu area
  stats.setCursor(0, 0); // Set cursor to 1, 1 if drawing bounding box

  // Change battery % colour
  switch ((int)batt_percent) {
    case  21 ... 50:
      stats.setTextColor(ST77XX_YELLOW); break;
    case  0 ... 20:
      stats.setTextColor(ST77XX_RED); break;
    default:
      stats.setTextColor(ST77XX_WHITE); break;
  }
  // Unless charging which forces green
  if (charge_rate > 0) stats.setTextColor(ST77XX_GREEN);
  stats.printf("Battery: %.00f%%\n", batt_percent);
  // stats.printf("%%/hr: %.02f%%\n", charge_rate);
  stats.setTextColor(ST77XX_WHITE);
  // stats.printf("Buttons: 0x%02X\n", getPressed());

  // Print the temperature range settings in the stats menu
  stats.printf("Mode: %s\n", mode_strs[mode_func]); stats.println();
  stats.printf("Render: %s\n", render_strs[mlx.getMode()]);
  stats.printf("Refresh: %dHz\n", refresh_rates[mlx.getRefreshRate()]);
  stats.printf("Auto Range: %d\n", auto_mode);
  stats.printf("Min Temp: %d\nMax Temp: %d\n", min_temp, max_temp);

  // Print the framerate in the bottom right
  stats.setCursor(0, 104);
  stats.printf("FPS: %.2f", fps);
  stats.setCursor(0, 112);
  stats.printf("FT (ms): %d", mspf);

  tft.drawRGBBitmap(FRAME_WIDTH*RENDER_WIDTH_SCALE, 15, stats.getBuffer(), stats.width(), stats.height());
}

// Draws the frame from the thermal camera to the display
void drawThermalFrame() {
  // Get a frame from the camera
  mlx.getFrame(frame);

  // Automatically find hotspot and coldspot values and set as the new range
  if (auto_mode) {
    int16_t hottest = -500;
    int16_t coldest =  500;

    for (uint16_t i=0; i < FRAME_WIDTH*FRAME_HEIGHT; i++) {
      if (frame[i] > hottest) { hottest = (int16_t)frame[i] * min_max_pad; }
      if (frame[i] < coldest) { coldest = (int16_t)frame[i] * min_max_pad; }
    }

    max_temp = hottest;
    min_temp = coldest;
  }

  // Update the screen buffer
  for (uint8_t h=0; h<FRAME_HEIGHT; h++) {
    for (uint8_t w=0; w<FRAME_WIDTH; w++) {
      // Read a temperature and clamp it between set temperature range
      float t = frame[h*FRAME_WIDTH + w];
      t = constrain(t, min_temp, max_temp);

      // Map the colour index to a colour and draw the pixel
      uint8_t colorIndex = map(t, min_temp, max_temp, 0, 255);
      therm_frame.fillRect((FRAME_WIDTH*RENDER_WIDTH_SCALE-RENDER_WIDTH_SCALE)-(RENDER_WIDTH_SCALE * w), (RENDER_HEIGHT_SCALE * h), RENDER_WIDTH_SCALE, RENDER_HEIGHT_SCALE, camColors[colorIndex]);
    }
  }

  // Draw the frame buffer to the screen
  tft.drawRGBBitmap(0, 15, therm_frame.getBuffer(), therm_frame.width(), therm_frame.height());
}

void setup() {
  Serial.begin(115200);

  /* MISC INIT */
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP); // Pulled HIGH (boot button)
  pinMode(1, INPUT); // Pulled LOW (for interrupts)
  pinMode(2, INPUT); // Pulled LOW (for interrupts)

  /* PMIC INIT */
  Serial.println("Setting up PMIC...");
  batt.begin();
  Serial.println("PMIC initialized");

  /* DISPLAY INIT */
  Serial.println("Setting up display...");
  pinMode(TFT_BACKLITE, OUTPUT);
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  tft.init(135,240);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);

  // Draw project title
  // tft.fillScreen(ST77XX_MAGENTA); // Available space behind everything
  tft.fillRect(0, 0, 240, 15, ST77XX_RED);
  tft.setCursor(8, 0);
  tft.print("OpenFlame Cam v0.1a");
  Serial.println("Display initialized");

  /* THERMAL CAM INIT */
  if (mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
    Serial.println("Setting up thermal camera...");
    mlx.setMode(MLX90640_CHESS);
    mlx.setResolution(MLX90640_ADC_16BIT); // No real point in using anything more than 16-bit resolution
    mlx.setRefreshRate(MLX90640_16_HZ); // Max ~20 FPS at 32Hz with frequent lows hitting 16 FPS, keep at 16Hz for stability (maybe test 64Hz with Teensy or Pi? be wary of I2C clock stretching)
    Wire.setClock(1000000); // Max 1MHz
    Serial.println("Thermal camera initialized");
  } else {
    Serial.println("Camera module not found, check connections and restart the device");

    therm_frame.fillScreen(ST77XX_BLACK);
    therm_frame.setCursor(0, 15);
    therm_frame.setTextSize(3);
    therm_frame.setTextColor(ST77XX_RED);
    therm_frame.print("Camera\nmodule\nnot\nfound");
    tft.drawRGBBitmap(0, 15, therm_frame.getBuffer(), therm_frame.width(), therm_frame.height());

    while (true) {
      delay(1000);
    }
  }
}

void loop() {
  uint32_t start = millis();

  // Print battery information
  getBattery();

  // Handle inputs
  handleMode();

  // Update the stats menu
  drawStats();

  // Draw the thermal camera frame on the display
  drawThermalFrame();

  uint32_t end = millis();
  fps = 2000.0 / (end - start);
  mspf = (end - start) / 2;
}
