// Define supported processors.
// -------------------------------------------------------------------------------------------------

#if !defined(__AVR_ATmega328P__)
#error "ATmega328 only is supported at the moment."
#endif

// Includes.
// -------------------------------------------------------------------------------------------------

#include <SoftwareSerial.h>

// Command definitions.
// -------------------------------------------------------------------------------------------------

const unsigned int BYTE_ORDER_MARK = 0xFEFF;

#define COMMAND_NOOP    0x01
#define COMMAND_STOP    0x02
#define COMMAND_MOVE    0x03

#define SPEED_FULL      255
#define SPEED_INVERSE   1

// Pins.
// -------------------------------------------------------------------------------------------------

#define PIN_RX           7
#define PIN_TX           8
#define PIN_AHEAD_SPEED  10
#define PIN_AHEAD_1      11
#define PIN_AHEAD_2      12
#define PIN_INDICATOR    13

SoftwareSerial bluetooth(PIN_RX, PIN_TX);

// Main loop.
// -------------------------------------------------------------------------------------------------

int blockingRead() {
    while (!bluetooth.available());
    return bluetooth.read();
}

void stop() {
    digitalWrite(PIN_AHEAD_1, LOW);
    digitalWrite(PIN_AHEAD_2, LOW);
    analogWrite(PIN_AHEAD_SPEED, SPEED_FULL);
}

bool receiveCommand() {
    switch (blockingRead()) {

        case COMMAND_NOOP:
            break;

        case COMMAND_MOVE:
            {
                int speed = blockingRead();
                int inverse = blockingRead();
                analogWrite(PIN_AHEAD_SPEED, speed);
                digitalWrite(PIN_AHEAD_1, inverse == SPEED_INVERSE ? HIGH : LOW);
                digitalWrite(PIN_AHEAD_2, inverse == SPEED_INVERSE ? LOW : HIGH);
            }
            break;

        case COMMAND_STOP:
            stop();
            break;

        default:
            return false;
    }
    return true;
}

long readVcc() {
    long vcc;

    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(1);
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    vcc = ADCL;
    vcc |= ADCH << 8;
    vcc = 1126400L / vcc;

    return vcc;
}

void sendTelemetry() {
    digitalWrite(PIN_INDICATOR, HIGH);
    bluetooth.write((char*)&BYTE_ORDER_MARK, sizeof(BYTE_ORDER_MARK));
    long vcc = readVcc();
    bluetooth.write((char*)&vcc, sizeof(vcc));
    digitalWrite(PIN_INDICATOR, LOW);
}

// Entry point.
// -------------------------------------------------------------------------------------------------

void setup() {
    pinMode(PIN_AHEAD_SPEED, OUTPUT);
    pinMode(PIN_AHEAD_1, OUTPUT);
    pinMode(PIN_AHEAD_2, OUTPUT);
    pinMode(PIN_INDICATOR, OUTPUT);
    stop();
    bluetooth.begin(9600);
}

void loop() {
    if (receiveCommand()) {
        sendTelemetry();
    }
}
