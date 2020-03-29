/*
* Utility Class to use sensisive touch from CircuitPlayground
* Inspired deeply from button fever https://github.com/mickey9801/ButtonFever
*/

#ifndef ThresholdDebouncer_h
#define ThresholdDebouncer_h
#include "Arduino.h"

class ThresholdDebouncer
{
public:


    uint8_t pin;
    unsigned long readValue;
    
    typedef enum {
      IDLE,
      TOUCHING
    } touch_state_t;
    
    typedef enum {
      SINGLE_PRESS,
      /*DOUBLE_PRESS,*/
      ONGOING_LONG_PRESS,
      OFF_LONG_PRESS
    } press_pattern_t;

    typedef void (*callback_t)(ThresholdDebouncer *cplaytouch, press_pattern_t pattern);

    /*
     * Assign event callbacks
     */
    ThresholdDebouncer& onPress(callback_t callback);
    ThresholdDebouncer& onDoublePress(callback_t callback, unsigned long timeout=300);
    ThresholdDebouncer& onPressFor(callback_t callback, unsigned long timeout=3000);
    ThresholdDebouncer& offLongPress(callback_t callback);


    /*
        * Constructor
        * Params:
        * pin          pin number of the touch button
        */
    ThresholdDebouncer(uint8_t pin, unsigned long threshold = 200, unsigned long _debounce = 30);
    ~ThresholdDebouncer() {}

    uint8_t getPin();

    void reactToRead(uint16_t externalReadValue);

    ThresholdDebouncer& updateState(touch_state_t state);

private:
    // callbacks
    callback_t _onPressCallback;
    /* Not supported for now
    callback_t _onDoublePressCallback;
    */
    callback_t _onPressForCallback;
    callback_t _offLongPressCallback;

    touch_state_t _prevState = ThresholdDebouncer::IDLE;
    touch_state_t _state = ThresholdDebouncer::IDLE;

    //unsigned long _doublePressTimeout = 300; // 0.3 seconds
    unsigned long _pressForTimeOut=500; // 0.5 seconds
    unsigned long _threshold; //to filter out lower values
    unsigned long _debounce; //to filter out lower values
    uint8_t _loopInterval = 10;
    unsigned long _lastLoop = 0;
    unsigned long _firstPressedAt = 0;
    

};

#endif
