# Thermo

__*This is a work in progress so some of the code might be messy or
  unclear.*__

The goal of this project is to learn about measuring body temperature,
so the software will implement whatever I need to experiment with
this.  The goal isn't necessarily to produce a polished product at the
end.

If you find this code usable or if you are able to learn from it,
please feel free to clone the project on Github.  That way I can
perhaps learn something from you and you can contribute features that
I might include in my version.

## How to get this to work

### `private.h`

There's a header file called `private.h` which you will notice is
missing from this project.  This is because this is where private
information such as WiFi ssid and password is stored as well as Xively
credentials.

I've provided a file named `private_template.h` which you should copy
to `private.h` and then edit that.  __Take care not to check
`private.h` into any public repositories, or your WiFi and Xively
credentials will become public__.

### `thermo_config.h`

This file contains a bunch of configuration settings and a few useful
global definitions.  Have a look at it.  In particular you will need
to modify this if you want to add more sensors than the default max
number (3 sensors).

(If you add more sensors you'll have to look at the code in
`thermo_xively.cpp` as well.  There's a terribly stupid hack there to
define feeds, and if you need more feeds you need to edit this)

## Hardware.

To use this software you need the following hardware:

  - 4-digit, 8-segment LED display from SparkFun
  - Maxim MAX7219 LED driver
  - Dallas DS18B20 digital temperature sensors
  - Arduino Uno

You will also need a few resistors and capacitors.

## Software and dependencies.

You will need the Arduino IDE or a suitable toolchain able to run
`avr-gcc`, upload the software to the Arduino etc.

The following libraries are used by this software

  - OneWire library
  - DallasTemperature
  - LedControl
  - WiFi
  
I suggest you get the latest versions of everything since having older
versions of some libraries may result in compile-time errors that may
be confusing.


## Possible future enhancements.

  - Networking in order to log sensor to some service capable of
    recording timeseries (there are several).
  - Low-power features to run this system off a battery so I can use
    it for wearable systems.
  - Document hardware including wiring diagram, component values and
    some explanations.

I might also try out different temperature sensors.  If you know of
sensors that might be more suitable do not hesitate to drop me a line.
I'd love for something that can sample faster and provide higher
accuracy.

## Display status codes

### Boot sequence

  - **b-01** - Boot-time sensor scan.  Looks for sensors connected to
    the OneWire bus.
  - **b-02** - Connecting to WiFi.
  - **b-99** - Boot finished.  (Normally just a brief flash).

### Errors

The following error codes may be displayed:

  - **E-01** - No temperature sensor or sensor has been unplugged.

  - **E-02** - Negative temperature (*display only supports positive
    temperatures up to 99.99 degrees Celsius.  May also be indicative
    of sensors being unplugged.*).

  - **E-03** - Getting DS18x20 sensor address failed.

  - **E-20** - Sensor index out of bounds.  Indicates a programming
    error in `thermo_sensor.cpp`

  - **E-21** - Sensor index out of bounds. Indicates a programming
    error in `thermo_xively.cpp`.

### Reboot

  - **P-01** - No temperature sensors connected.  Keeps rebooting
    every 2 seconds until one is connected.

## Useful links

### Dallas DS18B20 temperature sensor

  - http://www.milesburton.com/Dallas_Temperature_Control_Library

### Maxim MAX7219 Led display driver
  - http://playground.arduino.cc/Main/MAX72XXHardware
  - http://tronixstuff.com/2013/10/11/tutorial-arduino-max7219-led-display-driver-ic/

