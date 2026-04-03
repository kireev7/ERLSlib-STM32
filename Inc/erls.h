#ifndef ERLS_H
#define ERLS_H

#include "main.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ERLS_CHANNELS_COUNT 12

typedef struct
{
    UART_HandleTypeDef *huart;
    uint8_t packet[64];
    uint16_t raw[ERLS_CHANNELS_COUNT];
    uint16_t us[ERLS_CHANNELS_COUNT];
} ERLS_HandleTypeDef;

/**
 * Ініціалізація бібліотеки
 */
void ERLS_Init(ERLS_HandleTypeDef *herls, UART_HandleTypeDef *huart);

/**
 * Читання одного валідного пакета каналів CRSF (type 0x16)
 * Повертає:
 * 1  -> нові дані успішно отримані
 * 0  -> даних нема / пакет невалідний / таймаут
 */
uint8_t ERLS_ReadChannels(ERLS_HandleTypeDef *herls, uint32_t timeout);

/**
 * Конвертація одного raw значення CRSF у діапазон 1000..2000
 */
uint16_t ERLS_RawToUs(uint16_t raw);

/**
 * Оновлює масив us[] з raw[]
 */
void ERLS_UpdateUs(ERLS_HandleTypeDef *herls);

#ifdef __cplusplus
}
#endif

#endif /* ERLS_H */