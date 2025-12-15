/*
******************************************************************************
* @file    Main program for STM32 Blue Pill and VL53L8CX (Elderly Care RObot)
* @brief   Versi modifikasi: menampilkan hasil dalam bentuk tabel 8x8 (cm)
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

void print_result(VL53L8CX_ResultsData *Result);
void clear_screen(void);
void handle_cmd(uint8_t cmd);
void display_commands_banner(void);

VL53L8CX sensor_vl53l8cx_top(&DEV_I2C, LPN_PIN);

bool EnableAmbient = false;
bool EnableSignal = false;
uint8_t res = VL53L8CX_RESOLUTION_8X8;  // Default 8x8
char report[256];
uint8_t status;

/* Setup ---------------------------------------------------------------------*/
void setup()
{
  // Enable PWREN pin if present
  if (PWREN_PIN >= 0) {
    pinMode(PWREN_PIN, OUTPUT);
    digitalWrite(PWREN_PIN, HIGH);
    delay(10);
  }

  SerialPort.begin(115200);
  DEV_I2C.begin();

  // Initialize sensor
  sensor_vl53l8cx_top.begin();
  status = sensor_vl53l8cx_top.init();
  status = sensor_vl53l8cx_top.set_resolution(res);
  status = sensor_vl53l8cx_top.start_ranging();

  Serial.println("VL53L8CX siap. Menampilkan hasil dalam cm (8x8 grid):");
}

/* Main Loop -----------------------------------------------------------------*/
void loop()
{
  VL53L8CX_ResultsData Results;
  uint8_t NewDataReady = 0;

  do {
    status = sensor_vl53l8cx_top.check_data_ready(&NewDataReady);
  } while (!NewDataReady);

  if ((!status) && (NewDataReady != 0)) {
    status = sensor_vl53l8cx_top.get_ranging_data(&Results);
    print_result(&Results);
  }

  if (Serial.available() > 0) {
    handle_cmd(Serial.read());
  }

  delay(200);
}

/* Print result: 8x8, table, cm ---------------------------------------------*/
void print_result(VL53L8CX_ResultsData *Result)
{
  Serial.println("\n=============================");
  Serial.println("Hasil Pengukuran (cm) - 8x8 Grid");
  Serial.println("=============================");

  for (uint8_t y = 0; y < 8; y++) {
    for (uint8_t x = 0; x < 8; x++) {
      uint8_t index = y * 8 + x;

      if (Result->nb_target_detected[index] > 0) {
        float distance_cm = Result->distance_mm[index] / 10.0;
        Serial.print(distance_cm, 1);
        Serial.print("\t");
      } else {
        Serial.print("X\t");
      }
    }
    Serial.println();
  }

  Serial.println("=============================\n");
}

/* Include Program for STM32 Blue PIll ------------------------------------- */
void toggle_resolution(void)
{
  status = sensor_vl53l8cx_top.stop_ranging();

  switch (res) {
    case VL53L8CX_RESOLUTION_4X4:
      res = VL53L8CX_RESOLUTION_8X8;
      break;
    case VL53L8CX_RESOLUTION_8X8:
      res = VL53L8CX_RESOLUTION_4X4;
      break;
    default:
      break;
  }

  status = sensor_vl53l8cx_top.set_resolution(res);
  status = sensor_vl53l8cx_top.start_ranging();
}

void toggle_signal_and_ambient(void)
{
  EnableAmbient = !EnableAmbient;
  EnableSignal = !EnableSignal;
}

void clear_screen(void)
{
  snprintf(report, sizeof(report), "%c[2J", 27); /* ESC command */
  SerialPort.print(report);
}

void display_commands_banner(void)
{
  snprintf(report, sizeof(report), "%c[2H", 27); /* ESC command */
  SerialPort.print(report);

  Serial.print("VL53L8CX Simple Ranging Demo\n");
  Serial.print("--------------------------------------\n\n");
  Serial.print(" 'r' : change resolution\n");
  Serial.print(" 's' : enable signal and ambient\n");
  Serial.print(" 'c' : clear screen\n");
  Serial.print("\n");
}

void handle_cmd(uint8_t cmd)
{
  switch (cmd) {
    case 'r':
      toggle_resolution();
      clear_screen();
      break;
    case 's':
      toggle_signal_and_ambient();
      clear_screen();
      break;
    case 'c':
      clear_screen();
      break;
    default:
      break;
  }
}
