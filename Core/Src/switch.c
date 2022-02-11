#include "switch.h"

uint8 switch_status1,switch_status2,switch_status3,switch_status4;
uint8 cup_in;

void Switch_Control(void)
{
    switch_status1 = HAL_GPIO_ReadPin(SWITCH_GROUP_1,SWITCH_PIN_1);
    switch_status2 = HAL_GPIO_ReadPin(SWITCH_GROUP_2,SWITCH_PIN_2);
    switch_status3 = HAL_GPIO_ReadPin(SWITCH_GROUP_3,SWITCH_PIN_3);
    cup_in = HAL_GPIO_ReadPin(CUP_GROUP,CUP_PIN); 

    switch_status1 = 0;
    switch_status2 = 0;
    switch_status3 = 0;

    if((1 == switch_status1)||(1 == switch_status2)||(1 == switch_status3))
    {   
        HAL_GPIO_WritePin(BUZZER_GROUP,BUZZER_PIN,GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(BUZZER_GROUP,BUZZER_PIN,GPIO_PIN_RESET);       
    }

    return;
}