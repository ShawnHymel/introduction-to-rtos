/**
 * ESP32 Multicore ISR Demo
 * 
 * Demonstration of critical sections and ISRs with multicore processor.
 * 
 * Date: March 3, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// Core definitions (assuming you have dual-core ESP32)
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;

// Settings
static const uint16_t timer_divider = 80; // 1 MHz hardware timer
static const uint64_t timer_max_count = 100000;  // 1 second ISR interval
static const TickType_t task_wait = 10; // Time (ms) hogging the CPU
static const TickType_t isr_wait = 20; // Time (ms) hogging the CPU

// Globals
static SemaphoreHandle_t bin_sem;
static hw_timer_t *timer = NULL;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

//*****************************************************************************
// Interrupt Service Routines (ISRs)

// This function executes when timer reaches max (and resets)
void IRAM_ATTR onTimer() {

  TickType_t timestamp;
  
  // Hog the CPU in the ISR (remember: this is a terrible idea)
  Serial.print("ISR...");
  portENTER_CRITICAL_ISR(&spinlock);
  timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
  while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < isr_wait);
  portEXIT_CRITICAL_ISR(&spinlock);
  Serial.println("Done");
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
    Serial.println("L");
    
    // Hog the processor for a while doing nothing
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < task_wait);
  }
}

// Task H (high priority)
void doTaskH(void *parameters) {
  
  TickType_t timestamp;
  char str[20];

  // Do forever
  while (1) {

    // Say something
    Serial.print("spinning...");
    portENTER_CRITICAL(&spinlock);
    Serial.println("H");
    portEXIT_CRITICAL(&spinlock);
    
    // Hog the processor for a while doing nothing
    timestamp = xTaskGetTickCount() * portTICK_PERIOD_MS;
    while ((xTaskGetTickCount() * portTICK_PERIOD_MS) - timestamp < task_wait);
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

  // Hardware timer (core 1) triggers ISR every 2 seconds
  timer = timerBegin(0, timer_divider, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, timer_max_count, true);
  timerAlarmEnable(timer);
  
  // Start Task L (low priority)
  xTaskCreatePinnedToCore(doTaskL,
                          "Task L",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start Task H (low priority)
  xTaskCreatePinnedToCore(doTaskH,
                          "Task H",
                          1024,
                          NULL,
                          2,
                          NULL,
                          pro_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
