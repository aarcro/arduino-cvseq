#include <CvSeq.h>

#define LED_PIN 13
#define OUT_PIN_1 6
#define OUT_PIN_2 5
#define TEMPO_PIN 2

#define OCTAVES 3

/*
    To play around with the Sequencer, wire a photo resitor to A0
    and an LED + speaker to pin 3
*/

CvSeq seq(
    12,  // int top_active
    11,  // int bot_active
    0,  // int top_in (Analog)
    1   // int bot_in (Analog)
);

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_PIN, OUTPUT);
    pinMode(OUT_PIN_1, OUTPUT);
    pinMode(OUT_PIN_2, OUTPUT);

    Serial.begin(9600);

    // Run 2x8
    seq.setOneSixteen();
    // seq.setTwoEights();

}

void loop() {
    int data;

    // put your main code here, to run repeatedly:
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    if (Serial.available() > 0) {
        // Adjust for ASCII offset
        data = Serial.read() - 48;
        Serial.println("  Data: " + String(data));

        switch(data) {
            case 1:
                seq.setLoop();
                Serial.println("FWD");
                break;
            case 2:
                seq.setReverse();
                Serial.println("RWD");
                break;
            case 3:
                seq.setPingPong();
                Serial.println("PingPong");
                break;
            case 8:
                seq.setTwoEights();
                Serial.println("Two by Eight");
                break;
            case 16:
                seq.setOneSixteen();
                Serial.println("One by Sixteen");
                break;
        }
    }

    // step and Quant Primary
    int note = seq.step();
    int val = map(note, 0, 1023, 0, 24);

    // Quant Secondary
    int val2= map(seq.getSecondary(), 0, 1023, 0, 24);
    /*
    Serial.println(
        "Step: " + String(seq.getStep()) +
        " Primary: " + String(note) +
        " Primary Note: " + String(val) +
        " Secondary: " + String(seq.getSecondary())
    );
    */

    // Write
    analogWrite(OUT_PIN_1, val * 5);
    analogWrite(OUT_PIN_2, val2 * 5);

    // Quant Tempo off Analog input
    // Sleep in ms.
    // 60 bpm = 1000ms
    // 300 bpm = 200ms
    // 600 bpm = 100ms
    // 60 to 2400 bpm (exponetial scale)
    int tempo = map(analogRead(TEMPO_PIN), 0, 1023, 33, 5);
    delay(tempo * tempo);
}
