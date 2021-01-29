// BLE Scanning app
//
// Receives BLE advertisements

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "simple_ble.h"

#include "nrf52840dk.h"

// BLE configuration
// This is mostly irrelevant since we are scanning only
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:4e:00:02
        .platform_id       = 0x4E,    // used as 4th octet in device BLE address
        .device_id         = 0x0002,  // used as the 5th and 6th octet in the device BLE address, you will need to change this for each device you have
        .adv_name          = "CS397/497", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t *simple_ble_app;


// Callback handler for advertisement reception
void ble_evt_adv_report(ble_evt_t const *p_ble_evt) {

    // extract the fields we care about
    ble_gap_evt_adv_report_t const *adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t const *ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
    uint8_t *adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
    /*
     * 0x01	«Flags»
     * 0x09	«Complete Local Name»
     * 0x2B	«Mesh Beacon»
     * 0x2D	«Broadcast_Code»
     * 0xFF	«Manufacturer Specific Data»
     */
    uint16_t adv_len = adv_report->data.len; // length of advertisement payload data
    //c0:98:e5:4e:03:97
    if (ble_addr[0] == 0x03 &&
        ble_addr[1] == 0x97 &&
        ble_addr[2] == 0x4E &&
        ble_addr[3] == 0xE5 &&
        ble_addr[4] == 0x98 &&
        ble_addr[5] == 0xC0) {
        printf("%s\r",(&adv_buf[7]));
//        printf("Received an advertisement!\n");
//        printf("BLE address: ");
//        for (int i = 0; i <= 5; i++) {
//            printf("%x", ble_addr[i]);
//        }

//        printf("\nadvertisement payload data length: %d\n", adv_len);
//        for (int i = 0; i < adv_len; i++) {
//            printf("\n%c", adv_buf[i]);
//        }
//        printf("\n%X Flags\n",adv_buf[1]);
//        printf("\n%c Name\n",adv_buf[4]);

    }

}


int main(void) {

    // Setup BLE
    // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
    simple_ble_app = simple_ble_init(&ble_config);
    advertising_stop();

    // Start scanning
    scanning_start();

    // go into low power mode
    while (1) {
        power_manage();
    }
}



