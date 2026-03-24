/*
 * ============================================================
 * NEO TALENT LAND - Demostraciones Visuales Impactantes
 * UNIT ELECTRONICS - WS2812B 8x80 RGB LED Matrix Module
 * ============================================================
 * 
 * ORIENTACIÓN FÍSICA DEL DISPLAY:
 *   LED Y (0-79) = eje HORIZONTAL del display (ancho)
 *   LED X (0-7)  = eje VERTICAL del display (alto)
 *   Pantalla: 80px de ancho × 8px de alto
 *
 * Efectos incluidos:
 *   1. Matrix Rain          2. Plasma Effect
 *   3. Fire Effect           4. Shooting Stars
 *   5. Hypnotic Spiral       6. Beating Heart
 *   7. DNA Helix             8. Pulse Wave
 *   9. VU Meter             10. Tetris Rain
 *  11. Cylon Scanner        12. Confetti Burst
 *  13. Lava Lamp            14. Lightning Storm
 *  15. Rainbow Wave         16. Fireworks Show
 *  17. Mexican Flag Wave    18. Game of Life
 *  19. Snake Animation      20. Ripple Effect
 *  21. Countdown            22. Warp Speed
 *  23. Rainbow Cycle        24. Bandera Mexicana (estática)
 *  25. Bandera Ondeando     26. Bandera Ondulante
 *  27. Fuego Individual     28. Fuegos Simultáneos
 *  29. Espectáculo Fuegos
 *
 * Para: Talent Land 2026 - UNIT Electronics
 * ============================================================
 */

#include <Adafruit_NeoPixel.h>

// ============ CONFIGURACIÓN DE HARDWARE ============
#define PIN           12
#define NUM_LEDS      640
#define MATRIX_WIDTH  8       // LED X dimension (vertical en display)
#define MATRIX_HEIGHT 80      // LED Y dimension (horizontal en display)
#define BRILLO_GENERAL 100    // 0-255

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// ============ ESPACIO DE PANTALLA ============
// La pantalla física: ancho=80 (LED Y), alto=8 (LED X)
#define SCR_W  MATRIX_HEIGHT  // 80 - ancho horizontal de pantalla
#define SCR_H  MATRIX_WIDTH   // 8  - alto vertical de pantalla

// ============ TABLAS TRIGONOMÉTRICAS ============
const int8_t sinTable[] PROGMEM = {
  0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,48,51,54,57,59,62,65,67,
  70,73,75,78,80,82,85,87,89,91,94,96,98,100,102,103,105,107,108,110,
  112,113,114,116,117,118,119,120,121,122,123,124,124,125,126,126,127,
  127,127,127,127,127,127,127,127,127,126,126,125,124,124,123,122,121,
  120,119,118,117,116,114,113,112,110,108,107,105,103,102,100,98,96,94,
  91,89,87,85,82,80,78,75,73,70,67,65,62,59,57,54,51,48,45,42,39,36,
  33,30,27,24,21,18,15,12,9,6,3,0,-3,-6,-9,-12,-15,-18,-21,-24,-27,-30,
  -33,-36,-39,-42,-45,-48,-51,-54,-57,-59,-62,-65,-67,-70,-73,-75,-78,
  -80,-82,-85,-87,-89,-91,-94,-96,-98,-100,-102,-103,-105,-107,-108,-110,
  -112,-113,-114,-116,-117,-118,-119,-120,-121,-122,-123,-124,-124,-125,
  -126,-126,-127,-127,-127,-127,-127,-127,-127,-127,-127,-127,-126,-126,
  -125,-124,-124,-123,-122,-121,-120,-119,-118,-117,-116,-114,-113,-112,
  -110,-108,-107,-105,-103,-102,-100,-98,-96,-94,-91,-89,-87,-85,-82,-80,
  -78,-75,-73,-70,-67,-65,-62,-59,-57,-54,-51,-48,-45,-42,-39,-36,-33,
  -30,-27,-24,-21,-18,-15,-12,-9,-6,-3
};

int8_t fastSin(uint8_t angle) { return pgm_read_byte(&sinTable[angle]); }
int8_t fastCos(uint8_t angle) { return pgm_read_byte(&sinTable[(angle + 64) & 255]); }

// ============ FUNCIÓN ESCALADO DE COLOR ============
uint32_t escalarColor(uint8_t r, uint8_t g, uint8_t b) {
  return strip.Color(
    (r * BRILLO_GENERAL) / 255,
    (g * BRILLO_GENERAL) / 255,
    (b * BRILLO_GENERAL) / 255
  );
}

uint32_t escalarColor32(uint32_t color) {
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  return escalarColor(r, g, b);
}

// ============ FUNCIONES PIXEL EN ESPACIO LED ============
int getPixelIndex(int x, int y) {
  if (x < 0 || x >= MATRIX_WIDTH || y < 0 || y >= MATRIX_HEIGHT) return -1;
  return y * MATRIX_WIDTH + x;
}

void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  int idx = getPixelIndex(x, y);
  if (idx >= 0) strip.setPixelColor(idx, escalarColor(r, g, b));
}

void setPixelRaw(int x, int y, uint32_t color) {
  int idx = getPixelIndex(x, y);
  if (idx >= 0) strip.setPixelColor(idx, color);
}

// ============ FUNCIONES PIXEL EN ESPACIO PANTALLA ============
// sx = horizontal (0..SCR_W-1=79), sy = vertical (0..SCR_H-1=7, 0=arriba)
void sP(int sx, int sy, uint8_t r, uint8_t g, uint8_t b) {
  setPixel(sy, sx, r, g, b);  // Transponer: LED_x=sy, LED_y=sx
}

void sPR(int sx, int sy, uint32_t color) {
  setPixelRaw(sy, sx, color);
}

int sIdx(int sx, int sy) {
  return getPixelIndex(sy, sx);
}

// ============ HSV a RGB rápido ============
uint32_t hsvToRgb(uint8_t h, uint8_t s, uint8_t v) {
  uint8_t region = h / 43;
  uint8_t remainder = (h - (region * 43)) * 6;
  uint8_t p = (v * (255 - s)) >> 8;
  uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:  return escalarColor(v, t, p);
    case 1:  return escalarColor(q, v, p);
    case 2:  return escalarColor(p, v, t);
    case 3:  return escalarColor(p, q, v);
    case 4:  return escalarColor(t, p, v);
    default: return escalarColor(v, p, q);
  }
}

