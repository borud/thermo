/*
 * Software for collecting and displaying temperature data.
 *
 */
#include <DallasTemperature.h>
#include <LedControl.h>
#include <OneWire.h>

// Serial speed
#define SERIAL_SPEED 9600

// Configure which pins the MAX7219 is connected to.
//
#define MAX7219_LOAD       10
#define MAX7219_CLK        11
#define MAX7219_DATA_INPUT 12

// Thermometer
#define THERMOMETER_PIN              2
#define THERMOMETER_RESOLUTION_BITS 11

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
#define DEFAULT_INTENSITY 4

// Error codes
#define ERR_NO_TEMPERATURE_SENSOR  1
#define ERR_NEGATIVE_TEMPERATURE   2

// How many samples do we perform smoothing over?  Higher number of
// samples results in slower response to change.
#define SMOOTHING_SIZE 6

// Display intensity
static int intensity = DEFAULT_INTENSITY;

// MAX7219 LED controller
static LedControl lc = LedControl(MAX7219_DATA_INPUT, MAX7219_CLK, MAX7219_LOAD, MAX7219_NUM_DEVICES);

// DS18x20 digital thermometer chip
static OneWire onewire(THERMOMETER_PIN);
static DallasTemperature ds18x20(&onewire);
static DeviceAddress thermometer;

static float temperature = 0.0;
static char buffer[10];

/**
 * Convert float to string.  Very limited funcitonality.  Just make
 * sure there is enough room in the buffer or things will become very
 * sad.
 */
void floatToString(float f, char buffer[]) {
    // Minimum width 5, 2 digits after decimal point.
    dtostrf((double)f, 5, 2, buffer);
}


/**
 * Print thermometer address.
 */
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

/**
 * Display temperature on LED display.  If an error is detected the
 * appropriate error code is printed.
 */
void display_temperature(float temp) {
    // Show error on negative values
    if (temp < 0.0) {
        display_error(ERR_NEGATIVE_TEMPERATURE);
        return;
    }

    // Sensor returns 0.0 if unplugged.  Can use this float
    // representation to check for this case.
    if (temp == 0.0) {
        display_error(ERR_NO_TEMPERATURE_SENSOR);
        Serial.println("Sensor unplugged");
        return;
    }
    
    floatToString(temp, buffer);
    lc.setChar(DISPLAY,1, buffer[0], DECIMAL_DOT_OFF);
    lc.setChar(DISPLAY,2, buffer[1], DECIMAL_DOT_ON);
    lc.setChar(DISPLAY,3, buffer[3], DECIMAL_DOT_OFF);
    lc.setChar(DISPLAY,4, buffer[4], DECIMAL_DOT_OFF);
}

void display_error(int err) {
    lc.setChar(DISPLAY,1, 'E', DECIMAL_DOT_OFF);
    lc.setChar(DISPLAY,2, '-', DECIMAL_DOT_OFF);
    if (err < 10) {
        lc.setChar(DISPLAY,3, '0', DECIMAL_DOT_OFF);
        lc.setChar(DISPLAY,4, err, DECIMAL_DOT_OFF);
        return;
    }
    lc.setChar(DISPLAY, 3, err, DECIMAL_DOT_OFF);
}

/**
 * Very simple smoothing function.
 */
float smooth(float temp) {
    static float readings[SMOOTHING_SIZE];
    static int offset = 0;
    static int count = 0;

    readings[offset] = temp;
    offset++;
    offset = offset % SMOOTHING_SIZE;

    // Only increment until smoothing array is filled.
    if (count < SMOOTHING_SIZE) {
        count++;
    }

    float sum = 0.0;
    for (int i = 0; i < count ; i++) {
        sum += readings[i];
    }
    return (sum / (float)count);
}

/**
 * Initialize the display.  Puts it in power-save mode to avoid
 * display flickering and displaying nonsense during startup.  Then
 * set the default intensity and clear the display so the display is
 * in a known state when starting.
 */
void setup() {
  // Initialize the display first so we turn it off before we start doing 
  // anything else.
  lc.shutdown(DISPLAY, POWER_SAVE);
  lc.setIntensity(DISPLAY, intensity);
  lc.clearDisplay(DISPLAY);
  
  // Initialize serial interface.
  Serial.begin(SERIAL_SPEED);

  // Initial scan for DS18x20 sensors.
  Serial.println("Looking for temperature sensor");
  ds18x20.begin();
  Serial.print("Found ");
  Serial.print(ds18x20.getDeviceCount(), DEC);
  Serial.println(" devices.");

  if (!ds18x20.getAddress(thermometer, 0)) {
      Serial.println("Unable to find temperature sensor");
      display_error(ERR_NO_TEMPERATURE_SENSOR);
  }

  ds18x20.setResolution(thermometer, THERMOMETER_RESOLUTION_BITS);
}

/**
 * Main loop.
 */
void loop() {
    // Read thermometer
    ds18x20.requestTemperatures();
    temperature = smooth(ds18x20.getTempC(thermometer));

    Serial.print("Temperature: ");
    Serial.println(temperature);

    display_temperature(temperature);
}
