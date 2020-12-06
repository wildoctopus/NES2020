#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/etimer.h"

PROCESS(led_pt, "Blink the LED");
PROCESS(btn_pt, "Handle button pressed");
PROCESS(energy_pt, "Energy estimation");

AUTOSTART_PROCESSES(&led_pt, &btn_pt, &energy_pt);

/* LED on/off durations */
static float time_on = 0.002;
static float time_off = 0.198;
static int duty_cycle = 10;

PROCESS_THREAD(btn_pt, ev, data) {
    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(button_sensor);
    
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
        leds_toggle(LEDS_GREEN);

        switch (duty_cycle) {
            case 10:
                duty_cycle = 50;
                time_on = 0.002;
                time_off = 0.198;
                break;
            case 50:
                duty_cycle = 90;
                time_on = 0.01;
                time_off = 0.01;
                break;
            case 90:
                duty_cycle = 10;
                time_on = 0.018;
                time_off = 0.002;
                break;
        }
        
    }
    
    PROCESS_END();
}

PROCESS_THREAD(led_pt, ev, data) {
    static struct etimer timer;
    static int i = 0;
    PROCESS_BEGIN();
    etimer_set(&timer, CLOCK_SECOND);
   while(1) {
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
     leds_toggle(LEDS_RED);
     if( i== 0 ){
         i = 1;
         etimer_set(&timer, CLOCK_SECOND * time_on);
     }else if(i == 1){
        i = 0;
        etimer_set(&timer, CLOCK_SECOND * time_off);
     }
   }

    PROCESS_END();
}

PROCESS_THREAD(energy_pt, ev, data) {
    PROCESS_BEGIN();
    
    // static struct etimer et;
    
    /* Real-time clock */
    printf("RTIMER_SECOND: %u\n", RTIMER_SECOND);
    
    // TODO: Implement here
    
    PROCESS_END();
}