// ============ FUENTE 5x8 (para texto - usa espacio LED directo) ============
const uint8_t font5x8[][5] = {
  {0x00,0x00,0x00,0x00,0x00}, // 32: Espacio
  {0x00,0x00,0x5F,0x00,0x00}, // 33: !
  {0x00,0x07,0x00,0x07,0x00}, // 34: "
  {0x14,0x7F,0x14,0x7F,0x14}, // 35: #
  {0x24,0x2A,0x7F,0x2A,0x12}, // 36: $
  {0x23,0x13,0x08,0x64,0x62}, // 37: %
  {0x36,0x49,0x55,0x22,0x50}, // 38: &
  {0x00,0x05,0x03,0x00,0x00}, // 39: '
  {0x00,0x1C,0x22,0x41,0x00}, // 40: (
  {0x00,0x41,0x22,0x1C,0x00}, // 41: )
  {0x14,0x08,0x3E,0x08,0x14}, // 42: *
  {0x08,0x08,0x3E,0x08,0x08}, // 43: +
  {0x00,0x50,0x30,0x00,0x00}, // 44: ,
  {0x08,0x08,0x08,0x08,0x08}, // 45: -
  {0x00,0x60,0x60,0x00,0x00}, // 46: .
  {0x20,0x10,0x08,0x04,0x02}, // 47: /
  {0x3E,0x51,0x49,0x45,0x3E}, // 48: 0
  {0x00,0x42,0x7F,0x40,0x00}, // 49: 1
  {0x42,0x61,0x51,0x49,0x46}, // 50: 2
  {0x21,0x41,0x45,0x4B,0x31}, // 51: 3
  {0x18,0x14,0x12,0x7F,0x10}, // 52: 4
  {0x27,0x45,0x45,0x45,0x39}, // 53: 5
  {0x3C,0x4A,0x49,0x49,0x30}, // 54: 6
  {0x01,0x71,0x09,0x05,0x03}, // 55: 7
  {0x36,0x49,0x49,0x49,0x36}, // 56: 8
  {0x06,0x49,0x49,0x29,0x1E}, // 57: 9
  {0x00,0x36,0x36,0x00,0x00}, // 58: :
  {0x00,0x56,0x36,0x00,0x00}, // 59: ;
  {0x08,0x14,0x22,0x41,0x00}, // 60: <
  {0x14,0x14,0x14,0x14,0x14}, // 61: =
  {0x00,0x41,0x22,0x14,0x08}, // 62: >
  {0x02,0x01,0x51,0x09,0x06}, // 63: ?
  {0x32,0x49,0x79,0x41,0x3E}, // 64: @
  {0x7E,0x11,0x11,0x11,0x7E}, // 65: A
  {0x7F,0x49,0x49,0x49,0x36}, // 66: B
  {0x3E,0x41,0x41,0x41,0x22}, // 67: C
  {0x7F,0x41,0x41,0x22,0x1C}, // 68: D
  {0x7F,0x49,0x49,0x49,0x41}, // 69: E
  {0x7F,0x09,0x09,0x09,0x01}, // 70: F
  {0x3E,0x41,0x49,0x49,0x7A}, // 71: G
  {0x7F,0x08,0x08,0x08,0x7F}, // 72: H
  {0x00,0x41,0x7F,0x41,0x00}, // 73: I
  {0x20,0x40,0x41,0x3F,0x01}, // 74: J
  {0x7F,0x08,0x14,0x22,0x41}, // 75: K
  {0x7F,0x40,0x40,0x40,0x40}, // 76: L
  {0x7F,0x02,0x0C,0x02,0x7F}, // 77: M
  {0x7F,0x04,0x08,0x10,0x7F}, // 78: N
  {0x3E,0x41,0x41,0x41,0x3E}, // 79: O
  {0x7F,0x09,0x09,0x09,0x06}, // 80: P
  {0x3E,0x41,0x51,0x21,0x5E}, // 81: Q
  {0x7F,0x09,0x19,0x29,0x46}, // 82: R
  {0x46,0x49,0x49,0x49,0x31}, // 83: S
  {0x01,0x01,0x7F,0x01,0x01}, // 84: T
  {0x3F,0x40,0x40,0x40,0x3F}, // 85: U
  {0x1F,0x20,0x40,0x20,0x1F}, // 86: V
  {0x3F,0x40,0x38,0x40,0x3F}, // 87: W
  {0x63,0x14,0x08,0x14,0x63}, // 88: X
  {0x07,0x08,0x70,0x08,0x07}, // 89: Y
  {0x61,0x51,0x49,0x45,0x43}  // 90: Z
};

int getCharIndex(char c) {
  if (c >= 32 && c <= 90) return c - 32;
  if (c >= 97 && c <= 122) return (c - 97) + (65 - 32);
  return 0;
}

// displayChar - usa espacio LED directo (ya calibrado para el hardware)
void displayChar(char c, int offsetX, int offsetY, uint32_t color) {
  int ci = getCharIndex(c);
  for (int col = 0; col < 5; col++) {
    uint8_t columnData = font5x8[ci][4 - col];
    for (int row = 0; row < 8; row++) {
      if (columnData & (1 << row)) {
        int px = offsetX + row;
        int py = offsetY + col;
        int idx = getPixelIndex(px, py);
        if (idx >= 0 && idx < NUM_LEDS) strip.setPixelColor(idx, color);
      }
    }
  }
}

String reverseString(String text) {
  String r = "";
  for (int i = text.length() - 1; i >= 0; i--) r += text[i];
  return r;
}

void scrollText(String text, uint32_t color, int delayTime) {
  int textLen = text.length() * 6;
  for (int offset = -textLen; offset < MATRIX_HEIGHT + 5; offset++) {
    strip.clear();
    for (int i = 0; i < (int)text.length(); i++) {
      int charY = offset + (i * 6);
      if (charY >= -5 && charY < MATRIX_HEIGHT + 5) {
        displayChar(text[i], 0, charY, color);
      }
    }
    strip.show();
    delay(delayTime);
  }
}

void rainbowScrollText(String text, int delayTime) {
  int textLen = text.length() * 6;
  uint16_t hue = 0;
  for (int offset = -textLen; offset < MATRIX_HEIGHT + 5; offset++) {
    strip.clear();
    for (int i = 0; i < (int)text.length(); i++) {
      int charY = offset + (i * 6);
      if (charY >= -5 && charY < MATRIX_HEIGHT + 5) {
        uint32_t color = escalarColor32(strip.gamma32(
          strip.ColorHSV((hue + i * 65536 / text.length()) % 65536)));
        displayChar(text[i], 0, charY, color);
      }
    }
    strip.show();
    delay(delayTime);
    hue += 512;
  }
}

// Atenuar todo el framebuffer (factor: 0-9, más bajo = más fade)
void fadeAll(uint8_t factor) {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint32_t c = strip.getPixelColor(i);
    uint8_t r = ((c >> 16) & 0xFF) * factor / 10;
    uint8_t g = ((c >> 8) & 0xFF) * factor / 10;
    uint8_t b = (c & 0xFF) * factor / 10;
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

// ================================================================
//  1. MATRIX RAIN - Lluvia estilo Matrix
//     Gotas caen verticalmente (sy: 0→7) en columnas (sx: 0-79)
// ================================================================
void matrixRain(int duration) {
  #define MAX_DROPS 40
  int dropSX[MAX_DROPS], dropSY[MAX_DROPS], dropSpd[MAX_DROPS], dropLen[MAX_DROPS];
  
  for (int i = 0; i < MAX_DROPS; i++) {
    dropSX[i] = random(0, SCR_W);
    dropSY[i] = random(-SCR_H, 0);
    dropSpd[i] = random(1, 3);
    dropLen[i] = random(2, SCR_H);
  }
  
  for (int frame = 0; frame < duration; frame++) {
    fadeAll(6);
    
    for (int i = 0; i < MAX_DROPS; i++) {
      for (int t = 0; t < dropLen[i]; t++) {
        int py = dropSY[i] - t;
        if (py >= 0 && py < SCR_H && dropSX[i] >= 0 && dropSX[i] < SCR_W) {
          if (t == 0) {
            sP(dropSX[i], py, 200, 255, 200);
          } else {
            int intensity = 255 - (t * 255 / dropLen[i]);
            sP(dropSX[i], py, 0, intensity, 0);
          }
        }
      }
      
      dropSY[i] += dropSpd[i];
      if (dropSY[i] - dropLen[i] > SCR_H) {
        dropSX[i] = random(0, SCR_W);
        dropSY[i] = random(-4, -1);
        dropSpd[i] = random(1, 3);
        dropLen[i] = random(2, SCR_H);
      }
    }
    
    strip.show();
    delay(40);
  }
}

// ================================================================
//  2. PLASMA EFFECT - Efecto plasma psicodélico
// ================================================================
void plasmaEffect(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        uint8_t a1 = (uint8_t)(sx * 8 + frame * 3);
        uint8_t a2 = (uint8_t)(sy * 32 + frame * 2);
        uint8_t a3 = (uint8_t)((sx + sy) * 10 + frame * 4);
        uint8_t a4 = (uint8_t)((sx - sy + 80) * 12 + frame * 5);
        
        int v = fastSin(a1) + fastSin(a2) + fastSin(a3) + fastSin(a4);
        uint8_t hue = (uint8_t)((v + 512) / 4);
        
        sPR(sx, sy, hsvToRgb(hue, 255, 200));
      }
    }
    strip.show();
    delay(15);
  }
}

// ================================================================
//  3. FIRE EFFECT - Fuego realista (80 ancho × 8 alto)
//     Fuego sube desde abajo (sy=7) hacia arriba (sy=0)
// ================================================================
void fireEffect(int duration) {
  uint8_t heat[SCR_W][SCR_H];
  memset(heat, 0, sizeof(heat));
  
  for (int frame = 0; frame < duration; frame++) {
    // Enfriar
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        int cool = random(0, 4);
        heat[sx][sy] = (heat[sx][sy] > cool) ? heat[sx][sy] - cool : 0;
      }
    }
    
    // Propagar calor hacia arriba (sy decrece)
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H - 1; sy++) {
        int nsx = sx + random(-1, 2);
        nsx = constrain(nsx, 0, SCR_W - 1);
        heat[sx][sy] = (heat[nsx][sy + 1] + heat[sx][sy + 1] + heat[sx][sy]) / 3;
      }
    }
    
    // Chispas en la base
    for (int sx = 0; sx < SCR_W; sx++) {
      if (random(0, 3) == 0) {
        int sy = SCR_H - 1 - random(0, 2);
        heat[sx][sy] = min(255, (int)(heat[sx][sy] + random(160, 256)));
      }
    }
    
    // Renderizar
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        uint8_t h = heat[sx][sy];
        uint8_t r, g, b;
        if (h < 85) { r = h * 3; g = 0; b = 0; }
        else if (h < 170) { r = 255; g = (h - 85) * 3; b = 0; }
        else { r = 255; g = 255; b = (h - 170) * 3; }
        sP(sx, sy, r, g, b);
      }
    }
    
    strip.show();
    delay(30);
  }
}

