#include "pwm.h"

static void Pwm_Peltier_Set_Duty(uint16 duty);
static void Pwm_Heater_Set_Duty(uint16 duty);
static void Pwm_Servo_Set_Duty(uint16 duty);
static void Pwm_Fan_Set_Duty(uint16 duty);

uint16 Pwm_Duty_Peltier = 0;
uint16 Pwm_Duty_Heater = 0;
uint16 Pwm_Duty_Servo = 0;
uint16 Pwm_Duty_Fan = 0;
void Pwm_Output(void)
{
    if(1 == cup_in)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET); // FAN Run
        Pwm_Duty_Peltier = 900;
        Pwm_Duty_Fan = 300;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);    
        Pwm_Duty_Peltier = 0;    
        Pwm_Duty_Fan = 0;
    }

    Pwm_Peltier_Set_Duty(Pwm_Duty_Peltier);
    Pwm_Heater_Set_Duty(Pwm_Duty_Heater);
    Pwm_Servo_Set_Duty(Pwm_Duty_Servo);
    Pwm_Fan_Set_Duty(Pwm_Duty_Fan);

    return;
}

static void Pwm_Peltier_Set_Duty(uint16 duty)
{
    /* Resolution : 0.1[%] */
    PWM_TIM_MODULE_PELTIER-> PWM_TIM_CH_PELTIER = duty;
}

static void Pwm_Heater_Set_Duty(uint16 duty)
{
    /* Resolution : 0.1[%] */
    PWM_TIM_MODULE_HEATER-> PWM_TIM_CH_HEATER = duty;
}

static void Pwm_Servo_Set_Duty(uint16 duty)
{
    /* Resolution : 0.1[%] */
    PWM_TIM_MODULE_SERVO-> PWM_TIM_CH_SERVO = duty;
}

static void Pwm_Fan_Set_Duty(uint16 duty)
{
    /* Resolution : 0.1[%] */
    PWM_TIM_MIDULE_FAN-> PWM_TIM_CH_FAN = duty;
}