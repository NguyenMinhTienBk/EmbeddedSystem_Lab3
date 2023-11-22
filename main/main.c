//#include <stdio.h>
//#include <stdbool.h>
//#include <unistd.h>
//#include "freertos/FreeRTOS.h"
//#include "freertos/FreeRTOSConfig.h"
//#include "freertos/task.h"
//#include "driver/gpio.h"
//#include "esp_log.h"
//
//
//#define BUTTON_GPIO GPIO_NUM_17
//
//
//volatile uint32_t ulIdleCycleCount = 0UL;
///* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
//and return void. */
//void vApplicationIdleHook( void )
//{
//	/* This hook function does nothing but increment a counter. */
//	ulIdleCycleCount++;
//
//}
//
//static const char *TAG = "MyApp";
//void vPrintStringAndNumber(const char *pcString, uint32_t ulNumber)
//{
//    // Print the string and the number using ESP-IDF logging functions
//    ESP_LOGI(TAG, "%s: %lu", pcString, ulNumber);
//}
//
//
//void vTaskFunction( void *pvParameters )
//{
//	char *pcTaskName;
//	const TickType_t xDelay250ms = pdMS_TO_TICKS( 250 );
//	/* The string to print out is passed in via the parameter. Cast this to a
//	character pointer. */
//	pcTaskName = ( char * ) pvParameters;
//	/* As per most tasks, this task is implemented in an infinite loop. */
//	for( ;; )
//	{
//	/* Print out the name of this task AND the number of times ulIdleCycleCount
//	has been incremented. */
////	ESP_LOGI(TAG, "This is an informational message: %lu", ulIdleCycleCount);
//	printf("Idle Task is running ");
//	vPrintStringAndNumber( pcTaskName, ulIdleCycleCount );
//	/* Delay for a period of 250 milliseconds. */
//	vTaskDelay( xDelay250ms );
//	}
//}
//
//void vTaskFunction1( void * pvParameters )
//{
//	char * pcTaskName ;
//	const TickType_t xDelay250ms = pdMS_TO_TICKS ( 250 );
//	/* The string to print out is passed in via the parameter .
//	Cast this to a character pointer . */
//	pcTaskName = ( char * ) pvParameters ;
//
//	 /* As per most tasks , this task is implemented in
//	 an infinite loop . */
//	 for ( ;; )
//	 {
//		 /* Print out the name of this task . */
//		 printf(pcTaskName);
//
//		 /* Delay for a period . This time a call to vTaskDelay ()
//		 is used which places the task into the Blocked state
//		 until the delay period has expired . The parameter takes
//		 a time specified in " ticks ", and the pdMS_TO_TICKS () macro
//		 is used ( where the xDelay250ms constant is declared ) to
//		 convert 250 milliseconds into an equivalent time in ticks .*/
//		 vTaskDelay ( xDelay250ms );
//	 }
//}
//
//bool button_pressed = false;
//
//void button_task(void *pvParameters) {
//
//	char * pcTaskName ;
//	const TickType_t xDelay10ms = pdMS_TO_TICKS (10 );
//	/* The string to print out is passed in via the parameter .
//	Cast this to a character pointer . */
//	pcTaskName = ( char * ) pvParameters ;
//
//    gpio_pad_select_gpio(BUTTON_GPIO);
//    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
//    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
//
//    while (1) {
//        int button_state = gpio_get_level(BUTTON_GPIO);
//
//        if (button_state == 0 && !button_pressed) {
//			printf(pcTaskName);
//			button_pressed = true; // Đánh dấu nút đã được nhấn
//		}
//		else if (button_state == 1 && button_pressed) {
//			button_pressed = false; // Đánh dấu nút đã được thả ra
//		}
//        vTaskDelay(xDelay10ms); // Đợi một khoảng thời gian ngắn trước khi đọc trạng thái nút bấm lại
//    }
//}
//
////static const char * pcTextForIdleTask = " Idle Task is running with counter: \r\n";
//
//static const char *pcTextForTask1 = "Task 1 is running \r\n";
//static const char *pcTextForTask2 = "Task 2 is running \r\n";
//static const char *pcTextForIdleTask = "Idle Task  is running with \r\n";
//
//void app_main(void)
//{
//	 xTaskCreate(button_task, "button_task", 1000, ( void*)pcTextForTask1, 2, NULL);
//	 xTaskCreate(vTaskFunction1,"task2", 1000, ( void*)pcTextForTask2 , 0, NULL);
//	 xTaskCreate(vTaskFunction,"Idle task", 1000, (void*)pcTextForIdleTask, 1, NULL);
//
////	 while(1){
////		 vTaskDelay(1000/portTICK_PERIOD_MS);
////	 }
//	 vTaskStartScheduler();
//}

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define configUSE_PREEMPTION 0       //Espressif\frameworks\esp-idf-v4.4.2\components\freertos\include\freertos\FreeRTOS.h
#define configUSE_TIME_SLICING 1     //Espressif\frameworks\esp-idf-v4.4.2\components\freertos\include\esp_additions\freertos\FreeRTOSConfig.h

volatile uint32_t ulIdleCycleCount = 0UL;

volatile uint32_t ulIdleTask1Count = 0UL;       // PREEMPTION = 0
volatile uint32_t ulIdleTask2Count = 0UL;
volatile uint32_t ulIdleTask3Count = 0UL;
// =============================================

void func_2(void *pvParameter)
{
    int count = 0;
    while (1)
    {
        if (configUSE_PREEMPTION == 0)
        {
            count++;
            printf("Co-operative task 2 count: %d\n",count);
            if(count == 4)
            {
                printf("Co-operative task 2 end !!!\n");
                vTaskDelete(NULL);
            }
        }
        else if (configUSE_PREEMPTION == 1)
        {
            ulIdleTask2Count++;
        }
    }
}

void func_3(void *pvParameter)
{
    int count = 0;
    while (1)
    {
        if (configUSE_PREEMPTION == 0)
        {
            count++;
            printf("===> Co-operative task 3 count: %d\n",count);
            if(count == 3)
            {
                printf("===> Co-operative task 3 end !!!\n");
                vTaskDelete(NULL);
            }
        }
        else if (configUSE_PREEMPTION == 1)
        {
            ulIdleTask3Count++;
        }
    }
}

void func_1(void *pvParameter)
{
    while (1)
    {
        if (configUSE_PREEMPTION == 0)
        {
            if (ulIdleTask1Count == 0)
            {
                xTaskCreatePinnedToCore(&func_2,"task2",1024*5,NULL,2,NULL,0);
            }
            else if (ulIdleTask1Count == 2)
            {
                xTaskCreatePinnedToCore(&func_3,"task3",1024*5,NULL,5,NULL,0);
            }
            ulIdleTask1Count++;

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            printf("Preemptive task 2 count:  %ld \n", ulIdleTask2Count);
            printf("Preemptive task 3 count: =======|| %ld \n", ulIdleTask3Count);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void app_main()
{
    printf("Preemption : %d \n",configUSE_PREEMPTION);
    printf("Time-slicing : %d \n\n",configUSE_TIME_SLICING);

    if (configUSE_PREEMPTION == 0)
    {
        xTaskCreate(&func_1,"task1",1024*5,NULL,0,NULL);
    }
    else
    {
        xTaskCreatePinnedToCore(&func_1,"task1",1024*5,NULL,2,NULL,1);
        xTaskCreatePinnedToCore(&func_2,"task2",1024*5,NULL,0,NULL,0);
        xTaskCreatePinnedToCore(&func_3,"task3",1024*5,NULL,0,NULL,0);
    }
}

