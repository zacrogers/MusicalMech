#ifndef MAIN_H
#define MAIN_H

// #include <Arduino>
#include <Servo.h>
#include <Encoder.h>
#include <stdint.h>

/* Pin assignments */
// DC Motor pins
#define MOT_ENCA 2
#define MOT_ENCB 3
#define MOT_D1   4
#define MOT_D2   5
#define MOT_SF   6
#define MOT_EN   7
#define MOT_SLEW 8
#define MOT_INV  9
#define MOT_IN1  10
#define MOT_IN2  11

// Servo pins
#define SERVO1_PIN 47
#define SERVO2_PIN 48

// Solenoid pins
#define SOL1  49
#define SOL2  46

// Stepper pins
#define StpRef A15
#define StpMS1 36
#define StpMS2 34
#define StpMS3 32
#define StpRST 30
#define StpSLP 28
#define StpSTP 26
#define StpDIR 24

// Midi defines
#define MIDI_NOTE_OFF (uint8_t)0x80
#define MIDI_NOTE_ON  (uint8_t)0x90
#define MIDI_CC       (uint8_t)0x44C


// Servo Angles for different damping materials 
#define DAMP_FOAM_ANGLE     0
#define DAMP_SILICONE_ANGLE 180

typedef struct
{
    uint8_t command, note, velocity;
}midi_msg;

typedef enum
{
    FOAM,
    SILICONE
}DampMaterial;

void init_stepper(void);
void init_servos(void);
void init_motor(void);
void read_encoder(long *enc_old_pos, long *enc_new_pos);

void midi_read(midi_msg *msg);
void set_damp_material(DampMaterial mat);
void pick(int amplitude);
void damp(void);

#endif /*MAIN_H*/
