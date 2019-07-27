int sensorPin = A0;   
const int arraySize=150; /*Vector size, the bigger the number, more smooth it will be*/
int arrayPosition=0 , i, sensorValue=0,absSensorValue, pinValue=0, oldPinValue=0;
long sumSensor[arraySize];


long first_time=0.0 , last_time=0.0, time_diference=0.0;  /*first_time gives the time when the muscule activated, las_time gives the time when the muscule relaxed, time_diference gives the activation time*/
int state_sign=0;
float maximum_value=0.0;  /*maximum_value gives the maximum value in the activation time period*/

long diff_peak=0.0, peak_1=0.0, peak_2=0.0;
int count=0;

float smoothed=0.0;
long sumNumbers;
unsigned long t, t0;
long dt = 1000; /* Time between samples */
float activation= 3.0; /*Threshold needed to be surpassed*/

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
//if ((micros()-t0)>=dt) {   /*Everything inside this if statment happens at the same time as the acquisition */
  smoothedFunction();
  FeatureExtraction(); 
  Serial.print(values.times); //Print the average value to the plotter or the monitor
  Serial.print(",");
  Serial.print(values.raw);
  Serial.print(",");
  Serial.println(values.smooth);
  
//}

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
  values.raw=pinValue;
  values.smooth=smoothed;
  
}
void FeatureExtraction (){
if((values.smooth>activation)&&(state_sign==0)){
  if(values.smooth>maximum_value){
    
    maximum_value=values.smooth;
  }
  else if((values.smooth<=maximum_value)&&(count==0)){
    
    peak_1=values.times;
    count=1;
    state_sign=1;
  }
}
else if(values.smooth<=activation){
  count=0;
  maximum_value=0;
}
else if((values.smooth>activation)&&(state_sign==1)){
  if(values.smooth>maximum_value){
    maximum_value=values.smooth;
  }
  else if((values.smooth<maximum_value)&&(count=0)){
    peak_2=values.times;
    count=1;
    state_sign=1;
    diff_peak=peak_2-peak_1;
    peak_1=peak_2;
  }
}


}
