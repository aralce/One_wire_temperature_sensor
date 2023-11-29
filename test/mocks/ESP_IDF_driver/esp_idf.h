#include "CppUTestExt/MockSupport.h"

/**
 * @brief Get time in microseconds since boot
 * @return number of microseconds since underlying timer has been started
 */
int64_t esp_timer_get_time(void)
{
    mock().actualCall("esp_timer_get_time");
    return mock().returnLongLongIntValueOrDefault(0);
}