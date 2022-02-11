#include "main.h"

#define SWITCH_1                (0u)
#define SWITCH_2                (1u)
#define SWITCH_3                (2u)
#define SWITCH_4                (3u)

#define SWITCH_GROUP_1          (GPIOA)
#define SWITCH_GROUP_2          (GPIOA)
#define SWITCH_GROUP_3          (GPIOA)
#define SWITCH_GROUP_4          (GPIOA)
#define CUP_GROUP               (GPIOA)
#define BUZZER_GROUP            (GPIOB)

#define SWITCH_PIN_1            (GPIO_PIN_10)
#define SWITCH_PIN_2            (GPIO_PIN_11)
#define SWITCH_PIN_3            (GPIO_PIN_12)
#define SWITCH_PIN_4            (GPIO_PIN_3)
#define CUP_PIN                 (GPIO_PIN_9)
#define BUZZER_PIN              (GPIO_PIN_13)

extern uint8 cup_in;

extern void Switch_Control(void);

