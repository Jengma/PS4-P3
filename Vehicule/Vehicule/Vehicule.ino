#include <EasyTransfer.h>
#include <Servo.h>

EasyTransfer ET;

struct RECIVIE_DATA_STRUCTURE{
  int16_t R1_Switch;
  int16_t L1_Switch;
  float X_LeftJ;
  float Y_RightJ;
};

int Capt_Galv_Interupt = 3;           // Capteur galvanique
bool Busy = true;                     //
int sens = 7;                          //Sens de rotation du moteur
int Vitesse;                          // Vitesse du moteur (indépendant du sens)
int MoteurProp = 6;                   // Patte de commande du moteur de propultion
RECIVIE_DATA_STRUCTURE mydata;
Servo MoteurDir;                      // Immoblile à 1500 sens trigo à >1500 et sens anti-trigo à <1500
Servo MoteurImpul;            

void setup() 
{
  cli();
  pinMode(Capt_Galv_Interupt, INPUT);   //Capteur galvanique mis en entrée
  attachInterrupt(digitalPinToInterrupt(Capt_Galv_Interupt),Armed_Cata, FALLING);
  pinMode(sens,OUTPUT);
  pinMode(MoteurProp,OUTPUT);
  Serial.begin(9600);
  ET.begin(details(mydata),&Serial);
  MoteurImpul.attach(9);
  MoteurDir.attach(10);
  //Charger la catapulte à l'enclanchement
  while(Capt_Galv_Interupt == HIGH)
  {
    MoteurImpul.write(120);
  }
  MoteurImpul.write(90);
  Busy = false;
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ET.receiveData()){
    //controlle du moteur direction
    MoteurDir.write(mydata.X_LeftJ);              


    //Test si catapulte acitvable
    if(mydata.L1_Switch == LOW && Busy == false)    
    {
      Busy = true;
      //faire légerment tourner le moteur afin de lancer la catapulte   
      MoteurImpul.write(100);
      delay(200);
      MoteurImpul.write(150);  
    }

    //analogread donne une valeur entre 0 &255
    // Le véhicule avance (Controle du moteur)
    if(mydata.Y_RightJ > 136)                       
    {
      digitalWrite(sens,HIGH);
      Vitesse = (mydata.Y_RightJ -130)*2;
      analogWrite(MoteurProp,Vitesse); 
    }
    
    // Le véhicule recule (Controle du moteur) Attention Correction à faire
    else if(mydata.Y_RightJ < 120)                      
    {
      digitalWrite(sens,LOW);
      Vitesse = (255 - (mydata.Y_RightJ*2));
      analogWrite(MoteurProp,Vitesse);
    }
    
     // Le véhicule reste sur place
    else if(mydata.Y_RightJ >= 120 && mydata.Y_RightJ <= 136)                                      
    {
      analogWrite(MoteurProp,0);
    }         
  }
  delay(50);
}

void Armed_Cata()
{
  MoteurImpul.write(90);  //Arreter le moteur de 
  Busy = false;           //Catapulte près à faire feu
}

