/*
  CvSeq.h - Library for running a cv sequencer

  The Circuit:
    * Input - 2x 4051 multiplexer 8x 10k pot each
    * Output - 2x 4051 multiplexer 8x LED each to align with input pot
    *   Z (input) held high

  Pins:
    * addr[1,2,3] - Digital pins to write multiplex addresses
    * top_active - Digital pin E on the top bank (active low)
    * bot_active - Digital pin E on the bottom bank (active low)
    * top_in - Analog pin to read top bank
    * bot_in - Analog pin to read bottom bank

  Created by Aaron McMillin (aarcro), November 16, 2015.
  MIT Licence
*/
#ifndef CvSeq_h
#define CvSeq_h

#include "Arduino.h"

class CvSeq
{
  public:
    CvSeq(
        int top_active,
        int bot_active,
        int top_in,
        int bot_in,
    );
    int step();              // run the next step
    void setLoop();
    void setReverse();
    void setPingPong();
    void setOneSixteen();
    void setTwoEights();
    int getTopVal();
    int getBotVal();
  private:
    int _loop_mode;
    int _out_mode;
    int _top_active,
    int _bot_active,
    int _top_in,
    int _bot_in,
    int _top_value,
    int _bot_value,
    int _out1,
    int _out2,
    int _cur_step,
};

#endif
