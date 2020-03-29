#include "Arduino.h"
#include "ThresholdDebouncer.h"


ThresholdDebouncer::ThresholdDebouncer(unsigned long threshold, unsigned long debounce, uint8_t intervalToRun) {
    this->_threshold = threshold;
    this->_debounce = debounce;
    this->_intervalToRun=intervalToRun;
}

ThresholdDebouncer& ThresholdDebouncer::onSingleOvershoot(callback_t callback) {
  this->_onSingleOvershootCallback = callback;
  return *this;
}

/* not supported for now
ThresholdDebouncer& ThresholdDebouncer::onDoubleOvershoot(callback_t callback, unsigned long timeout) {
  this->_onDoubleOvershootCallback = callback;
  this->_doubleOvershootTimeout = timeout;
  return *this;
}*/

ThresholdDebouncer& ThresholdDebouncer::ongoingLongOvershootFor(callback_t callback, unsigned long timeout) {
  this->_ongoingOvershootForCallback = callback;
  this->_overshootForTimeOut = timeout;
  return *this;
}

ThresholdDebouncer& ThresholdDebouncer::offLongOvershoot(callback_t callback) {
  this->_offLongOvershootCallback = callback;
  return *this;
}

/* Analyse the value coming from read and callback if needed
*/
void ThresholdDebouncer::analyseSignalValue(uint16_t signalReadValue) {
    this->_signalReadValue = signalReadValue;
    unsigned long curTime = millis();
    if (curTime - this->_lastLoop >= this->_intervalToRun) {
        this->_lastLoop = curTime;
        //Filter noisy values
        if (this->_signalReadValue >this->_threshold) {
        this->_state = ThresholdDebouncer::OVERSHOOTING;
        } else {
        this->_state = ThresholdDebouncer::IDLE;
        }
        if(this->_state == ThresholdDebouncer::OVERSHOOTING && this->_prevState == ThresholdDebouncer::IDLE) {
            //Signal just overshot threshold, start counting for how long
            this->_firstOvershotAt = curTime;
            this->_prevState = this->_state;
            return;
        } else if (this->_state == ThresholdDebouncer::OVERSHOOTING && this->_prevState == ThresholdDebouncer::OVERSHOOTING) {
            if(curTime > this->_firstOvershotAt + this-> _overshootForTimeOut ) {
              // being overshot for more than the time out
              this->_ongoingOvershootForCallback(this, ThresholdDebouncer::ONGOING_LONG_OVERSHOOT);
            }
        } else if (this->_state == ThresholdDebouncer::IDLE && this->_prevState == ThresholdDebouncer::OVERSHOOTING) {
            //Overshoot is over, check if it was a single event or a long event
            //TODO actually use debounce to make sure it's not a false stop
            this->_prevState = ThresholdDebouncer::IDLE;
            if(this->_firstOvershotAt + this-> _overshootForTimeOut > curTime) {
                //only call it we weren't in long press already
                this->_onSingleOvershootCallback(this, ThresholdDebouncer::SINGLE_OVERSHOOT);
            } else {
                this->_offLongOvershootCallback(this, ThresholdDebouncer::OFF_LONG_OVERSHOOT);
            }
        }
    }
    return;
}