// ================================================================
//  4. SHOOTING STARS - Estrellas fugaces cruzando horizontalmente
// ================================================================
void shootingStars(int duration) {
  #define MAX_STARS 8
  float starSX[MAX_STARS], starSY[MAX_STARS];
  float starDSX[MAX_STARS], starDSY[MAX_STARS];
  uint8_t starHue[MAX_STARS];
  int starLen[MAX_STARS];
  bool starActive[MAX_STARS];
  
  for (int i = 0; i < MAX_STARS; i++) starActive[i] = false;
  
  for (int frame = 0; frame < duration; frame++) {
    fadeAll(6);
    
    for (int i = 0; i < MAX_STARS; i++) {
      if (!starActive[i] && random(0, 6) == 0) {
        starActive[i] = true;
        starHue[i] = random(0, 256);
        starLen[i] = random(4, 12);
        
        int dir = random(0, 4);
        switch (dir) {
          case 0:
            starSX[i] = 0; starSY[i] = random(0, SCR_H);
            starDSX[i] = 2.5; starDSY[i] = (random(-10, 10)) / 20.0;
            break;
          case 1:
            starSX[i] = SCR_W - 1; starSY[i] = random(0, SCR_H);
            starDSX[i] = -2.5; starDSY[i] = (random(-10, 10)) / 20.0;
            break;
          case 2:
            starSX[i] = 0; starSY[i] = SCR_H - 1;
            starDSX[i] = 2.0; starDSY[i] = -0.3;
            break;
          case 3:
            starSX[i] = SCR_W - 1; starSY[i] = SCR_H - 1;
            starDSX[i] = -2.0; starDSY[i] = -0.3;
            break;
        }
      }
    }
    
    for (int i = 0; i < MAX_STARS; i++) {
      if (starActive[i]) {
        int sx = (int)starSX[i], sy = (int)starSY[i];
        if (sx >= 0 && sx < SCR_W && sy >= 0 && sy < SCR_H)
          sPR(sx, sy, hsvToRgb(starHue[i], 80, 255));
        
        for (int t = 1; t < starLen[i]; t++) {
          int tx = (int)(starSX[i] - starDSX[i] * t);
          int ty = (int)(starSY[i] - starDSY[i] * t);
          if (tx >= 0 && tx < SCR_W && ty >= 0 && ty < SCR_H) {
            uint8_t bright = 200 - t * (200 / starLen[i]);
            sPR(tx, ty, hsvToRgb(starHue[i], 255, bright));
          }
        }
        
        starSX[i] += starDSX[i];
        starSY[i] += starDSY[i];
        
        if (starSX[i] < -15 || starSX[i] > SCR_W + 15 ||
            starSY[i] < -5 || starSY[i] > SCR_H + 5)
          starActive[i] = false;
      }
    }
    
    strip.show();
    delay(20);
  }
}

// ================================================================
//  5. HYPNOTIC SPIRAL - Espiral hipnótica
// ================================================================
void hypnoticSpiral(int duration) {
  int cx = SCR_W / 2;
  int cy = SCR_H / 2;
  
  for (int frame = 0; frame < duration; frame++) {
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        int dx = sx - cx;
        int dy = (sy - cy) * 5;
        
        int angle = (int)(atan2((float)dy, (float)dx) * 40.74);
        int dist = (int)(sqrt((float)(dx * dx + dy * dy)) * 6);
        
        uint8_t hue = (uint8_t)(angle + dist - frame * 4);
        uint8_t bright = 180 + (fastSin((uint8_t)(dist + frame * 2)) / 2);
        
        sPR(sx, sy, hsvToRgb(hue, 255, bright));
      }
    }
    strip.show();
    delay(20);
  }
}

// ================================================================
//  6. BEATING HEART - Corazón latiendo
// ================================================================
const uint8_t heartBitmap[7] = {
  0b01101100,
  0b11111110,
  0b11111110,
  0b11111110,
  0b01111100,
  0b00111000,
  0b00010000
};

void beatingHeart(int beats) {
  for (int beat = 0; beat < beats; beat++) {
    // Sístole
    for (int phase = 0; phase < 20; phase++) {
      fadeAll(5);
      uint8_t brightness = 150 + (phase < 10 ? phase * 10 : (20 - phase) * 10);
      
      for (int rep = 0; rep < SCR_W / 10; rep++) {
        int baseX = rep * 10 + 1;
        for (int row = 0; row < 7; row++) {
          for (int col = 0; col < 8; col++) {
            if (heartBitmap[row] & (1 << (7 - col))) {
              int sx = baseX + col;
              int sy = row;
              if (sx >= 0 && sx < SCR_W && sy >= 0 && sy < SCR_H) {
                uint8_t g = phase < 10 ? 0 : (phase - 10) * 5;
                uint8_t b = phase < 10 ? 0 : (phase - 10) * 8;
                sP(sx, sy, brightness, g, b);
              }
            }
          }
        }
      }
      strip.show();
      delay(25);
    }
    delay(100);
    
    // Segundo latido rápido
    for (int phase = 0; phase < 12; phase++) {
      fadeAll(5);
      uint8_t brightness = 100 + (phase < 6 ? phase * 25 : (12 - phase) * 25);
      
      for (int rep = 0; rep < SCR_W / 10; rep++) {
        int baseX = rep * 10 + 1;
        for (int row = 0; row < 7; row++) {
          for (int col = 0; col < 8; col++) {
            if (heartBitmap[row] & (1 << (7 - col))) {
              int sx = baseX + col;
              int sy = row;
              if (sx >= 0 && sx < SCR_W && sy >= 0 && sy < SCR_H)
                sP(sx, sy, brightness, 0, brightness / 4);
            }
          }
        }
      }
      strip.show();
      delay(20);
    }
    delay(300);
  }
}

// ================================================================
//  7. DNA HELIX - Doble hélice horizontal (80px de largo)
// ================================================================
void dnaHelix(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    for (int sx = 0; sx < SCR_W; sx++) {
      float angle = (sx * 0.3 + frame * 0.15);
      int sy1 = (int)(3.5 + 3.0 * sin(angle));
      int sy2 = (int)(3.5 + 3.0 * sin(angle + 3.14159));
      
      float depth1 = cos(angle);
      float depth2 = cos(angle + 3.14159);
      
      uint8_t bright1 = (uint8_t)(128 + 127 * depth1);
      uint8_t bright2 = (uint8_t)(128 + 127 * depth2);
      
      if (sy1 >= 0 && sy1 < SCR_H) sP(sx, sy1, 0, bright1, bright1);
      if (sy2 >= 0 && sy2 < SCR_H) sP(sx, sy2, bright2, 0, bright2);
      
      if (sx % 6 == (frame / 2) % 6) {
        int minY = min(sy1, sy2);
        int maxY = max(sy1, sy2);
        for (int by = minY; by <= maxY; by++) {
          if (by >= 0 && by < SCR_H) {
            if ((by + sx) % 2 == 0) sP(sx, by, 200, 200, 0);
            else sP(sx, by, 0, 200, 100);
          }
        }
      }
    }
    
    strip.show();
    delay(30);
  }
}

// ================================================================
//  8. PULSE WAVE - Ondas expandiéndose desde centros
// ================================================================
void pulseWave(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    int numPulses = 4;
    int pulseCenter[] = {10, 30, 50, 70};
    
    for (int p = 0; p < numPulses; p++) {
      int center = pulseCenter[p];
      int radius = (frame + p * 12) % 25;
      uint8_t hue = (frame * 3 + p * 64) & 0xFF;
      
      for (int sx = 0; sx < SCR_W; sx++) {
        for (int sy = 0; sy < SCR_H; sy++) {
          int dist = abs(sx - center) + abs(sy - SCR_H / 2) * 5;
          int diff = abs(dist - radius);
          
          if (diff < 3) {
            uint8_t bright = (3 - diff) * 85;
            int idx = sIdx(sx, sy);
            if (idx >= 0) {
              uint32_t existing = strip.getPixelColor(idx);
              uint8_t er = (existing >> 16) & 0xFF;
              uint8_t eg = (existing >> 8) & 0xFF;
              uint8_t eb = existing & 0xFF;
              uint32_t nc = hsvToRgb(hue, 255, bright);
              uint8_t nr = (nc >> 16) & 0xFF;
              uint8_t ng = (nc >> 8) & 0xFF;
              uint8_t nb = nc & 0xFF;
              strip.setPixelColor(idx, strip.Color(
                min(255, er + nr), min(255, eg + ng), min(255, eb + nb)));
            }
          }
        }
      }
    }
    
    strip.show();
    delay(30);
  }
}

