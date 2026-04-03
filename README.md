## ERLSlib for STM32
Ця бібліотека призначена для з'днання ELRS модуля з STM. В данному проекті я використовую STM32Nucleo-F334R8. Базовий код ви можете знайти в main.c, або нижче:

## ERLSlib for STM32
This library is designed to connect an ELRS module to an STM microcontroller. In this project, I am using the STM32Nucleo-F334R8. The base code can be found in main.c, or below:
  
```c
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "erls.h"
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
ERLS_HandleTypeDef erls;
char msg[256];
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
  char startMsg[] = "UART START";
  HAL_UART_Transmit(&huart1, (uint8_t*)startMsg, sizeof(startMsg) - 1, HAL_MAX_DELAY);

  ERLS_Init(&erls, &huart3);
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(ERLS_ReadChannels(&erls, 50)){
		  int n = sprintf(msg,
			  "CH1=%4u CH2=%4u CH3=%4u CH4=%4u CH5=%4u CH6=%4u "
			  "CH7=%4u CH8=%4u CH9=%4u CH10=%4u CH11=%4u CH12=%4u\r\n",
			  erls.us[0], erls.us[1], erls.us[2], erls.us[3],
			  erls.us[4], erls.us[5], erls.us[6], erls.us[7],
			  erls.us[8], erls.us[9], erls.us[10], erls.us[11]);

		  HAL_UART_Transmit(&huart1, (uint8_t*)msg, n, HAL_MAX_DELAY);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
```
