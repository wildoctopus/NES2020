#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"

PROCESS(led_pt, "Blink the LED");
PROCESS(btn_pt, "Handle button pressed");
PROCESS(energy_pt, "Energy estimation");

AUTOSTART_PROCESSES(&led_pt, &btn_pt, &energy_pt);

/* LED on/off durations */
static float time_on;
static float time_off;

PROCESS_THREAD(btn_pt, ev, data) {
    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(button_sensor);
    
    // TODO: Implement here
    
    PROCESS_END();
}

PROCESS_THREAD(led_pt, ev, data) {
    static struct etimer timer;
        
    PROCESS_BEGIN();
    
    // TODO: Implement here
    
    PROCESS_END();
}

PROCESS_THREAD(energy_pt, ev, data) {
    PROCESS_BEGIN();
    
    static struct etimer et;
    
    /* Real-time clock */
    printf("RTIMER_SECOND: %u\n", RTIMER_SECOND);
    
    // TODO: Implement here
    
    PROCESS_END();
}
