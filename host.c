#define CLOCK  2
#define LATCH  3
#define DATA   4

#define BUTTON_A      8
#define BUTTON_B      0
#define BUTTON_X      9
#define BUTTON_Y      1
#define BUTTON_L      10
#define BUTTON_R      11
#define BUTTON_SELECT 2
#define BUTTON_START  3
#define BUTTON_UP     4
#define BUTTON_DOWN   5
#define BUTTON_LEFT   6
#define BUTTON_RIGHT  7

typedef union
{
     uint16_t  state_word;
     
     struct
     {
          uint8_t high;
          uint8_t low;
     } state_byte;
          
} buttonState_t;

int data = 0;
/* Top 4 bits are always set */
buttonState_t buttonState = {0xF000};


void setup()
{
     /* Pins 2-4 correspond to the pins 2-4 on the controller */
     /* Clock is normally high */
     pinMode(CLOCK,OUTPUT);   /* Data Clock */
     digitalWrite(CLOCK,HIGH);
     
     pinMode(LATCH,OUTPUT);   /* Data Latch */
     digitalWrite(LATCH,LOW);

     /* A Low data bit means the button is activated */
     pinMode(DATA,INPUT);     /* Serial Data Out */
     
     Serial.begin(9600);
}

void loop()
{
     requestData();
     getData();
     exportData();
     delay(60);
}

void requestData()
{
     /* Send a 12us wide high signal to begin polling for data */
     digitalWrite(LATCH,HIGH);
     delayMicroseconds(12);
     /* 6us after sending the latch to low, data will arrive on data pin */
     digitalWrite(LATCH,LOW);
     delayMicroseconds(6);
}
void getData()
{
     /* Send 16 clock signals, 50% duty cycle, 12us period */
     for(int i = 0; i < 16; ++i)
     {
          digitalWrite(CLOCK,LOW);
          delayMicroseconds(6);

          /* Only the first 12 pulses represent the buttons */
          if(i < 12)
               if(digitalRead(DATA) == HIGH)
                    buttonState.state_word |= (1 << i);
               else
                    buttonState.state_word &= ~(1 << i);
          
          digitalWrite(CLOCK,HIGH);
          delayMicroseconds(6);
     }
}

void exportData()
{
     /* This will write the serial data to the port */
     Serial.write(buttonState.state_byte.high);
     Serial.write(buttonState.state_byte.low);
}