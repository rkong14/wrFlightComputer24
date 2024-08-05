#include <SD.h>

File sdFile;

#include <Adafruit_BMP3XX.h>
#include <bmp3.h>
#include <bmp3_defs.h>

int count = 0;

float currAlt = 0.00;
float prevAlt = 0.00;
float initAlt = 0.00;

int explosiveCharge = 7;

bool checkFalling = false;
float apogee = 0.00;
float heightFallen = 0.00;
bool haveDeployed = false;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  if (!SD.begin(10)) while (1);

  sdFile = SD.open("data.txt", FILE_WRITE); //file name can't be too long
  sdFile.println(F("CODE LAST UPDATED 5/21/24"));
  sdFile.println();
  sdFile.close();


  accelSet();
  altSet();
  alt(); //first reading always a little ridiculous, don't know why
  initAlt = alt();//second reading always back to normal

  pinMode(explosiveCharge, OUTPUT); //tels pin with igniter to be ready to output voltage
  digitalWrite(explosiveCharge, LOW);  //turns off the voltage from that pin

}

void loop() {
  
  sdFile = SD.open("data.txt", FILE_WRITE);
  accelTake();

  prevAlt = currAlt;
  currAlt = alt();
  sdFile.print("currAlt: ");
  sdFile.println(currAlt);

  float heightFallen = apogee - currAlt;

  checkFalling = prevAlt > currAlt;
  if (checkFalling && !haveDeployed) {
    sdFile.print("heightFallen: ");
    sdFile.println(heightFallen);
    sdFile.print("apogee: ");
    sdFile.println(apogee);
    if (heightFallen >= 1) {
      haveDeployed = true;
      sdFile.println("---------------------");
      sdFile.println("   DEPLOYING!!!!!");
      sdFile.println("---------------------");
    }
  } else if (!haveDeployed && apogee <= currAlt) {
    apogee = currAlt;
  }

  double msElasped = millis();
  String minString = String(msElasped / 60000);
  if (minString[minString.length() - 2] == '0' && minString[minString.length() - 1] == '0') {
    sdFile.print(F("mins elasped: "));
    sdFile.println(msElasped / 60000);
  }


  sdFile.println();
  sdFile.close();

  delay(10); // 1/100 second

}
