#include "main.h"

midi_msg msg = {0};

void setup(void)
{
    Serial.begin(115200);
}

void loop(void)
{

    msg = midi_read(&msg);

}

midi_msg midi_read(midi_msg *msg)
{
    do
    {
        if (Serial.available())
        {
            msg->command = Serial.read();            
            msg->note = Serial.read();
            msg->velocity = Serial.read();
        }
    }
    while (Serial.available() > 2);//when at least three bytes available
}

void pick(uint8_t amplitude)
{

}
void damp(void)
{

}