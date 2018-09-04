
#include "ch.h"
#include "hal.h"
//#include "ch_test.h" Is this useful for this use? Probably not. TODO: Remove inc and src.
#include "chprintf.h"

/*
 * This is a periodic thread that does absolutely nothing.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("time_waster");
  while (true) {
    chThdSleepMilliseconds(500);
  }
}

int main(void) {

  // sdInit handled in halInit()
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

  /*
   * Create the time_waster thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  // Print some debug info.
  chnWrite(&SD2, (uint8_t *)"Starting...\r\n", 13);
  chprintf((BaseSequentialStream *) &SD2, "%u baud\r\n", SERIAL_DEFAULT_BITRATE);
  chprintf((BaseSequentialStream *) &SD2, "%u bytes buffer\r\n", SERIAL_BUFFERS_SIZE);
  while (true) {

    // Loopback Serial Test
    uint8_t byte = chnGetTimeout(&SD2, TIME_INFINITE);
    chnPutTimeout(&SD2, byte, TIME_INFINITE);
  }

}
