#include <Wire.h>
#include "SFE_ISL29125.h"

SFE_ISL29125 RGB_sensor;                        // RGB-szenzor változó létrehozása

#define BTN_EN 22                               //Enable button, if pressed, no new color is processed
#define BTN_R 23                                //Red button, if pressed the lamp should turn to red
#define BTN_G 24                                //Green button, if pressed the lamp should turn to green
#define BTN_B 25                                //Blue button, if pressed the lamp should turn to blue

word redColor = 0;                              //The actual red value of the light
word greenColor = 0;                            //The actual green value of the light
word blueColor = 0;                             //The actual blue value of the light

//Define the inputs and outputs
void setup()
{

 pinMode(13, OUTPUT);                                                 //RED
 pinMode(12, OUTPUT);                                                 //GREEN
 pinMode(11, OUTPUT);                                                 //BLUE    
 pinMode(8, OUTPUT);                                                 //GREEB LED on arduino
 pinMode(BTN_EN, INPUT_PULLUP);               
 pinMode(BTN_R, INPUT_PULLUP);  
 pinMode(BTN_G, INPUT_PULLUP);  
 pinMode(BTN_B, INPUT_PULLUP);  
 
 Serial.begin(9600);
 Wire.begin();
 delay(200);

 if (RGB_sensor.init())                                               //if sensor initialization was succesful
 {
   Serial.println("Sensor Initialization Successful\n\r");
 }

 analogWrite(8,20);
 delay(1000);
 digitalWrite(8,LOW);
}


//Proper work cycle for new colors
void Sensor_allowed ()
{
   digitalWrite(13,0);                                                //Switch off the Light
   digitalWrite(12,0);
   digitalWrite(11,0);
   redColor = 0;                                                      //Reset the data after the lighting.
   greenColor = 0;
   blueColor = 0; 
   analogWrite(8,20);                                   //Signal for the User, new color is processed after the light.                                   
   delay(4000);                                                       //The sensor lights up green for 4,5 secs
   digitalWrite(8,LOW);                                      //Signal for the User, its not processing new color.
   delay(1000);                                                   

   //Reading the colors's datas. Also 
   for (int i = 0; i < 5; i++) {
    redColor = (RGB_sensor.readRed() + (redColor * i))/(i+1);
    greenColor = (RGB_sensor.readGreen() + (greenColor *i))/(i+1);
    blueColor =  (RGB_sensor.readBlue() + (blueColor * i))/(i+1);
   }
  
   map(redColor, 0, 65535, 0, 255);
   map(greenColor, 0, 65535, 0, 255);
   map(blueColor, 0, 65535, 0, 255);

   //Print data to serial output.
   Serial.print(redColor);
   Serial.print("-");
   Serial.print(greenColor);
   Serial.print("-");
   Serial.println(blueColor);

   //Send data to the lamp. For 5 sec the data is lit up on the lamp.
   analogWrite(13,redColor);  
   analogWrite(12,greenColor);    
   analogWrite(11,blueColor);
   delay(5000);
}

void loop() 
{
  //Red button pressed, the lamp turns red.
  if(digitalRead(BTN_R) == 0)                                        
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);  
  }
  //Green button pressed, the lamp turns green.
  else if(digitalRead(BTN_G) == 0) 
  {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW); 
  }
  //Blue button pressed, the lamp turns blue.
  else if(digitalRead(BTN_B) == 0) 
  {
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);  
  }
  //Process a new color only if no button is pressed.
  else if((digitalRead(BTN_EN) == 1) && (digitalRead(BTN_R) == 1) && (digitalRead(BTN_G) == 1) && (digitalRead(BTN_B) == 1))
  {
          Sensor_allowed ();
  }
  delay(1000);
}
