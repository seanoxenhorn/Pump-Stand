unsigned char FM = 2; // Flow Meter - 2 is for
volatile int pulseCount = 0; //Pulse count for flow meter
float flowrate = 0; // Flow rate in gallons per minute
unsigned long previousMS = 0; //Will be used for calculating flow rate
const unsigned long interval = 200;  // Interval to calculate flow rate (in milliseconds)
const float PulsePerGal = 1380; //Pulses per gallon
float frequency = 0;

void pulseCounter()
{
  pulseCount ++;
}

void setup() 
{
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  
  Serial.println("Configuring ADCs and sensors");
  // Set up flow sensor pin
  pinMode(FM, OUTPUT);
  digitalWrite(FM, HIGH); //  
  // Attach interrupt to the flow sensor pin????
 attachInterrupt(digitalPinToInterrupt(FM), pulseCounter, RISING); //Adds one to the pulse counter upon each fall
    // attachInterrupt(FM, pulseCounter, FALLING);

  Serial.println("Initialized");
  delay(3000);
}

void loop() 
{
  unsigned long currentMS = millis();

  //Calculate flow rate
  if (currentMS - previousMS >= interval)
  {
    // detachInterrupt(FM);
    detachInterrupt(digitalPinToInterrupt(FM));
    frequency = pulseCount;

    float flowrate = (frequency*5*60)/PulsePerGal; //gal/sec (pulse/sec * sec/min * gal/pulse) -- *5 turns interval of .2 sec to 1 sec
  
    // Read the value from each analog pin (0 to 1023)
    int count = pulseCount;

    Serial.print("FM: ");
    Serial.print(flowrate);
    Serial.print("gal/min");
    Serial.println(" ");
    Serial.println(pulseCount);
    

    pulseCount = 0;
    previousMS = millis();
    
    attachInterrupt(digitalPinToInterrupt(FM), pulseCounter, FALLING);


  }

 
  // Wait for 50 milliseconds before the next loop
  delay(100);


} 