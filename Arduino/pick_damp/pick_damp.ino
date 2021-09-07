#include "main.h"

Servo servo_mat_sel;  
Servo servo_damping;  
Encoder dc_encoder(MOT_ENCA, MOT_ENCB);

AccelStepper amplitude_stepper;

long enc_old_pos = -999;
long enc_new_pos = 0;
midi_msg msg = {0};

int current_amplitude = 0;
bool damped = false;

void setup(void)
{
    Serial.begin(115200);
    Serial.setTimeout(1);
    amplitude_stepper.setMaxSpeed(1000);
    // init_stepper();
    init_servos();
    // init_motor();
}

void loop(void)
{
    read_encoder(&enc_old_pos, &enc_new_pos);
    midi_read(&msg);
    undamp(1500);

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
    // if(damped)
    // {
    //     digitalWrite(StpSTP, HIGH);
    //     delay(10);
    //     digitalWrite(StpSTP, LOW);
    //     delay(10);
    // }
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
        int new_pos = map(amplitude, 0, 127, STEPPER_MIN, STEPPER_MAX);

        amplitude_stepper.moveTo(new_pos);
        amplitude_stepper.setSpeed(100);
        amplitude_stepper.runSpeedToPosition();

        current_amplitude = amplitude;
    }
    //Do picking

}

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;


/* https://gist.github.com/curiores/1ee6a942fbb7052ab3c530ad8423e419.js */
void set_dc_pos()
{
    // set target position
    //int target = 1200;
    int target = 250*sin(prevT/1e6);

    // PID constants
    float kp = 1;
    float kd = 0.025;
    float ki = 0.0;

    // time difference
    long currT = micros();
    float deltaT = ((float) (currT - prevT))/( 1.0e6 );
    prevT = currT;

    // Read the position in an atomic block to avoid a potential
    // misread if the interrupt coincides with this code running
    // see: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
    int pos = 0; 
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
    {
        pos = posi;
    }

    // error
    int e = pos - target;

    // derivative
    float dedt = (e-eprev)/(deltaT);

    // integral
    eintegral = eintegral + e*deltaT;

    // control signal
    float u = kp*e + kd*dedt + ki*eintegral;

    // motor power
    float pwr = fabs(u);
    if( pwr > 255 )
    {
        pwr = 255;
    }

    // motor direction
    int dir = 1;
    if(u<0)
    {
        dir = -1;
    }

    // signal the motor
    // setMotor(dir,pwr,PWM,IN1,IN2);


    // store previous error
    eprev = e;    
}


void damp(void)
{
    // Damp string
    if(!damped)
    {
        servo_damping.write(DAMP_ANGLE);
        damped = true;
    }
}


unsigned long damper_prev_millis = 0;

void undamp(unsigned long damp_period_millis)
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