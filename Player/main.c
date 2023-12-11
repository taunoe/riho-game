/*
Player Client
Tauno Erik
11.12.2023

- https://github.com/raspberrypi/pico-examples/tree/master/gpio/hello_gpio_irq

*/
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"

#define BTN_PIN     2  // GP2 - pin 4
#define BTN_LED_PIN 3  // GP3 - pin 5

static char event_str[128];

static const char *gpio_irq_str[] = {
    "LEVEL_LOW",  // 0x1
    "LEVEL_HIGH", // 0x2
    "EDGE_FALL",  // 0x4
    "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);

        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}


void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    gpio_event_string(event_str, events);
    printf("GPIO %d %s\n", gpio, event_str);
}


int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return 1;
    }

    // BTN - interrupt
    printf("Hello GPIO IRQ\n");
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // LED
    gpio_init(BTN_LED_PIN);
    gpio_set_dir(BTN_LED_PIN, GPIO_OUT);
    gpio_put(BTN_LED_PIN, 0);

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        printf("LED on\n");
        sleep_ms(500);

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        printf("LED off\n");
        sleep_ms(500);
    }
}
