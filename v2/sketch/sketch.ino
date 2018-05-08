#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define STATE_INIT 0
#define STATE_STOP 1
#define STATE_PAUSED 2
#define STATE_COUNTING 3

int BUTTON_START = 5;
int BUTTON_PAUSE = 6;
int BUTTON_STOP = 7;

int hours = 0;
int minutes = 0;
int seconds = 0;

int counter_200_ms = 0;

int STATE = STATE_INIT;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

void display_time(void);
void reset_time(void);
void increase_time(void);
void task_200_ms(void);
void read_buttons(void);

void setup()
{
  pinMode(BUTTON_START, INPUT);
  pinMode(BUTTON_PAUSE, INPUT);
  pinMode(BUTTON_STOP, INPUT);
  
  lcd.begin(16,2);   
  
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(100/16);
    lcd.noBacklight();
    delay(100/16);
  }
  lcd.backlight(); 

  lcd.setCursor(0,0); 
  lcd.print("Cronometru");
  delay(1000/16);
  lcd.setCursor(0,1);
  lcd.print("AEMC");
  delay(2000/16);  

  lcd.clear();
  lcd.setCursor(4,0); 
  //lcd.print("00:00:00");
  display_time();
  lcd.setCursor(0,1);
  lcd.print("START PAUSE STOP");

  STATE = STATE_STOP;
}

void loop()
{
  task_200_ms();
  delay(200/16);
}

void task_200_ms(void)
{
  read_buttons();
  switch(STATE)
  {
    case STATE_COUNTING:
    {
      counter_200_ms ++;
      if (counter_200_ms >= 5)
      {
        counter_200_ms = 0;
        increase_time();
        display_time();
      }
    }
  }
}

void increase_time(void)
{
  seconds ++;
  if (seconds >= 60)
  {
    seconds = 0;
    minutes ++;
    if(minutes >= 60)
    {
      minutes = 0;
      hours ++;
    }
  }
}

void reset_time(void)
{
  seconds = 0;
  minutes = 0;
  hours = 0;
}

void display_time(void)
{
  lcd.setCursor(4,0); 
  lcd.print(hours / 10);
  lcd.print(hours % 10);
  lcd.print(":");
  lcd.print(minutes / 10);
  lcd.print(minutes % 10);
  lcd.print(":");
  lcd.print(seconds / 10);
  lcd.print(seconds % 10);
}

void read_buttons(void)
{
  switch(STATE)
  {
    case STATE_STOP:
    {
      if(digitalRead(BUTTON_START) == LOW)
      {
        STATE = STATE_COUNTING;
      }
      if(digitalRead(BUTTON_PAUSE) == LOW)
      {
        // nothing
      }
      if(digitalRead(BUTTON_STOP) == LOW)
      {
        // nothing
      }
      break;
    }
    case STATE_PAUSED:
    {
      if(digitalRead(BUTTON_START) == LOW)
      {
        STATE = STATE_COUNTING;
      }
      if(digitalRead(BUTTON_PAUSE) == LOW)
      {
        // nothing
      }
      if(digitalRead(BUTTON_STOP) == LOW)
      {
        STATE = STATE_STOP;
        reset_time();
        display_time();
      }
      break;
    }
    case STATE_COUNTING:
    {
      if(digitalRead(BUTTON_START) == LOW)
      {
        // nothing
      }
      if(digitalRead(BUTTON_PAUSE) == LOW)
      {
        STATE = STATE_PAUSED;
      }
      if(digitalRead(BUTTON_STOP) == LOW)
      {
        STATE = STATE_STOP;
        reset_time();
        display_time();
      }
      break;
    }
    default:
    {
      break;
    }
  }
}



