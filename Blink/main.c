
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

const uint LED_PIN = 3;


int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        gpio_put(LED_PIN, 1);
        printf("LED on\n");
        sleep_ms(500);

        //cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        gpio_put(LED_PIN, 0);
        printf("LED off\n");
        sleep_ms(500);
    }
}
