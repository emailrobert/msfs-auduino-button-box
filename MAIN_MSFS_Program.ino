#include <HID-Project.h>
#include <HID-Settings.h>

int row11 = 0;
int row12 = 1;
int row13 = 2;
int row14 = 3;
int row21 = 7;
int row22 = 8;
int row23 = 9;
int row24 = 10;
int sensorValue = 0;

#define CLK 4
#define DT 5
#define SW 6

int state;
int Laststate;


int curCWButton = 5;
int curCCWButton = 15;

int period = 100;
int radiochangeperiod = 2000;
int firstcapture = 0;
int firstcapture1 = 0;

unsigned long timenow;
unsigned long timenow1;

int row11state;
int row12state;
int row13state;
int row14state;

int togglePress = 0;

void setup() {
    pinMode(row11, INPUT);
    pinMode(row12, INPUT);
    pinMode(row13, INPUT);
    pinMode(row14, INPUT);
    Gamepad.begin();
    Serial.begin(9600);
    Laststate = digitalRead(CLK);
    row11state = digitalRead(row11);
    row12state = digitalRead(row12);
    row13state = digitalRead(row13);
    row14state = digitalRead(row14);
}

void loop() {


    /////// BLOCK OF CODE FOR MSFS RADIO TUNING
    /////// TUNE WHOLE NUMBERS THEN SHORT PRESS AND TUNE FRACTIONAL NUMBERS
    /////// LONG PRESS TO SWITCH STANDBY RADIO TO PRIMARY
    
    state = digitalRead(CLK);
    
    if(state != Laststate)
    {
      if(digitalRead(DT) != state) // IF ENCODER IS TURNED CW
      {
          timenow = millis();              ////// MSFS IS ONLY LOOKING FOR A PULSE OF SIGNAL FOR THE SWITCH TURN.
          Gamepad.press(curCWButton);      ////// BY DEFAULT THE GAMEPAD.PRESS COMMAND LEAVES THE BUTTON HOT AND SENDS CONTINUOS BUTTON PRESS TO SOFTWARE AND THAT IS BAD
      }                                    ////// SO WE SET A TIMER (TIMENOW) SO WE CAN RELEASE THE BUTTON AFTER 300 MS IN THE ELSE STATEMENT
      else   // IF ENCODER IS TURNED CCW
      {
          timenow = millis();
          Gamepad.press(curCCWButton);
      }
    }
    else   ////// 
    {
        if(millis() >= timenow + period)
        {
            Gamepad.release(curCWButton);
            Gamepad.release(curCCWButton);
        }
    }

    if(digitalRead(SW) != 1)   // IF ENCODER IS PRESSED
    {     
        togglePress = 1;
        if(firstcapture == 0)    ////// SINCE WE ARE IN A CONTINOUS ARDUINO LOOP WE ONLY WANT TO BE NOTIFIED OF THE VERY FIRST ENCODER PRESS
        {                        ////// SO WE SET A TOGGLEPRESS VARIABLE TO 1 TO LET CODE DOWNSTREAM KNOW WE ARE IN A PRESSED STATE
            timenow = millis();  ////// THE FIRSTCAPTURE VARIABLE GUARANTEES THAT OUR TIMER TO RELEASE THE BUTTON PRESS IS ONLY CAPTURED ONE TIME IN THE LOOP
            firstcapture = 1;
        }
    }
    else
    {
        firstcapture1 = 0;     /////// ONCE THE ENCODER BUTTON IS FINALLY RELEASED EITHER FROM A LONG OR SHORT PRESS, RESET THE FIRSTCAPTURES TO START OVER IN CASE OF ANOTHER PRESS
        firstcapture = 0;
    }

    if(digitalRead(SW) != 1 && millis() >= timenow + radiochangeperiod)     //////// LONG PRESS DETECTION. IF THE BUTTON IS HELD FOR THE RADIOCHANGEPERIOD IT WILL SEND A GAMEPAD 7 PRESS
        {                                                                   //////// AND ALSO SET A TIMER TO RELEASE THE GAMEPAD 7 AFTER 300 MS BECAUSE WE ONLY NEED A PULSE TO CHANGE THE RADIOS
          if(firstcapture1 == 0){                                           //////// THE FIRSTCAPTURE1 VARIABLE GUARANTEES THAT OUR GAMEPAD PRESS COMMAND IS ONLY FIRED ONE TIME IN THE LOOP
              Gamepad.press(7);                                             //////// ONCE IT IS FIRED, WE SET FIRSTCAPTURE1 TO 1 SO IT WONT SEND ANOTHER GAMEPAD PRESS COMMAND UNTIL WE RELEASE THE BUTTON AND DO ANOTHER LONG PRESS
              Serial.println(firstcapture1);
              timenow1 = millis();
              firstcapture1 = 1;
          }
        }

    if(millis() >= timenow1 + period)
    {
        Gamepad.release(7);
    }
        
    if(digitalRead(SW) == 1 && togglePress == 1)  //////// FOR A SHORT PRESS WAIT FOR THE BUTTON STATE TO COME BACK OUT BEFORE CHANGING OVER FROM WHOLE TO FRACTIONAL RADIO DIALS 
    {                                             //////// WITH BUTTON RELEASED AND TOGGLEPRESS VARIABLE AT 1 WE ARE TELLING THE CODE TO SWITCH FROM WHOLE TO FRACTIONAL AND VICE VERSA
      if (curCWButton == 5)
        {
            curCWButton = 6;
            curCCWButton = 16;
        }
        else{
            curCWButton = 5;
            curCCWButton = 15;
        }
        togglePress = 0;
    }

    Laststate=state;
    
    if(digitalRead(row11) != row11state){          ////// MSFS IS ONLY LOOKING FOR A PULSE OF SIGNAL FOR THE TOGGLE SWITCH. A HIGH SWITCH SIGNAL SENDS ONE COMMAND AND A LOW SWITCH SIGNAL SENDS A SECOND COMMAND
      timenow = millis();                          ////// SO FOR ALL THE BUTTONS BELOW WE SEND THESE GAMEPAD SIGNALS FOR EACH STATE AND THEN SET A TIMER TO RELEASE THE COMMANDS AFTER 300 MS
      if (digitalRead(row11) == HIGH)
      {
          Gamepad.press(1);
      }
      else
      {
          Gamepad.press(11);        
      }  
      row11state = digitalRead(row11);
      }
    else
    {
        if(millis() >= timenow + period)
        {
            Gamepad.release(1);
            Gamepad.release(11);
        }
    }

    if(digitalRead(row12) != row12state){
      timenow = millis();
      if (digitalRead(row12) == HIGH)
      {
          Gamepad.press(2);
      }
      else
      {
          Gamepad.press(12);        
      }  
      row12state = digitalRead(row12);
      }
    else
    {
        if(millis() >= timenow + period)
        {
            Gamepad.release(2);
            Gamepad.release(12);
        }
    }

    if(digitalRead(row13) != row13state){
      timenow = millis();
      if (digitalRead(row13) == HIGH)
      {
          Gamepad.press(3);
      }
      else
      {
          Gamepad.press(13);        
      }  
      row13state = digitalRead(row13);
      }
    else
    {
        if(millis() >= timenow + period)
        {
            Gamepad.release(3);
            Gamepad.release(13);
        }
    }

    if(digitalRead(row14) != row14state){
      timenow = millis();
      if (digitalRead(row14) == HIGH)
      {
          Gamepad.press(4);
      }
      else
      {
          Gamepad.press(14);        
      }  
      row14state = digitalRead(row14);
      }
    else
    {
        if(millis() >= timenow + period)
        {
            Gamepad.release(4);
            Gamepad.release(14);
        }
    }

    if (digitalRead(row21) == HIGH)
      {
          Gamepad.press(8);
      }
      else
      {
          Gamepad.release(8);        
      }  
      if (digitalRead(row22) == HIGH)
      {
          Gamepad.press(9);
      }
      else
      {
          Gamepad.release(9);        
      }  

      if (digitalRead(row23) == HIGH)
      {
          Gamepad.press(10);
      }
      else
      {
          Gamepad.release(10);        
      }  

      if (digitalRead(row24) == HIGH)
      {
          Gamepad.press(17);
      }
      else
      {
          Gamepad.release(17);        
      }  
        
    Gamepad.write();
}
