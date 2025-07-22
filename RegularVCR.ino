#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv0;
Adafruit_DRV2605 drv1;
Adafruit_DRV2605 drv2;
Adafruit_DRV2605 drv3;


#define I2C_SDA 19
#define I2C_SCL 18


const int PWM_CHANNEL0 = 0;
const int PWM_CHANNEL1 = 1;
const int PWM_CHANNEL2 = 2;
const int PWM_CHANNEL3 = 3;      // ESP32 has 16 channels which can generate 16 independent waveforms
const int PWM_FREQ = 32000;     // Recall that Arduino Uno is ~490 Hz. Official ESP32 example uses 5,000Hz
const int PWM_RESOLUTION = 8;
const int JITTER = 23.5;        // We'll use same resolution as Uno (8 bits, 0-255) but ESP32 can go up to 16 bits 
int dutyCycle = 140; // corresponds to voltage delivered by PWM

const int DRV1 = 5;
const int DRV2 = 6;
const int DRV3 = 7;
const int DRV4 = 8;
const int DRVEN = 14;


int perms[24][4] = {
    {DRV1, DRV2, DRV3, DRV4},
    {DRV2, DRV1, DRV3, DRV4},
    {DRV3, DRV1, DRV2, DRV4},
    {DRV1, DRV3, DRV2, DRV4},
    {DRV2, DRV3, DRV1, DRV4},
    {DRV3, DRV2, DRV1, DRV4},
    {DRV3, DRV2, DRV4, DRV1},
    {DRV2, DRV3, DRV4, DRV1},
    {DRV4, DRV3, DRV2, DRV1},
    {DRV3, DRV4, DRV2, DRV1},
    {DRV2, DRV4, DRV3, DRV1},
    {DRV4, DRV2, DRV3, DRV1},
    {DRV4, DRV1, DRV3, DRV2},
    {DRV1, DRV4, DRV3, DRV2},
    {DRV3, DRV4, DRV1, DRV2},
    {DRV4, DRV3, DRV1, DRV2},
    {DRV1, DRV3, DRV4, DRV2},
    {DRV3, DRV1, DRV4, DRV2},
    {DRV2, DRV1, DRV4, DRV3},
    {DRV1, DRV2, DRV4, DRV3},
    {DRV4, DRV2, DRV1, DRV3},
    {DRV2, DRV4, DRV1, DRV3},
    {DRV1, DRV4, DRV2, DRV3},
    {DRV4, DRV1, DRV2, DRV3}
};

void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {



  ledcAttachChannel(DRV1, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL0);
  ledcAttachChannel(DRV2, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL1);
  ledcAttachChannel(DRV3, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL2);
  ledcAttachChannel(DRV4, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL3);
  
  ledcWrite(DRV1, 127);
  ledcWrite(DRV2, 127);
  ledcWrite(DRV3, 127);
  ledcWrite(DRV4, 127);


  pinMode(DRVEN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(DRVEN, 1);



  Wire.begin(I2C_SDA, I2C_SCL);
  TCA9548A(0);
  delay(50);

  Serial.println("Adafruit DRV2605 Basic test");
  while (! drv0.begin()) {
    Serial.println("Could not find DRV2605 0");
    delay(5000);

  }
  drv0.useLRA();
  drv0.setMode(3);
  drv0.writeRegister8(0x1D, 161);


  TCA9548A(1);
  while (! drv1.begin()) {
    Serial.println("Could not find DRV2605 1");
    delay(5000);
  }
  drv1.useLRA();
  drv1.setMode(3);
  drv1.writeRegister8(0x1D, 161);


  TCA9548A(2);
  while (! drv2.begin()) {
    Serial.println("Could not find DRV2605 2");
    delay(5000);
  }
  drv2.useLRA();
  drv2.setMode(3);
  drv2.writeRegister8(0x1D, 161);


  TCA9548A(3);
  while (! drv3.begin()) {
    Serial.println("Could not find DRV2605 3");
    delay(5000);
  }
  drv3.useLRA();
  drv3.setMode(3);
  drv3.writeRegister8(0x1D, 161);





  digitalWrite(LED_BUILTIN, 1);
  

}

void loop() {
  //Regular VCR
  for(int period = 1; period < 4; period++){
    int pattern = random(25); // picks random perumtation for fingers in current period T_CR
    //Serial.println(pattern);
    for(int j = 0; j < 4; j++){
      ledcWrite(perms[pattern][j], dutyCycle);
      delay(100);
      ledcWrite(perms[pattern][j], 127);
      delay(66);
    }
  }
  delay(1300);
}
