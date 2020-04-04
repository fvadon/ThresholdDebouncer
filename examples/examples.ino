#include <ThresholdDebouncer.h>

#include <Adafruit_Circuit_Playground.h>

#define TOUCH_PIN 1

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

#define READ_SAMPLES 10 //Adafruit touch sampling value

//instanciate a new thresholdDebouncer
ThresholdDebouncer thresholdDebouncer = ThresholdDebouncer(OVERSHOOT_THRESHOLD, OVERSHOOT_DEBOUNCE_INTERVAL, INTERVAL_TO_RUN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  CircuitPlayground.begin();

  // Attach the callback function or functions
  // Note : Could be different callback functions
  thresholdDebouncer.onSingleOvershoot(callbackHandler)
      .ongoingLongOvershootFor(callbackHandler, LONG_OVERSHOOT_TIMEOUT)
      .offLongOvershoot(callbackHandler);
}

void loop()
{
  // put your main code here, to run repeatedly:
  thresholdDebouncer.analyseSignalValue(CircuitPlayground.readCap(TOUCH_PIN,READ_SAMPLES));

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