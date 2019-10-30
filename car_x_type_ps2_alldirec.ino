#include <PS2X_lib.h>  //for v1.6

/******************************************************************
 * set pins connected to PS2 controller:
 *   - 1e column: original 
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use
 ******************************************************************/
#define PS2_DAT        2  //14    
#define PS2_CMD        3  //15
#define PS2_SEL        4  //16
#define PS2_CLK        5  //17

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you connect the controller, 
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;




int motor_1_cw=22;
int motor_1_acw=23;
int motor_4_cw=24;
int motor_4_acw=25;
int motor_3_cw=26;
int motor_3_acw=27;
int motor_2_cw=28;
int motor_2_acw=29;


int motor_1_pwm=8;
int motor_4_pwm=9;
int motor_3_pwm=10;
int motor_2_pwm=11;


 int rx_initial;int ry_initial;int lx_initial;


int run_motor_1(int,int,int);
int run_motor_2(int,int,int);
int run_motor_3(int,int,int);
int run_motor_4(int,int,int);

void setup(){
 
  Serial.begin(57600);

  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
   
  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 




pinMode(motor_1_cw,OUTPUT);
pinMode(motor_1_acw,OUTPUT);
pinMode(motor_2_cw,OUTPUT);
pinMode(motor_2_acw,OUTPUT);
pinMode(motor_3_cw,OUTPUT);
pinMode(motor_3_acw,OUTPUT);
pinMode(motor_4_cw,OUTPUT);
pinMode(motor_4_acw,OUTPUT);



pinMode(motor_1_pwm,OUTPUT);
pinMode(motor_2_pwm,OUTPUT);
pinMode(motor_3_pwm,OUTPUT);
pinMode(motor_4_pwm,OUTPUT);


 rx_initial=ps2x.Analog(PSS_RX);
 ry_initial=ps2x.Analog(PSS_RY);
 lx_initial=ps2x.Analog(PSS_LX);
}












void loop() {
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */  
  if(error == 1) //skip loop if no controller found
    return; 
     ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
   /* if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");      

    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      Serial.print("Up held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.print("DOWN held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
    }   

    vibrate = ps2x.Analog(PSAB_CROSS);  //this will set the large motor vibrate speed based on how hard you press the blue (X) button
    if (ps2x.NewButtonState()) {        //will be TRUE if any button changes state (on to off, or off to on)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed");
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed");
      if(ps2x.Button(PSB_TRIANGLE))
        Serial.println("Triangle pressed");        
    }

    if(ps2x.ButtonPressed(PSB_CIRCLE))               //will be TRUE if button was JUST pressed
      Serial.println("Circle just pressed");
    if(ps2x.NewButtonState(PSB_CROSS))               //will be TRUE if button was JUST pressed OR released
      Serial.println("X just changed");
    if(ps2x.ButtonReleased(PSB_SQUARE))              //will be TRUE if button was JUST released
      Serial.println("Square just released");     

  
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); */



      {{digitalWrite(motor_1_cw,LOW);
 digitalWrite(motor_1_acw,HIGH);
 digitalWrite(motor_1_pwm,0);}

{digitalWrite(motor_2_cw,LOW);
 digitalWrite(motor_2_acw,HIGH);
 digitalWrite(motor_2_pwm,0);}


 {digitalWrite(motor_3_cw,LOW);
 digitalWrite(motor_3_acw,HIGH);
 digitalWrite(motor_3_pwm,0);}



{digitalWrite(motor_4_cw,LOW);
 digitalWrite(motor_4_acw,HIGH);
 digitalWrite(motor_4_pwm,0);}

}

     
      
            int rx=ps2x.Analog(PSS_RX);
            int ry=ps2x.Analog(PSS_RY);
            int lx=ps2x.Analog(PSS_LX);
            rx=rx-rx_initial;ry=ry-ry_initial;ry=ry*(-1);lx=lx-lx_initial;

             //Serial.println(lx);//Serial.print("                "); Serial.println(ry);
            
            int abs_rx=abs(rx);int abs_ry=abs(ry);int abs_lx=abs(lx);


            if(rx<25 && rx>-25)
            {rx=0;}

            


            if(ry<25 && ry>-25)
            {ry=0;}


            if(lx<25  &&  lx>-25)
            {lx=0;}
         
             

//Serial.print(rx);Serial.print("       ");Serial.println(ry);

            rx=rx/1.41;ry=ry/1.41;
            lx=lx*2;
            lx=constrain(lx,-254,254);


            rx=rx*2;
            ry=ry*2;
      

            run_motor_1(rx,ry,lx);
            run_motor_2(rx,ry,lx);
            run_motor_3(rx,ry,lx);
            run_motor_4(rx,ry,lx);


            


            
   delay(200);

}

