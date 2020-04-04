# ThresholdDebouncer

Arduino Lib to de-bounce a signal compared to a threshold and trigger a callback like single event, ongoing long event... Usefull for capacitive touch interface like the one on the Adafruit Circuit Playground.

# Installation
The lib is not available on Arduino Library Manager for now so installation is made by [Downloading or cloning this repository into your arduino libraries directory](https://help.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository)

# Usage
General logic is :

- Instanciate the threshold debouncer before the setup
- Attach the callback function(s) in the setup
- Call the analyseValue function in the loop

Please refer to the example sketch or here under for available variables and actual usage of the library.

# Example

```cpp
#include <ThresholdDebouncer.h>
	
// Threshold Debouncer Variables
	
//How long (in Millis) before detecting it is a long press
#define LONG_OVERSHOOT_TIMEOUT 700     
//Treshold to filter out small (possibly noisy) values
#define OVERSHOOT_THRESHOLD 100         
//how long in millis before detecting long press if off
#define OVERSHOOT_DEBOUNCE_INTERVAL 150 
//How often in millis does the logic run, good way to control the frequency of ongoing overshooting callback.
//Call it often enough to make you will detect a single quick event though
#define INTERVAL_TO_RUN 10              
	
//instanciate a new thresholdDebouncer
ThresholdDebouncer thresholdDebouncer = ThresholdDebouncer(OVERSHOOT_THRESHOLD, OVERSHOOT_DEBOUNCE_INTERVAL, INTERVAL_TO_RUN);
	
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
	
  // Attach the callback function or functions
  // Note : Could be different callback functions
  thresholdDebouncer.onSingleOvershoot(callbackHandler)
      .ongoingLongOvershootFor(callbackHandler, LONG_OVERSHOOT_TIMEOUT)
      .offLongOvershoot(callbackHandler);
}
	
void loop()
{
  // put your main code here, to run repeatedly:
  thresholdDebouncer.analyseSignalValue(anyUnsignedLongSignalToDebounce); //anyUnsignedLongSignalToDebounce not defined in this fake example
	
}
	
void callbackHandler(ThresholdDebouncer *ThresholdDebouncer, ThresholdDebouncer::overshoot_pattern_t pattern)
{
  switch (pattern)
  {
  case ThresholdDebouncer::SINGLE_OVERSHOOT:
    Serial.println("Single Overshoot");
    break;
  case ThresholdDebouncer::ONGOING_LONG_OVERSHOOT:
    Serial.println("Ongoing Long Overshoot");
    break;
  case ThresholdDebouncer::OFF_LONG_OVERSHOOT:
    Serial.println("Stopped long overshoot");
    break;
  }
} 
```
# Misc.

- Only used and tested with Adafruit Circuit Playground Capacitive touch but should work with any other signal.
- Heavily inspired from Button Fever Library by Mickey Chan for the callback logic, so hats off to him: [ButtonFever Github Page](https://github.com/mickey9801/ButtonFever)