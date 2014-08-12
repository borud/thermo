/*
 * 
 */

#include <stdio.h>
#include <LedControl.h>

// Serial speed
#define SERIAL_SPEED 9600

// Configure which pins the MAX7219 is connected to.
//
#define MAX7219_LOAD       10
#define MAX7219_CLK        11
#define MAX7219_DATA_INPUT 12

// Number of chained MAX7219 chips
#define MAX7219_NUM_DEVICES 1

// The display number
#define DISPLAY             0

// Parameters to the shutdown command
#define POWER_DOWN true
#define POWER_SAVE false

// Decimal dot
#define DECIMAL_DOT_ON  true
#define DECIMAL_DOT_OFF false


// Defaults
#define DEFAULT_INTENSITY 4;

// Display intensity
static int intensity = DEFAULT_INTENSITY;

// MAX7219 LED controller
static LedControl lc = LedControl(MAX7219_DATA_INPUT, MAX7219_CLK, MAX7219_LOAD, MAX7219_NUM_DEVICES);

/**
 * Initialize the display.  Puts it in power-save mode to avoid display flickering and displaying
 * nonsense during startup.  Then set the default intensity and clear the display so the display
 * is in a known state when starting.
 */
void setup() {
  // Initialize the display first so we turn it off before we start doing 
  // anything else.
  lc.shutdown(DISPLAY, POWER_SAVE);
  lc.setIntensity(DISPLAY, intensity);
  lc.clearDisplay(DISPLAY);
  
  // 
  Serial.begin(SERIAL_SPEED);
}

/**
 * Convert float to string.  Very limited funcitonality.  Just make sure there is 
 * enough room in the buffer or things will become very sad.
 */
void floatToString(float f, char buffer[]) {
  dtostrf(f, 2, 2, buffer);
}
  

/**
 * Main loop.
 */
void loop() {
   float floater = 30.10;
   char buffer[6];
   
   // Test conversion to string
   floatToString(floater, buffer);
   Serial.print("Result: ");
   Serial.println(buffer);
  
   for (int i=0; i < 10; i++) {
    lc.setChar(DISPLAY,1, i, DECIMAL_DOT_ON);
    lc.setChar(DISPLAY,2, i, DECIMAL_DOT_ON);
    lc.setChar(DISPLAY,3, i, DECIMAL_DOT_ON);
    lc.setChar(DISPLAY,4, i, DECIMAL_DOT_ON);
    delay(300);
  }

  lc.setChar(DISPLAY, 1, 'E', DECIMAL_DOT_OFF);
  lc.setChar(DISPLAY, 2, '-', DECIMAL_DOT_OFF);
  lc.setChar(DISPLAY, 3, '0', DECIMAL_DOT_OFF);
  lc.setChar(DISPLAY, 4, '2', DECIMAL_DOT_OFF);
  delay(2000);
}
