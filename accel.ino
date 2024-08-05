#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL375.h>

Adafruit_ADXL375 accel = Adafruit_ADXL375(12345);

void accelSet() {
  if(!accel.begin()) {
    sdFile.println("accelFail");
    while(1);
  }

  accel.setTrimOffsets(0, 0, 0);
  
  delay(1000);
  int16_t x, y, z;
  x = accel.getX();
  y = accel.getY();
  z = accel.getZ();
  
  accel.setTrimOffsets(-(x+2)/4, 
                       -(y+2)/4, 
                       -(z+2)/4);
  
  int8_t x_offset, y_offset, z_offset;
  accel.getTrimOffsets(&x_offset, &y_offset, &z_offset);
}

void accelTake() {
  sensors_event_t event;
  accel.getEvent(&event);

  // m/2^2
  sdFile.print("X: "); sdFile.println(event.acceleration.x);
  sdFile.print("Y: "); sdFile.println(event.acceleration.y);
  sdFile.print("Z: "); sdFile.println(event.acceleration.z);
  delay(500);

}
