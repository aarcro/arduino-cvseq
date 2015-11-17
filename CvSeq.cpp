/*
  CvSeq.h - Library for running a cv sequencer

  Created by Aaron McMillin (aarcro), November 16, 2015.
  MIT Licence
*/

#include "Arduino.h"
#include "CvSeq.h"

#define _PINGPONG_UP = 1;     // cycle left to right then reverse
#define _PINGPONG_DOWN = -1;  // cycle right to left then reverse
#define _FWD_MODE = 3;        // cycle left to right
#define _REV_MODE = 4;        // cycle right to left
#define _ONE_OUT = 1;         // run 16 steps to one output pin
#define _TWO_OUT = 2;         // run 8 steps, top and bottom bank to their own pin


CvSeq::CvSeq(
  int top_active,
  int bot_active,
  int top_in,
  int bot_in,
  int out1,
  int out2,
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

    pinMode(out1, OUTPUT);
    pinMode(out2, OUTPUT);
    _out1 = out1;
    _out2 = out2;

    // TODO: call reset
    _loop_mode = _FWD_MODE;
    _out_mode = _ONE_OUT;
    _cur_step = 0;
    _pingpong_dir = 1;
}

int CvSeq::step(){
    byte tmp_port;
    int ret_value;

    // Move Current
    switch(_loop_mode) {
        case _FWD_MODE:
            _cur_step++;
            // If we overflowed, reset to zero
            if (_cur_step == _out_mode * 8) {
                _cur_step = 0;
            }
            break;
        case _REV_MODE:
            // If we're at the start, reset to max
            if (_cur_step == 0) {
                _cur_step = _out_mode * 8 ;
            }
            _cur_step--;
            break;
        case _PINGPONG_UP:
            _cur_step++;
            // At the end, time to reverse
            if (_cur_step == _out_mode * 8) {
                _cur_step--;
                _loop_mode = _PINGPONG_DOWN;
            }
            break;
        case _PINGPONG_DOWN:
            // At the start, time to reverse
            if (_cur_step == 0) {
                _loop_mode = _PINGPONG_UP;
            } else {
                _cur_step--;
            }
            break;
    }

    // Set address lines
    tmp_port = PINB;            // Read PORTB (pins 8-15)
    tmp_port &= B11111000;      // Mask out the addr pins
    tmp_port |= _cur_step % 8;  // Set the addr pins with 0-7
    PORTB = tmp_port;           // Write new value

    // Read pot(s)
    _top_value = analogRead(_top_in);
    _bot_value = analogRead(_bot_in);

    // Pick Outputs
    switch (_out_mode){
        case _ONE_OUT:
            // Send top bank for low values
            // Or bottom bank for high values
            ret_value = _cur_step < 8 ? top_value : bot_value;
            break;
        case _TWO_OUT:
            // Always the top bank in double read
            ret_value = top_value;
            break;
    }

    return ret_value;
}

int CvSeq::getTopVal(){
    return _top_value;
}

int CvSeq::getBotVal(){
    return _bot_value;
}

void CvSeq::setLoop(){
    _loop_mode = _FWD_MODE;
}

void CvSeq::setReverse(){
    _loop_mode = _REV_MODE;
}

void CvSeq::setPingPong(){
    _loop_mode = _PINGPONG_MODE;
}

void CvSeq::setOneSixteen(){
    _out_mode = _ONE_OUT;
}

void CvSeq::setTwoEights(){
    _out_mode = _TWO_OUT;
}
