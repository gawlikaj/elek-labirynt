#include "driver/uart.h"
#include "driver/gpio.h"

void init_debug(void);
void debug_out(char* data, size_t len);