int run_motor_1(int x,int y,int r)
{int vel=(x-y);
Serial.print(vel);Serial.print("       ");
vel=constrain(vel,-254,254);
if(vel>0)
{digitalWrite(motor_1_cw,LOW);                                  
digitalWrite(motor_1_acw,HIGH);
analogWrite(motor_1_pwm,abs(vel));}

if(vel<0)
{digitalWrite(motor_1_cw,HIGH);                                  
digitalWrite(motor_1_acw,LOW);
analogWrite(motor_1_pwm,abs(vel));}

if(r>0)
{digitalWrite(motor_1_cw,LOW);                                   
 digitalWrite(motor_1_acw,HIGH);
 analogWrite(motor_1_pwm,abs(r));}


if(r<0)
{digitalWrite(motor_1_cw,HIGH);                                   
 digitalWrite(motor_1_acw,LOW);
 analogWrite(motor_1_pwm,abs(r));}

}


int run_motor_2(int x,int y,int r)
{int vel=(-y-x);
vel=constrain(vel,-254,254);
Serial.print(vel);Serial.print("       ");
if(vel>0)
{digitalWrite(motor_2_cw,LOW);                                  
digitalWrite(motor_2_acw,HIGH);
analogWrite(motor_2_pwm,abs(vel));}

if(vel<0)
{digitalWrite(motor_2_cw,HIGH);                                  
digitalWrite(motor_2_acw,LOW);
analogWrite(motor_2_pwm,abs(vel));}


if(r>0)
{digitalWrite(motor_2_cw,LOW);                                   
 digitalWrite(motor_2_acw,HIGH);
 analogWrite(motor_2_pwm,abs(r));}


if(r<0)
{digitalWrite(motor_2_cw,HIGH);                                   
 digitalWrite(motor_2_acw,LOW);
 analogWrite(motor_2_pwm,abs(r));}



}


int run_motor_3(int x,int y,int r)
{int vel=(-x+y);
vel=constrain(vel,-254,254);
Serial.print(vel);Serial.print("       ");
if(vel>0)
{digitalWrite(motor_3_cw,LOW);                                  
digitalWrite(motor_3_acw,HIGH);
analogWrite(motor_3_pwm,abs(vel));}

if(vel<0)
{digitalWrite(motor_3_cw,HIGH);                                  
digitalWrite(motor_3_acw,LOW);
analogWrite(motor_3_pwm,abs(vel));}

if(r>0)
{digitalWrite(motor_3_cw,LOW);                                   
 digitalWrite(motor_3_acw,HIGH);
 analogWrite(motor_3_pwm,abs(r));}


if(r<0)
{digitalWrite(motor_3_cw,HIGH);                                   
 digitalWrite(motor_3_acw,LOW);
 analogWrite(motor_3_pwm,abs(r));}

}



int run_motor_4(int x,int y,int r)
{int vel=(x+y);
vel=constrain(vel,-254,254);
Serial.print(vel);Serial.println("       ");Serial.print(r);Serial.println("       ");
if(vel>0)
{digitalWrite(motor_4_cw,LOW);                                  
digitalWrite(motor_4_acw,HIGH);
analogWrite(motor_4_pwm,abs(vel));}

if(vel<0)
{digitalWrite(motor_4_cw,HIGH);                                  
digitalWrite(motor_4_acw,LOW);
analogWrite(motor_4_pwm,abs(vel));}


if(r>0)
{digitalWrite(motor_4_cw,LOW);                                   
 digitalWrite(motor_4_acw,HIGH);
 analogWrite(motor_4_pwm,abs(r));}


if(r<0)
{digitalWrite(motor_4_cw,HIGH);                                   
 digitalWrite(motor_4_acw,LOW);
 analogWrite(motor_4_pwm,abs(r));}

}


