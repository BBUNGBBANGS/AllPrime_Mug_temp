#include "oled.h"
uint8 OLED_Address;
OLED_t OLED;

uint8 Tx_data[100]; 
/* OLED data buffer */
static uint8 OLED_Buffer[OLED_WIDTH * OLED_HEIGHT / 8];
static void Clear_TxBuffer(void);
static void Print_TxBuffer(void);
static void OLED_Write_Command(uint8 command);
static void OLED_Fill_Screen(OLED_COLOR_t color);
static void OLED_Update_Screen(void);
static void OLED_DrawPixel(uint16 x, uint16 y, OLED_COLOR_t color);
static char OLED_Set_Char(char ch, OLED_Font_t* Font, OLED_COLOR_t color);
static char OLED_Set_Str(char* str, OLED_Font_t* Font, OLED_COLOR_t color); 
static void OLED_Set_X_Y(uint16 x, uint16 y);
static void OLED_Print_Run(void);
static void OLED_Print_Select(void);
static void OLED_Print_Idle(void);
static void OLED_Print_Custom(void);

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

    Clear_TxBuffer();
    sprintf(Tx_data,"https://www.allprime.co.kr");
    length = strlen(Tx_data);
    Tx_data[length] = ' ';
    Print_TxBuffer();
    
    OLED_Update_Screen();
}
extern uint16 Temp_Adc_TP_Volt;
extern sint16 Temp_Adc_TP;
extern uint16 Target_Run_Time;
extern uint16 CO2_Data16;
void OLED_Display(void)
{
    uint8 dummy;
    uint8 n1,n10,n100,n1000;
    uint8 t1,t10,t100,t1000;
    uint16 n0=0;
    uint16 length;
    sint16 Temp;
    if(switch_mode == SWITCH_MODE_RUN)
    {
        OLED_Print_Run();
    }
    else if(switch_mode == SWITCH_MODE_MENUSELECT)
    {
        OLED_Print_Select();
    }
    else if(switch_mode == SWITCH_MODE_CUSTOM)
    {
        OLED_Print_Custom();
    }
    else
    {
        OLED_Print_Idle();
    }
}

static void Clear_TxBuffer(void)
{
    for(int i=0;i<100;i++)
    {
        Tx_data[i] = ' ';
    }
}

static void Print_TxBuffer(void)
{
    uint8 dummy;
    for(uint8 idx=0;idx<69;idx++)
    {
        if(7*(idx+1)<128)
        {
            OLED_Set_X_Y(7*idx,0); 
        }
        else if(7*(idx+1)<256)
        {
            OLED_Set_X_Y(7*(idx-18),10);  
        }
        else
        {
            OLED_Set_X_Y(7*(idx-36),20);                 
        }
        dummy = OLED_Set_Char(Tx_data[idx]-32,&OLED_Font_7x10,OLED_COLOR_WHITE);
    }
}

static void OLED_Print_Run(void)
{
    uint8 n1,n10,n100,n1000;
    uint8 t1,t10,t100,t1000;
    uint16 n0=0;
    uint16 length;
    sint16 Temp;
    Clear_TxBuffer();
    sprintf(Tx_data,"Run Time : ");
    length = strlen(Tx_data);
    t1 = ((Target_Run_Time/100)%60)%10;
    t10 = (((Target_Run_Time/100)%60)/10)%10;
    t100 = (Target_Run_Time/100/60)%10;
    t1000 = ((Target_Run_Time/100/60)/10)%10;
    Tx_data[length] = t1000 + 16 + 32;
    Tx_data[length+1] = t100 + 16 + 32;
    Tx_data[length+2] = ':';        
    Tx_data[length+3] = t10 + 16 + 32;
    Tx_data[length+4] = t1 + 16 + 32;
    length = length + 7;
    sprintf(&Tx_data[length],"Temp     :");
    length = length+8; 
    Temp = Temp_Adc_TP;
    if(Temp<0)
    {
        Temp = -Temp;
        n0 = '-';
    }
    else
    {
        n0 = ' ';
    }
    n1 = (Temp%10);
    n10 = ((Temp%100)/10);
    n100 = ((Temp%1000)/100);
    Tx_data[length+2] = n0;
    if(n100 == 0)
    {
        Tx_data[length+3] = ' ';
        Tx_data[length+4] = n10 + 16 + 32;
        Tx_data[length+5] = n1 + 16 + 32;            
    }
    else
    {
        Tx_data[length+3] = n100 + 16 + 32;
        Tx_data[length+4] = n10 + 16 + 32;
        Tx_data[length+5] = n1 + 16 + 32;
    }
    sprintf(&Tx_data[length+6],"*C");
    length = strlen(Tx_data);
    Tx_data[length] = ' ';
    length = length+2;
    sprintf(&Tx_data[length],"Menu:");
    length = length + 5;
    Tx_data[length] = ' ';

    n1 = switch_submode%10;
    n10 = (switch_submode%100)/10;
    if(n10>0)
    {
        Tx_data[length] = n10 + 16 + 32;
        Tx_data[length+1] = n1 + 16 + 32;
    }
    else
    {
        length = length + 1;
        Tx_data[length] = n1 + 16 + 32;
    }
    length = length + 2;
    Tx_data[length] = ' ';
    length = length + 1;
    sprintf(&Tx_data[length],"CO2:");
    length = strlen(Tx_data);
    Tx_data[length] = ' ';
    n1 = (CO2_Data16%10);
    n10 = ((CO2_Data16%100)/10);
    n100 = ((CO2_Data16%1000)/100);
    n1000 = ((CO2_Data16%10000)/1000);
    if(n1000>0)
    {
        Tx_data[length+0] = n1000 + 16 + 32;
        Tx_data[length+1] = n100 + 16 + 32;
        Tx_data[length+2] = n10 + 16 + 32;
        Tx_data[length+3] = n1 + 16 + 32;
    }
    else
    {
        Tx_data[length+1] = n100 + 16 + 32;
        Tx_data[length+2] = n10 + 16 + 32;
        Tx_data[length+3] = n1 + 16 + 32;
    }

    Print_TxBuffer();
    OLED_Update_Screen();
}

