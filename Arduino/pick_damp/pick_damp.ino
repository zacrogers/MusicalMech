#include "main.h"

Servo servo_mat_sel;  
Servo servo_damping;  
Encoder dc_encoder(MOT_ENCA, MOT_ENCB);

long enc_old_pos = -999;
long enc_new_pos = 0;
midi_msg msg = {0};

int current_amplitude = 0;
bool damped = false;

void setup(void)
{
    Serial.begin(115200);
    Serial.setTimeout(1);
    init_stepper();
    init_servos();
    init_motor();
}

void loop(void)
{
    read_encoder(&enc_old_pos, &enc_new_pos);
    midi_read(&msg);
    undamp();

    // switch (msg.command)
    // {
    //     case MIDI_NOTE_ON:
    //     {
    //         pick(msg.velocity);
    //         break;
    //     }
    //     case MIDI_NOTE_OFF:
    //     {
    //         damp();
    //         break;
    //     }
    //     case MIDI_CC:
    //     {
    //         if(msg.note == FOAM || msg.note == SILICONE)
    //         {
    //             set_damp_material((DampMaterial)msg.note);
    //         }            
    //         break; 
    //     }   
    // }

    if(msg.command == MIDI_NOTE_ON)
    {
        pick(msg.velocity);
    }
    if(msg.command == MIDI_NOTE_OFF)
    {
        damp();
    }
    if(msg.command == MIDI_CC)
    {
        if(msg.note == FOAM || msg.note == SILICONE)
        {
            set_damp_material((DampMaterial)msg.note);
        }
    }

    msg.command = 0;
    if(damped)
    {
        digitalWrite(StpSTP, HIGH);
        delay(10);
        digitalWrite(StpSTP, LOW);
        delay(10);
    }
}


void init_stepper(void)
{
    pinMode(StpMS1, OUTPUT);
    pinMode(StpMS2, OUTPUT);
    pinMode(StpMS3, OUTPUT);
    pinMode(StpRST, OUTPUT);
    pinMode(StpSLP, OUTPUT);
    pinMode(StpSTP, OUTPUT);
    pinMode(StpDIR, OUTPUT);

    //set microstep resolution
    digitalWrite(StpMS1, LOW);
    digitalWrite(StpMS2, LOW);
    digitalWrite(StpMS3, LOW);

    digitalWrite(StpRST, HIGH);
    digitalWrite(StpSLP, HIGH);    
}


void init_servos(void)
{
    servo_mat_sel.attach(SERVO1_PIN); 
    servo_damping.attach(SERVO2_PIN); 
}


void init_motor(void)
{
    pinMode(MOT_D1, OUTPUT);
    digitalWrite(MOT_D1, LOW);

    //our main speed control (uninverted)
    pinMode(MOT_D2, OUTPUT);
    digitalWrite(MOT_D2, LOW);

    pinMode(MOT_SF, INPUT);

    pinMode(MOT_EN, OUTPUT);
    digitalWrite(MOT_EN, HIGH);

    pinMode(MOT_SLEW, OUTPUT);
    digitalWrite(MOT_SLEW, LOW);

    pinMode(MOT_INV, OUTPUT);
    digitalWrite(MOT_INV, LOW);

    pinMode(MOT_IN1, OUTPUT);
    digitalWrite(MOT_IN1, HIGH);

    pinMode(MOT_IN2, OUTPUT);
    digitalWrite(MOT_IN2, LOW);
}


void read_encoder(long *enc_old_pos, long *enc_new_pos)
{
    *enc_new_pos = dc_encoder.read();

    if(*enc_new_pos != *enc_old_pos)
    {
        *enc_old_pos = *enc_new_pos;
    }
}


void midi_read(midi_msg *msg)
{
    msg->command = Serial.read();            
    msg->note = Serial.read();
    msg->velocity = Serial.read();
}


void set_damp_material(DampMaterial mat)
{
    if(FOAM == mat)
    {
        servo_mat_sel.write(DAMP_FOAM_ANGLE);
        // digitalWrite(StpDIR, HIGH);
    }
    if(SILICONE == mat)
    {
        servo_mat_sel.write(DAMP_SILICONE_ANGLE);
        // digitalWrite(StpDIR, LOW);
    }
}


void pick(uint8_t amplitude)
{
    if(amplitude != current_amplitude)
    {
        //Rotate stepper
    }
    //Do picking

}


void damp(void)
{
    // Damp string
    // servo_damping.write(DAMP_ANGLE);
    damped = true;
}


unsigned long damper_prev_millis = 0;
unsigned long damp_period_millis = 1500;

void undamp(void)
{
    if (!damped) return;
    
    // Wait a bit
    unsigned long current_millis = millis();

    if (current_millis - damper_prev_millis >= damp_period_millis) 
    {
        // Remove damper
        // servo_damping.write(NOT_DAMP_ANGLE);
        damped = false;
        damper_prev_millis = current_millis;
    }
}