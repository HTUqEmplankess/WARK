#include "command_responder.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "driver/uart.h"
#include "string.h"

// UART configuration
#define UART_PORT_NUM      UART_NUM_1
#define UART_BAUD_RATE     115200
#define UART_TX_PIN        19  // Set the TX pin number
#define UART_RX_PIN        18  // Set the RX pin number
#define UART_BUFFER_SIZE   1024

bool is_uart_initialized = false;

// UART Initialization
void uart_init() {
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Configure UART parameters
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Install UART driver
    uart_driver_install(UART_PORT_NUM, UART_BUFFER_SIZE, 0, 0, NULL, 0);

    is_uart_initialized = true;  // Mark UART as initialized
}

// RespondToCommand Implementation
void RespondToCommand(int32_t current_time, const char* found_command,
                      int16_t score, bool is_new_command) {
    if (is_new_command) {
        if (!is_uart_initialized) {
            uart_init();  // Initialize UART if not already done
        }

        // Log the detected command
        MicroPrintf("Heard %s (%d) @%dms", found_command, score, current_time);

        // Create JSON string
        char json[128];
        snprintf(json, sizeof(json),
                 "{\"command\": \"%s\", \"score\": %d, \"time\": %ld}\n",  // Add newline for easier parsing
                 found_command, score, current_time);

        // Send JSON over UART
        int bytes_written = uart_write_bytes(UART_PORT_NUM, json, strlen(json));
        if (bytes_written < 0) {
            MicroPrintf("Error: UART transmission failed");
        }
    }
}
