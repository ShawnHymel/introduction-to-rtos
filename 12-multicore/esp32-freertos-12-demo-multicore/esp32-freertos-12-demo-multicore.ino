/**
 * ESP32 Multicore Demo
 * 
 * How to run two tasks on different cores
 * 
 * Date: March 3, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

// Settings
static const TickType_t time_hog = 200;  // Time (ms) hogging the CPU

//*****************************************************************************
// Functions

// Hogs the processor. Accurate to about 1 second (no promises).
static void hog_delay(uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    for (uint32_t j = 0; j < 40000; j++) {
      asm("nop");
    }
  }
}

//*****************************************************************************
// Tasks

// Task L (low priority)
void doTaskL(void *parameters) {
  
  TickType_t timestamp;
  char str[20];

  // Do forever
  while (1) {

    // Say something
    sprintf(str, "Task L, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    
    // Hog the processor for a while doing nothing (this is a bad idea)
    hog_delay(time_hog);
  }
}

// Task H (high priority)
void doTaskH(void *parameters) {
  
  TickType_t timestamp;
  char str[20];

  // Do forever
  while (1) {

    // Say something
    sprintf(str, "Task H, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    
    // Hog the processor for a while doing nothing (this is a bad idea)
    hog_delay(time_hog);
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
  Serial.println("---FreeRTOS Priority Inheritance Demo---");
  
  // Start Task L (low priority)
  xTaskCreatePinnedToCore(doTaskL,
                          "Task L",
                          2048,
                          NULL,
                          1,
                          NULL,
//                          pro_cpu);
                          tskNO_AFFINITY);

  // Start Task H (low priority)
  xTaskCreatePinnedToCore(doTaskH,
                          "Task H",
                          2048,
                          NULL,
                          2,
                          NULL,
//                          pro_cpu);
                          tskNO_AFFINITY);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
