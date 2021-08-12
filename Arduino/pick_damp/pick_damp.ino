#include "main.h"

Servo servo_1;  
Servo servo_2;  


midi_msg msg = {0};

void setup(void)
{
    Serial.begin(115200);
    Serial.setTimeout(1);
    init_stepper();
    // init_servos();
    // init_motor();
}

void loop(void)
{
    midi_read(&msg);

    if(msg.command == MIDI_NOTE_ON)
    {
        digitalWrite(StpDIR, HIGH);
    }
    if(msg.command == MIDI_NOTE_OFF)
    {
        digitalWrite(StpDIR, LOW);
    }

    digitalWrite(StpSTP, HIGH);
    delay(10);
    digitalWrite(StpSTP, LOW);
    delay(10);
}


void init_stepper()
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


void init_servos()
{
    servo_1.attach(SERVO1_PIN); 
    servo_2.attach(SERVO2_PIN); 
}


void init_motor()
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


void midi_read(midi_msg *msg)
{
    msg->command = Serial.read();            
    msg->note = Serial.read();
    msg->velocity = Serial.read();
}


void pick(uint8_t amplitude)
{

}


void damp(void)
{

}