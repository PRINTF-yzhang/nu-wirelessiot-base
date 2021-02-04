// BLE Service example app
//
// Creates a BLE environmental sensing service

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    // c0:98:e5:4e:xx:xx
    .platform_id = 0x4E, // used as 4th octect in device BLE address
    .device_id = 0xAABB,
    .adv_name = "CS397/497", // used in advertisements if there is room
    .adv_interval = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t environmental_sensing_service = {{.uuid128 = {0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80,
                                                                          0x00, 0x10, 0x00, 0x00, 0x1A, 0x18, 0x00, 0x00}}};

// static simple_ble_char_t led_state_char = {.uuid16 = 0x1089};
// static bool led_state = false;

static simple_ble_char_t Temperature_char = {.uuid16 = 0x2A6E};
static int16_t Temperature = -20;

static simple_ble_char_t Humidity_char = {.uuid16 = 0x2A6F};
static uint16_t Humidity = 16;

static simple_ble_char_t True_Wind_Speed_char = {.uuid16 = 0x2A70};
static uint16_t True_Wind_Speed = 89;

//a wind coming from the west is given as 270 degrees.

static simple_ble_char_t True_Wind_Direction_char = {.uuid16 = 0x2A71};
static uint16_t True_Wind_Direction = 270;

//The Irradiance characteristic is used to represent the irradiance, the radiant flux received by a surface per unit area.
static simple_ble_char_t Irradiance_char = {.uuid16 = 0x2A77};
static uint16_t Irradiance = 20;

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t *simple_ble_app;

void ble_evt_write(ble_evt_t const *p_ble_evt)
{
  printf("Got write to a characteristic!\n");
}

int main(void)
{

  printf("Board started. Initializing BLE: \n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&environmental_sensing_service);
  //copied from service example
  // simple_ble_add_characteristic(1, 1, 0, 0,
  //   sizeof(led_state), (uint8_t*)&led_state,
  //   &led_service, &led_state_char);

  simple_ble_add_characteristic(1, 1, 0, 0,
                                sizeof(Temperature), (uint8_t *)&Temperature,
                                &environmental_sensing_service, &Temperature_char);

  simple_ble_add_characteristic(1, 1, 0, 0,
                                sizeof(Humidity), (uint8_t *)&Humidity,
                                &environmental_sensing_service, &Humidity_char);
  simple_ble_add_characteristic(1, 1, 0, 0,
                                sizeof(True_Wind_Speed), (uint8_t *)&True_Wind_Speed,
                                &environmental_sensing_service, &True_Wind_Speed_char);
  simple_ble_add_characteristic(1, 1, 0, 0,
                                sizeof(True_Wind_Direction), (uint8_t *)&True_Wind_Direction,
                                &environmental_sensing_service, &True_Wind_Direction_char);
  simple_ble_add_characteristic(1, 1, 0, 0,
                                sizeof(Irradiance), (uint8_t *)&Irradiance,
                                &environmental_sensing_service, &Irradiance_char);

  // Start Advertising
  simple_ble_adv_only_name();

  while (1)
  {
    power_manage();
  }
}
