#include "stdint.h"
#include "system_stm32f1xx.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_def.h"
#include "main.h"
#include "os.h"

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

extern void __libc_init_array();

void Reset_Handler()
{
    uint32_t *dataInit = &_sidata;
    uint32_t *data = &_sdata;
    while(data < &_edata)
    {
        *data++ = *dataInit++;
    }

    uint32_t *bss = &_sbss;
    while(bss < &_ebss)
    {
        *bss++ = 0;
    }

    SystemInit(); 
    __libc_init_array();
    main();

    while (1);
}

void Default_Handler(void)
{
    while (1);
}

void TIM2_Handler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

void TIM4_Handler(void)
{
    HAL_TIM_IRQHandler(&htim4);
}

void UART2_Handler(void)
{
    HAL_UART_IRQHandler(&huart2);
}

void SystemTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */
    Os_Handler_1ms();
    /* USER CODE END SysTick_IRQn 1 */
}

__weak void NMI_Handler(void)                          {Default_Handler();}
__weak void HardFault_Handler(void)                    {Default_Handler();}
__weak void MemManage_Handler(void)                    {Default_Handler();}
__weak void BusFault_Handler(void)                     {Default_Handler();}
__weak void UsageFault_Handler(void)                   {Default_Handler();}
__weak void SVC_Handler(void)                          {Default_Handler();}
__weak void DebugMon_Handler(void)                     {Default_Handler();}
__weak void PendSV_Handler(void)                       {Default_Handler();}
__weak void SysTick_Handler(void)                      {SystemTick_Handler();}
__weak void WWDG_IRQHandler(void)                      {Default_Handler();}  
__weak void PVD_IRQHandler(void)                       {Default_Handler();} 
__weak void TAMPER_IRQHandler(void)                    {Default_Handler();}             
__weak void RTC_IRQHandler(void)                       {Default_Handler();}            
__weak void FLASH_IRQHandler(void)                     {Default_Handler();}                  
__weak void RCC_IRQHandler(void)                       {Default_Handler();}
__weak void EXTI0_IRQHandler(void)                     {Default_Handler();}  
__weak void EXTI1_IRQHandler(void)                     {Default_Handler();}  
__weak void EXTI2_IRQHandler(void)                     {Default_Handler();}  
__weak void EXTI3_IRQHandler(void)                     {Default_Handler();}  
__weak void EXTI4_IRQHandler(void)                     {Default_Handler();}  
__weak void DMA1_Channel1_IRQHandler(void)             {Default_Handler();}
__weak void DMA1_Channel2_IRQHandler(void)             {Default_Handler();}
__weak void DMA1_Channel3_IRQHandler(void)             {Default_Handler();}
__weak void DMA1_Channel4_IRQHandler(void)             {Default_Handler();}
__weak void DMA1_Channel5_IRQHandler(void)             {Default_Handler();}
__weak void DMA1_Channel6_IRQHandler(void)             {Default_Handler();}
__weak void DMA1_Channel7_IRQHandler(void)             {Default_Handler();}          
__weak void ADC1_IRQHandler(void)                      {Default_Handler();} 
__weak void USB_HP_CAN1_TX_IRQHandler(void)            {Default_Handler();} 
__weak void USB_LP_CAN1_RX0_IRQHandler(void)           {Default_Handler();}
__weak void CAN1_RX1_IRQHandler(void)                  {Default_Handler();}
__weak void CAN1_SCE_IRQHandler(void)                  {Default_Handler();}
__weak void EXTI9_5_IRQHandler(void)                   {Default_Handler();}
__weak void TIM1_BRK_IRQHandler(void)                  {Default_Handler();}
__weak void TIM1_UP_IRQHandler(void)                   {Default_Handler();}
__weak void TIM1_TRG_COM_IRQHandler(void)              {Default_Handler();}                 
__weak void TIM1_BRK_UP_TRG_COM_IRQHandler(void)       {Default_Handler();}    
__weak void TIM1_CC_IRQHandler(void)                   {Default_Handler();}               
__weak void TIM3_IRQHandler(void)                      {Default_Handler();}                
__weak void TIM2_IRQHandler(void)                      {TIM2_Handler();}               
__weak void TIM4_IRQHandler(void)                      {TIM4_Handler();}     
__weak void I2C1_EV_IRQHandler(void)                   {Default_Handler();} 
__weak void I2C1_ER_IRQHandler(void)                   {Default_Handler();} 
__weak void I2C2_EV_IRQHandler(void)                   {Default_Handler();} 
__weak void I2C2_ER_IRQHandler(void)                   {Default_Handler();}              
__weak void SPI1_IRQHandler(void)                      {Default_Handler();}             
__weak void SPI2_IRQHandler(void)                      {Default_Handler();}               
__weak void USART1_IRQHandler(void)                    {Default_Handler();}              
__weak void USART2_IRQHandler(void)                    {UART2_Handler();}              
__weak void USART3_IRQHandler(void)                    {Default_Handler();}              
__weak void EXTI15_10_IRQHandler(void)                 {Default_Handler();} 
__weak void RTC_Alarm_IRQHandler(void)                 {Default_Handler();} 
__weak void USBWakeUp_IRQHandler(void)                 {Default_Handler();} 
__weak void BootRAM(void)                              {Default_Handler();} 

__attribute__((section(".isr_vector")))

const void (*VectorTable[])(void) = 
{
    (const void (*)(void))&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler ,                  /* Window WatchDog              */
    PVD_IRQHandler,                    /* Reserved                     */
    TAMPER_IRQHandler,                 /* Reserved                     */
    RTC_IRQHandler,                    /* RTC through the EXTI line    */
    FLASH_IRQHandler,                  /* FLASH                        */
    RCC_IRQHandler,                    /* RCC                          */
    EXTI0_IRQHandler,                  /* EXTI Line 0                  */
    EXTI1_IRQHandler,                  /* EXTI Line 1                  */
    EXTI2_IRQHandler,                  /* EXTI Line 2                  */
    EXTI3_IRQHandler,                  /* EXTI Line 3                  */
    EXTI4_IRQHandler,                  /* EXTI Line 4                  */
    DMA1_Channel1_IRQHandler,          /* DMA1 Channel 1               */
    DMA1_Channel2_IRQHandler,          /* DMA1 Channel 2               */
    DMA1_Channel3_IRQHandler,          /* DMA1 Channel 3               */
    DMA1_Channel4_IRQHandler,          /* DMA1 Channel 4               */
    DMA1_Channel5_IRQHandler,          /* DMA1 Channel 5               */
    DMA1_Channel6_IRQHandler,          /* DMA1 Channel 6               */
    DMA1_Channel7_IRQHandler,          /* DMA1 Channel 7               */
    ADC1_2_IRQHandler,                 /* ADC1_2                         */
    USB_HP_CAN1_TX_IRQHandler,
    USB_LP_CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_IRQHandler,
    TIM1_UP_IRQHandler,
    TIM1_TRG_COM_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,
    RTC_Alarm_IRQHandler,
    USBWakeUp_IRQHandler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    BootRAM          /* @0x108. This is for boot in RAM mode for*/
};
