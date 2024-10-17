// CALIBRATED 1600 PSI PRESSURE TRANSDUCER


const int PT1 = A0;  // Pressure Transducer 1 (before pump)
const float sensorMin0 = 0.5; // Minimum sensor output voltage in volts (e.g., 0.5V for 0 psi)
const float sensorMax0 = 4.5; // Maximum sensor output voltage in volts (e.g., 4.5V for max psi) MAY HAVE TO CHANGE LATER
const int pressureMin0 = 0;   // Minimum pressure range (e.g., 0 psi)
const int pressureMax0 = 1600; // Maximum pressure range (e.g., 1000 psi)
const float PT1_offset = 7.23; //PT 1 offset value to get to 0 psig

//ADC0 *adc = new ADC();

void setup() 
{
  // Initialize serial communication at 9600 bits per second
  Serial.begin(9600);
  
  Serial.println("Configuring ADCs and sensors");
 
// PT 1 init
  // adc->adc0->setAveraging(2);
  // adc->adc0->setResolution(10);
  // adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  // adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

  pinMode(A0, INPUT);
  Serial.println("Initialized");
  delay(3000);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  float sensor1 = analogRead(PT1); // Read the analog value from the transducer

  float v1 = (sensor1*5)/1023.0; //MAKE SURE TO CHANGE TO PROPER VOLTAGE SUPPLY

  float pres1 = ((v1-sensorMin0)*(pressureMax0-pressureMin0)/(sensorMax0-sensorMin0)) + pressureMin0-PT1_offset;

  Serial.println("PT1: ");
  Serial.print(pres1);
  

  // Wait for 50 milliseconds before the next loop
  delay(500);
}
