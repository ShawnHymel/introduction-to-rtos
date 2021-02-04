/**
 * ESP32 Timer Interrupt Demo
 * 
 * Blink LED with hardware timer interrupt.
 * 
 * Date: February 3, 2021
 * Author: Shawn Hymel
 * License: 0BSD
 */

// Settings
static const uint16_t timer_divider = 80;
static const uint64_t timer_max_count = 1000000;

// Pins (change this if your Arduino board does not have LED_BUILTIN defined)
static const int led_pin = LED_BUILTIN;

// Globals
static hw_timer_t *timer = NULL;

//*****************************************************************************
// Interrupt Service Routines (ISRs)

// This function executes when timer reaches max (and resets)
void IRAM_ATTR onTimer() {

  // Toggle LED
  int pin_state = digitalRead(led_pin);
  digitalWrite(led_pin, !pin_state);
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure LED pin
  pinMode(led_pin, OUTPUT);

  // Create and start timer (num, divider, countUp)
  timer = timerBegin(0, timer_divider, true);

  // Provide ISR to timer (timer, function, edge)
  timerAttachInterrupt(timer, &onTimer, true);

  // At what count should ISR trigger (timer, count, autoreload)
  timerAlarmWrite(timer, timer_max_count, true);

  // Allow ISR to trigger
  timerAlarmEnable(timer);
}

void loop() {
  // Do nothing
}
