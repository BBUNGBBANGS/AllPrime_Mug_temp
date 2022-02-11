#include "oled.h"
uint8 OLED_Address;
OLED_t OLED;
char OLED_WRITE[] = "HELO";

uint8 Tx_data[100]; 
/* OLED data buffer */
static uint8 OLED_Buffer[OLED_WIDTH * OLED_HEIGHT / 8];
static void OLED_Write_Command(uint8 command);
static void OLED_Fill_Screen(OLED_COLOR_t color);
static void OLED_Update_Screen(void);
static void OLED_DrawPixel(uint16 x, uint16 y, OLED_COLOR_t color);
static char OLED_Set_Char(char ch, OLED_Font_t* Font, OLED_COLOR_t color);
static char OLED_Set_Str(char* str, OLED_Font_t* Font, OLED_COLOR_t color); 
static void OLED_Set_X_Y(uint16 x, uint16 y);

void OLED_FindAddress(void)
{
    if(HAL_OK == HAL_I2C_IsDeviceReady(&OLED_HW_MODULE,OLED_Address,3,10))
    {
        /*do noThing*/
    }
    else
    {
        OLED_Address++;
    }
}

void OLED_Init(void)
{
    uint8 length;
 /* Init LCD */
    OLED_Write_Command(0xAE); // Set display off
    OLED_Write_Command(0xA8); // Set multiplex ratio
    OLED_Write_Command(0x1F); // -- from default 63 to 31 (i.e. 32MUX)
    OLED_Write_Command(0xD3); // Set display offset
    OLED_Write_Command(0x00); // -- no offset
    OLED_Write_Command(0x40); // Set display start line
    OLED_Write_Command(0xA1); // Set segment re-map, column address 127 is mapped to SEG0
    OLED_Write_Command(0xC8); // Set COM output scan direction - remapped mode
    OLED_Write_Command(0x81); // Set contrast control for BANK0
    OLED_Write_Command(0x7F); // -- range 0x00 to 0xFF => 50%
    OLED_Write_Command(0xA4); // Enable display outputs according to the GDDRAM contents.
    OLED_Write_Command(0xA6); // Set normal display
    OLED_Write_Command(0xD5); // Set display clock divide ration and oscillator frequency
    OLED_Write_Command(0x80); // -- frequency (1000 - default); display clock divide ratio (0000 - divide ration 1)
    OLED_Write_Command(0x8D); // Charge pump setting
    OLED_Write_Command(0x14); // -- enable charge pump

    OLED_Write_Command(0x2E); // Deactivate scroll
    OLED_Write_Command(0x20); // Set memory addressing mode
    OLED_Write_Command(0x10); // -- Page Addressing Mode (RESET)
    OLED_Write_Command(0xDA); // Set COM pins hardware configuration
    OLED_Write_Command(0x02); // --
    OLED_Write_Command(0xD9); // Set pre-charge period
    OLED_Write_Command(0x22); // --
    OLED_Write_Command(0xDB); // Set Vcomh deselect level
    OLED_Write_Command(0x20); // -- 0.77 x Vcc (RESET)

    OLED_Write_Command(0xB0); // Set page start address for page addressing mode
    OLED_Write_Command(0x00); // Set lower column start address for page addressing mode
    OLED_Write_Command(0x10); // Set higher column start address for page addressing mode

    OLED_Write_Command(0xAF); // Set display on    

    OLED_Fill_Screen(OLED_COLOR_BLACK);
    OLED_Update_Screen();
    uint8 dummy;
    #if 0
    sprintf(Tx_data,"https://www.allprime.co.kr");
    length = strlen(Tx_data);
    
    for(uint8 idx=0;idx<length;idx++)
    {
        if(7*idx>112)
        {
            OLED_Set_X_Y(7*idx-112,10);
        }
        else
        {
            OLED_Set_X_Y(7*idx,0);            
        }
        dummy = OLED_Set_Char(Tx_data[idx]-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    }
    #endif
    #if 0
    OLED_Set_X_Y(0,0);    
    dummy = OLED_Set_Char('h'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);

    OLED_Set_X_Y(7,0);
    dummy = OLED_Set_Char('t'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(14,0);
    dummy = OLED_Set_Char('t'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(21,0);
    dummy = OLED_Set_Char('p'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(28,0);
    dummy = OLED_Set_Char('s'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(35,0);
    dummy = OLED_Set_Char(':'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(42,0);
    dummy = OLED_Set_Char('/'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(49,0);
    dummy = OLED_Set_Char('/'-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    //OLED_Set_X_Y(56,0);
    dummy = OLED_Set_Char('a'-33,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(99,0);
    dummy = OLED_Set_Char('l'-33,&OLED_Font_7x10,OLED_COLOR_WHITE);
    OLED_Set_X_Y(110,0);
    dummy = OLED_Set_Char('l'-33,&OLED_Font_7x10,OLED_COLOR_WHITE);

    OLED_Set_X_Y(96,0);
    dummy = OLED_Set_Char(8,&OLED_Font_12x16,OLED_COLOR_WHITE);
    OLED_Set_X_Y(108,0);
    dummy = OLED_Set_Char(9,&OLED_Font_12x16,OLED_COLOR_WHITE);
    OLED_Set_X_Y(0,15);
    dummy = OLED_Set_Char(10,&OLED_Font_12x16,OLED_COLOR_WHITE);
    OLED_Set_X_Y(12,15);
    dummy = OLED_Set_Char(11,&OLED_Font_12x16,OLED_COLOR_WHITE);
    OLED_Set_X_Y(24,15);
    dummy = OLED_Set_Char(12,&OLED_Font_12x16,OLED_COLOR_WHITE);
    OLED_Set_X_Y(36,15);
    dummy = OLED_Set_Char(13,&OLED_Font_12x16,OLED_COLOR_WHITE);
    #endif
    OLED_Update_Screen();

}
extern uint16 Temp_Adc_TP_Volt;
void OLED_Display(void)
{
    uint8 dummy;
    uint16 n1,n10,n100,n1000;

    n1 = Temp_Adc_TP_Volt%10;
    n10 = (Temp_Adc_TP_Volt%100)/10;
    n100 = (Temp_Adc_TP_Volt%1000)/100;
    n1000 = (Temp_Adc_TP_Volt%10000)/1000;
    
    OLED_Set_X_Y(7*0,0);   
    Tx_data[0] = n1;
    dummy = OLED_Set_Char(Tx_data[0]-32,&OLED_Font_7x10,OLED_COLOR_WHITE);    
    OLED_Set_X_Y(7*1,0);   
    Tx_data[1] = n10;
    dummy = OLED_Set_Char(Tx_data[1]-32,&OLED_Font_7x10,OLED_COLOR_WHITE);    
    OLED_Set_X_Y(7*2,0);   
    Tx_data[2] = n100;
    dummy = OLED_Set_Char(Tx_data[2]-32,&OLED_Font_7x10,OLED_COLOR_WHITE);    
    OLED_Set_X_Y(7*3,0);   
    Tx_data[3] = n1000;
    dummy = OLED_Set_Char(Tx_data[3]-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
}

static void OLED_Write_Command(uint8 command)
{
    uint8 data[2] = {0x00,command};
    HAL_I2C_Master_Transmit(&OLED_HW_MODULE,OLED_ADDRESS<<1,data,sizeof(data),1000);
}

static void OLED_Fill_Screen(OLED_COLOR_t color)
{
    /* Set memory */
    memset(OLED_Buffer, (color == OLED_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(OLED_Buffer));    
}

static void OLED_Update_Screen(void)
{
    for (uint8 idx = 0; idx < 8; idx++) 
    {
        OLED_Write_Command(0xB0 + idx);
        OLED_Write_Command(0x00);
        OLED_Write_Command(0x10);

        HAL_I2C_Mem_Write(&OLED_HW_MODULE,OLED_ADDRESS<<1,0x40,I2C_MEMADD_SIZE_8BIT,&OLED_Buffer[OLED_WIDTH * idx],OLED_WIDTH,1000);
    }    
}

static void OLED_Set_X_Y(uint16 x, uint16 y) 
{
    /* Set write pointers */
    OLED.X_point = x;
    OLED.Y_point = y;
}

static char OLED_Set_Char(char ch, OLED_Font_t* Font, OLED_COLOR_t color) 
{
    /* Check available space in LCD */
    if (OLED_WIDTH <= (OLED.X_point + Font->FontWidth) || OLED_HEIGHT <= (OLED.Y_point + Font->FontHeight)) 
    {/* Error */ return 0;}

    /* Go through font */
    for (uint32 i = 0; i < Font->FontHeight; i++) 
    {
        uint32 b = Font->data[ch * (Font->FontHeight) + i];

        for (uint32 j = 0; j < Font->FontWidth; j++) 
        {
            if ((b << j) & 0x8000) 
            {
                OLED_DrawPixel(OLED.X_point + j, (OLED.Y_point + i), (OLED_COLOR_t) color);
            } 
            else 
            {
                OLED_DrawPixel(OLED.X_point + j, (OLED.Y_point + i), (OLED_COLOR_t)!color);
            }
        }
    }

    /* Increase pointer */
    OLED.X_point += Font->FontWidth;

    /* Return character written */
    return ch;
}

static char OLED_Set_Str(char* str, OLED_Font_t* Font, OLED_COLOR_t color) 
{
    /* Write characters */
    while (*str) 
    {
        /* Write character by character */
        if (OLED_Set_Char(*str, Font, color) != *str) 
        {
            /* Return error */
            return *str;
        }

        /* Increase string pointer */
        str++;
    }

    /* Everything OK, zero should be returned */
    return *str;
}

static void OLED_DrawPixel(uint16 x, uint16 y, OLED_COLOR_t color) 
{
    if ((x >= OLED_WIDTH) || (y >= OLED_HEIGHT)) 
    {
        /* Error */
        return;
    }

    /* Set color */
    if (color == OLED_COLOR_WHITE) 
    {
        OLED_Buffer[x + (y / 8) * OLED_WIDTH] |= 1 << (y % 8);
    } 
    else 
    {
        OLED_Buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y % 8));
    }
}