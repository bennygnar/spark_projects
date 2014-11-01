// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_LEDBackpack/Adafruit_LEDBackpack.h"

// This #include statement was automatically added by the Spark IDE.
#include "OneWire/OneWire.h"

#define TEMP_SENSOR 0x27
#define MATRICES 3

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix ();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix ();
Adafruit_8x8matrix matrix3 = Adafruit_8x8matrix ();

double dimmRead = 0.0;
int potRead = 0;
double dimmVolt = 0.0;
double dimmCalc = 0.0;
double potVolt = 0.0;

int dimmInt = 0;

int previousPotRead = 0;

//Variables for Temperature
float percentHumidity = 0.0;
uint8_t b = 0;
int currentTemperature = 0;
int desiredTemperature = 0;
int potCheck = 0;
int loops = 0;

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
  
  Spark.variable("percentHumid", &percentHumidity, DOUBLE);
  Spark.variable("currentTemp", &currentTemperature, INT);
  Spark.variable("tempRead", &potRead, INT);
  Spark.variable("tempVolt", &previousPotRead, INT);

// Spark functions and variables
    Spark.function("txt", displaytxt);

}


void loop()
{

  Wire.beginTransmission(TEMP_SENSOR);
  Wire.endTransmission();
  delay(40);
  Wire.requestFrom(TEMP_SENSOR, 4);
  b = Wire.read();
  int humidity = (b << 8) & 0x3f00;
  humidity |= Wire.read();
  percentHumidity = humidity / 163.83;
  int temp = (Wire.read() << 6) & 0x3fc0;
  temp |= Wire.read() >> 2;
  temp *= 165;
  float fTemp = temp / 16383.0 - 40.0;
  fTemp = fTemp * 1.8 + 32.0; // convert to fahrenheit
  currentTemperature = fTemp*10;
  
  //See if Input Temperature has changed
  //Analog Read shows voltages as a number ranging from 0 - 4096.
  previousPotRead = potRead;
  delay(50);
  potRead = analogRead(A0);
  delay(50);
  
  if (previousPotRead > 0){
     if (abs(potRead - previousPotRead) > 64){
         desiredTemperature = getTemperature();
         potRead = analogRead(A0);
    }
  }

  //Multiply tempCalc by 10 to get ones and first decimal places
  //tempVolt = tempVolt * 10;
  //tempCalc = tempVolt;
  char ones = currentTemperature % 10;
  char tens = (currentTemperature / 10) % 10;
  char hundreds = (currentTemperature / 100) % 10;

  matrix3.clear();
  matrix3.setCursor(2, 0);
  matrix3.write(hundreds + '0');
  matrix3.writeDisplay();

  matrix2.clear();
  matrix2.setCursor(0, 0);
  matrix2.write(tens + '0');
  matrix2.drawRect(6,5, 2,2, LED_ON);
  matrix2.writeDisplay();

  matrix1.clear();
  matrix1.setCursor(1, 0);
  matrix1.write(ones + '0');
  matrix1.writeDisplay();

  dimmRead = analogRead(A6);
  dimmVolt = (dimmRead / 4096);
  dimmCalc = 15 * dimmVolt;
  dimmInt = dimmCalc;
  matrix3.setBrightness(dimmInt);
  matrix2.setBrightness(dimmInt);
  matrix1.setBrightness(dimmInt);
  
  delay(250);

}

int getTemperature(){
    // Horiz. position of text -- starts off right edge
  int potCheckOld = 0;
  int counts = 0;
  int potValue = 0.0;
  int potInt = 0;
  
  while (counts < 10){
        potValue = analogRead(A0);
        potVolt = (potValue/4096.0);
        //tempVolt ranges from .5 - 1.0
        potInt = potVolt*100;
        
        if (potCheckOld > 0){
            if (abs(potValue - potCheckOld) < 64){
                counts++;
            }
            else {
                counts = 0;
            }
        }
        
        potCheckOld = potValue;
        
        char ones = potInt % 10;
        char tens = (potInt / 10) % 10;

        matrix3.clear();
        matrix3.setCursor(2, 0);
        matrix3.write(tens + '0');
        matrix3.writeDisplay();
        
        matrix2.clear();
        matrix2.setCursor(0, 0);
        matrix2.write(ones + '0');
        matrix2.writeDisplay();
        
        matrix1.clear();
        matrix1.setCursor(1, 0);
        matrix1.write('F');
        matrix1.writeDisplay();
        delay(200);
  }

  return potCheck;
}

int displaytxt(String txt){
    // Horiz. position of text -- starts off right edge
    int x = 8 * MATRICES;
    int textCount = -6 * txt.length();
    int textLoop = textCount - x;

    for (int i=0; i>textLoop; i--){
    // Draw message in each matrix buffer, offseting each by 8 pixels

    matrix3.clear();
    matrix3.setCursor(x - 0 * 8, 0);
    matrix3.print(txt);

    matrix2.clear();
    matrix2.setCursor(x - 1 * 8, 0);
    matrix2.print(txt);

    matrix1.clear();
    matrix1.setCursor(x - 2 * 8, 0);
    matrix1.print(txt);

    // Write data to matrices in separate loop so it's less jumpy

    matrix1.writeDisplay();
    matrix2.writeDisplay();
    matrix3.writeDisplay();

    // Move text position left by 1 pixel.
    // When it's completely gone off left edge, start over off right.
    x--;
    //if(x < textCount) x = 8 * MATRICES; // 30 = 'HELLO' * 6 pixels/char

    delay(50);
    }
    return 1;
}
