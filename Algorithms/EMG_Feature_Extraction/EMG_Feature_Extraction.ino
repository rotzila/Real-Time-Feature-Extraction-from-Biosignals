int sensorPin = A0;   
const int arraySize=150; /*Vector size, the bigger the number, more smooth it will be*/
int arrayPosition=0 , i, sensorValue=0,absSensorValue, pinValue=0, oldPinValue=0;
long sumSensor[arraySize];

long first_time=0.0 , last_time=0.0, time_diference=0.0;  /*first_time gives the time when the muscule activated, las_time gives the time when the muscule relaxed, time_diference gives the activation time*/
int state_sign=0;
float maximum_value=0.0;  /*maximum_value gives the maximum value in the activation time period*/

float smoothed=0.0;
long sumNumbers;
unsigned long t, t0;
long dt = 1000; /* Time between samples */
float activation= 10.0;   /*Treshold needed to be surpassed*/

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
  
  sumNumbers=0;
  state_sign=0;
}


void loop() {
if ((micros()-t0)>=dt) {   /*Everything inside this if statment happens at the same time as the acquisition */
  smoothedFunction();
  FeatureExtraction(); 
  
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
void FeatureExtraction (){
if((values.smooth>activation) && (state_sign==0)){ /*Activation time*/
    state_sign=1;
    first_time=values.times;
    if(maximum_value<values.smooth){
      maximum_value=values.smooth;
    }
  }
  else if((values.smooth>activation) && (state_sign==1)){  /*Between activation and end of muscular activity*/
    
    if(maximum_value<values.smooth){
      maximum_value=values.smooth;
    }
  }
  else if((values.smooth<activation) && (state_sign==1)){  /*End of muscular activity*/
    state_sign=0;
    last_time=values.times;
    time_diference=(last_time-first_time)/1000.;    /*It is just to convert from microseconds to milliseconds*/
    maximum_value=0;
  }
  else{
    state_sign=0;
    maximum_value=0;
  }

}
