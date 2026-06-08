// hcsr04.h
#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>
#include "sl_status.h"

sl_status_t hcsr04_init(void);
int32_t hcsr04_measure_cm(void);

#endif
 hcsr04.c
// hcsr04.c
#include "hcsr04.h"
#include "sl_si91x_driver_gpio.h"
#include "sl_gpio_board.h"
#include "sl_sleeptimer.h"
#include <stdio.h>

// Echo pin: ULP Port, Pin 7
// TRIG is driven by Arduino — SiWG917 only reads ECHO
static sl_si91x_gpio_pin_config_t echo_gpio_config = {
  { SL_GPIO_ULP_PORT, 7 },
  GPIO_INPUT
};

// Timeout: ~38ms echo pulse = no object in range (HC-SR04 spec)
// sleeptimer @ 32768 Hz → 38ms = ~1245 ticks. Use 1300 as safe ceiling.
#define ECHO_TIMEOUT_TICKS  1300

sl_status_t hcsr04_init(void)
{
  sl_status_t status;

  status = sl_gpio_driver_init();
  if (status != SL_STATUS_OK) {
    printf("HC-SR04: GPIO init failed: 0x%lx\n", status);
    return status;
  }

  status = sl_gpio_set_configuration(echo_gpio_config);
  if (status != SL_STATUS_OK) {
    printf("HC-SR04: Echo pin config failed: 0x%lx\n", status);
    return status;
  }

  return SL_STATUS_OK;
}

int32_t hcsr04_measure_cm(void)
{
  uint8_t pin_val = 0;
  uint32_t t_start, t_end, t_now;

  // --- Wait for echo to go HIGH (rising edge) ---
  t_start = sl_sleeptimer_get_tick_count();
  do {
    sl_gpio_driver_get_pin(&echo_gpio_config.port_pin, &pin_val);
    t_now = sl_sleeptimer_get_tick_count();
    if ((t_now - t_start) > ECHO_TIMEOUT_TICKS) {
      printf("HC-SR04: Timeout waiting for echo HIGH\n");
      return -1;
    }
  } while (pin_val == 0);

  t_start = sl_sleeptimer_get_tick_count();  // mark rising edge

  // --- Wait for echo to go LOW (falling edge) ---
  do {
    sl_gpio_driver_get_pin(&echo_gpio_config.port_pin, &pin_val);
    t_now = sl_sleeptimer_get_tick_count();
    if ((t_now - t_start) > ECHO_TIMEOUT_TICKS) {
      printf("HC-SR04: Timeout waiting for echo LOW (out of range)\n");
      return -1;
    }
  } while (pin_val == 1);

  t_end = sl_sleeptimer_get_tick_count();

  // --- Convert tick delta to microseconds ---
  // sl_sleeptimer_get_timer_frequency() returns ticks/sec (typically 32768)
  uint32_t freq = sl_sleeptimer_get_timer_frequency();
  uint32_t delta_ticks = t_end - t_start;

  // delta_us = delta_ticks * 1,000,000 / freq
  // Avoid overflow: delta_ticks max ~1245, so 1245 * 1000000 = 1.245e9 — fits in uint32_t
  uint32_t delta_us = (delta_ticks * 1000000UL) / freq;

  // Distance (cm) = pulse_width_us / 58
  int32_t distance_cm = (int32_t)(delta_us / 58);

  return distance_cm;
}
