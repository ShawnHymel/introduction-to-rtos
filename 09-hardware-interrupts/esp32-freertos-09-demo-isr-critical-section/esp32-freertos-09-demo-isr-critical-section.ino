/**
 * ESP32 ISR Critical Section Demo
 * 
 * Increment global variable in ISR.
 * 
 * Date: February 3, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint16_t timer_divider = 8;
static const uint64_t timer_max_count = 1000000;
static const TickType_t task_delay = 2000 / portTICK_PERIOD_MS;

// Globals
static hw_timer_t *timer = NULL;
static volatile int isr_counter;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;

//*****************************************************************************
// Interrupt Service Routines (ISRs)

// This function executes when timer reaches max (and resets)
void IRAM_ATTR onTimer() {
  
  // ESP-IDF version of a critical section (in an ISR)
  portENTER_CRITICAL_ISR(&spinlock);
  isr_counter++;
  portEXIT_CRITICAL_ISR(&spinlock);

  // Vanilla FreeRTOS version of a critical section (in an ISR)
  //UBaseType_t saved_int_status;
  //saved_int_status = taskENTER_CRITICAL_FROM_ISR();
  //isr_counter++;
  //taskEXIT_CRITICAL_FROM_ISR(saved_int_status);
}

//*****************************************************************************
// Tasks

// Wait for semaphore and print out ADC value when received
void printValues(void *parameters) {

  // Loop forever
  while (1) {
    
    // Count down and print out counter value
    while (isr_counter > 0) {

      // Print value of counter
      Serial.println(isr_counter);
  
      // ESP-IDF version of a critical section (in a task)
      portENTER_CRITICAL(&spinlock);
      isr_counter--;
      portEXIT_CRITICAL(&spinlock);

      // Vanilla FreeRTOS version of a critical section (in a task)
      //taskENTER_CRITICAL();
      //isr_counter--;
      //taskEXIT_CRITICAL();
    }
  
    // Wait 2 seconds while ISR increments counter a few times
    vTaskDelay(task_delay);
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
  Serial.println("---FreeRTOS ISR Critical Section Demo---");

  // Start task to print out results
  xTaskCreatePinnedToCore(printValues,
                          "Print values",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Create and start timer (num, divider, countUp)
  timer = timerBegin(0, timer_divider, true);

  // Provide ISR to timer (timer, function, edge)
  timerAttachInterrupt(timer, &onTimer, true);

  // At what count should ISR trigger (timer, count, autoreload)
  timerAlarmWrite(timer, timer_max_count, true);

  // Allow ISR to trigger
  timerAlarmEnable(timer);

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
