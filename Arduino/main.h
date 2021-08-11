#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#define MIDI_NOTE_OFF (uint8_t)0x80
#define MIDI_NOTE_ON  (uint8_t)0x90

// DC Motor pins
#define MOT_D1   4
#define MOT_D2   5
#define MOT_IN1  6
#define MOT_IN2  7
#define MOT_SF   14
#define MOT_EN   15
#define MOT_SLEW 16
#define MOT_INV  17

// Servo pins
#define SERVO1_PIN 47
#define SERVO2_PIN 48

// Solenoid pins
#define SOL1  49
#define SOL2  46

typedef struct
{
    uint8_t command, note, velocity;
}midi_msg;


midi_msg midi_read(midi_msg *msg);
void pick(int amplitude);
void damp(void);

#endif /*MAIN_H*/
