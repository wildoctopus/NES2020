#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "sys/energest.h"

#define ENERGEST_CONF_ON 1

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
        // Check change_time method to undeerstand the time_on and time_off values
        switch (duty_cycle) {
            case 10:
                duty_cycle = 50;
                time_on = 0.01;
                time_off = 0.01;
                printf("switching to %d percent\n", duty_cycle);
                break;
            case 50:
                duty_cycle = 90;
                time_on = 0.018;
                time_off = 0.002;
                printf("switching to %d percent\n", duty_cycle);
                break;
            case 90:
                duty_cycle = 10;
                time_on = 0.002;
                time_off = 0.198;
                printf("switching to %d percent\n", duty_cycle);
                break;
        }
        // change_time(duty_cycle);
        
    }
    
    PROCESS_END();
}

// TODO: Implement this method properly, could not print float values here to check the correctness
void change_time(int brightness_level) {
    // Below formulas are  applied to calculate the time_on and time_off intervals
    // 1) Frequnecy is 50 hz
    // 2) Period ( T ) =  1 / f ;
    // 3) Period ( T ) = time_on + time_off;
    // 4) Duty_cycle = ( time_on / T ) * 100 ; => time_on = (Duty_cycle / 100 ) * T ;

    int frequency = 50;
    float period = 1 / frequency ;
    // printf("period %lf \n", period );

    time_on = (brightness_level / 100 ) * period;
    time_off = period - time_on;
}

PROCESS_THREAD(led_pt, ev, data) {
    static struct etimer timer;
    static int i = 0;
    PROCESS_BEGIN();
    etimer_set(&timer, CLOCK_SECOND);
   while(1) {
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
     // leds_toggle(LEDS_RED);
     if( i== 0 ){
         i = 1;
         leds_on(LEDS_RED);
         etimer_set(&timer, CLOCK_SECOND * time_on);
     }else if(i == 1){
        i = 0;
        leds_off(LEDS_RED);
        etimer_set(&timer, CLOCK_SECOND * time_off);
     }
   }

    PROCESS_END();
}

static unsigned long to_seconds(uint64_t time) {
  return (unsigned long)(time / RTIMER_SECOND);
}

PROCESS_THREAD(energy_pt, ev, data) {
    PROCESS_BEGIN();
    
    static struct etimer et;
    etimer_set(&et, CLOCK_SECOND);

    energest_init();

    while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    energest_flush();
    /* Real-time clock */
    printf("\nEnergest:\n");
    printf(" CPU %4lus\n", to_seconds(energest_type_time(ENERGEST_TYPE_CPU)));
    printf(" LED %4lus\n", to_seconds(energest_type_time(ENERGEST_TYPE_LED_RED)));
    etimer_reset(&et);
    printf("RTIMER_SECOND: %u\n", RTIMER_SECOND);

    // TODO: Implement here
    }

    PROCESS_END();
}
