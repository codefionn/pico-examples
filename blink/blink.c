/**
 * Copyright (c) 2021 Fionn Langhans
 * 
 * This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions *
 * The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * This notice may not be removed or altered from any source distribution.
 */
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

const uint LED_PINS_LENGTH = 2;
const uint LED_PINS[2] = {13, 25};

static void setLeds(bool);

int main() {
    bi_decl(bi_program_description("This is a test binary"));
    bi_decl(bi_1pin_with_name(25, "On-board LED"));
    bi_decl(bi_1pin_with_name(13, "GPIO Pin 13"));

    stdio_init_all();

    for (int i = 0; i < LED_PINS_LENGTH; ++i) {
        gpio_init(LED_PINS[i]);
        gpio_set_dir(LED_PINS[i], GPIO_OUT);
    }

    while (true) {
        setLeds(true);
        sleep_ms(250);
        setLeds(false);
        sleep_ms(250);
    }

    return 0;
}

/**
 * @param value
 */
static void setLeds(bool value) {
    for (uint i = 0; i < LED_PINS_LENGTH; ++i) {
        gpio_put(LED_PINS[i], value);
    }
}
