// Define supported processors.
// ------------------------------------------------------------------------------------------------

#if !defined(__AVR_ATmega328P__)
#error "ATmega328 only is supported at the moment."
#endif

// Includes.
// ------------------------------------------------------------------------------------------------

#include <SoftwareSerial.h>

// Command definitions.
// ------------------------------------------------------------------------------------------------

#define MAGIC 0x55

// No operation.
#define COMMAND_NOOP 'N'
// Full stop.
#define COMMAND_STOP 'S'
// Go. Read direction and power.
#define COMMAND_GO   'G'

#define DIRECTION_FORWARDS  0
#define DIRECTION_BACKWARDS 1

// Globals.
// ------------------------------------------------------------------------------------------------

// Bluetooth module: RX, TX.
SoftwareSerial bluetooth(7, 8);

// Debug indicator.
#define DEBUG_PIN 13

// Main loop.
// ------------------------------------------------------------------------------------------------

char receive_command() {
    char command = bluetooth.read();
    switch (command) {

        case COMMAND_NOOP:
            break;

        default:
            return 0;
    }
    return 1;
}

long read_vcc() {
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

void send_telemetry() {
    digitalWrite(DEBUG_PIN, HIGH);
    
    bluetooth.write(MAGIC);

    long vcc = read_vcc();
    bluetooth.write((char*)&vcc, sizeof(vcc));

    digitalWrite(DEBUG_PIN, LOW);
}

// Entry point.
// ------------------------------------------------------------------------------------------------

void setup() {
    pinMode(DEBUG_PIN, OUTPUT);
    bluetooth.begin(9600);
}

void loop() {
    if (receive_command()) {
        send_telemetry();
    }
}
