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

#define DIRECTION_FORWARDS      0x01
#define DIRECTION_BACKWARDS     0x02

// Pins.
// -------------------------------------------------------------------------------------------------

#define INDICATOR_PIN   13

SoftwareSerial bluetooth(/* RX */ 7, /* TX */ 8);

// Main loop.
// -------------------------------------------------------------------------------------------------

bool receiveCommand() {
    switch (bluetooth.read()) {

        case COMMAND_NOOP:
            break;

        case COMMAND_MOVE:
            // TODO.
            break;

        case COMMAND_STOP:
            // TODO.
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
    digitalWrite(INDICATOR_PIN, HIGH);
    bluetooth.write((char*)&BYTE_ORDER_MARK, sizeof(BYTE_ORDER_MARK));
    long vcc = readVcc();
    bluetooth.write((char*)&vcc, sizeof(vcc));
    digitalWrite(INDICATOR_PIN, LOW);
}

// Entry point.
// -------------------------------------------------------------------------------------------------

void setup() {
    pinMode(INDICATOR_PIN, OUTPUT);
    bluetooth.begin(9600);
}

void loop() {
    if (receiveCommand()) {
        sendTelemetry();
    }
}
