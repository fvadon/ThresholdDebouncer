#ifndef ThresholdDebouncer_h
#define ThresholdDebouncer_h
#include "Arduino.h"

class ThresholdDebouncer
{
public:
    
    typedef enum {
      IDLE,
      OVERSHOOTING
    } signal_state_t;
    
    typedef enum {
      SINGLE_OVERSHOOT,
      ONGOING_LONG_OVERSHOOT,
      OFF_LONG_OVERSHOOT
    } overshoot_pattern_t;

    typedef void (*callback_t)(ThresholdDebouncer *thresholdebouncer, overshoot_pattern_t pattern);

    /*
     * Callback if the signal overshoots thresholed once
     */
    ThresholdDebouncer& onSingleOvershoot(callback_t callback);

    /*
    * Callback for event ongoing longer than timeout, 
    * callback will keep firing until even stop overshooting signal
    * */
    ThresholdDebouncer& ongoingLongOvershootFor(callback_t callback, unsigned long timeout=3000);

    /*
    * Callback for end of signal overshooting threshold
    */
    ThresholdDebouncer& offLongOvershoot(callback_t callback);

    /*
      * Constructor
      * Params:
      * threshold     int value to compare the signal
      * debounce     time in millis used to filter noisy signal
      * intervalToRun time in millis to make we don't run it too often
      */
    ThresholdDebouncer(unsigned long threshold = 200, 
      unsigned short debounceInterval = 30, 
      unsigned short intervalToRun=10);
    ~ThresholdDebouncer() {}

    void analyseSignalValue(unsigned long signalReadValue);

private:

    //Signal value read to analyse for callbacks
    unsigned long _signalReadValue;
    //Threshold to filter out lower values
    unsigned long _threshold; 


    // callbacks
    callback_t _onSingleOvershootCallback;
    callback_t _ongoingOvershootForCallback;
    callback_t _offLongOvershootCallback;

    signal_state_t _prevState = ThresholdDebouncer::IDLE;
    signal_state_t _state = ThresholdDebouncer::IDLE;

    unsigned short _overshootForTimeOut;//Millis to determine it's a loog event
    unsigned short _debounceInterval; //millis to filter noisy signal going under threshold
    unsigned short _intervalToRun = 10; //millis to make sure we don't run too often
    unsigned long _lastLoop = 0; //Used to track time of when loop was lastly called
    unsigned long _firstOvershotAt = 0;
    boolean _debouncing = false;
    unsigned long _stopOvershotAt;
    
};

#endif