// ================================================================
//  9. VU METER - Barras de audio (10 canales × 8px alto)
// ================================================================
void vuMeter(int duration) {
  #define VU_CHANNELS 10
  #define VU_CH_W (SCR_W / VU_CHANNELS)
  uint8_t levels[VU_CHANNELS], targets[VU_CHANNELS];
  uint8_t peaks[VU_CHANNELS], peakDecay[VU_CHANNELS];
  
  for (int i = 0; i < VU_CHANNELS; i++) {
    levels[i] = 0; targets[i] = 0;
    peaks[i] = 0; peakDecay[i] = 0;
  }
  
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    if (frame % 3 == 0) {
      for (int i = 0; i < VU_CHANNELS; i++) {
        int base = 2 + (int)(3 * sin(frame * 0.1 + i * 0.8));
        targets[i] = constrain(base + (int)random(-2, 3), 1, SCR_H);
      }
    }
    
    for (int ch = 0; ch < VU_CHANNELS; ch++) {
      if (levels[ch] < targets[ch]) levels[ch]++;
      else if (levels[ch] > targets[ch]) levels[ch]--;
      
      if (levels[ch] > peaks[ch]) { peaks[ch] = levels[ch]; peakDecay[ch] = 0; }
      else { peakDecay[ch]++; if (peakDecay[ch] > 6 && peaks[ch] > 0) { peaks[ch]--; peakDecay[ch] = 4; } }
      
      int chX = ch * VU_CH_W;
      
      for (int barH = 0; barH < levels[ch]; barH++) {
        int sy = SCR_H - 1 - barH;
        float ratio = (float)barH / SCR_H;
        uint8_t r, g, b;
        if (ratio < 0.5) { r = 0; g = 255; b = 0; }
        else if (ratio < 0.75) { r = 255; g = 255; b = 0; }
        else { r = 255; g = 0; b = 0; }
        
        for (int dx = 1; dx < VU_CH_W - 1; dx++)
          sP(chX + dx, sy, r, g, b);
      }
      
      int peakSY = SCR_H - 1 - peaks[ch];
      if (peakSY >= 0 && peakSY < SCR_H) {
        for (int dx = 1; dx < VU_CH_W - 1; dx++)
          sP(chX + dx, peakSY, 255, 255, 255);
      }
    }
    
    strip.show();
    delay(35);
  }
}

// ================================================================
// 10. TETRIS RAIN - Bloques de colores cayendo
// ================================================================
void tetrisRain(int duration) {
  #define MAX_BLOCKS 20
  struct { int sx, sy; uint8_t r, g, b; bool active; } blocks[MAX_BLOCKS];
  for (int i = 0; i < MAX_BLOCKS; i++) blocks[i].active = false;
  
  uint32_t landed[SCR_W * SCR_H];
  memset(landed, 0, sizeof(landed));
  
  const uint8_t colors[][3] = {
    {255,255,0}, {0,255,255}, {0,255,0}, {255,0,0},
    {0,0,255}, {255,128,0}, {128,0,255}
  };
  
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    // Dibujar aterrizados
    for (int sy = 0; sy < SCR_H; sy++)
      for (int sx = 0; sx < SCR_W; sx++) {
        uint32_t c = landed[sy * SCR_W + sx];
        if (c) sPR(sx, sy, c);
      }
    
    // Limpiar filas llenas
    for (int sy = SCR_H - 1; sy >= 0; sy--) {
      bool full = true;
      for (int sx = 0; sx < SCR_W; sx++)
        if (!landed[sy * SCR_W + sx]) { full = false; break; }
      if (full) {
        for (int sx = 0; sx < SCR_W; sx++) sP(sx, sy, 255, 255, 255);
        strip.show(); delay(50);
        for (int yy = sy; yy > 0; yy--)
          for (int sx = 0; sx < SCR_W; sx++)
            landed[yy * SCR_W + sx] = landed[(yy - 1) * SCR_W + sx];
        for (int sx = 0; sx < SCR_W; sx++) landed[sx] = 0;
        sy++;
      }
    }
    
    // Lanzar nuevos bloques
    for (int i = 0; i < MAX_BLOCKS; i++) {
      if (!blocks[i].active && random(0, 8) == 0) {
        blocks[i].active = true;
        blocks[i].sx = random(0, SCR_W);
        blocks[i].sy = 0;
        int ci = random(0, 7);
        blocks[i].r = colors[ci][0];
        blocks[i].g = colors[ci][1];
        blocks[i].b = colors[ci][2];
      }
    }
    
    for (int i = 0; i < MAX_BLOCKS; i++) {
      if (blocks[i].active) {
        bool canFall = true;
        if (blocks[i].sy + 1 >= SCR_H) canFall = false;
        else if (landed[(blocks[i].sy + 1) * SCR_W + blocks[i].sx]) canFall = false;
        
        if (canFall) blocks[i].sy++;
        else {
          if (blocks[i].sy >= 0 && blocks[i].sy < SCR_H)
            landed[blocks[i].sy * SCR_W + blocks[i].sx] =
              escalarColor(blocks[i].r, blocks[i].g, blocks[i].b);
          blocks[i].active = false;
        }
        
        if (blocks[i].active && blocks[i].sy >= 0 && blocks[i].sy < SCR_H)
          sP(blocks[i].sx, blocks[i].sy, blocks[i].r, blocks[i].g, blocks[i].b);
      }
    }
    
    if (frame % 150 == 149) memset(landed, 0, sizeof(landed));
    
    strip.show();
    delay(50);
  }
}

// ================================================================
// 11. CYLON SCANNER - Ojo Cylon barriendo horizontalmente
// ================================================================
void cylonScanner(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    fadeAll(7);
    
    int scanSX = frame % (SCR_W * 2);
    if (scanSX >= SCR_W) scanSX = SCR_W * 2 - scanSX - 1;
    
    for (int sy = 0; sy < SCR_H; sy++) {
      int distFromCenter = abs(sy - SCR_H / 2);
      uint8_t bright = 255 - distFromCenter * 40;
      
      sP(scanSX, sy, bright, 0, 0);
      if (scanSX > 0) sP(scanSX - 1, sy, bright / 3, 0, 0);
      if (scanSX < SCR_W - 1) sP(scanSX + 1, sy, bright / 3, 0, 0);
    }
    
    strip.show();
    delay(8);
  }
}

// ================================================================
// 12. CONFETTI BURST - Explosión de confeti
// ================================================================
void confettiBurst(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    fadeAll(9);
    for (int c = 0; c < 3; c++)
      sPR(random(0, SCR_W), random(0, SCR_H), hsvToRgb(random(0, 256), 255, 255));
    strip.show();
    delay(15);
  }
}

// ================================================================
// 13. LAVA LAMP - Lámpara de lava
// ================================================================
void lavaLamp(int duration) {
  #define NUM_BLOBS 6
  float blobSX[NUM_BLOBS], blobSY[NUM_BLOBS];
  float blobDSX[NUM_BLOBS], blobDSY[NUM_BLOBS];
  uint8_t blobHue[NUM_BLOBS];
  
  for (int i = 0; i < NUM_BLOBS; i++) {
    blobSX[i] = random(0, SCR_W * 10) / 10.0;
    blobSY[i] = random(0, SCR_H * 10) / 10.0;
    blobDSX[i] = (random(-10, 10)) / 15.0;
    blobDSY[i] = (random(-10, 10)) / 30.0;
    blobHue[i] = random(0, 256);
  }
  
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    for (int i = 0; i < NUM_BLOBS; i++) {
      blobSX[i] += blobDSX[i];
      blobSY[i] += blobDSY[i];
      if (blobSX[i] <= 0 || blobSX[i] >= SCR_W - 1) blobDSX[i] = -blobDSX[i];
      if (blobSY[i] <= 0 || blobSY[i] >= SCR_H - 1) blobDSY[i] = -blobDSY[i];
      blobHue[i]++;
    }
    
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        float totalInf = 0;
        float wHue = 0;
        for (int i = 0; i < NUM_BLOBS; i++) {
          float dx = sx - blobSX[i];
          float dy = (sy - blobSY[i]) * 5;
          float dist = dx * dx + dy * dy + 1;
          float inf = 80.0 / dist;
          totalInf += inf;
          wHue += blobHue[i] * inf;
        }
        if (totalInf > 0.8) {
          uint8_t hue = (uint8_t)(wHue / totalInf);
          uint8_t bright = min(255, (int)(totalInf * 120));
          sPR(sx, sy, hsvToRgb(hue, 255, bright));
        }
      }
    }
    strip.show();
    delay(30);
  }
}

