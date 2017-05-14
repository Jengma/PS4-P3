#include <EasyTransfer.h>

EasyTransfer ET;

struct SEND_DATA_STRUCTURE{
  int16_t R1_Switch;
  int16_t L1_Switch;
  float X_LeftJ;
  float Y_RightJ;
};

SEND_DATA_STRUCTURE mydata;

int SW_R1 = 2;          //Bouton R1
int SW_L1 = 3;          //Bouton L1

//Left Joystick
int X_pinL = 6;

//Right Joystick 
int Y_pinR = 7;

void setup() {
  pinMode(SW_R1, INPUT_PULLUP);  
  pinMode(SW_L1, INPUT_PULLUP);  
  Serial.begin(9600);
  ET.begin(details(mydata),&Serial);
}

void loop() {
  /*Serial.print("this is the joystick: ");
  Serial.print(analogRead(X_pinL));
  Serial.print("\n");
  Serial.print("this is the button: ");
  Serial.print(digitalRead(SW_L1));
  Serial.print("\n");*/
  
  mydata.R1_Switch = digitalRead(SW_R1);
  mydata.L1_Switch = digitalRead(SW_L1);
  //analogread revoie une valeur entre 0 & 1023
  mydata.X_LeftJ = (150 - (analogRead(X_pinL)/9));      //Savoir valeur neutre donne valeur entre 30 & 150
  mydata.Y_RightJ = (analogRead(Y_pinR)/4);        //Envoie valeur de 0 à 256
  ET.sendData();
  delay(200);
}
