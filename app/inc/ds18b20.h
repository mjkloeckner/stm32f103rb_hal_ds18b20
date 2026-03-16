/*
 * Copyright (c) 2026 Martin Javier Klöckner <mklockner@fi.uba.ar>
 *
 * See file `LICENSE` for full details
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#ifdef __cplusplus
extern "C" {
#endif

void UART1_SetBaud(uint32_t baud);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void DS18B20_Init(void);
void DS18B20_Write(uint8_t data);
void DS18B20_Read_temp(void);
uint8_t DS18B20_Read(void);
uint8_t DS18B20_Reset(void);

#ifdef __cplusplus
}
#endif

#endif /* DS18B20_H_ */
