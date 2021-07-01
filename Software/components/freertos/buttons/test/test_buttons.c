#include "unity.h"
#include "test_utils.h"             // unity_send_signal
#include "esp_log.h"
#include "buttons.h"
#include "driver/timer.h"           //hardware timer

#define NUM_BUTTONS 5
const char *tag = "Buttons Test";
static uint8_t countRed, countYellow, countGreen, countBlue, countBlack;

#define TIMEOUT 5
#define DIVIDER 2

inline void timeout_init()
{
  timer_config_t timeoutConfig = {
    .divider = DIVIDER,
    .counter_dir = TIMER_COUNT_DOWN,
    .counter_en = TIMER_PAUSE,
    .alarm_en = TIMER_ALARM_DIS,
    .auto_reload = TIMER_AUTORELOAD_DIS,
  }; // default clock source is APB
  
  timer_init(TIMER_GROUP_1, TIMER_1, &timeoutConfig);
  timer_set_counter_value(TIMER_GROUP_1,TIMER_1,TIMER_BASE_CLK*TIMEOUT/DIVIDER);
  timer_start(TIMER_GROUP_1,TIMER_1);
}

inline bool is_timedout()
{
  uint64_t val;
  timer_get_counter_value(TIMER_GROUP_1,TIMER_1,&val);  
  return ((int64_t)val <= 0);
}

inline void timeout_deinit()
{
  timer_pause(TIMER_GROUP_1,TIMER_1);
  timer_deinit(TIMER_GROUP_1,TIMER_1);
}

IRAM_ATTR void red_button_callback(void* vpParam)
{
  countRed++;
}

IRAM_ATTR void yellow_button_callback(void* vpParam)
{
  countYellow++;
}

IRAM_ATTR void green_button_callback(void* vpParam)
{
  countGreen++;
}

IRAM_ATTR void blue_button_callback(void* vpParam)
{
  countBlue++;
}

IRAM_ATTR void black_button_callback(void* vpParam)
{
  countBlack++;
}

TEST_CASE("test initialization", "[button]")
{
  TEST_ASSERT(buttons_init() == ESP_OK);
  buttons_deinit();
}

TEST_CASE("test Button0 (Red) VIA button_read", "[button]")
{
  uint8_t buttonPressed = 0; //state of button is not pressed

  buttons_init();     //initialize buttons
  timeout_init();
  
  while((buttonPressed == 0) && (!is_timedout()))
  {
    buttonPressed = buttons_read(0);
  }
  
  timeout_deinit();
  buttons_deinit();   //free resources from buttons
  
  TEST_ASSERT_MESSAGE(buttonPressed == 1,"Button press not detected");
}

TEST_CASE("test Button1 (Yellow) VIA button_read", "[button]")
{
  uint8_t buttonPressed = 0; //state of button is not pressed

  buttons_init();     //initialize buttons
  timeout_init();
  
  while((buttonPressed == 0) && (!is_timedout()))
  {
    buttonPressed = buttons_read(1);
  }
  
  timeout_deinit();
  buttons_deinit();   //free resources from buttons
  
  TEST_ASSERT_MESSAGE(buttonPressed == 1,"Button press not detected");
}

TEST_CASE("test Button2 (Green) VIA button_read", "[button]")
{
  uint8_t buttonPressed = 0; //state of button is not pressed
  
  buttons_init();     //initialize buttons
  timeout_init();
  
  while((buttonPressed == 0) && (!is_timedout()))
  {
    buttonPressed = buttons_read(2);
  }
  
  timeout_deinit();
  buttons_deinit();   //free resources from buttons
  
  TEST_ASSERT_MESSAGE(buttonPressed == 1,"Button press not detected");
}

TEST_CASE("test Button3 (Blue) VIA button_read", "[button]")
{
  uint8_t buttonPressed = 0;

  buttons_init();     //initialize buttons
  timeout_init();
  
  while((buttonPressed == 0) && (!is_timedout()))
  {
    buttonPressed = buttons_read(3);
  }
  timeout_deinit();
  buttons_deinit();   //free resources from buttons
  
  TEST_ASSERT_MESSAGE(buttonPressed == 1,"Button press not detected");
}

TEST_CASE("test Button4 (Black) VIA button_read", "[button]")
{
  uint8_t buttonPressed = 0;

  buttons_init();     //initialize buttons
  timeout_init();
  
  while((buttonPressed == 0) && (!is_timedout()))
  {
    buttonPressed = buttons_read(4);
  }
  
  timeout_deinit();
  buttons_deinit();   //free resources from buttons
  
  TEST_ASSERT_MESSAGE(buttonPressed == 1,"Button press not detected");
}

TEST_CASE("test Button0 (Red) VIA interrupt", "[button]")
{
  countRed = 0;
  buttons_init();
  buttons_add_handler(0,red_button_callback,NULL);
  timeout_init();
  
  while(!is_timedout()) {};  //wait for timeout
  timeout_deinit();
  buttons_remove_handler(0);
  buttons_deinit();
  
  TEST_ASSERT_MESSAGE(countRed > 0, "Interrupt did not respond");
  TEST_ASSERT_MESSAGE(countRed == 1, "Interrupt responded excessively (check debounce)");
}

TEST_CASE("test Button1 (Yellow) VIA interrupt", "[button]")
{
  countYellow = 0;
  buttons_init();
  buttons_add_handler(1,yellow_button_callback,NULL);
  timeout_init();
  
  while(!is_timedout()) {};  //wait for timeout
  timeout_deinit();
  buttons_remove_handler(1);
  buttons_deinit();
  
  TEST_ASSERT_MESSAGE(countYellow > 0, "Interrupt did not respond");
  TEST_ASSERT_MESSAGE(countYellow == 1, "Interrupt responded excessively (check debounce)");
}

TEST_CASE("test Button2 (Green) VIA interrupt", "[button]")
{
  countGreen = 0;
  buttons_init();
  buttons_add_handler(2,green_button_callback,NULL);
  timeout_init();
  
  while(!is_timedout()) {};  //wait for timeout
  timeout_deinit();
  buttons_remove_handler(2);
  buttons_deinit();
  
  TEST_ASSERT_MESSAGE(countGreen > 0, "Interrupt did not respond");
  TEST_ASSERT_MESSAGE(countGreen == 1, "Interrupt responded excessively (check debounce)");
}

TEST_CASE("test Button3 (Blue) VIA interrupt", "[button]")
{
  countBlue = 0;
  buttons_init();
  buttons_add_handler(3,blue_button_callback,NULL);
  timeout_init();
  
  while(!is_timedout()) {};  //wait for timeout
  timeout_deinit();
  buttons_remove_handler(3);
  buttons_deinit();
  
  TEST_ASSERT_MESSAGE(countBlue > 0, "Interrupt did not respond");
  TEST_ASSERT_MESSAGE(countBlue == 1, "Interrupt responded excessively (check debounce)");
}

TEST_CASE("test Button4 (Black) VIA interrupt", "[button]")
{
  countBlack = 0;
  buttons_init();
  buttons_add_handler(4,black_button_callback,NULL);
  timeout_init();
  
  while(!is_timedout()) {};  //wait for timeout
  timeout_deinit();
  buttons_remove_handler(1);
  buttons_deinit();
  
  TEST_ASSERT_MESSAGE(countBlack > 0, "Interrupt did not respond");
  TEST_ASSERT_MESSAGE(countBlack == 1, "Interrupt responded excessively (check debounce)");
}
