/*
  CvSeq.h - Library for running a cv sequencer

  Created by Aaron McMillin (aarcro), November 16, 2015.
  MIT Licence
*/

#include "Arduino.h"
#include "CvSeq.h"

#define _PINGPONG_UP 1     // cycle left to right then reverse
#define _PINGPONG_DOWN -1  // cycle right to left then reverse
#define _FWD_MODE 3        // cycle left to right
#define _REV_MODE 4        // cycle right to left
#define _ONE_OUT 2         // run (2*8) 16 steps to one output pin
#define _TWO_OUT 1         // run (1*8) 8 steps, top and bottom bank to their own pin


CvSeq::CvSeq(
  int top_active,
  int bot_active,
  int top_in,
  int bot_in
){
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    pinMode(top_active, OUTPUT);
    pinMode(bot_active, OUTPUT);
    _top_active = top_active;
    _bot_active = bot_active;

    _top_in = top_in;
    _bot_in = bot_in;

    // TODO: call reset
    _loop_mode = _FWD_MODE;
    _out_mode = _ONE_OUT;
    _cur_step = -1;
}

int CvSeq::step(){
    byte tmp_port;
    int ret_value;

    // Move Current
    switch(_loop_mode) {
        case _FWD_MODE:
            _cur_step++;
            // If we overflowed, reset to zero
            if (_cur_step >= _out_mode * 8) {
                _cur_step = 0;
            }
            break;
        case _REV_MODE:
            // If we're at the start, reset to max
            if (_cur_step <= 0) {
                _cur_step = _out_mode * 8 ;
            }
            _cur_step--;
            break;
        case _PINGPONG_UP:
            _cur_step++;
            // At the end, time to reverse
            if (_cur_step >= _out_mode * 8) {
                _cur_step--;
                _loop_mode = _PINGPONG_DOWN;
            }
            break;
        case _PINGPONG_DOWN:
            // At the start, time to reverse
            if (_cur_step <= 0) {
                _loop_mode = _PINGPONG_UP;
            } else {
                _cur_step--;
            }
            break;
    }

    // Set address lines
    tmp_port = PINB;            // Read PORTB (pins 8-15)
    Serial.println("read " + String(tmp_port));
    tmp_port &= B11111000;      // Mask out the addr pins
    Serial.println("masked " + String(tmp_port));
    tmp_port |= _cur_step % 8;  // Set the addr pins with 0-7
    Serial.println("write " + String(tmp_port));
    PORTB = tmp_port;           // Write new value

    // Read pot(s)
    _top_value = analogRead(_top_in);
    _bot_value = analogRead(_bot_in);

    // Pick Outputs
    switch (_out_mode){
        case _ONE_OUT:
            // Send top bank for low values
            // Or bottom bank for high values
            ret_value = _cur_step < 8 ? _top_value : _bot_value;
            digitalWrite(_top_active, _cur_step < 8 ? LOW : HIGH);  // active low
            digitalWrite(_bot_active, _cur_step > 7 ? LOW : HIGH);  // active low
            break;
        case _TWO_OUT:
            // Always the top bank in double read
            ret_value = _top_value;
            // Both lights active
            digitalWrite(_top_active, LOW);
            digitalWrite(_bot_active, LOW);
            break;
    }

    return ret_value;
}

int CvSeq::getStep(){
    return _cur_step;
}

int CvSeq::getPrimary(){
    int ret_value;
    switch (_out_mode){
        case _ONE_OUT:
            // Pick top (low count) or bottom (high count)
            return _cur_step < 8 ? _top_value : _bot_value;
        case _TWO_OUT:
            // Always the top bank in double read
            return  _top_value;
    }
}

int CvSeq::getSecondary(){
    if (_out_mode == _TWO_OUT) {
        return _bot_value;
    } else {
        return -1;
    }
}

bool CvSeq::isDoubleOut(){
    return _out_mode == _TWO_OUT;
}

void CvSeq::setLoop(){
    _loop_mode = _FWD_MODE;
}

void CvSeq::setReverse(){
    _loop_mode = _REV_MODE;
}

void CvSeq::setPingPong(){
    switch(_loop_mode){
        // If going backwards, stay backwards
        case _REV_MODE:
        case _PINGPONG_DOWN:
            _loop_mode = _PINGPONG_DOWN;
            break;
        default:
            _loop_mode = _PINGPONG_UP;
            break;
    }
}

void CvSeq::setOneSixteen(){
    _out_mode = _ONE_OUT;
}

void CvSeq::setTwoEights(){
    _out_mode = _TWO_OUT;
    // make sure step is 0-7
    _cur_step = _cur_step % 8;
}
