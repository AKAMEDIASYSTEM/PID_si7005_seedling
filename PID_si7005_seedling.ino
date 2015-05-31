/********************************************************
 * PID RelayOutput Example
 * Same as basic example, except that this time, the output
 * is going to a digital pin which (we presume) is controlling
 * a relay.  the pid is designed to Output an analog value,
 * but the relay can only be On/Off.
 *
 *   to connect them together we use "time proportioning
 * control"  it's essentially a really slow version of PWM.
 * first we decide on a window size (5000mS say.) we then
 * set the pid to adjust its output between 0 and that window
 * size.  lastly, we add some logic that translates the PID
 * output into "Relay On Time" with the remainder of the
 * window being "Relay Off Time"
 ********************************************************/

#include <PID_v1.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Si7005.h>

#define RelayPin 5
#define OLED_RESET 4
#define STATUS_LED 13
#define Si7005_CS_PIN A3

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(OLED_RESET);
Si7005 Si(Si7005_CS_PIN);

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, 200, 5, 1, DIRECT);

int WindowSize = 5000;
unsigned long windowStartTime;

float temperature = 10.2;
float humidity = 12.7;


void setup() {
  pinMode(STATUS_LED, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();   // clears the screen and buffer
  if (!Si.detectSensor())
  {
    while (1)
    {
      display.print("No sensor");
      display.display();
    } // The sensor is not detected. Let's just stay here forever.
  }

  windowStartTime = millis();

  //initialize the variables we're linked to
  Setpoint = 20.0; // googled best seedling temps, consensus was 19-24 deg C

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);
  
  // we don't need to sample fast, every second is fine
  myPID.SetSampleTime(1000);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  display.setTextSize(1);
  display.setTextColor(WHITE);

}

void loop() {
  display.clearDisplay();
  boolean isHeating = false;
  temperature = Si.getTemperature();
  humidity = Si.getHumidity();
  Input = temperature;
  myPID.Compute();

  // pid sate display removed from here
  if (millis() - windowStartTime > WindowSize) { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if (Output > millis() - windowStartTime) {
    digitalWrite(RelayPin, HIGH);
    digitalWrite(STATUS_LED, HIGH);
    isHeating = true;
  }
  else {
    digitalWrite(RelayPin, LOW);
    digitalWrite(STATUS_LED, LOW);
    isHeating = false;
  }
  updateDisplay(temperature, humidity, isHeating);

  delay(100);

}
