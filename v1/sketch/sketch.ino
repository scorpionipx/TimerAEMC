/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#define STATE_STOP 1
#define STATE_PAUSED 2
#define STATE_COUNTING 3


int BUTTON_START = 16;
int BUTTON_STOP = 19;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int minutes = 0;
int seconds = 0;
int miliseconds = 0;

int STATE = STATE_STOP;


void setup() {
  // set up the LCD's number of columns and rows:
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_STOP, INPUT);
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("m  s  ms");
}

void loop() 
{
  delay(50);
  if (STATE == STATE_COUNTING)
  {
    increase_time();
  }
  display_time();
  read_buttons();
}


void increase_time(void)
{
  miliseconds ++;
  if (miliseconds >= 10)
  {
    miliseconds = 0;
    seconds ++;
    if(seconds >= 60)
    {
      seconds = 0;
      minutes ++;
    }
  }
}


void display_time(void)
{
  lcd.setCursor(0,1); 
  lcd.print(minutes / 10);
  lcd.print(minutes % 10);
  lcd.print(":");
  lcd.print(seconds / 10);
  lcd.print(seconds % 10);
  lcd.print(":");
  lcd.print(miliseconds % 10);
  lcd.print(0);
}

int button_delay_ms = 300;

void read_buttons(void)
{
  switch(STATE)
  {
    case STATE_STOP:
    {
      if(digitalRead(BUTTON_START) == LOW)
      {
        STATE = STATE_COUNTING;
        delay(button_delay_ms);
      }
      if(digitalRead(BUTTON_STOP) == LOW)
      {
        STATE = STATE_PAUSED;
        delay(button_delay_ms);
      }
      break;
    }
    case STATE_PAUSED:
    {
      if(digitalRead(BUTTON_START) == LOW)
      {
        STATE = STATE_COUNTING;
        delay(button_delay_ms);
      }
      if(digitalRead(BUTTON_STOP) == LOW)
      {
        STATE = STATE_STOP;
        reset_time();
        display_time();
        delay(button_delay_ms);
      }
      break;
    }
    case STATE_COUNTING:
    {
      if(digitalRead(BUTTON_START) == LOW)
      {
        // nothing
      }
      if(digitalRead(BUTTON_STOP) == LOW)
      {
        STATE = STATE_PAUSED;
        delay(button_delay_ms);
      }
      break;
    }
    default:
    {
      break;
    }
  }
}

void reset_time(void)
{
  seconds = 0;
  minutes = 0;
  miliseconds = 0;
}

