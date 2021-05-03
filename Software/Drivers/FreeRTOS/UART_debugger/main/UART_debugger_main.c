/* UART_debugger test app

*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "UART_debugger.h"

static void debug_task(void *arg)
{
  init_debug();  //initialize the debugger
  while(1)
  {
    debug_out("Hello World\r\n",13);  //output Hello World a carriage return and a new line to the debugger
  }
}

void app_main(void)
{  
  xTaskCreate(debug_task, "uart_debugger_task", 1024, NULL, 10, NULL); //use debug_task as the function with a name "uart_debugger_task", a stack size of 1024, no parameters, a priority of 10, and no handle
}
