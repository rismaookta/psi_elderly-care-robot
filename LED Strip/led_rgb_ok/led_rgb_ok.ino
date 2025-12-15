#include <Adafruit_NeoPixel.h>

#define PIN 6            // Pin data untuk LED strip
#define NUM_LEDS 120     // Jumlah LED
#define BRIGHTNESS 80    // Sesuaikan agar aman bila pakai 5V Arduino

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Array warna (R, G, B)
uint32_t warna[] = {
  strip.Color(255, 0, 0),     // Merah
  strip.Color(0, 255, 0),     // Hijau
  strip.Color(0, 0, 255),     // Biru
  strip.Color(255, 255, 0),   // Kuning
  strip.Color(255, 0, 255),   // Ungu
  strip.Color(0, 255, 255),   // Cyan
  strip.Color(255, 255, 255)  // Putih
};

int jumlahWarna = sizeof(warna) / sizeof(warna[0]);

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Matikan semua LED
}

void loop() {
  for (int w = 0; w < jumlahWarna; w++) {
    tampilkanWarna(warna[w]);
    delay(2000); // 2 detik per warna
  }
}

// Fungsi untuk menampilkan 1 warna ke semua LED
void tampilkanWarna(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
