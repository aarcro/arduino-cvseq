#include <CvSeq.h>

#define LED_PIN 13

CvSeq seq(
    2,  // int top_active
    3,  // int bot_active
    0,  // int top_in
    1   // int bot_in
);

void setup() {
    pinMode(LED_PIN, OUTPUT);

    // put your setup code here, to run once:
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

    int note = seq.step();
    Serial.println(
        "Step: " + String(seq.getStep()) +
        " Primary: " + String(note) +
        " Secondary: " + String(seq.getSecondary())
    );
    // TODO Tempo factor
    delay(500);
}
