/*
* Utility Class to use sensisive touch from CircuitPlayground
*/

#include "Arduino.h"
#include "ThresholdDebouncer.h"


ThresholdDebouncer::ThresholdDebouncer(uint8_t pin, unsigned long threshold, unsigned long debounce) {
    this->pin = pin;
    this->_threshold = threshold;
    this->_debounce = debounce;
}

ThresholdDebouncer& ThresholdDebouncer::onPress(callback_t callback) {
  this->_onPressCallback = callback;
  return *this;
}

/* not supported for now
ThresholdDebouncer& ThresholdDebouncer::onDoublePress(callback_t callback, unsigned long timeout) {
  this->_onDoublePressCallback = callback;
  this->_doublePressTimeout = timeout;
  return *this;
}*/

ThresholdDebouncer& ThresholdDebouncer::onPressFor(callback_t callback, unsigned long timeout) {
  this->_onPressForCallback = callback;
  this->_pressForTimeOut = timeout;
  return *this;
}

ThresholdDebouncer& ThresholdDebouncer::offLongPress(callback_t callback) {
  this->_offLongPressCallback = callback;
  return *this;
}

uint8_t ThresholdDebouncer::getPin(){
    return this->pin;
}

/*
ThresholdDebouncer& ThresholdDebouncer::updateState(touch_state_t state) {
  //this->_prevState = this->_state;
  this->_state = state;
  return *this;
}*/

/* Analyse the value coming from read and callback if needed
*/
void ThresholdDebouncer::reactToRead(uint16_t externalReadValue) {
    this->readValue = externalReadValue;
    unsigned long curTime = millis();
    if (curTime - this->_lastLoop >= this->_loopInterval) {
        this->_lastLoop = curTime;
        
        //Filter noisy values
        if (this->readValue >this->_threshold) {
        this->_state = ThresholdDebouncer::TOUCHING;
        } else {
        this->_state = ThresholdDebouncer::IDLE;
        }

        if(this->_state == ThresholdDebouncer::TOUCHING && this->_prevState == ThresholdDebouncer::IDLE) {
            //start counting for how long
            this->_firstPressedAt = curTime;
            this->_prevState = this->_state;
            return;
        } else if (this->_state == ThresholdDebouncer::TOUCHING && this->_prevState == ThresholdDebouncer::TOUCHING) {
            if(curTime > this->_firstPressedAt + this-> _pressForTimeOut ) {
                this->_onPressForCallback(this, ThresholdDebouncer::ONGOING_LONG_PRESS);
            }
        } else if (this->_state == ThresholdDebouncer::IDLE && this->_prevState == ThresholdDebouncer::TOUCHING) {
            this->_prevState = ThresholdDebouncer::IDLE;
            if(this->_firstPressedAt + this-> _pressForTimeOut > curTime) {
                //only call it we weren't in long press already
                this->_onPressCallback(this, ThresholdDebouncer::SINGLE_PRESS);
            } else {
                this->_offLongPressCallback(this, ThresholdDebouncer::OFF_LONG_PRESS);
            }
        }
    }
    return;
}