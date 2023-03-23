/**
 * @file ds18x20.h
 * @defgroup ds18x20 ds18x20
 * @{
 *
 * ESP-IDF driver for the DS18S20/DS18B20 one-wire temperature sensor ICs
 *
 * Ported from esp-open-rtos
 *
 * Copyright (c) 2016 Grzegorz Hetman <ghetman@gmail.com>\n
 * Copyright (c) 2016 Alex Stewart <foogod@gmail.com>\n
 * Copyright (c) 2018 Ruslan V. Uss <unclerus@gmail.com>
 *
 * BSD Licensed as described in the file LICENSE
 */
#include <stdint.h>

#include "CppUTestExt/MockSupport.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int esp_err_t;

/* Definitions for error constants. */
#define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM              0x101   /*!< Out of memory */
#define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define ESP_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define ESP_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define ESP_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT             0x107   /*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define ESP_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid */
#define ESP_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
#define ESP_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */
#define ESP_ERR_NOT_FINISHED        0x10C   /*!< There are items remained to retrieve */


#define ESP_ERR_WIFI_BASE           0x3000  /*!< Starting number of WiFi error codes */
#define ESP_ERR_MESH_BASE           0x4000  /*!< Starting number of MESH error codes */
#define ESP_ERR_FLASH_BASE          0x6000  /*!< Starting number of flash error codes */
#define ESP_ERR_HW_CRYPTO_BASE      0xc000  /*!< Starting number of HW cryptography module error codes */

typedef enum {
    GPIO_NUM_NC = -1,    /*!< Use to signal not connected to S/W */
    GPIO_NUM_0 = 0,     /*!< GPIO0, input and output */
    GPIO_NUM_1 = 1,     /*!< GPIO1, input and output */
    GPIO_NUM_2 = 2,     /*!< GPIO2, input and output */
    GPIO_NUM_3 = 3,     /*!< GPIO3, input and output */
    GPIO_NUM_4 = 4,     /*!< GPIO4, input and output */
    GPIO_NUM_5 = 5,     /*!< GPIO5, input and output */
    GPIO_NUM_6 = 6,     /*!< GPIO6, input and output */
    GPIO_NUM_7 = 7,     /*!< GPIO7, input and output */
    GPIO_NUM_8 = 8,     /*!< GPIO8, input and output */
    GPIO_NUM_9 = 9,     /*!< GPIO9, input and output */
    GPIO_NUM_10 = 10,   /*!< GPIO10, input and output */
    GPIO_NUM_11 = 11,   /*!< GPIO11, input and output */
    GPIO_NUM_12 = 12,   /*!< GPIO12, input and output */
    GPIO_NUM_13 = 13,   /*!< GPIO13, input and output */
    GPIO_NUM_14 = 14,   /*!< GPIO14, input and output */
    GPIO_NUM_15 = 15,   /*!< GPIO15, input and output */
    GPIO_NUM_16 = 16,   /*!< GPIO16, input and output */
    GPIO_NUM_17 = 17,   /*!< GPIO17, input and output */
    GPIO_NUM_18 = 18,   /*!< GPIO18, input and output */
    GPIO_NUM_19 = 19,   /*!< GPIO19, input and output */
    GPIO_NUM_20 = 20,   /*!< GPIO20, input and output */
    GPIO_NUM_21 = 21,   /*!< GPIO21, input and output */
    GPIO_NUM_22 = 22,   /*!< GPIO22, input and output */
    GPIO_NUM_23 = 23,   /*!< GPIO23, input and output */
    GPIO_NUM_25 = 25,   /*!< GPIO25, input and output */
    GPIO_NUM_26 = 26,   /*!< GPIO26, input and output */
    GPIO_NUM_27 = 27,   /*!< GPIO27, input and output */
    GPIO_NUM_28 = 28,   /*!< GPIO28, input and output */
    GPIO_NUM_29 = 29,   /*!< GPIO29, input and output */
    GPIO_NUM_30 = 30,   /*!< GPIO30, input and output */
    GPIO_NUM_31 = 31,   /*!< GPIO31, input and output */
    GPIO_NUM_32 = 32,   /*!< GPIO32, input and output */
    GPIO_NUM_33 = 33,   /*!< GPIO33, input and output */
    GPIO_NUM_34 = 34,   /*!< GPIO34, input mode only */
    GPIO_NUM_35 = 35,   /*!< GPIO35, input mode only */
    GPIO_NUM_36 = 36,   /*!< GPIO36, input mode only */
    GPIO_NUM_37 = 37,   /*!< GPIO37, input mode only */
    GPIO_NUM_38 = 38,   /*!< GPIO38, input mode only */
    GPIO_NUM_39 = 39,   /*!< GPIO39, input mode only */
    GPIO_NUM_MAX,
} gpio_num_t;

