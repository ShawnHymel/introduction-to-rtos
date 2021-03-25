/**
 * ESP32 Deadlock Demo - Timeout
 * 
 * Demonstrate why kernel object timeouts are important to alleviate
 * deadlock. Note that this still can cause "livelock."
 * 
 * Date: February 8, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// You'll likely need this on vanilla FreeRTOS
//#include <semphr.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
TickType_t mutex_timeout = 1000 / portTICK_PERIOD_MS;

// Globals
static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

//*****************************************************************************
// Tasks

// Task A (high priority)
void doTaskA(void *parameters) {

  // Loop forever
  while (1) {

    // Take mutex 1
    if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {

      // Say we took mutex 1 and wait (to force deadlock)
      Serial.println("Task A took mutex 1");
      vTaskDelay(1 / portTICK_PERIOD_MS);
  
      // Take mutex 2
      if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {

        // Say we took mutex 2
        Serial.println("Task A took mutex 2");
  
        // Critical section protected by 2 mutexes
        Serial.println("Task A doing some work");
        vTaskDelay(500 / portTICK_PERIOD_MS);
      } else {
        Serial.println("Task A timed out waiting for mutex 2");
      }
    } else {
      Serial.println("Task A timed out waiting for mutex 1");
    }

    // Give back mutexes
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// Task B (low priority)
void doTaskB(void *parameters) {

  // Loop forever
  while (1) {

    // Take mutex 2
    if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {

      // Say we took mutex 2 and wait (to force deadlock)
      Serial.println("Task B took mutex 2");
      vTaskDelay(1 / portTICK_PERIOD_MS);
  
      // Take mutex 1
      if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {

        // Say we took mutex 1
        Serial.println("Task B took mutex 1");
  
        // Critical section protected by 2 mutexes
        Serial.println("Task B doing some work");
        vTaskDelay(500 / portTICK_PERIOD_MS);
      } else {
        Serial.println("Task B timed out waiting for mutex 1");
      }
    } else {
      Serial.println("Task B timed out waiting for mutex 2");
    }

    // Give back mutexes
    xSemaphoreGive(mutex_1);
    xSemaphoreGive(mutex_2);

    // Wait to let the other task execute
    Serial.println("Task B going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {
  
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Deadlock Demo---");

  // Create mutexes before starting tasks
  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  // Start Task A (high priority)
  xTaskCreatePinnedToCore(doTaskA,
                          "Task A",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  // Start Task B (low priority)
  xTaskCreatePinnedToCore(doTaskB,
                          "Task B",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