static void OLED_Print_Select(void)
{    
    uint8 n1,n10;
    uint16 length;
    Clear_TxBuffer();
    sprintf(Tx_data,"  < Select Menu >");
    length = strlen(Tx_data);
    Tx_data[length] = ' ';
    length = length + 9;
    n1 = switch_submode % 10;
    n10 = (switch_submode % 100) / 10;
    Tx_data[length] = '[';
    Tx_data[length+1] = n10 + 16 + 32;
    Tx_data[length+2] = n1 + 16 + 32;
    Tx_data[length+3] = ']';
    Print_TxBuffer();
    OLED_Update_Screen();
}

static void OLED_Print_Custom(void)
{
    uint8 n1,n10,n100;
    uint16 length;

    Clear_TxBuffer();
    if(switch_custom_mode == SWITCH_CUSTOM_TARGET_TEMP_SET)
    {
        sprintf(Tx_data,"  < Target Temp >");
        length = strlen(Tx_data);
        Tx_data[length] = ' ';
        length = length + 5;
        n1 = switch_trg_temp % 10;
        n10 = (switch_trg_temp % 100) / 10;
        n100 = (switch_trg_temp % 1000) / 100;
        if(n100>0)
        {
            Tx_data[length+1] = n100 + 16 + 32;
            Tx_data[length+2] = n10 + 16 + 32;
            Tx_data[length+3] = n1 + 16 + 32;
        }
        else if(n10>0)
        {
            Tx_data[length+2] = n10 + 16 + 32;
            Tx_data[length+3] = n1 + 16 + 32;            
        }
        else
        {
            Tx_data[length+3] = n1 + 16 + 32;                     
        }
        sprintf(&Tx_data[length+4]," [*C]");
        length = strlen(Tx_data);
        Tx_data[length] = ' ';
    }
    else if(switch_custom_mode == SWITCH_CUSTOM_TARGET_TIME_SET)
    {
        sprintf(Tx_data,"  < Target Time >");
        length = strlen(Tx_data);
        Tx_data[length] = ' ';
        length = length + 5;
        n1 = switch_trg_time % 10;
        n10 = (switch_trg_time % 100) / 10;
        if(n10>0)
        {
            Tx_data[length+2] = n10 + 16 + 32;
            Tx_data[length+3] = n1 + 16 + 32;            
        }
        else
        {
            Tx_data[length+3] = n1 + 16 + 32;                     
        }
        sprintf(&Tx_data[length+4]," [s]");
        length = strlen(Tx_data);
        Tx_data[length] = ' ';
    }
    else if(switch_custom_mode == SWITCH_CUSTOM_IDLE_SET)
    {
        sprintf(Tx_data,"   < Idle Temp >");
        length = strlen(Tx_data);
        Tx_data[length] = ' ';
        length = length + 6;
        n1 = switch_idle_temp % 10;
        n10 = (switch_idle_temp % 100) / 10;
        n100 = (switch_idle_temp % 1000) / 100;
        if(n100>0)
        {
            Tx_data[length+1] = n100 + 16 + 32;
            Tx_data[length+2] = n10 + 16 + 32;
            Tx_data[length+3] = n1 + 16 + 32;
        }
        else if(n10>0)
        {
            Tx_data[length+2] = n10 + 16 + 32;
            Tx_data[length+3] = n1 + 16 + 32;            
        }
        else
        {
            Tx_data[length+3] = n1 + 16 + 32;                     
        }
        sprintf(&Tx_data[length+4]," [*C]");
        length = strlen(Tx_data);
        Tx_data[length] = ' ';
    }
    else{}
    Print_TxBuffer();
    OLED_Update_Screen();
}

static void OLED_Print_Idle(void)
{    
    uint8 n1,n10;
    uint16 length;

    Clear_TxBuffer();
    sprintf(Tx_data,"   < IDLE Mode >  ");
    length = strlen(Tx_data);    
    Tx_data[length] = ' ';  
    length = length +5;
    sprintf(&Tx_data[length],"Menu :");
    length = strlen(Tx_data);    
    Tx_data[length] = ' ';  
    n1 = switch_submode%10;
    n10 = (switch_submode%100)/10;
    if(n10>0)
    {
        Tx_data[length] = n10 + 16 + 32;
        Tx_data[length+1] = n1 + 16 + 32;
    }
    else
    {
        length = length + 1;
        Tx_data[length] = n1 + 16 + 32;
    }
    Print_TxBuffer();  
    OLED_Update_Screen();
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