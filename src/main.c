/************LIBRARIES************/
#include <../include/BIT_MATH.h>
#include <../include/STD_Types.h>

/************MCAL************/
#include <../include/MRCC_Interface.h>
#include <../include/MSTK_Interface.h>
#include <../include/MGPIO_Interface.h>
#include <../include/MNVIC_Interface.h>
#include <../include/MDMA_Interface.h>

void DMA_INTERRUPT_CALLBACK();

int main(void){
    // Define arrays
    u32 array1[6000];
    u32 array2[6000];
    u32 array3[6000];
    u32 array4[6000];

    // Initialize arrays
    for(u16 i = 0 ; i < 6000 ; i++){
        array1[i] = 1;
        array3[i] = 2;
    }

    // Initialize peripherals
    MRCC_voidInit(); // Initialize Reset and Clock Control peripheral
    MSTK_voidInit(); // Initialize SysTick Timer

    MRCC_voidEnablePeripheral(RCC_AHB1, RCC_AHB1_GPIOA); // Enable GPIOA clock
    MRCC_voidEnablePeripheral(RCC_AHB1, RCC_AHB1_DMA2); // Enable DMA2 clock

    // Configure GPIO pins
    MGPIO_voidSetPinMode(GPIO_PORTA, GPIO_PIN0, GPIO_OUTPUT);
    MGPIO_voidSetPinOutputMode(GPIO_PORTA, GPIO_PIN0, GPIO_PUSH_PULL, GPIO_HIGH_SPEED);

    MGPIO_voidSetPinMode(GPIO_PORTA, GPIO_PIN2, GPIO_OUTPUT);
    MGPIO_voidSetPinOutputMode(GPIO_PORTA, GPIO_PIN2, GPIO_PUSH_PULL, GPIO_HIGH_SPEED);

    // Enable DMA interrupt
    MNVIC_voidEnablePeripheralInterrupt(DMA_Stream0);

    // Initialize DMA
    DMA2_Init();
    DMA2_SetStreamConfigration(0, 0, array3, array4, 2, 6000); // Configure DMA stream
    DMA2_SetCallBack(0, DMA_INTERRUPT_CALLBACK); // Set callback function for DMA interrupt
    DMA2_enableStream(0); // Enable DMA stream

    // Copy data from array1 to array2 using CPU
    for(u16 i = 0 ; i < 6000 ; i++){
        array2[i] = array1[i];
    }

    MSTK_voidDelayms(1000); // Delay for 1000 milliseconds
    MGPIO_voidSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_PIN_HIGH); // Set GPIO_PIN0 to HIGH

}

// Callback function for DMA interrupt
void DMA_INTERRUPT_CALLBACK(void){
    MGPIO_voidSetPinValue(GPIO_PORTA, GPIO_PIN2, GPIO_PIN_HIGH); // Set GPIO_PIN2 to HIGH
    MSTK_voidDelayms(1000); // Delay for 1000 milliseconds
}
