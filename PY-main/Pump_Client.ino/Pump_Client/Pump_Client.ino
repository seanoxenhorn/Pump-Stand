//#include <NativeEthernet.h>
//#include <NativeEthernetUdp.h>
#include <ADC.h>
#include "Wire.h"

//#include "PCF8575.h" // relay board 
//#include "MAX6675.h" // thermocouples
// #include <SparkFun_I2C_Mux_Arduino_Library.h>
// #include <SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h>

#define mux_wire Wire
#define relay_wire Wire2

byte mac[] = { 0x04, 0xE9, 0xE5, 0x15, 0xAB, 0x0E };  // MAC of the ethernet adapter
IPAddress ip(192, 168, 5, 5); // IP of the teensy
IPAddress remoteIP(192, 168, 5, 2); // IP of the attached PC
unsigned int remotePort = 65432; // Random port

//PCF8575 relay_module(0x27, &Wire2); // i2c addr of the relay board


const int PT1 = A0;  // Pressure Transducer 1 (before pump)
const float sensorMin0 = 0.5; // Minimum sensor output voltage in volts (e.g., 0.5V for 0 psi)
const float sensorMax0 = 4.5; // Maximum sensor output voltage in volts (e.g., 4.5V for max psi) MAY HAVE TO CHANGE LATER
const int pressureMin0 = 0;   // Minimum pressure range (e.g., 0 psi)
const int pressureMax0 = 1000; // Maximum pressure range (e.g., 1000 psi)

const int PT2 = A1;  // Pressure Transducer 2 (after pump)
const float sensorMin1 = 0.5; // Minimum sensor output voltage in volts (e.g., 0.5V for 0 psi)
const float sensorMax1 = 4.5; // Maximum sensor output voltage in volts (e.g., 5V for max psi)
const int pressureMin1 = 0;   // Minimum pressure range (e.g., 0 psi)
const int pressureMax1 = 1600; // Maximum pressure range (e.g., 1600 psi)

const int FM = 2; // Flow Meter - 2 is for
volatile int pulseCount = 0; //Pulse count for flow meter
float flowrate = 0; // Flow rate in gallons per minute
unsigned long previousMS = 0; //Will be used for calculating flow rate
const unsigned long interval = 200;  // Interval to calculate flow rate (in milliseconds)
const float PulsePerGal = 1380; //Pulses per gallon

ADC *adc = new ADC();

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
  pinMode(FM, INPUT_PULLUP)
  
  // Attach interrupt to the flow sensor pin????
  attachInterrupt(digitalPinToInterrupt(FM), pulseCounter, FALLING); //Adds one to the pulse counter upon each fall

// PT 1 init
  adc->adc0->setAveraging(2);
  adc->adc0->setResolution(10);
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

// PT 2 init
  adc->adc1->setAveraging(2);
  adc->adc1->setResolution(10);
  adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
  adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

  Serial.println("Initialized");
  delay(3000);

}

void loop() 
{
  unsigned long currentMS = millis();

  //Calculate flow rate
  if (currentMS - previousMS >= interval)
  {
  noInterrupts();
  float frequency = pulseCount;
  pulseCount = 0;
  Interrupts();

  flowrate = (frequency*60*5)/PulsePerGal //gal/min (pulse/sec * sec/min * gal/pulse) -- why *5????
  }

  // Read the value from each analog pin (0 to 1023)
  int count = pulseCount;
  float sensor1 = adc->adc0->analogRead(PT1); // Read the analog value from the transducer
  float sensor2 = adc->adc1->analogRead(PT2);
  
  // Assuming all 3.3V reference and 10bit ADC resolution
  float v1 = sensor1 * (3.3/1023); //MAKE SURE TO CHANGE TO PROPER VOLTAGE SUPPLY
  float v2 = sensor2 * (3.3/1023);
  
  float pres1 = ((v1-sensorMin0)*(pressureMax0-pressureMin0)/(sensorMax0-sensorMin0)) + pressureMin0;
  float pres2 = ((v2-sensorMin1)*(pressureMax1-pressureMin1)/(sensorMax1-sensorMin1)) + pressureMin1;

  // Print the values to the Serial Monitor
  Serial.println("PT1: ")
  Serial.print(pres1)
  Serial.print("psi");
  Serial.println("PT2: ")
  Serial.print(pres2)
  Serial.print("psi");
  Serial.println("FM: ")
  Serial.print(flowrate)
  Serial.print("gal/min");

  // Wait for 50 milliseconds before the next loop
  delay(50);
}