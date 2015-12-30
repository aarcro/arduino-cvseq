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

//Arpeggiator define
// vvv #include <werkstatt.h> vvv

#define tonic 0
#define minor2nd 5
#define major2nd 10
#define minor3rd 15
#define major3rd 20
#define fourth 25
#define tritone 30
#define fifth 35
#define minor6th 40
#define major6th 45
#define minor7th 50
#define major7th 55
#define octave 60
#define octave2 120

// ^^^ #include <werkstatt.h> ^^^

// Major
// int scale[] = {tonic, major2nd, major3rd, fourth, fifth, major6th, major7th};

// Minor
// int scale[] = {tonic, major2nd, minor3rd, fourth, fifth, minor6th, minor7th};

// Dorian
// int scale[] = {tonic, major2nd, minor3rd, fourth, fifth, major6th, minor7th};

// Phrygian
// int scale[] = {tonic, minor2nd, minor3rd, fourth, fifth, minor6th, minor7th};

// Locrian
// int scale[] = {tonic, minor2nd, minor3rd, fourth, tritone, minor6th, minor7th};

// Major Pentatonic (Jazz)
//int scale[] = {tonic, major2nd, major3rd, fifth, major6th};

// Blues
int scale[] = {tonic, major2nd, minor3rd, fourth, fifth, major6th, minor7th};

int scale_size = sizeof(scale)/sizeof(int);
// Repeat scale OCTAVES times, plus one note to end on tonic
int idx_range = (scale_size * OCTAVES) + 1;


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

    // seq.setOneSixteen();
    // Run 2x8
    seq.setTwoEights();
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
    // int val = map(note, 0, 1023, 0, 24);

    // Map input to range of Octaves plus one note (final tonic)
    int idx = map(note, 0, 1023, 0, idx_range);
    // int mod -> which degree
    // int div -> which octave
    int val = scale[(idx % scale_size)] + (octave * (idx / scale_size));

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

    // Write Pin1 Scale tone
    analogWrite(OUT_PIN_1, val);

    // Write Pin2
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
