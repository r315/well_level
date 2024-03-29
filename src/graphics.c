#include <graphics.h>
#include <board.h>

const uint8_t tank[] = { // 45x64
0x00,0x03,0xff,0xfe,0x00,0x00,0x00,0x7f,0x80,0x0f,0xf0,0x00,0x0f,0xf8,0x00,0x00,0xff,0x80,0x3f,0x00,0x00,0x00,0x07,0xe0,
0x78,0x00,0x00,0x00,0x00,0xf0,0xe0,0x00,0x00,0x00,0x00,0x38,0xe0,0x00,0x00,0x00,0x00,0x38,0xf8,0x00,0x00,0x00,0x00,0xf8,
0xff,0x00,0x00,0x00,0x07,0xf8,0xcf,0xf8,0x00,0x00,0xff,0x98,0xc0,0x7f,0x80,0x0f,0xf0,0x18,0xc0,0x03,0xff,0xfe,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,
0xc0,0x00,0x00,0x00,0x00,0x18,0xc0,0x00,0x00,0x00,0x00,0x18,0xff,0xff,0xff,0xff,0xff,0xf8,0x7f,0xff,0xff,0xff,0xff,0xf0,
0x3f,0xff,0xff,0xff,0xff,0xe0,0x0f,0xff,0xff,0xff,0xff,0x80,0x00,0x7f,0xff,0xff,0xf0,0x00,0x00,0x03,0xff,0xfe,0x00,0x00,
};


const uint8_t alert[] = { //11x11 two icons for icon ON and icon OFF
    0x1f,0x00,0x20,0x80,0x44,0x40,0x84,0x20,0x84,0x20,0x84,0x20,0x84,0x20,0x80,0x20,0x44,0x40,0x20,0x80,0x1f,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

const uint8_t _f[] = { // 11x11 test 'F'
    0xff,0xe0,0x80,0x20,0x9f,0x20,0x90,0x20,0x90,0x20,0x9c,0x20,0x90,0x20,0x90,0x20,0x90,0x20,0x80,0x20,0xff,0xe0
};

static uint8_t s_fb[LCD_W * LCD_H/8];

void initGraphics(void){
    // Clear frame buffer
    for (uint32_t i = 0; i < sizeof(s_fb); i++){
        s_fb[i] = 0;
    }
    
    LCD_Clear();

    drawVline(48, 0, 64);    
    drawHline(50, 18, 128 - 50);
    drawHline(50, 44, 128 - 50);
    drawBitmap(TANK_POS,(uint8_t*)tank, TANK_W, TANK_H);
    
    TEXT_Print(LINE1_TEXT_POS, "Disponivel:");
    TEXT_Print(LINE4_TEXT_POS, "Altura:");
    TEXT_Print(LINE5_TEXT_POS, "Total:");
    #if DEBUG
    TEXT_Print(LINE7_TEXT_POS, "Sensor:");
    #endif
}

/**
 * @param x : pixel x coordinate
 * @param y : pixel y coordinate
 * @param buf : destination buffer
 * @param bw : buffer width in pixels
 * @param color : 0 off, 1 on
 * */
static void writePixel(uint16_t x, uint16_t y, uint8_t *buf, uint8_t color){
        uint8_t tmp = buf[x + (y >> 3) * LCD_W] & ~(1 << (y & 7));
        buf[x + (y >> 3) * LCD_W] = tmp | (color << (y & 7));
}

static void fillSolid( uint8_t x, uint8_t y, uint8_t *buf, uint8_t w, uint8_t h, uint8_t color){
    for(uint8_t i = y; i < y + h; i++){
        for(uint8_t j = x; j < x + w; j++){
            writePixel(j, i, s_fb, color);
        }
    }
}

/**
 * @brief Copy tile data (1bpp) to lcd frame buffer
 * 
 * @param buf   : lcd frame buffer
 * @param *tile : tile data
 * @param w     : tile width
 * @param h     : tile hight
 * 
 * */
static void copy2FrameBuf(uint8_t x, uint8_t y, uint8_t *dst, uint8_t *src, uint8_t w, uint8_t h){
uint8_t data, color;

    for(int16_t j = y; j < y + h; j++){
        for(uint8_t i = x, m = 0; i <  x + w; i++, m++){
            if( (m&7) == 0){
                data = *src++;
            }

            color = !!(data & (0x80 >> (m&7)));
            
            //writePixel(-(i-x)+(x+w), -(j-y) + (y + h), dst, color);
            writePixel(i, j, dst, color);
        }
    }
}

void drawBitmap(uint16_t x, uint16_t y, uint8_t *bitmap, uint8_t w, uint8_t h){
    copy2FrameBuf(x, y, s_fb, bitmap, w, h);
    LCD_PartialFrameBuffer(x, y, s_fb, w, h);
}

void drawHline(uint8_t x, uint8_t y, uint8_t size){
    for(uint8_t i = x; i < x + size; i++){
        writePixel(i, y, s_fb, 1);
    }
    LCD_PartialFrameBuffer(x, y, s_fb, size, 1);
}

void drawVline(uint8_t x, uint8_t y, uint8_t size){
    for(uint8_t i = y; i < y + size; i++){
        writePixel(x, i, s_fb, 1);
    }
    LCD_PartialFrameBuffer(x, y, s_fb, 1, size);
}

static uint8_t drawIntText(uint8_t x, uint8_t y, uint16_t val, char *txt, int8_t dig){
    x = TEXT_PrintInt(x,y, val, dig);
    return TEXT_Print(x, y, txt);
}

void drawLevel(uint8_t level){
uint8_t dec;
    dec = level / 10;
    fillSolid(2, TANK_TOP, s_fb, 41, 40 - dec * 4, NO_COLOR);
    fillSolid(2, TANK_TOP + 40 - dec * 4, s_fb, 41, dec * 4, FILLED);
    LCD_PartialFrameBuffer(2,TANK_TOP, s_fb, 41, 40);
}

void printAlert(uint8_t visible){
    if(visible){
        drawBitmap(ALERT_POS, (uint8_t*)alert, 11, 11);
    }else{
        drawBitmap(ALERT_POS, (uint8_t*)alert + (2 * 11), 11, 11);
    }
}

void printDistance(int32_t distance){
    uint16_t x;
    int32_t aux = distance / 1000;

    x = drawIntText(LEVEL_POS, aux % 10, ",", 1);
    aux = distance / 100;
    if(aux < 0) aux = -aux;
    drawIntText(x, LEVEL_LINE, aux % 10, "m", 0);
}

void printPercent(uint8_t level){
    drawIntText(PERCENT_POS, level, "%  ", 3);
}

void printCapacity(uint32_t cap){
    drawIntText(CAPACITY_POS, cap, "L  ", 5);
}

void printAvalilable(uint32_t max, uint8_t level){
    uint16_t x = drawIntText(AVAILABLE_POS, level, "% (", 3);
    drawIntText(x ,1 , (max * level) / 100, "L)    ", 0);
}

void printSensor(uint16_t data){
    uint16_t x = TEXT_PrintInt(LINE8_TEXT_POS, data, 0);
    TEXT_Print(x, 7, "mm  ");
}


