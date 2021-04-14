/* UART_debugger test app

*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "UART_debugger.h"

static void debug_task(void *arg)
{
  init_debug();
  while(1)
  {
    debug_out("Hello World\r\n",13);
  }
}

void app_main(void)
{
  xTaskCreate(debug_task, "uart_debugger_task", 1024, NULL, 10, NULL);
}