// ================================================================
// 14. LIGHTNING STORM - Tormenta eléctrica
// ================================================================
void lightningStorm(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    fadeAll(7);
    
    // Nubes
    for (int sx = 0; sx < SCR_W; sx++) {
      uint8_t cb = random(5, 25);
      sP(sx, 0, cb, cb, cb + 10);
    }
    
    if (random(0, 12) == 0) {
      // Flash
      for (int sx = 0; sx < SCR_W; sx++)
        for (int sy = 0; sy < SCR_H; sy++)
          sP(sx, sy, 30, 30, 50);
      
      int startSX = random(5, SCR_W - 5);
      int curSX = startSX;
      
      for (int sy = 0; sy < SCR_H; sy++) {
        sP(curSX, sy, 255, 255, 255);
        if (curSX > 0) sP(curSX - 1, sy, 100, 100, 200);
        if (curSX < SCR_W - 1) sP(curSX + 1, sy, 100, 100, 200);
        curSX += random(-2, 3);
        curSX = constrain(curSX, 1, SCR_W - 2);
        
        if (random(0, 4) == 0 && sy > 2) {
          int brSX = curSX;
          int brDir = random(0, 2) == 0 ? 1 : -1;
          for (int bx = 0; bx < random(5, 15); bx++) {
            brSX += brDir;
            brSX = constrain(brSX, 0, SCR_W - 1);
            sP(brSX, sy, 150, 150, 255);
          }
        }
      }
      
      strip.show();
      delay(50);
      
      if (random(0, 2) == 0) {
        delay(80);
        for (int sy = 0; sy < SCR_H; sy++)
          sP(startSX + random(-2, 3), sy, 200, 200, 255);
        strip.show();
        delay(30);
      }
    }
    
    if (random(0, 2) == 0) sP(random(0, SCR_W), random(1, SCR_H), 0, 0, 60);
    
    strip.show();
    delay(30);
  }
}

// ================================================================
// 15. RAINBOW WAVE - Onda arcoíris fluida
// ================================================================
void rainbowWave(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        uint8_t a1 = (uint8_t)(sx * 3 + frame * 2);
        uint8_t a2 = (uint8_t)(sy * 32 + frame * 3);
        int wave = fastSin(a1) + fastSin(a2);
        uint8_t hue = (uint8_t)(sx * 3 + sy * 10 + frame * 2);
        uint8_t bright = (uint8_t)(150 + wave / 3);
        sPR(sx, sy, hsvToRgb(hue, 255, bright));
      }
    }
    strip.show();
    delay(15);
  }
}

// ================================================================
// 16. FIREWORKS SHOW - Fuegos artificiales
// ================================================================
void fireworksShow(int numShots) {
  for (int shot = 0; shot < numShots; shot++) {
    uint8_t fwHue = random(0, 256);
    int targetSX = random(10, SCR_W - 10);
    int targetSY = random(1, SCR_H - 2);
    
    // Lanzamiento desde abajo
    for (int sy = SCR_H - 1; sy > targetSY; sy--) {
      fadeAll(3);
      sP(targetSX, sy, 255, 200, 100);
      if (sy + 1 < SCR_H) sP(targetSX, sy + 1, 100, 80, 0);
      strip.show();
      delay(30);
    }
    
    // Explosión
    #define FW_PARTICLES 20
    float fpx[FW_PARTICLES], fpy[FW_PARTICLES];
    float fvx[FW_PARTICLES], fvy[FW_PARTICLES];
    uint8_t fLife[FW_PARTICLES], fH[FW_PARTICLES];
    
    for (int i = 0; i < FW_PARTICLES; i++) {
      fpx[i] = targetSX;
      fpy[i] = targetSY;
      float angle = (float)i / FW_PARTICLES * 6.2832;
      float speed = 0.5 + random(0, 20) / 10.0;
      fvx[i] = cos(angle) * speed * 1.5;
      fvy[i] = sin(angle) * speed * 0.4;
      fLife[i] = 200 + random(0, 55);
      fH[i] = fwHue + random(-20, 20);
    }
    
    for (int f = 0; f < 35; f++) {
      fadeAll(6);
      for (int i = 0; i < FW_PARTICLES; i++) {
        if (fLife[i] > 0) {
          fpx[i] += fvx[i];
          fpy[i] += fvy[i];
          fvy[i] += 0.03;
          fLife[i] -= 7;
          int sx = (int)fpx[i], sy = (int)fpy[i];
          if (sx >= 0 && sx < SCR_W && sy >= 0 && sy < SCR_H)
            sPR(sx, sy, hsvToRgb(fH[i], 200, fLife[i]));
        }
      }
      strip.show();
      delay(25);
    }
    delay(100);
  }
}

// ================================================================
// 17. BANDERA MEXICANA ONDEANDO
//     3 franjas verticales: Verde | Blanco | Rojo
// ================================================================
void banderaMexicanaWave(int duration) {
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        float wave = sin((sx * 0.15) + (frame * 0.2)) * 1.5;
        int section = (int)((sx + wave) * 3.0 / SCR_W);
        section = constrain(section, 0, 2);
        uint8_t fb = 180 + (int)(40 * sin(sx * 0.3 + frame * 0.1));
        switch (section) {
          case 0: sP(sx, sy, 0, fb, 0); break;
          case 1: sP(sx, sy, fb, fb, fb); break;
          case 2: sP(sx, sy, fb, 0, 0); break;
        }
      }
    }
    strip.show();
    delay(30);
  }
}

// ================================================================
// 18. GAME OF LIFE - Juego de la vida de Conway
// ================================================================
void gameOfLife(int generations) {
  bool grid[SCR_W][SCR_H];
  bool newGrid[SCR_W][SCR_H];
  
  for (int sx = 0; sx < SCR_W; sx++)
    for (int sy = 0; sy < SCR_H; sy++)
      grid[sx][sy] = random(0, 4) == 0;
  
  uint8_t hueOff = 0;
  
  for (int gen = 0; gen < generations; gen++) {
    strip.clear();
    for (int sx = 0; sx < SCR_W; sx++)
      for (int sy = 0; sy < SCR_H; sy++)
        if (grid[sx][sy])
          sPR(sx, sy, hsvToRgb((uint8_t)(hueOff + sx * 3), 255, 200));
    strip.show();
    delay(80);
    
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        int n = 0;
        for (int dx = -1; dx <= 1; dx++)
          for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = (sx + dx + SCR_W) % SCR_W;
            int ny = (sy + dy + SCR_H) % SCR_H;
            if (grid[nx][ny]) n++;
          }
        newGrid[sx][sy] = grid[sx][sy] ? (n == 2 || n == 3) : (n == 3);
      }
    }
    memcpy(grid, newGrid, sizeof(grid));
    hueOff += 2;
    
    if (gen % 50 == 49)
      for (int i = 0; i < 40; i++)
        grid[random(0, SCR_W)][random(0, SCR_H)] = true;
  }
}

