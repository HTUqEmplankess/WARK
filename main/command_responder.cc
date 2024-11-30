/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "command_responder.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "driver/uart.h"
#include "string.h"

#define UART_PORT_NUM      UART_NUM_1
#define UART_BAUD_RATE     9600
#define UART_TX_PIN        17  // Set the TX pin number
#define UART_RX_PIN        16  // Set the RX pin number
#define UART_BUFFER_SIZE   1024

int check = 0;

void uart_init()
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // Install UART driver
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT_NUM, UART_BUFFER_SIZE, 0, 0, NULL, 0);
    check = 1;
}


// Append JSON functionality to the existing RespondToCommand
void RespondToCommand(int32_t current_time, const char* found_command,
                      int16_t score, bool is_new_command) {
  if (is_new_command) {
    if(check==0){
      uart_init();
    }
    
    // Existing functionality
    MicroPrintf("Heard %s (%d) @%dms", found_command, score, current_time);

    // JSON creation
    char json[128];
    snprintf(json, sizeof(json),
             "{\"command\": \"%s\", \"score\": %d, \"time\": %ld}",
             found_command, score, current_time);

    // Send JSON over UART
    uart_write_bytes(UART_PORT_NUM, json, strlen(json));
  }
}
