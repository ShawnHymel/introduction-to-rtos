/**
 * FreeRTOS Software Timer Demo
 * 
 * Demonstrate basic timer usage.
 * 
 * Date: February 1, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// You'll likely need this on vanilla FreeRTOS
//#include <timers.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;

//*****************************************************************************
// Callbacks

// Called when one of the timers expires
void myTimerCallback(TimerHandle_t xTimer) {

  // Print message if timer 0 expired
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 0) {
    Serial.println("One-shot timer expired");
  }

  // Print message if timer 1 expired
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {
    Serial.println("Auto-reload timer expired");
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
  Serial.println("---FreeRTOS Timer Demo---");

  // Create a one-shot timer
  one_shot_timer = xTimerCreate(
                      "One-shot timer",           // Name of timer
                      2000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                      pdFALSE,                    // Auto-reload
                      (void *)0,                  // Timer ID
                      myTimerCallback);           // Callback function

  // Create an auto-reload timer
  auto_reload_timer = xTimerCreate(
                      "Auto-reload timer",        // Name of timer
                      1000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                      pdTRUE,                     // Auto-reload
                      (void *)1,                  // Timer ID
                      myTimerCallback);           // Callback function

  // Check to make sure timers were created
  if (one_shot_timer == NULL || auto_reload_timer == NULL) {
    Serial.println("Could not create one of the timers");
  } else {
    
    // Wait and then print out a message that we're starting the timers
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Starting timers...");

    // Start timers (max block time if command queue is full)
    xTimerStart(one_shot_timer, portMAX_DELAY);
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }

  // Delete self task to show that timers will work with no user tasks
  vTaskDelete(NULL);
}


void loop() {
  // Execution should never get here
}