// ================================================================
// 19. SNAKE ANIMATION - Serpiente arcoíris
// ================================================================
void snakeAnimation(int duration) {
  #define SNAKE_LEN 25
  int snkSX[SNAKE_LEN], snkSY[SNAKE_LEN];
  int dSX = 1, dSY = 0;
  
  for (int i = 0; i < SNAKE_LEN; i++) {
    snkSX[i] = SCR_W / 2 - i;
    snkSY[i] = SCR_H / 2;
  }
  
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    int hSX = snkSX[0] + dSX;
    int hSY = snkSY[0] + dSY;
    
    if (hSX < 0 || hSX >= SCR_W) {
      dSX = -dSX; hSX = snkSX[0] + dSX;
      if (random(0, 2) == 0) dSY = random(-1, 2);
    }
    if (hSY < 0 || hSY >= SCR_H) {
      dSY = -dSY; hSY = snkSY[0] + dSY;
      if (random(0, 2) == 0) dSX = random(0, 2) == 0 ? 1 : -1;
    }
    
    if (random(0, 15) == 0) {
      dSX = random(-1, 2);
      dSY = random(-1, 2);
      if (dSX == 0 && dSY == 0) dSX = 1;
    }
    
    for (int i = SNAKE_LEN - 1; i > 0; i--) {
      snkSX[i] = snkSX[i - 1];
      snkSY[i] = snkSY[i - 1];
    }
    snkSX[0] = constrain(hSX, 0, SCR_W - 1);
    snkSY[0] = constrain(hSY, 0, SCR_H - 1);
    
    for (int i = 0; i < SNAKE_LEN; i++) {
      uint8_t hue = (uint8_t)(frame * 3 + i * 10);
      uint8_t bright = 255 - (i * 200 / SNAKE_LEN);
      sPR(snkSX[i], snkSY[i], hsvToRgb(hue, 255, bright));
    }
    sP(snkSX[0], snkSY[0], 255, 255, 255);
    
    strip.show();
    delay(40);
  }
}

// ================================================================
// 20. RIPPLE EFFECT - Ondas concéntricas
// ================================================================
void rippleEffect(int duration) {
  #define MAX_RIPPLES 5
  int rCX[MAX_RIPPLES], rCY[MAX_RIPPLES], rRad[MAX_RIPPLES];
  uint8_t rHue[MAX_RIPPLES];
  bool rAct[MAX_RIPPLES];
  
  for (int i = 0; i < MAX_RIPPLES; i++) rAct[i] = false;
  
  for (int frame = 0; frame < duration; frame++) {
    strip.clear();
    
    if (random(0, 12) == 0) {
      for (int i = 0; i < MAX_RIPPLES; i++) {
        if (!rAct[i]) {
          rAct[i] = true;
          rCX[i] = random(5, SCR_W - 5);
          rCY[i] = random(1, SCR_H - 1);
          rRad[i] = 0;
          rHue[i] = random(0, 256);
          break;
        }
      }
    }
    
    for (int i = 0; i < MAX_RIPPLES; i++) {
      if (rAct[i]) {
        for (int sx = 0; sx < SCR_W; sx++) {
          for (int sy = 0; sy < SCR_H; sy++) {
            float dx = sx - rCX[i];
            float dy = (sy - rCY[i]) * 5.0;
            float dist = sqrt(dx * dx + dy * dy);
            int diff = abs((int)dist - rRad[i]);
            
            if (diff < 2) {
              uint8_t bright = (2 - diff) * 120;
              bright = (bright * (30 - min(30, rRad[i]))) / 30;
              if (bright > 5) {
                int idx = sIdx(sx, sy);
                if (idx >= 0) {
                  uint32_t ex = strip.getPixelColor(idx);
                  uint32_t nc = hsvToRgb(rHue[i], 255, bright);
                  uint8_t er = (ex >> 16) & 0xFF, eg = (ex >> 8) & 0xFF, eb = ex & 0xFF;
                  uint8_t nr = (nc >> 16) & 0xFF, ng = (nc >> 8) & 0xFF, nb = nc & 0xFF;
                  strip.setPixelColor(idx, strip.Color(
                    min(255, er + nr), min(255, eg + ng), min(255, eb + nb)));
                }
              }
            }
          }
        }
        rRad[i]++;
        if (rRad[i] > 50) rAct[i] = false;
      }
    }
    
    strip.show();
    delay(25);
  }
}

// ================================================================
// 21. COUNTDOWN - Cuenta regresiva con dígitos grandes
// ================================================================
const uint8_t bigDigit[10][8] = {
  {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C}, // 0
  {0x10,0x30,0x10,0x10,0x10,0x10,0x10,0x38}, // 1
  {0x3C,0x42,0x02,0x04,0x08,0x10,0x20,0x7E}, // 2
  {0x3C,0x42,0x02,0x1C,0x02,0x02,0x42,0x3C}, // 3
  {0x04,0x0C,0x14,0x24,0x44,0x7E,0x04,0x04}, // 4
  {0x7E,0x40,0x40,0x7C,0x02,0x02,0x42,0x3C}, // 5
  {0x1C,0x20,0x40,0x7C,0x42,0x42,0x42,0x3C}, // 6
  {0x7E,0x02,0x04,0x08,0x10,0x10,0x10,0x10}, // 7
  {0x3C,0x42,0x42,0x3C,0x42,0x42,0x42,0x3C}, // 8
  {0x3C,0x42,0x42,0x42,0x3E,0x02,0x04,0x38}  // 9
};

void drawBigDigit(int digit, int offsetSX, uint8_t r, uint8_t g, uint8_t b) {
  if (digit < 0 || digit > 9) return;
  for (int row = 0; row < 8; row++) {
    uint8_t rowData = bigDigit[digit][row];
    for (int col = 0; col < 8; col++) {
      if (rowData & (1 << col)) {
        int sx = offsetSX + col;
        int sy = row;
        if (sx >= 0 && sx < SCR_W && sy >= 0 && sy < SCR_H)
          sP(sx, sy, r, g, b);
      }
    }
  }
}

void countdown(int from) {
  for (int num = from; num >= 0; num--) {
    for (int phase = 0; phase < 15; phase++) {
      strip.clear();
      
      uint8_t bright = phase * 17;
      uint8_t hue = num * 25;
      uint32_t col = hsvToRgb(hue, 255, bright);
      uint8_t r = (col >> 16) & 0xFF;
      uint8_t g = (col >> 8) & 0xFF;
      uint8_t b = col & 0xFF;
      
      for (int rep = 0; rep < SCR_W / 10; rep++)
        drawBigDigit(num, rep * 10 + 1, r, g, b);
      
      strip.show();
      delay(30);
    }
    
    if (num == 0) {
      // Pulso brillante suave en lugar de parpadeo
      for (int fade = 0; fade < 20; fade++) {
        uint8_t v = (fade < 10) ? fade * 25 : (20 - fade) * 25;
        for (int i = 0; i < NUM_LEDS; i++)
          strip.setPixelColor(i, escalarColor(v, v, v));
        strip.show(); delay(25);
      }
    }
    delay(400);
  }
}

// ================================================================
// 22. WARP SPEED - Estrellas desde el centro
// ================================================================
void warpSpeed(int duration) {
  #define MAX_WARP 30
  float wsX[MAX_WARP], wsY[MAX_WARP], wsSpd[MAX_WARP];
  bool wsAct[MAX_WARP];
  
  int cx = SCR_W / 2;
  int cy = SCR_H / 2;
  
  for (int i = 0; i < MAX_WARP; i++) wsAct[i] = false;
  
  for (int frame = 0; frame < duration; frame++) {
    fadeAll(5);
    
    for (int i = 0; i < MAX_WARP; i++) {
      if (!wsAct[i] && random(0, 3) == 0) {
        wsAct[i] = true;
        float angle = random(0, 628) / 100.0;
        wsX[i] = cx + cos(angle) * 0.5;
        wsY[i] = cy + sin(angle) * 0.3;
        wsSpd[i] = 0.3;
      }
    }
    
    for (int i = 0; i < MAX_WARP; i++) {
      if (wsAct[i]) {
        float dx = wsX[i] - cx;
        float dy = (wsY[i] - cy) * 3;
        float dist = sqrt(dx * dx + dy * dy);
        
        if (dist > 0.1) {
          wsX[i] += (dx / dist) * wsSpd[i] * 1.5;
          wsY[i] += ((wsY[i] - cy) / max(0.1f, dist / 3.0f)) * wsSpd[i] * 0.5;
          wsSpd[i] *= 1.08;
        }
        
        int sx = (int)wsX[i], sy = (int)wsY[i];
        if (sx >= 0 && sx < SCR_W && sy >= 0 && sy < SCR_H) {
          uint8_t bright = min(255, (int)(dist * 15 + 50));
          sP(sx, sy, bright, bright, min(255, bright + 30));
        } else {
          wsAct[i] = false;
        }
      }
    }
    strip.show();
    delay(15);
  }
}

// ================================================================
// 23. RAINBOW CYCLE - Arcoíris cíclico completo
// ================================================================
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return escalarColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return escalarColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return escalarColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbowCycle(int cycles) {
  for (int j = 0; j < 256 * cycles; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, Wheel((i * 256 / NUM_LEDS + j) & 255));
    }
    strip.show();
    delay(3);
  }
}