typedef uint64_t onewire_addr_t;
typedef onewire_addr_t ds18x20_addr_t;

/** An address value which can be used to indicate "any device on the bus" */
#define ONEWIRE_NONE ((onewire_addr_t)(0xffffffffffffffffLL))
#define DS18X20_ANY ONEWIRE_NONE

/** Family ID (lower address byte) of DS18B20 sensors */
#define DS18B20_FAMILY_ID 0x28

/** Family ID (lower address byte) of DS18S20 sensors */
#define DS18S20_FAMILY_ID 0x10

/**
 * @brief Find the addresses of all ds18x20 devices on the bus.
 *
 * Scans the bus for all devices and places their addresses in the supplied
 * array. If there are more than `addr_count` devices on the bus, only the
 * first `addr_count` are recorded.
 *
 * @param pin         The GPIO pin connected to the ds18x20 bus
 * @param addr_list   A pointer to an array of ::ds18x20_addr_t values.
 *                    This will be populated with the addresses of the found
 *                    devices.
 * @param addr_count  Number of slots in the `addr_list` array. At most this
 *                    many addresses will be returned.
 * @param found       The number of devices found. Note that this may be less
 *                    than, equal to, or more than `addr_count`, depending on
 *                    how many ds18x20 devices are attached to the bus.
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18x20_scan_devices(gpio_num_t pin, ds18x20_addr_t *addr_list, size_t addr_count, size_t *found) {
    mock().actualCall("ds18x20_scan_devices")
          .withUnsignedIntParameter("pin", (uint8_t)pin)
          .withOutputParameter("addr_list", (void*)addr_list)
          .withUnsignedLongIntParameter("addr_count", addr_count)
          .withOutputParameter("found", found);
    return mock().returnUnsignedIntValueOrDefault(ESP_OK);
}

/**
 * @brief Tell one or more sensors to perform a temperature measurement and
 * conversion (CONVERT_T) operation.
 *
 * This operation can take up to 750ms to complete.
 *
 * If `wait=true`, this routine will automatically drive the pin high for the
 * necessary 750ms after issuing the command to ensure parasitically-powered
 * devices have enough power to perform the conversion operation (for
 * non-parasitically-powered devices, this is not necessary but does not
 * hurt). If `wait=false`, this routine will drive the pin high, but will
 * then return immediately. It is up to the caller to wait the requisite time
 * and then depower the bus using onewire_depower() or by issuing another
 * command once conversion is done.
 *
 * @param pin   The GPIO pin connected to the ds18x20 device
 * @param addr  The 64-bit address of the device on the bus. This can be set
 *              to ::DS18X20_ANY to send the command to all devices on the bus
 *              at the same time.
 * @param wait  Whether to wait for the necessary 750ms for the ds18x20 to
 *              finish performing the conversion before returning to the
 *              caller (You will normally want to do this).
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18x20_measure(gpio_num_t pin, ds18x20_addr_t addr, bool wait) {
    mock().actualCall("ds18x20_measure")
          .withUnsignedIntParameter("pin", (uint8_t)pin)
          .withUnsignedLongLongIntParameter("addr", addr)
          .withBoolParameter("wait", wait);
    return mock().returnIntValueOrDefault(ESP_OK);
}

/**
 * @brief Read the value from the last CONVERT_T operation.
 *
 * This should be called after ds18x20_measure() to fetch the result of the
 * temperature measurement.
 *
 * @param pin         The GPIO pin connected to the ds18x20 device
 * @param addr        The 64-bit address of the device to read. This can be set
 *                    to ::DS18X20_ANY to read any device on the bus (but note
 *                    that this will only work if there is exactly one device
 *                    connected, or they will corrupt each others' transmissions)
 * @param temperature The temperature in degrees Celsius
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18x20_read_temperature(gpio_num_t pin, ds18x20_addr_t addr, float *temperature) {
    mock().actualCall("ds18x20_read_temperature")
          .withUnsignedIntParameter("pin", (uint8_t)pin)
          .withUnsignedLongLongIntParameter("addr", addr)
          .withOutputParameter("temperature", temperature);
    return mock().returnIntValueOrDefault(ESP_OK);
}

/**
 * @brief Read the value from the last CONVERT_T operation (ds18b20 version).
 *
 * This should be called after ds18x20_measure() to fetch the result of the
 * temperature measurement.
 *
 * @param pin         The GPIO pin connected to the ds18x20 device
 * @param addr        The 64-bit address of the device to read. This can be set
 *                    to ::DS18X20_ANY to read any device on the bus (but note
 *                    that this will only work if there is exactly one device
 *                    connected, or they will corrupt each others' transmissions)
 * @param temperature The temperature in degrees Celsius
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18b20_read_temperature(gpio_num_t pin, ds18x20_addr_t addr, float *temperature) {
    mock().actualCall("ds18b20_read_temperature")
          .withUnsignedIntParameter("pin", (uint8_t)pin)
          .withUnsignedLongLongIntParameter("addr", addr)
          .withOutputParameter("temperature", (void*)temperature);
    return mock().returnIntValueOrDefault(ESP_OK);
}

/**
 * @brief Read the scratchpad data for a particular ds18x20 device.
 *
 * This is not generally necessary to do directly. It is done automatically
 * as part of ds18x20_read_temperature().
 *
 * @param pin     The GPIO pin connected to the ds18x20 device
 * @param addr    The 64-bit address of the device to read. This can be set
 *                to ::DS18X20_ANY to read any device on the bus (but note
 *                that this will only work if there is exactly one device
 *                connected, or they will corrupt each others' transmissions)
 * @param buffer  An 8-byte buffer to hold the read data.
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18x20_read_scratchpad(gpio_num_t pin, ds18x20_addr_t addr, uint8_t *buffer) {
    mock().actualCall("ds18x20_read_scratchpad")
          .withUnsignedIntParameter("pin", (uint8_t)pin)
          .withUnsignedLongLongIntParameter("addr", addr)
          .withOutputParameter("buffer", buffer);
    return mock().returnIntValueOrDefault(ESP_OK);
}

/**
 * @brief Write the scratchpad data for a particular ds18x20 device.
 *
 * @param pin     The GPIO pin connected to the ds18x20 device
 * @param addr    The 64-bit address of the device to write. This can be set
 *                to ::DS18X20_ANY to read any device on the bus (but note
 *                that this will only work if there is exactly one device
 *                connected, or they will corrupt each others' transmissions)
 * @param buffer  An 3-byte buffer to hold the data to write
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18x20_write_scratchpad(gpio_num_t pin, ds18x20_addr_t addr, uint8_t *buffer) {
    mock().actualCall("ds18x20_write_scratchpad")
          .withUnsignedIntParameter("pin", (uint8_t)pin)
          .withUnsignedLongLongIntParameter("addr", addr)
          .withMemoryBufferParameter("buffer", buffer, 3*sizeof(uint8_t));
    return mock().returnIntValueOrDefault(ESP_OK);
}

/**
 * @brief Issue the copy scratchpad command, copying current scratchpad to
 *        EEPROM.
 *
 * @param pin     The GPIO pin connected to the ds18x20 device
 * @param addr    The 64-bit address of the device to command. This can be set
 *                to ::DS18X20_ANY to read any device on the bus (but note
 *                that this will only work if there is exactly one device
 *                connected, or they will corrupt each others' transmissions)
 *
 * @returns `ESP_OK` if the command was successfully issued
 */
inline esp_err_t ds18x20_copy_scratchpad(gpio_num_t pin, ds18x20_addr_t addr) {
    mock().actualCall("ds18x20_copy_scratchpad")
          .withUnsignedIntParameter("pin", pin)
          .withUnsignedLongLongIntParameter("addr", addr);
    return mock().returnUnsignedIntValueOrDefault(ESP_OK);
}


#ifdef __cplusplus
}
#endif

/**@}*/
