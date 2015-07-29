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

// Maximum command payload size.
#define MAX_PAYLOAD_SIZE 1

// Full stop.
#define COMMAND_STOP 0
// Go. Read direction and power.
#define COMMAND_GO   1

// Globals.
// ------------------------------------------------------------------------------------------------

// Bluetooth module: RX, TX.
SoftwareSerial bluetooth(12, 13);

// Main loop.
// ------------------------------------------------------------------------------------------------

void receive_command() {
    // TODO.
}

void send_telemetry() {
    // TODO.
}

// Entry point.
// ------------------------------------------------------------------------------------------------

void setup() {
    bluetooth.begin(9600);
}

void loop() {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
}
