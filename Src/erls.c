#include "erls.h"

#define ERLS_CRSF_SYNC_BYTE         0xC8
#define ERLS_CRSF_TYPE_CHANNELS     0x16
#define ERLS_CRSF_MIN               172
#define ERLS_CRSF_CENTER            992
#define ERLS_CRSF_MAX               1811

void ERLS_Init(ERLS_HandleTypeDef *herls, UART_HandleTypeDef *huart)
{
    if (herls == 0 || huart == 0)
        return;

    herls->huart = huart;

    for (int i = 0; i < 64; i++)
        herls->packet[i] = 0;

    for (int i = 0; i < ERLS_CHANNELS_COUNT; i++)
    {
        herls->raw[i] = 0;
        herls->us[i] = 0;
    }
}

uint16_t ERLS_RawToUs(uint16_t raw)
{
    if (raw < ERLS_CRSF_MIN)
        raw = ERLS_CRSF_MIN;

    if (raw > ERLS_CRSF_MAX)
        raw = ERLS_CRSF_MAX;

    return (uint16_t)(1000U + (((uint32_t)(raw - ERLS_CRSF_MIN) * 1000U) / (ERLS_CRSF_MAX - ERLS_CRSF_MIN)));
}

void ERLS_UpdateUs(ERLS_HandleTypeDef *herls)
{
    if (herls == 0)
        return;

    for (int i = 0; i < ERLS_CHANNELS_COUNT; i++)
    {
        herls->us[i] = ERLS_RawToUs(herls->raw[i]);
    }
}

uint8_t ERLS_ReadChannels(ERLS_HandleTypeDef *herls, uint32_t timeout)
{
    uint8_t b = 0;
    uint8_t len = 0;
    uint8_t *p = 0;

    if (herls == 0 || herls->huart == 0)
        return 0;

    if (HAL_UART_Receive(herls->huart, &b, 1, timeout) != HAL_OK)
        return 0;

    if (b != ERLS_CRSF_SYNC_BYTE)
        return 0;

    herls->packet[0] = b;

    if (HAL_UART_Receive(herls->huart, &herls->packet[1], 1, timeout) != HAL_OK)
        return 0;

    len = herls->packet[1];

    if (len < 2 || len > 62)
        return 0;

    if (HAL_UART_Receive(herls->huart, &herls->packet[2], len, timeout) != HAL_OK)
        return 0;

    if (herls->packet[2] != ERLS_CRSF_TYPE_CHANNELS)
        return 0;

    p = &herls->packet[3];

    herls->raw[0]  = ((p[0]       | (p[1] << 8)) & 0x07FF);
    herls->raw[1]  = (((p[1] >> 3) | (p[2] << 5)) & 0x07FF);
    herls->raw[2]  = (((p[2] >> 6) | (p[3] << 2) | (p[4] << 10)) & 0x07FF);
    herls->raw[3]  = (((p[4] >> 1) | (p[5] << 7)) & 0x07FF);
    herls->raw[4]  = (((p[5] >> 4) | (p[6] << 4)) & 0x07FF);
    herls->raw[5]  = (((p[6] >> 7) | (p[7] << 1) | (p[8] << 9)) & 0x07FF);
    herls->raw[6]  = (((p[8] >> 2) | (p[9] << 6)) & 0x07FF);
    herls->raw[7]  = (((p[9] >> 5) | (p[10] << 3)) & 0x07FF);
    herls->raw[8]  = ((p[11]      | (p[12] << 8)) & 0x07FF);
    herls->raw[9]  = (((p[12] >> 3) | (p[13] << 5)) & 0x07FF);
    herls->raw[10] = (((p[13] >> 6) | (p[14] << 2) | (p[15] << 10)) & 0x07FF);
    herls->raw[11] = (((p[15] >> 1) | (p[16] << 7)) & 0x07FF);

    ERLS_UpdateUs(herls);

    return 1;
}