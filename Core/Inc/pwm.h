#include "main.h"
#include "switch.h"

#define PWM_TIM_MODULE_PELTIER      (TIM3)
#define PWM_TIM_MODULE_HEATER       (TIM3)
#define PWM_TIM_MODULE_SERVO        (TIM1)
#define PWM_TIM_MIDULE_FAN          (TIM1)

#define PWM_TIM_CH_PELTIER          CCR2
#define PWM_TIM_CH_HEATER           CCR1
#define PWM_TIM_CH_SERVO            CCR3
#define PWM_TIM_CH_FAN              CCR1

extern void Pwm_Output(void);