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
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "pico/binary_info.h"

/**
 * Connect speaker (+) to port 13
 */
const uint SPEAKER_PIN = 13;

static void on_pwm_wrap() {
    // clear current interrupt falg
    pwm_clear_irq(pwm_gpio_to_slice_num(SPEAKER_PIN));

    const uint16_t MIN_VALUE = 100;
    const uint16_t MAX_VALUE = 1000;
    static uint16_t value = MIN_VALUE;
    static bool going_up = true;
    if (going_up) {
        value++;

        if (value == MAX_VALUE) {
            going_up = false;
            gpio_put(25, true); // a visual treat
        }
    } else {
        value--;

        if (value == MIN_VALUE) {
            going_up = true;
            gpio_put(25, false); // a visual treat
        }
    }

    pwm_set_gpio_level(SPEAKER_PIN, value);
}

int main() {
    bi_decl(bi_program_description("Plays different tunes on a speaker"));
    bi_decl(bi_1pin_with_name(13, "GPIO Pin 13 for the speaker"));
    bi_decl(bi_1pin_with_name(25, "On-board LED"));

    stdio_init_all();

    // Turn-on on-board LED to show we're working
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, true);

    gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SPEAKER_PIN);
    
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 2.f); // play with the clock!
    pwm_init(slice_num, &config, true);

    while (true) {
        tight_loop_contents();
    }

    return 0;
}