// ================================================================
// 24. BANDERA MEXICANA ESTÁTICA - 3 franjas verticales
// ================================================================
void banderaMexicana(int duration) {
  for (int cycle = 0; cycle < duration; cycle++) {
    strip.clear();
    for (int sx = 0; sx < SCR_W; sx++) {
      for (int sy = 0; sy < SCR_H; sy++) {
        int rango = (sx * 3) / SCR_W;
        // Águila en el centro
        bool isEagle = (sx >= SCR_W/2 - 1 && sx <= SCR_W/2 &&
                        sy >= SCR_H/2 - 1 && sy <= SCR_H/2);
        if (isEagle) {
          sP(sx, sy, 0, 0, 0);
        } else if (rango == 0) {
          sP(sx, sy, 0, 200, 0);     // Verde
        } else if (rango == 1) {
          sP(sx, sy, 200, 200, 200); // Blanco
        } else {
          sP(sx, sy, 200, 0, 0);     // Rojo
        }
      }
    }
    strip.show();
    delay(50);
  }
}

// ================================================================
// 25. BANDERA ONDEANDO - Con efecto de movimiento
// ================================================================
void banderaOndeando(int cycles) {
  for (int cycle = 0; cycle < cycles; cycle++) {
    for (int wave = 0; wave < 16; wave++) {
      strip.clear();
      for (int sx = 0; sx < SCR_W; sx++) {
        for (int sy = 0; sy < SCR_H; sy++) {
          // Onda que se mueve verticalmente
          int waveOffset = (sy + wave) % 4;
          int adjustedSX = sx;
          if (waveOffset == 1) adjustedSX = max(0, sx - 1);
          else if (waveOffset == 3) adjustedSX = min(SCR_W - 1, sx + 1);
          
          int section = (adjustedSX * 3) / SCR_W;
          // Águila en el centro
          bool isEagle = (sy >= SCR_H/2 - 1 && sy <= SCR_H/2 &&
                          adjustedSX >= SCR_W/2 - 2 && adjustedSX <= SCR_W/2 + 1);
          
          if (isEagle) {
            sP(sx, sy, 0, 0, 0);
          } else if (section == 0) {
            sP(sx, sy, 0, 200, 0);
          } else if (section == 1) {
            sP(sx, sy, 200, 200, 200);
          } else {
            sP(sx, sy, 200, 0, 0);
          }
        }
      }
      strip.show();
      delay(80);
    }
  }
}

// ================================================================
// 26. BANDERA ONDULANTE - Ondulación diagonal suave
// ================================================================
void banderaOndulante(int cycles) {
  for (int cycle = 0; cycle < cycles; cycle++) {
    for (int offset = 0; offset < 12; offset++) {
      strip.clear();
      for (int sx = 0; sx < SCR_W; sx++) {
        for (int sy = 0; sy < SCR_H; sy++) {
          int waveSX = sx + (sy + offset) / 3;
          int section = ((waveSX * 3) / SCR_W) % 3;
          
          bool isEagle = (sy >= SCR_H/2 - 1 && sy <= SCR_H/2 &&
                          sx >= SCR_W/2 - 2 && sx <= SCR_W/2 + 1);
          
          if (isEagle) {
            sP(sx, sy, 0, 0, 0);
          } else if (section == 0) {
            sP(sx, sy, 0, 200, 0);
          } else if (section == 1) {
            sP(sx, sy, 200, 200, 200);
          } else {
            sP(sx, sy, 200, 0, 0);
          }
        }
      }
      strip.show();
      delay(100);
    }
  }
}

// ================================================================
// 27. FUEGO INDIVIDUAL - Fuego artificial desde cualquier lado
//     Trayectoria + Explosión concéntrica + Partículas cayendo
// ================================================================
void fuegoIndividual(uint8_t fR, uint8_t fG, uint8_t fB) {
  int lado = random(0, 4);
  int launchSX, launchSY, targetSX, targetSY;
  int dsx = 0, dsy = 0;
  
  switch(lado) {
    case 0: // Desde abajo
      launchSX = random(10, SCR_W - 10);
      launchSY = SCR_H - 1;
      targetSX = launchSX + random(-3, 4);
      targetSY = random(1, 3);
      dsx = (targetSX > launchSX) ? 1 : ((targetSX < launchSX) ? -1 : 0);
      dsy = -1;
      break;
    case 1: // Desde arriba
      launchSX = random(10, SCR_W - 10);
      launchSY = 0;
      targetSX = launchSX + random(-3, 4);
      targetSY = random(SCR_H - 3, SCR_H - 1);
      dsx = (targetSX > launchSX) ? 1 : ((targetSX < launchSX) ? -1 : 0);
      dsy = 1;
      break;
    case 2: // Desde izquierda
      launchSX = 0;
      launchSY = random(1, SCR_H - 1);
      targetSX = random(SCR_W / 4, SCR_W / 2);
      targetSY = launchSY;
      dsx = 1;
      dsy = 0;
      break;
    case 3: // Desde derecha
      launchSX = SCR_W - 1;
      launchSY = random(1, SCR_H - 1);
      targetSX = random(SCR_W / 2, SCR_W * 3 / 4);
      targetSY = launchSY;
      dsx = -1;
      dsy = 0;
      break;
  }
  
  targetSX = constrain(targetSX, 3, SCR_W - 4);
  targetSY = constrain(targetSY, 1, SCR_H - 2);
  
  int cSX = launchSX, cSY = launchSY;
  
  // FASE 1: Trayectoria con estela
  while (cSX != targetSX || cSY != targetSY) {
    fadeAll(3);
    for (int t = 0; t < 3; t++) {
      int tSX = cSX - (dsx * t);
      int tSY = cSY - (dsy * t);
      if (tSX >= 0 && tSX < SCR_W && tSY >= 0 && tSY < SCR_H) {
        if (t == 0) sP(tSX, tSY, fR, fG, fB);
        else {
          int inten = 255 - t * 80;
          sP(tSX, tSY, inten / 4, inten / 4, 0);
        }
      }
    }
    strip.show();
    delay(40);
    if (cSX != targetSX) cSX += dsx;
    if (cSY != targetSY) cSY += dsy;
  }
  
  // FASE 2: Explosión en círculos concéntricos (aspecto ratio corregido)
  for (int radius = 0; radius <= 5; radius++) {
    fadeAll(4);
    for (int dx = -radius * 2; dx <= radius * 2; dx++) {
      for (int dy = -radius; dy <= radius; dy++) {
        if (abs(dx / 2) + abs(dy) <= radius) {
          int eSX = targetSX + dx;
          int eSY = targetSY + dy;
          if (eSX >= 0 && eSX < SCR_W && eSY >= 0 && eSY < SCR_H) {
            if (radius == 0) sP(eSX, eSY, 255, 255, 255);
            else if (radius == 1) sP(eSX, eSY, fR, fG, fB);
            else if (radius == 2) sP(eSX, eSY, 255, 100, 0);
            else if (radius == 3) sP(eSX, eSY, 255, 0, 0);
            else sP(eSX, eSY, 100, 0, 0);
          }
        }
      }
    }
    strip.show();
    delay(100);
  }
  
  // FASE 3: Partículas cayendo
  for (int fall = 0; fall < 6; fall++) {
    fadeAll(4);
    for (int p = 0; p < 6; p++) {
      int pSX = targetSX + random(-5, 6);
      int pSY = targetSY + random(-1, fall + 1);
      if (pSX >= 0 && pSX < SCR_W && pSY >= 0 && pSY < SCR_H) {
        int inten = 255 - fall * 40;
        if (inten > 0) sP(pSX, pSY, inten / 3, inten / 6, 0);
      }
    }
    strip.show();
    delay(60);
  }
}

// ================================================================
// 28. FUEGOS SIMULTÁNEOS - Múltiples fuegos secuenciales rápidos
// ================================================================
void fuegosSimultaneos(int numFuegos) {
  const uint8_t fcolors[][3] = {
    {255, 0, 0}, {0, 255, 0}, {255, 200, 0},
    {255, 255, 255}, {255, 100, 0}, {0, 100, 255}, {200, 0, 255}
  };
  
  for (int i = 0; i < numFuegos; i++) {
    int ci = random(0, 7);
    fuegoIndividual(fcolors[ci][0], fcolors[ci][1], fcolors[ci][2]);
    delay(30);
  }
}

