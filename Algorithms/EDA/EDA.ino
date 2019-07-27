int sensorPin = A0;   
const int arraySize=150; /*Vector size, the bigger the number, more smooth it will be*/
int arrayPosition=0 , i, sensorValue=0,absSensorValue, pinValue=0, oldPinValue=0;
long sumSensor[arraySize];

const int n = 10;



float smoothed=0.0;
long sumNumbers;
unsigned long t, t0;
long dt = 1000; /* Time between samples */


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
}


void loop() {
if ((micros()-t0)>=dt) {   /*Everything inside this if statment happens at the same time as the acquisition */
  smoothedFunction();
  
  Serial.print(values.times);  /*Just to show the values in a ploter or utilize in other language like python using serial communication */
  Serial.print(",");
  Serial.print(values.raw);
  Serial.print(",");
  Serial.println(values.smooth);

}

}
void smoothedFunction (){

  t0 = micros();
  sensorValue = analogRead(sensorPin); 
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
