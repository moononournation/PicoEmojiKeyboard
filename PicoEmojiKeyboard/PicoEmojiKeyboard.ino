/*******************************************************************************
 * Pico Emoji Keyboard
 * Start rewrite from Arduino_GFX PNG Image Viewer example
 *
 * Dependent libraries:
 * PNGdec: https://github.com/bitbank2/PNGdec.git
 *
 * Setup steps:
 * 2. Upload PNG file
 *   LittleFS:
 *     Pico: https://github.com/earlephilhower/arduino-littlefs-upload.git
 ******************************************************************************/
#define PNG_FILENAME "/emoji.png"

/* Uncomment one and only one your target OS */
// #define KEY_FOR_WINDOWS
#define KEY_FOR_MACOS
// #define KEY_FOR_LINUX

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#define GFX_BL 25
Arduino_DataBus *bus = new Arduino_RPiPicoSPI(8 /* DC */, 9 /* CS */, 10 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */, spi1 /* spi */);
Arduino_GFX *gfx = new Arduino_ST7735(bus, 12 /* RST */, 3 /* rotation */, true /* IPS */, 80 /* width */, 160 /* height */, 26 /* col offset 1 */, 1 /* row offset 1 */, 26 /* col offset 2 */, 1 /* row offset 2 */);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include <LittleFS.h>
File pngFile;

#include <PNGdec.h>
PNG png;

#include <Keyboard.h>
#define BTN_A 26
#define BTN_B 21
#define BTN_C 4
#define BTN_D 7

#define BTN_B_GND 18
#define BTN_C_GND 6

void *myOpen(const char *filename, int32_t *size)
{
  pngFile = LittleFS.open(filename, "r");

  if (!pngFile || pngFile.isDirectory())
  {
    Serial.println(F("ERROR: Failed to open " PNG_FILENAME " file for reading"));
    gfx->println(F("ERROR: Failed to open " PNG_FILENAME " file for reading"));
  }
  else
  {
    *size = pngFile.size();
    Serial.printf("Opened '%s', size: %d\n", filename, *size);
  }

  return &pngFile;
}

void myClose(void *handle)
{
  if (pngFile)
    pngFile.close();
}

int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
  if (!pngFile)
    return 0;
  return pngFile.read(buffer, length);
}

int32_t mySeek(PNGFILE *handle, int32_t position)
{
  if (!pngFile)
    return 0;
  return pngFile.seek(position);
}

// Function to draw pixels to the display
void PNGDraw(PNGDRAW *pDraw)
{
  uint16_t usPixels[320];
  uint8_t usMask[320];

  // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
  png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
  png.getAlphaMask(pDraw, usMask, 1);
  gfx->draw16bitRGBBitmapWithMask(0, pDraw->y, usPixels, usMask, pDraw->iWidth, 1);
}

void setup()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Pico Emoji Keyboard");

  // set buttons' GPIO as input and pulled-up
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);

  // set buttons' GND pins to LOW
  pinMode(BTN_B_GND, OUTPUT);
  digitalWrite(BTN_B_GND, LOW);
  pinMode(BTN_C_GND, OUTPUT);
  digitalWrite(BTN_C_GND, LOW);

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  if (!LittleFS.begin())
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
  }
  else
  {
    unsigned long start = millis();
    int rc;
    rc = png.open(PNG_FILENAME, myOpen, myClose, myRead, mySeek, PNGDraw);
    if (rc == PNG_SUCCESS)
    {
      rc = png.decode(NULL, 0);
      png.close();
    }
    else
    {
      Serial.println("png.open() failed!");
    }
  }
}

void loop()
{
  if (digitalRead(BTN_A) == LOW) {
    Serial.println("Button A pressed");
#if defined(KEY_FOR_WINDOWS)
    Keyboard.press(KEY_LEFT_ALT);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_2);
    delay(1);
    Keyboard.release(KEY_KP_2);
    delay(1);
    Keyboard.press(KEY_KP_8);
    delay(1);
    Keyboard.release(KEY_KP_8);
    delay(1);
    Keyboard.press(KEY_KP_5);
    delay(1);
    Keyboard.release(KEY_KP_5);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_3);
    delay(1);
    Keyboard.release(KEY_KP_3);
    delay(1);
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_MACOS)
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.write((const uint8_t*)"d83d+de01", 9);
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_LINUX)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.write('u');
    Keyboard.release(KEY_LEFT_SHIFT);
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.write((const uint8_t*)"1f601\n", 6);
#endif
    delay(200);
  } else if (digitalRead(BTN_B) == LOW) {
    Serial.println("Button B pressed");
#if defined(KEY_FOR_WINDOWS)
    Keyboard.press(KEY_LEFT_ALT);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_2);
    delay(1);
    Keyboard.release(KEY_KP_2);
    delay(1);
    Keyboard.press(KEY_KP_8);
    delay(1);
    Keyboard.release(KEY_KP_8);
    delay(1);
    Keyboard.press(KEY_KP_5);
    delay(1);
    Keyboard.release(KEY_KP_5);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_7);
    delay(1);
    Keyboard.release(KEY_KP_7);
    delay(1);
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_MACOS)
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.write((const uint8_t*)"d83d+de05", 9); /* MacOS */
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_LINUX)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.write('u');
    Keyboard.release(KEY_LEFT_SHIFT);
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.write((const uint8_t*)"1f605\n", 6);
#endif
    delay(200);
  } else if (digitalRead(BTN_C) == LOW) {
    Serial.println("Button C pressed");
#if defined(KEY_FOR_WINDOWS)
    Keyboard.press(KEY_LEFT_ALT);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_2);
    delay(1);
    Keyboard.release(KEY_KP_2);
    delay(1);
    Keyboard.press(KEY_KP_9);
    delay(1);
    Keyboard.release(KEY_KP_9);
    delay(1);
    Keyboard.press(KEY_KP_3);
    delay(1);
    Keyboard.release(KEY_KP_3);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_5);
    delay(1);
    Keyboard.release(KEY_KP_5);
    delay(1);
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_MACOS)
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.write((const uint8_t*)"d83e+dd23", 9); /* MacOS */
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_LINUX)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.write('u');
    Keyboard.release(KEY_LEFT_SHIFT);
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.write((const uint8_t*)"1f923\n", 6);
#endif
    delay(200);
  } else if (digitalRead(BTN_D) == LOW) {
    Serial.println("Button D pressed");
#if defined(KEY_FOR_WINDOWS)
    Keyboard.press(KEY_LEFT_ALT);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.press(KEY_KP_2);
    delay(1);
    Keyboard.release(KEY_KP_2);
    delay(1);
    Keyboard.press(KEY_KP_9);
    delay(1);
    Keyboard.release(KEY_KP_9);
    delay(1);
    Keyboard.press(KEY_KP_3);
    delay(1);
    Keyboard.release(KEY_KP_3);
    delay(1);
    Keyboard.press(KEY_KP_2);
    delay(1);
    Keyboard.release(KEY_KP_2);
    delay(1);
    Keyboard.press(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_KP_1);
    delay(1);
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_MACOS)
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.write((const uint8_t*)"d83e+dd29", 9); /* MacOS */
    Keyboard.release(KEY_LEFT_ALT);
#elif defined(KEY_FOR_LINUX)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.write('u');
    Keyboard.release(KEY_LEFT_SHIFT);
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.write((const uint8_t*)"1f929\n", 6);
#endif
    delay(200);
  }
}
