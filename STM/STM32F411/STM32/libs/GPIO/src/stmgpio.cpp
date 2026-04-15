#include "stmgpio.h"

STMGPIO::STMGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) : GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) { 

}

void STMGPIO::writeGPIO(bool levelHigh) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, levelHigh ? GPIO_PinState::GPIO_PIN_SET : GPIO_PinState::GPIO_PIN_RESET);
}

int STMGPIO::readGPIO() {
    HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PinState::GPIO_PIN_SET ? 1 : 0;
}

int STMGPIO::analogRead() {
    // Если хочешь читать ADC, нужно привязать этот пин к каналу АЦП
    // Здесь пока заглушка:
    return 0;
}

void STMGPIO::pullUpDown(PulUPDonw type) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

    switch(type) {
        case PulUPDonw::PULL_UP:
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
        case PulUPDonw::PULL_DOWN:
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;        
        default:
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
    }

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}