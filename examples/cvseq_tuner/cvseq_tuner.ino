#include <CvSeq.h>
#include <werkstatt.h>

#define LED_PIN 13
#define OUT_PIN_1 6
#define OUT_PIN_2 5
#define TEMPO_PIN 2

/*
    For Tuning werkstatt EXP in. Tempo driven.
    Raw voltage reads pots, encoded plays sequence defined below
*/

arpeggiator arpeggiator(OUT_PIN_1); //initiate and name the arpeggiator class (Output pin)

CvSeq seq(
    2,  // int top_active
    3,  // int bot_active
    0,  // int top_in (Analog)
    1   // int bot_in (Analog)
);

int notes[] = {tonic, major3rd, fifth, octave, fifth, major3rd}; // VCO EXP config
// int notes[] = {tonic, octave}; // VCO EXP config


// note values: w, h, q, qt, e, et, sx, sxt, th, sxf 
int note_values[] = {q, q, q, q, q, q, q, q, q, q, e}; //VCO EXP config

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_PIN, OUTPUT);
    pinMode(OUT_PIN_1, OUTPUT);
    pinMode(OUT_PIN_2, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    int tempo = map(analogRead(TEMPO_PIN), 0, 1023, 60, 400);
    for (int i = 0; i < ( sizeof(notes)/sizeof(int) ); i++) {
        // Just flash the lights
        int note = seq.step();

        // Quant Tempo off Analog input
        // Sleep in ms.
        // 60 bpm = 1000ms
        // 300 bpm = 200ms
        // 600 bpm = 100ms
        // 60 to 2400 bpm (exponetial scale)
        arpeggiator.play(tempo, notes[i], note_values[i]); 
    }
}
