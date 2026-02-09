/*
 * Ejemplo simple RGB para Matriz LED WS2812B 8x16
 * UNIT ELECTRONICS - RGB LED Matrix Module
 * 
 * Este ejemplo muestra un ciclo continuo de colores RGB en toda la matriz
 */

#include <Adafruit_NeoPixel.h>

// Configuración del hardware
#define PIN        6        // Pin de datos de la matriz
#define NUM_LEDS   128      // Total de LEDs (8x16 = 128)
#define BRILLO     64       // Brillo (0-255)

// Crear objeto NeoPixel
Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Inicializar la tira LED
  strip.begin();
  strip.setBrightness(BRILLO);
  strip.clear();
  strip.show();
  
  // Inicializar comunicación serial (opcional)
  Serial.begin(9600);
  Serial.println("=== Ejemplo RGB Simple ===");
  Serial.println("Matriz 8x16 iniciada");
}

void loop() {
  // Ciclo de colores RGB
  
  // ROJO
  colorCompleto(strip.Color(255, 0, 0));
  delay(1000);
  
  // VERDE
  colorCompleto(strip.Color(0, 255, 0));
  delay(1000);
  
  // AZUL
  colorCompleto(strip.Color(0, 0, 255));
  delay(1000);
  
  // AMARILLO (Rojo + Verde)
  colorCompleto(strip.Color(255, 255, 0));
  delay(1000);
  
  // CYAN (Verde + Azul)
  colorCompleto(strip.Color(0, 255, 255));
  delay(1000);
  
  // MAGENTA (Rojo + Azul)
  colorCompleto(strip.Color(255, 0, 255));
  delay(1000);
  
  // BLANCO (Todos los colores)
  colorCompleto(strip.Color(255, 255, 255));
  delay(1000);
  
  // APAGADO
  colorCompleto(strip.Color(0, 0, 0));
  delay(500);
}

// Función para mostrar un color en toda la matriz
void colorCompleto(uint32_t color) {
  for(int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
