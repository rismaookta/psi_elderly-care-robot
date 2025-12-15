#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUM_LEDS 120
#define BRIGHTNESS 80
#define DELAY_TIME 50   // Semakin kecil = semakin cepat

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Warna MEJIKUHIBINIU (Rainbow Indonesia)
uint32_t rainbow[] = {
  // M  E      J       I      K      U      H      I2    B       I3     N       I4     U2
  // Merah, Jingga, Kuning, Hijau, Biru, Nila, Ungu
  // Kita ambil 7 warna saja (MEJIKUH)
  // Untuk pola ular cukup 7 warna: Merah–Jingga–Kuning–Hijau–Biru–Nila–Ungu
  // (Urutan lengkap MEJIKUHIBINIU bisa ditambahkan jika ingin)
  0xFF0000, // Merah
  0xFF4500, // Jingga (Orange)
  0xFFFF00, // Kuning
  0x00FF00, // Hijau
  0x0000FF, // Biru
  0x4B0082, // Nila (Indigo)
  0x8A2BE2  // Ungu (Violet)
};

int jumlahWarna = sizeof(rainbow) / sizeof(rainbow[0]);

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  for (int head = 0; head < NUM_LEDS; head++) {

    strip.clear();

    // Kepala (LED paling depan)
    strip.setPixelColor(head, rainbow[0]); // Merah

    // Ekor mengikuti urutan warna
    for (int i = 1; i < jumlahWarna; i++) {
      int tail = head - i;
      if (tail >= 0) {
        strip.setPixelColor(tail, rainbow[i]);
      }
    }

    strip.show();
    delay(DELAY_TIME);
  }
}
