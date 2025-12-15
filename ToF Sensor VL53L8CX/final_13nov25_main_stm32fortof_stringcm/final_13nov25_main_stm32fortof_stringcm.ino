/*
******************************************************************************
* @file    Main program for STM32 Blue Pill and VL53L8CX (Elderly Care RObot)
* @brief   Output format rapat {0;1;2;3;...;63} satuan cm tanpa spasi
******************************************************************************
*/

#include <vl53l8cx.h>

#ifdef ARDUINO_SAM_DUE
  #define DEV_I2C Wire1
#else
  #define DEV_I2C Wire
#endif
#define SerialPort Serial

#define LPN_PIN A3
#define PWREN_PIN 11

VL53L8CX sensor_vl53l8cx_top(&DEV_I2C, LPN_PIN);
uint8_t res = VL53L8CX_RESOLUTION_8X8;
uint8_t status;

/* Setup ---------------------------------------------------------------------*/
void setup() {
  if (PWREN_PIN >= 0) {
    pinMode(PWREN_PIN, OUTPUT);
    digitalWrite(PWREN_PIN, HIGH);
    delay(100);
  }

  SerialPort.begin(115200);
  SerialPort.println("Program berjalan...");
  DEV_I2C.begin();
  SerialPort.println("Inisialisasi sensor...");

  sensor_vl53l8cx_top.begin();
  status = sensor_vl53l8cx_top.init();
  status = sensor_vl53l8cx_top.set_resolution(res);
  status = sensor_vl53l8cx_top.start_ranging();

  Serial.println("VL53L8CX siap. Output format {0;1;2;...;63} (cm)");
  Serial.println("Sensor berjalan...\n");
}

/* Main Loop ----------------------------------------------------------------*/
void loop() {
  VL53L8CX_ResultsData Results;
  uint8_t NewDataReady = 0;

  // tunggu sampai data baru tersedia
  do {
    status = sensor_vl53l8cx_top.check_data_ready(&NewDataReady);
  } while (!NewDataReady);

  if ((!status) && (NewDataReady != 0)) {
    status = sensor_vl53l8cx_top.get_ranging_data(&Results);
    print_result(&Results);
  }

  delay(100);
}

/* Print result: 8x8, string, cm --------------------------------------------*/
void print_result(VL53L8CX_ResultsData *Result) {
  Serial.print("{");
  for (uint8_t i = 0; i < 64; i++) {
    if (Result->nb_target_detected[i] > 0 && Result->distance_mm[i] > 0) {
      Serial.print(Result->distance_mm[i] / 10.0, 1); // mm → cm, 1 desimal
    } else {
      Serial.print("X");
    }
    if (i < 63) Serial.print(";");
  }
  Serial.println("}");
}
