#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// SDA no pino 23
// SDL no pino 18
#define TFT_CS        5 // Hallowing display control pins: chip select
#define TFT_RST       4 // Display reset
#define TFT_DC        16 // Display data/command select
#define TFT_BACKLIGHT  22 // Display backlight pin

#define SerialDebugging true


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define botaoEsq 2
#define botaoDir 27

// color definitions
const uint16_t  Display_Color_Black        = 0x0000;
const uint16_t  Display_Color_Blue         = 0x001F;
const uint16_t  Display_Color_Red          = 0xF800;
const uint16_t  Display_Color_Green        = 0x07E0;
const uint16_t  Display_Color_Cyan         = 0x07FF;
const uint16_t  Display_Color_Magenta      = 0xF81F;
const uint16_t  Display_Color_Yellow       = 0xFFE0;
const uint16_t  Display_Color_White        = 0xFFFF;

// The colors we actually want to use
uint16_t        Display_Text_Color         = Display_Color_White;
uint16_t        Display_Backround_Color    = Display_Color_Black;

// assume the display is off until configured in setup()
bool            isDisplayVisible        = false;

// declare size of working string buffers. Basic strlen("d hh:mm:ss") = 10
const size_t    MaxString               = 16;

// Para alterar variaveis dentro de uma interrupção tem que usar as variaveis do tipo volatile
// Para informar que a variavel pode ser alterada a qualquer instante
volatile bool   isButtonPressedEsq = false;
volatile bool   isButtonPressedDir = false;

bool testeCor = false;

// interrupt service routine
void senseButtonPressedEsq() {
    if (!isButtonPressedEsq) {
        isButtonPressedEsq = true;
    }
}
void senseButtonPressedDir() {
    if (!isButtonPressedDir) {
        isButtonPressedDir = true;
    }
}

void mudaCor(){
  if(testeCor){
    tft.fillScreen(Display_Color_Black);
    testeCor = false;
  }
  else{
    tft.fillScreen(Display_Color_White);
    testeCor = true;
  }
}

void setup() {

    // button press pulls pin LOW so configure HIGH
    pinMode(botaoEsq, INPUT_PULLUP);
    pinMode(botaoDir, INPUT_PULLUP);

    // Usa interrupção ou seja o microcontrolador vai estar verificando esse pino a todo momento
    // Quando ele vê que foi acionado ele pausa o código principal para executar o codigo atrelado a esse pino
    attachInterrupt(digitalPinToInterrupt(botaoEsq), senseButtonPressedEsq, FALLING);
    attachInterrupt(digitalPinToInterrupt(botaoDir), senseButtonPressedDir, FALLING);

    #if (SerialDebugging)
    Serial.begin(115200); while (!Serial); Serial.println();
    #endif

    // settling time
    delay(250);

    // ignore any power-on-reboot garbage
    isButtonPressedEsq = isButtonPressedDir = false;

    #ifdef ADAFRUIT_HALLOWING
      // HalloWing is a special case. It uses a ST7735R display just like the
      // breakout board, but the orientation and backlight control are different.
      tft.init(128, 128);

      tft.setRotation(0);  // 0, 1, 2 ou 3 para ajustar a rotação

        // Initialize HalloWing-oriented screen
      pinMode(TFT_BACKLIGHT, OUTPUT);
      digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
    #else
      // Use this initializer if using a 1.8" TFT screen:
      tft.initR(INITR_144GREENTAB);
    #endif

    // initialise the display
    tft.setFont();
    tft.fillScreen(Display_Backround_Color);
    tft.setTextColor(Display_Text_Color);
    tft.setTextSize(1);

    // the display is now on
    isDisplayVisible = true;
    tft.enableDisplay(isDisplayVisible);

}

void loop() {
    // has the button been pressed?
    if (isButtonPressedEsq) {
        // confirm button handled
        isButtonPressedEsq = false; 
        mudaCor();
    }
    if(isButtonPressedDir) {
        // confirm button handled
        isButtonPressedDir = false;
        mudaCor();
    }
    // no need to be in too much of a hurry
    delay(100);
   
}

