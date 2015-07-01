#include <Servo.h>
#include "RF_uartEngine_Tx.h"
#include "RF_uartEngine_Rx.h"

// "timer" variable, in milliseconds.  remembers the last time an update happened
// MUST be of type unsigned long!
unsigned long lastUpdate = 0;

// time between updates (20 ms for servo control) 
unsigned long updatePeriod = 20; // in milliseconds

Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
                
RF_uartEngine_Tx TxUART;
RF_uartEngine_Rx RxUART;

int RxStatus = 0;
msg_uart MSG_RX;

String inputStringPos = "";               // a string to hold incoming data
int posServo = 90;                        // variable to store the servo position (active position send to the servo)
int tmpposServo = posServo;               // variable to store the servo position (parameter store when set param received)
boolean newPosServo = false;              // flag indicating when the servo get new pos
  //Pos min & max
int posServoMin = 21;
int posServoMax = 169;

  //Flag indicating when a msg has been received
boolean msgReceive = false;

  //pin for led (DEBUG PURPOSE)
int led = 13;

int i = 0;
  
  
  
  /*====================================================================*/
  /*                            void setup()                            */
  /*====================================================================*/ 
void setup() {
  myservo.attach(3);                    // attaches the servo on pin 3 to the servo object
  myservo.write(posServo);              // set Servo to init pos (90°)
  Serial.begin(57600);                  // Uart init
  
  pinMode(led,OUTPUT);                  //Setup DEBUG LED
  digitalWrite(led,LOW);
}



  /*====================================================================*/
  /*                            void loop()                             */
  /*====================================================================*/ 
void loop() {
    //If Msg has been received:
  if(msgReceive){
    
      //Read msg
    MSG_RX = RxUART.getMsg();
    
      //Check status
    if(RxStatus >= 0){
      //If status >= 0 -> status = OK
      
        //Set msgReceive flag to false
      msgReceive = false;
      
        //Check function code 
      switch(MSG_RX.functionCode){
        
  
          //----------------------------------------//
          // Function move to target angle (action) //
          //----------------------------------------//
        case FCT_MOVE_PHI:
        
            //Assign tmpposServo (result of set param below) to pos Servo
          posServo = tmpposServo;
            //Set new pos servo to true
          newPosServo = true;
        break;
        
        
        
          //---------------------------------------------//
          // Function Set Param (only ANGLE PHI & ASCII) //
          //---------------------------------------------//
        case FCT_SET_PARAM:
            //Check if param = Angle Phi in ASCII (ID = 0x06)
          if(MSG_RX.ID[0] == ANGLE_PHI){ 
            
              //Store angle phi in ASCII in input string buffer
            for(i = 1 ; i < MSG_RX.sizeData ; i++){
              inputStringPos += (char)MSG_RX.Data[i];
            }
            
              //Check if angle is in range then set tmpposServo to input String data (in integer)
            if((inputStringPos.toInt() >= posServoMin) && (inputStringPos.toInt()  <= posServoMax )){
              tmpposServo = inputStringPos.toInt();  
            }else{
              
                //Indicate that an error occurs (Out of Range data)
              do{
                Serial.write((char)TxUART.sendErrCarte());
              }while(!TxUART.msgSent() && !TxUART.transmitError());                      
            }            
              //clear inputString
            inputStringPos = "";
            
          }else{
            
              // Indicate that an error occurs (Bad ID param)
            Serial.print("Erreur Fonction Set Param, ID: ");
            Serial.println(MSG_RX.ID[0]);
            do{
              Serial.write((char)TxUART.sendErrCarte());
            }while(!TxUART.msgSent() && !TxUART.transmitError());              
          }
        break;
        
        default:
          Serial.print("Erreur Fonction Code: ");
          Serial.println(MSG_RX.functionCode);
            //Send Unknow FCT MSG received
          do{
            Serial.write((char)TxUART.sendErrUnknowMsg());
          }while(!TxUART.msgSent() && !TxUART.transmitError());          
        break;
      }
    }else{
      
      msgReceive = false;
      Serial.print("Erreur Read Status: ");
      Serial.println(RxStatus);
            //Send Internal Err
      do{
        Serial.write((char)TxUART.sendErrCarte());
      }while(!TxUART.msgSent() && !TxUART.transmitError());
    }//RxStatus >= 0   
  }//msgReceive
    

  // Servo routine : update myservo.write every 20 ms (Arduino hardware requirement)  
  
  // millis() returns the number of milliseconds since the Arduino turned on
  unsigned int now = millis();
 
    // check to see if updatePeriod milliseconds have elapsed since lastUpdate
    // if so, update the new position and reset lastUpdate
  if( (now-lastUpdate) >= updatePeriod){
    myservo.write(posServo);
    lastUpdate = millis(); // reset our countdown timer
    if(newPosServo){
      do{
        Serial.write((char)TxUART.sendAnswerMoveAngle(true));
      }while(!TxUART.msgSent() && !TxUART.transmitError());    
      newPosServo = false;
    }
  }  
} // loop()
 
 
  
  /*====================================================================*/
  /*                        void serialEvent()                          */
  /*====================================================================*/ 
void serialEvent() {
  digitalWrite(led,HIGH);
  while(Serial.available()){
    char inChar = (char)Serial.read();
    msgReceive = RxUART.readChar(inChar);
    if(msgReceive){
      digitalWrite(led,LOW);
      RxStatus = RxUART.getStatus(); 
      break; 
    }
  }  
}