// ================================================================
// 29. ESPECTÁCULO FUEGOS - Combinación de fuegos artificiales
// ================================================================
void espectaculoFuegos() {
  fuegosSimultaneos(3);
  delay(100);
  fuegosSimultaneos(2);
  delay(100);
  fuegosSimultaneos(4);
}

// ================================================================
// TRANSICIONES
// ================================================================
void transitionWipe(uint8_t r, uint8_t g, uint8_t b, int delayTime) {
  // Barrido de color
  for (int sx = 0; sx < SCR_W; sx++) {
    for (int sy = 0; sy < SCR_H; sy++) sP(sx, sy, r, g, b);
    strip.show();
    delay(delayTime);
  }
  delay(200);
  // Fade suave al negro en vez de barrido abrupto
  for (int f = 0; f < 20; f++) {
    fadeAll(8);
    strip.show();
    delay(20);
  }
}

void transitionSparkle(int duration) {
  for (int i = 0; i < duration; i++) {
    fadeAll(7);
    for (int s = 0; s < 2; s++)
      sP(random(0, SCR_W), random(0, SCR_H), 255, 255, 255);
    strip.show();
    delay(10);
  }
  // Fade suave al negro
  for (int f = 0; f < 15; f++) {
    fadeAll(6);
    strip.show();
    delay(15);
  }
}

// ================================================================
// SETUP
// ================================================================
void setup() {
  strip.begin();
  strip.setBrightness(255);
  strip.clear();
  strip.show();
  
  randomSeed(analogRead(A0));
  
  Serial.begin(115200);
  Serial.println(F("╔══════════════════════════════════════╗"));
  Serial.println(F("║  UNIT ELECTRONICS - TALENT LAND 2026 ║"));
  Serial.println(F("║  WS2812B 8x80 RGB LED Matrix Demo   ║"));
  Serial.println(F("║  640 LEDs - Demostraciones Visuales  ║"));
  Serial.println(F("╚══════════════════════════════════════╝"));
}

// ================================================================
// LOOP PRINCIPAL - SECUENCIA DE DEMOS PARA TALENT LAND
// ================================================================
void loop() {
  
  // ──── INTRO: Cuenta regresiva ────
  Serial.println(F("[DEMO] Countdown"));
  countdown(5);
  
  // ──── MENSAJE: TALENT LAND ────
  Serial.println(F("[MSG] TALENT LAND"));
  rainbowScrollText(reverseString("  TALENT LAND 2026  "), 18);
  
  // ──── DEMO 1: Matrix Rain ────
  Serial.println(F("[DEMO] Matrix Rain"));
  transitionWipe(0, 80, 0, 3);
  matrixRain(200);
  
  // ──── MENSAJE: UNIT ELECTRONICS ────
  Serial.println(F("[MSG] UNIT ELECTRONICS"));
  scrollText(reverseString("   UNIT ELECTRONICS"), escalarColor(255, 60, 0), 20);

  // ──── DEMO 2: Plasma ────
  Serial.println(F("[DEMO] Plasma Effect"));
  plasmaEffect(200);
  
  // ──── MENSAJE: ECOSISTEMA DEVLAB ────
  scrollText(reverseString(" ECOSISTEMA DEVLAB "), escalarColor(0, 255, 100), 20);
  
  // ──── DEMO 3: Fuego ────
  Serial.println(F("[DEMO] Fire Effect"));
  fireEffect(250);
  
  // ──── TRANSICIÓN ────
  transitionSparkle(100);
  
  // ──── DEMO 4: Estrellas Fugaces ────
  Serial.println(F("[DEMO] Shooting Stars"));
  shootingStars(200);
  
  // ──── MENSAJE ────
  rainbowScrollText(reverseString("  HECHO EN MEXICO!  "), 18);
  
  // ──── DEMO 5: Espiral Hipnótica ────
  Serial.println(F("[DEMO] Hypnotic Spiral"));
  hypnoticSpiral(200);
  
  // ──── DEMO 6: Corazón Latiendo ────
  Serial.println(F("[DEMO] Beating Heart"));
  beatingHeart(4);
  
  // ──── MENSAJE ────
  scrollText(reverseString("  OPEN HARDWARE  "), escalarColor(255, 0, 150), 20);
  
  // ──── DEMO 7: ADN Helix ────
  Serial.println(F("[DEMO] DNA Helix"));
  dnaHelix(200);
  
  // ──── DEMO 8: Pulse Wave ────
  Serial.println(F("[DEMO] Pulse Wave"));
  pulseWave(150);
  
  // ──── MENSAJE ────
  scrollText(reverseString("  WS2812B  8X80  "), escalarColor(0, 200, 255), 20);
  
  // ──── DEMO 9: VU Meter ────
  Serial.println(F("[DEMO] VU Meter"));
  vuMeter(200);
  
  // ──── TRANSICIÓN ────
  transitionWipe(0, 0, 100, 3);
  
  // ──── DEMO 10: Tetris ────
  Serial.println(F("[DEMO] Tetris Rain"));
  tetrisRain(200);
  
  // ──── MENSAJE ────
  rainbowScrollText(reverseString("  640 RGB LEDS  "), 18);
  
  // ──── DEMO 11: Cylon Scanner ────
  Serial.println(F("[DEMO] Cylon Scanner"));
  cylonScanner(300);
  
  // ──── DEMO 12: Confetti ────
  Serial.println(F("[DEMO] Confetti Burst"));
  confettiBurst(200);
  
  // ──── MENSAJE ────
  scrollText(reverseString("  VISITANOS!  "), escalarColor(255, 255, 0), 20);
  
  // ──── DEMO 13: Lava Lamp ────
  Serial.println(F("[DEMO] Lava Lamp"));
  lavaLamp(150);
  
  // ──── DEMO 14: Lightning ────
  Serial.println(F("[DEMO] Lightning Storm"));
  lightningStorm(200);
  
  // ──── TRANSICIÓN ────
  transitionSparkle(80);
  
  // ──── DEMO 15: Rainbow Wave ────
  Serial.println(F("[DEMO] Rainbow Wave"));
  rainbowWave(200);
  
  // ──── DEMO 16: Warp Speed ────
  Serial.println(F("[DEMO] Warp Speed"));
  warpSpeed(200);
  
  // ──── DEMO 17: Game of Life ────
  Serial.println(F("[DEMO] Game of Life"));
  gameOfLife(100);
  
  // ──── DEMO 18: Snake ────
  Serial.println(F("[DEMO] Snake Animation"));
  snakeAnimation(200);
  
  // ──── DEMO 19: Ripple ────
  Serial.println(F("[DEMO] Ripple Effect"));
  rippleEffect(200);
  
  // ──── BANDERA MEXICANA ESTÁTICA ────
  Serial.println(F("[DEMO] Bandera Mexicana"));
  banderaMexicana(60);
  
  // ──── MENSAJE ────
  scrollText(reverseString("  VIVA MEXICO!  "), escalarColor(0, 255, 0), 20);
  
  // ──── BANDERA ONDEANDO ────
  Serial.println(F("[DEMO] Bandera Ondeando"));
  banderaOndeando(3);
  
  // ──── BANDERA ONDULANTE ────
  Serial.println(F("[DEMO] Bandera Ondulante"));
  banderaOndulante(3);
  
  // ──── BANDERA WAVE ────
  Serial.println(F("[DEMO] Bandera Mexicana Wave"));
  banderaMexicanaWave(150);
  
  // ──── TRANSICIÓN ────
  transitionSparkle(80);
  
  // ──── FUEGO INDIVIDUAL ────
  Serial.println(F("[DEMO] Fuego Individual"));
  fuegoIndividual(255, 200, 0);
  delay(100);
  fuegoIndividual(0, 255, 0);
  delay(100);
  fuegoIndividual(255, 0, 0);
  
  // ──── ESPECTÁCULO DE FUEGOS ARTIFICIALES ────
  Serial.println(F("[DEMO] Espectaculo Fuegos"));
  espectaculoFuegos();
  
  // ──── GRAN FINAL: FIREWORKS SHOW ────
  Serial.println(F("[DEMO] Fireworks Show - GRAN FINAL"));
  rainbowScrollText(reverseString("  UNIT ELECTRONICS  "), 15);
  fireworksShow(8);
  
  // ──── MENSAJE FINAL ────
  scrollText(reverseString("  TALENT LAND 2026  "), escalarColor(255, 200, 0), 18);
  rainbowScrollText(reverseString("  GRACIAS!  "), 20);
  
  delay(1000);
  Serial.println(F("=== REINICIANDO DEMO ==="));
}
