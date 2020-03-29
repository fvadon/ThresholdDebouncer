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
      /*DOUBLE_OVERSHOOT,*/
      ONGOING_LONG_OVERSHOOT,
      OFF_LONG_OVERSHOOT
    } overshoot_pattern_t;

    typedef void (*callback_t)(ThresholdDebouncer *thresholdebouncer, overshoot_pattern_t pattern);

    /*
     * Callback if the signal overshoots thresholed once
     */
    ThresholdDebouncer& onSingleOvershoot(callback_t callback);
    /*
     * Callback if the signal overshoots threshold twice
     */
    /* 
    * not supported yet 
    * ThresholdDebouncer& onDoubleOvershoot(callback_t callback, unsigned long timeout=300);
    */

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
      unsigned long debounce = 30, 
      uint8_t intervalToRun=10);
    ~ThresholdDebouncer() {}

    void analyseSignalValue(uint16_t signalReadValue);

private:

    //Signal value read to analyse for callbacks
    unsigned long _signalReadValue;

    // callbacks
    callback_t _onSingleOvershootCallback;
    /* Not supported for now
    callback_t _onDoubleOvershootCallback;
    */
    callback_t _ongoingOvershootForCallback;
    callback_t _offLongOvershootCallback;

    signal_state_t _prevState = ThresholdDebouncer::IDLE;
    signal_state_t _state = ThresholdDebouncer::IDLE;

    //unsigned long _doubleOvershootTimeout = 300; // 0.3 seconds
    unsigned long _overshootForTimeOut;//Millis to determine it's a loog event
    unsigned long _threshold; //to filter out lower values
    unsigned long _debounce; //millis to filter noisy signal going under threshold
    uint8_t _intervalToRun = 10; //millis to make sure we don't run too often
    unsigned long _lastLoop = 0; //Used to track time of when loop was lastly called
    unsigned long _firstOvershotAt = 0;
    
};

#endif
