int sensorPin = A0;   
const int arraySize=150; /*Vector size, the bigger the number, more smooth it will be*/
int arrayPosition=0 , i, sensorValue=0,absSensorValue, pinValue=0, oldPinValue=0;
long sumSensor[arraySize];


long first_time=0.0 , last_time=0.0, time_diference=0.0;
int state_sign=0;
float maximum_value=0.0;


int ledPin = 7;


float smoothed=0.0;
long sumNumbers;
unsigned long t, t0;
long dt = 1000; /* Time between samples */

int potenci1 = A4, potenci2=A5;
int timeSetup=0, tresholdSetup=0;




float activation= 0.0, timeActivation=0.0;

struct mainValues{         /* Creates an easier acess to the values */ 
  unsigned long times=t0;
  int raw=sensorValue;
  float smooth=smoothed;
  
};
mainValues values = {};  /* To acess time, raw and smooth use like values.times or values.raw or values.smooth */
void setup() {
Serial.begin(2000000); /*Bits per second*/

t0=micros();
for(i=0; i<arraySize; i++) {
  sumSensor[i]=0;
}
  pinMode(ledPin,OUTPUT); 
  sumNumbers=0;
  state_sign=0;
}


void loop() {
if ((micros()-t0)>=dt) {   /*Everything inside this if statment happens at the same time as the acquisition */
  smoothedFunction();
  
  Serial.print(values.times);  /*Just to show the values in a ploter or utilize in other language like python using serial communication */
  Serial.print(",");
  Serial.print(values.raw);
  Serial.print(",");
  Serial.println(values.smooth);
  timeSetup=analogRead(potenci1);
  tresholdSetup=analogRead(potenci2);
  activation=(float)tresholdSetup/51.2; /*0 to 20*/
  timeActivation=(float)timeSetup*9.765625; /*0 to 10000 microseconds(10 miliseconds)*/
  
  if((values.smooth>activation) && (state_sign==0)){
    state_sign=1;
    first_time=values.times;
    if(maximum_value<values.smooth){
      maximum_value=values.smooth;
    }
  }
  else if((values.smooth>activation) && (state_sign==1) ){
    if(values.times-first_time>timeActivation){
    digitalWrite(ledPin,HIGH);
    }
    if(maximum_value<values.smooth){
      maximum_value=values.smooth;
    }
  }
  else if((values.smooth<activation) && (state_sign==1)){
    state_sign=0;
    digitalWrite(ledPin,LOW);
    last_time=values.times;
    time_diference=(last_time-first_time)/1000.;
    

    maximum_value=0;
  }
  else{
    digitalWrite(ledPin,LOW);
    state_sign=0;
    maximum_value=0;
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
