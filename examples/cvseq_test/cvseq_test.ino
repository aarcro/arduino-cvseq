#include <CvSeq.h>

#define LED_PIN 13
#define OUT_PIN_1 3

/*
    To play around with the Sequencer, wire a photo resitor to A0
    and an LED + speaker to pin 3
*/


CvSeq seq(
    2,  // int top_active
    3,  // int bot_active
    0,  // int top_in
    1   // int bot_in
);

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_PIN, OUTPUT);
    pinMode(OUT_PIN_1, OUTPUT);

    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    Serial.begin(9600);
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

    // With only one input wired up, use 8 step, ignore secondary
    seq.setTwoEights();

    int note = seq.step();
    int val = map(note, 900, 1023, 0, 240);
    /*
    Serial.println(
        "Step: " + String(seq.getStep()) +
        " Primary: " + String(note) +
        " Primary Note: " + String(val) +
        " Secondary: " + String(seq.getSecondary())
    );
    */

    analogWrite(OUT_PIN_1, val);
    // TODO Tempo factor
    delay(300);
}
