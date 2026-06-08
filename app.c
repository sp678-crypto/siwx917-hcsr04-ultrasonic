#include "hcsr04.h"
#include "sl_sleeptimer.h"
#include "sl_iostream.h"
#include "sl_iostream_init_instances.h"
#include "sl_iostream_handles.h"
#include <stdio.h>
#include <string.h>

#define MEASURE_INTERVAL_MS  200

static sl_sleeptimer_timer_handle_t measure_timer;
static volatile bool do_measure = false;

static void on_measure_timeout(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;
  do_measure = true;
}

void app_init(void)
{
  setvbuf(stdout, NULL, _IONBF, 0);
  sl_iostream_set_default(sl_iostream_vcom_handle);

  hcsr04_init();

  sl_sleeptimer_start_periodic_timer_ms(
    &measure_timer,
    MEASURE_INTERVAL_MS,
    on_measure_timeout,
    NULL,
    0,
    SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
  );
}

void app_process_action(void)
{
  if (do_measure) {
    do_measure = false;
    int32_t dist = hcsr04_measure_cm();
    if (dist >= 0) {
      printf("Distance: %ld cm\r\n", dist);
    }
  }
}
