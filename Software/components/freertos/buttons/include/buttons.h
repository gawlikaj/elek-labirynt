#include "esp_err.h"

#ifndef BUTTONS_H
#define BUTTONS_H

typedef enum {BUTTON0=0,BUTTON1=1,BUTTON2=2,BUTTON3=3,BUTTON4=4} ButtonId;
typedef void(*ButtonHandler)(void*);

esp_err_t buttons_init(void);

uint8_t buttons_read(ButtonId selButtonId);

esp_err_t buttons_add_handler(ButtonId selButtonId, ButtonHandler buttonHandler, void* args);

esp_err_t buttons_remove_handler(ButtonId selButtonId);

void buttons_deinit(void);

void button0_handler(void* vpParam);
void button1_handler(void* vpParam);
void button2_handler(void* vpParam);
void button3_handler(void* vpParam);
void button4_handler(void* vpParam);

#endif
