const int sensor_pin = A1;     // Soil sensor Pin number
#define DCwater_pump 8        //Relay Pin Number
int flowPin = 2;    //This is the input pin on the Arduino
double flowRate;    //This is the value we intend to calculate.
volatile int count; //This integer needs to be set as volatile to ensure it updates correctly during the interrupt process. 
int i= 0; 


void Flow()
{
   count++; //Every time this function is called, increment "count" by 1
}

void setup()
{
  Serial.begin(9600);
  pinMode(DCwater_pump, OUTPUT);
  pinMode(flowPin, INPUT);           //Sets the pin as an input
  attachInterrupt(0, Flow, RISING);  //Configures interrupt 0 (pin 2 on the Arduino Uno) to run the function "Flow"  
}

void loop()
{
  float moisture_percentage;
  int sensor_analog;
  count = 0;      // Reset the counter so we start counting from 0 again
  interrupts();   //Enables interrupts on the Arduino
  delay (1000);   //Wait 1 second
  noInterrupts(); //Disable the interrupts on the Arduino
  sensor_analog = analogRead(sensor_pin);
   
  //Start the math
  flowRate = (count * 2.25);        //Take counted pulses in the last second and multiply by 2.25mL
  flowRate = flowRate * 60;         //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate / 100000;       //Convert mL to Liters, giving you Liters / Minute
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
     
  Serial.print("Moisture Percentage = ");
  Serial.println(moisture_percentage);
  Serial.print("Flow= ");
  Serial.println(flowRate);
     
  if(moisture_percentage < 26){
    digitalWrite(DCwater_pump,LOW);
    while (i == 1){
    Serial.println("DC Pump is ON Now!!");
    i = 1;}
  }
  else{
     digitalWrite(DCwater_pump,HIGH);
  }
  delay(5000);
}
