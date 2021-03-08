/**
 * ESP32 Multicore Semaphore Demo
 * 
 * How to use semaphores with multicore tasks.
 * 
 * Date: March 3, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

// Settings
static const uint32_t task_0_delay = 500;  // Time (ms) Task 0 blocks itself

// Pins
static const int pin_1 = 13;  // LED pin

// Globals
static SemaphoreHandle_t bin_sem;

//*****************************************************************************
// Tasks

// Task in Core 0
void doTask0(void *parameters) {

  // Configure pin
  pinMode(pin_1, OUTPUT);

  // Do forever
  while (1) {

    // Notify other task
    xSemaphoreGive(bin_sem);
    
    // Yield processor for a while
    vTaskDelay(task_0_delay / portTICK_PERIOD_MS);
  }
}

// Task in Core 1
void doTask1(void *parameters) {

  // Do forever
  while (1) {

    // Wait for semaphore
    xSemaphoreTake(bin_sem, portMAX_DELAY);

    // Toggle LED
    digitalWrite(pin_1, !digitalRead(pin_1));
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Create binary semaphore before starting tasks
  bin_sem = xSemaphoreCreateBinary();
  
  // Start Task 0 (in Core 0)
  xTaskCreatePinnedToCore(doTask0,
                          "Task 0",
                          1024,
                          NULL,
                          1,
                          NULL,
                          pro_cpu);

  // Start Task 1 (in Core 1)
  xTaskCreatePinnedToCore(doTask1,
                          "Task 1",
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
