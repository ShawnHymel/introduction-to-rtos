/**
 * ESP32 Multicore Spinlock Demo
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
static const TickType_t time_hog = 1;  // Time (ms) hogging the CPU in Task 1
static const TickType_t task_0_delay = 30;  // Time (ms) Task 0 blocks itself
static const TickType_t task_1_delay = 100; // Time (ms) Task 1 blocks itself

// Pins
static const int pin_0 = 12;
static const int pin_1 = 13;

// Globals
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

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

// Task in Core 0
void doTask0(void *parameters) {

  // Configure pin
  pinMode(pin_0, OUTPUT);

  // Do forever
  while (1) {

    // Toggle pin
    portENTER_CRITICAL(&spinlock);
    digitalWrite(pin_0, !digitalRead(pin_0));
    portEXIT_CRITICAL(&spinlock);
    
    // Yield processor for a while
    vTaskDelay(task_0_delay / portTICK_PERIOD_MS);
  }
}

// Task in Core 1
void doTask1(void *parameters) {

  // Configure pin
  pinMode(pin_1, OUTPUT);

  // Do forever
  while (1) {

    // Do some long critical section (this is bad practice)
    portENTER_CRITICAL(&spinlock);
    digitalWrite(pin_1, HIGH);
    hog_delay(time_hog);
    digitalWrite(pin_1, LOW);
    portEXIT_CRITICAL(&spinlock);
    
    // Yield processor for a while
    vTaskDelay(task_1_delay / portTICK_PERIOD_MS);
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

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
