
int sensorPin = A0;   
const int arraySize=150; /*Vector size, the bigger the number, more smooth it will be*/
int arrayPosition=0 , i, sensorValue=0,absSensorValue, pinValue=0, oldPinValue=0;
long sumSensor[arraySize];


long first_time=0.0 , last_time=0.0, time_diference=0.0;
int state_sign=0;
float maximum_value=0.0;

/*LCD Part*/
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

int randomized = 0;
int game_stat;

int ledPin = 7;



float smoothed=0.0;
long sumNumbers;
unsigned long t, t0;
long dt = 1000; /* Time between samples */

int potenci1 = A4, potenci2=A5;
int timeSetup=0, tresholdSetup=0;


long activation_time;



float activation= 0.0, timeActivation=0.0;

struct mainValues{         /* Creates an easier acess to the values */ 
  unsigned long times=t0;
  int raw=sensorValue;
  float smooth=smoothed;
  
};
mainValues values = {};  /* To acess time, raw and smooth use like values.times or values.raw or values.smooth */
void setup() {
Serial.begin(2000000); /*Bits per second*/
lcd.begin(16, 2);
lcd.clear();
t0=micros();

game_stat=0;
for(i=0; i<arraySize; i++) {
  sumSensor[i]=0;
}
  pinMode(ledPin,OUTPUT); 
  
  sumNumbers=0;
  state_sign=0;
}


void loop() {



  tresholdSetup=analogRead(potenci2);
  activation=(float)tresholdSetup/25.6; /*0 to 40*/
  //timeActivation=(float)timeSetup*9.765625; /*0 to 10000 microseconds(10 miliseconds)*/

  //Serial.println(game_stat);
  if(game_stat==0){
    game_stat=1;
    randomized=random(1000,5000);
    delay(randomized);
    activation_time=micros();
    digitalWrite(ledPin,HIGH);
    
  }
  smoothedFunction();
  Serial.println(activation);
  if((values.smooth>activation) && (game_stat==1) ){
     first_time=values.times;
     game_stat=2;
     if((first_time-activation_time<100000)&& (game_stat==2)){
      
      
     
      lcd.setCursor(0,0);
      lcd.write("You Cheater");
      lcd.setCursor(0,1);               
      lcd.print(((first_time-activation_time)));
      
    }
    else if((first_time-activation_time<1000000) && (game_stat==2)){
      lcd.setCursor(0,0);
      lcd.write("You are fast");
      lcd.setCursor(0,1);               
      lcd.print(first_time-activation_time);
    }
    else if((first_time-activation_time>1000000) && (game_stat==2)){
      lcd.setCursor(0,0);
      lcd.write("You are slow");
      lcd.setCursor(0,1);               
      lcd.print(first_time-activation_time);
  
    }
  }
}


void smoothedFunction (){

  t0 = micros();
  oldPinValue = pinValue; /* Needed to use in the derivation */
  pinValue = analogRead(sensorPin); 
  sensorValue = pinValue-oldPinValue;  /* With this line its possible to make the value 0 as our reference point */ 
  absSensorValue=abs(sensorValue); /*Transform negative values into positive values*/
  if (arrayPosition>=arraySize){ /*If the position that the program is writing in the array is bigger than the size of the array, it will replace the index nr 0 of the array*/
    arrayPosition=0;  
  }
  sumNumbers-=sumSensor[arrayPosition];  
  sumSensor[arrayPosition]=absSensorValue; /*Add the value received from the sensor to the array*/
  sumNumbers+=sumSensor[arrayPosition];
  ++arrayPosition; /*Increment 1 value to the array position, making it move to the next index from the array*/
  smoothed=(sumNumbers)/((float)arraySize); /*Make the average value using the values received*/
  values.times=t0;
  values.raw=sensorValue;
  values.smooth=smoothed;
  
}
