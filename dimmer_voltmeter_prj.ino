// This #include statement was automatically added by the Spark IDE.
#include "OneWire/OneWire.h"

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_LEDBackpack/Adafruit_LEDBackpack.h"

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix ();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix ();
Adafruit_8x8matrix matrix3 = Adafruit_8x8matrix ();

double dimmRead = 0.0;
double tempRead = 0.0;
double dimmVolt = 0.0;
int tempVolt = 0;
double dimmCalc = 0.0;
int dimmInt = 0;

void setup() {
  Serial.begin(9600);
    // Init/start matrices
  matrix1.begin(0x70);
  matrix1.setTextWrap(false); // Allow text to run off edges
  matrix1.clear();
  matrix2.begin(0x71);
  matrix2.setTextWrap(false); // Allow text to run off edges
  matrix2.clear();
  matrix3.begin(0x72);
  matrix3.setTextWrap(false); // Allow text to run off edges
  matrix3.clear();
  
  // Connect the temperature sensor to A7 and configure it
  // to be an input
  pinMode(A6, INPUT);
  pinMode(A0, INPUT);
  
  Spark.variable("dimmRead", &dimmRead, INT);
  Spark.variable("dimmInt", &dimmInt, INT);
  Spark.variable("dimmCalc", &dimmCalc, DOUBLE);
  Spark.variable("dimmVolt", &dimmVolt, DOUBLE);

}


void loop()
{

  dimmRead = analogRead(A6);
  tempRead = analogRead(A0);
  //Analog Read shows voltages as a number ranging from 0 - 4096, so use the math below to convert to volts
  tempVolt = (tempRead/4096)*330;
  dimmVolt = (dimmRead / 4096);

  //Multiply tempCalc by 10 to get ones and first decimal places
  //tempVolt = tempVolt * 10;
  //tempCalc = tempVolt;
  char ones = tempVolt % 10;
  char tens = (tempVolt / 10) % 10;
  char hundreds = (tempVolt / 100) % 10;

  matrix3.clear();
  matrix3.setCursor(0, 0);
  matrix3.write(hundreds + '0');
  matrix3.writeDisplay();

  matrix2.clear();
  matrix2.setCursor(0, 0);
  matrix2.write(tens + '0');
  matrix2.writeDisplay();

  matrix1.clear();
  matrix1.setCursor(0, 0);
  matrix1.write(ones + '0');
  matrix1.writeDisplay();

  dimmCalc = 15 * dimmVolt;
  dimmInt = dimmCalc;
  matrix3.setBrightness(dimmInt);
  matrix2.setBrightness(dimmInt);
  matrix1.setBrightness(dimmInt);
  
  delay(250);

}
