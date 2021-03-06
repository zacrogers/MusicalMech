#ifndef MAIN_H
#define MAIN_H

// #include <Arduino>
#include <Servo.h>
#include <Encoder.h>
#include <stdint.h>
#include <AccelStepper.h>
#include <util/atomic.h> // For the ATOMIC_BLOCK macro

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
#define MIDI_CC       (uint8_t)0x0B

// Servo Angles for different damping materials 
#define DAMP_FOAM_ANGLE     10
#define DAMP_SILICONE_ANGLE 150

#define DAMP_ANGLE 50       //Damper in contact with string
#define NOT_DAMP_ANGLE 100 //Damper not in contact with string

// Limits for amplitude controlling stepper motor
#define STEPPER_MAX 100
#define STEPPER_MIN 0

//Constants
#define NUM_PICKS 5
#define STEPS_PER_ROTATION 1800

/* 
    Basic midi message. 
    Since theres no pitch selection note is used to select damping material.
*/
typedef struct
{
    uint8_t command, note, velocity;
}midi_msg;

typedef enum
{
    FOAM,
    SILICONE
}DampMaterial;

/* Actuator initilisation functions*/
void init_stepper(void);
void init_servos(void);
void init_motor(void);

/* Read encoder on dc motor */
void read_encoder(long *enc_old_pos, long *enc_new_pos);

/* Read midi message from serial port */
void midi_read(midi_msg *msg);

/* Rotate servo to set damping material*/
void set_damp_material(DampMaterial material);

/* Set position of amplitude stepper and rotate dc motor for picking */
void pick(uint8_t amplitude);

void set_dc_motor();
void set_dc_pos();

/* Move servo to damping position */
void damp(void);

/* 
    Polled in main loop and moves servo to undamped position after time period
    if string is currently damped
*/
void undamp(unsigned long damp_period_millis);

#endif /*MAIN_H*/
