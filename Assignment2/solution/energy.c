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
// Intial values for 10% duty cycle
static float time_on = 0.002;
static float time_off = 0.198;
static int duty_cycle = 10;

PROCESS_THREAD(btn_pt, ev, data) {
    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(button_sensor);
    
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
        duty_cycle = (duty_cycle == 90)? 10: (duty_cycle + 40);
        change_time();
    }
    
    PROCESS_END();
}

void change_time() {
    // Below formulas are  applied to calculate the time_on and time_off intervals
    // 1) Frequnecy is 50 hz
    // 2) Period ( T ) =  1 / f ;
    // 3) Period ( T ) = time_on + time_off;
    // 4) Duty_cycle = ( time_on / T ) * 100 ; => time_on = (Duty_cycle / 100 ) * T ;

    float frequency = 50;
    float period = 1 / frequency ;

    time_on = (float) (duty_cycle* period) / 100;
    time_off = (float)period - time_on;
}

PROCESS_THREAD(led_pt, ev, data) {
    static struct etimer timer;
    static int i = 0;
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND);

    printf("starting with Initial duty cycle %d\n", duty_cycle);

   while(1) {
     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
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

PROCESS_THREAD(energy_pt, ev, data) {
    PROCESS_BEGIN();
  
    static unsigned long old_cpu_active_time;
    static unsigned long old_led_active_time;

    static struct etimer et;

    energest_init();

    // Initial active time of each component in ticks
    old_cpu_active_time = energest_type_time(ENERGEST_TYPE_CPU);
    old_led_active_time = energest_type_time(ENERGEST_TYPE_LED_RED);

    // Calculate enrgy consumption for every second
    etimer_set(&et, CLOCK_SECOND);
    /* Real-time clock */
    printf("RTIMER_SECOND: %u\n", RTIMER_SECOND);

    while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    energest_flush();

    //Total active time of each component untill current second in ticks
    unsigned long new_cpu_active_time = energest_type_time(ENERGEST_TYPE_CPU);
    unsigned long new_led_active_time = energest_type_time(ENERGEST_TYPE_LED_RED);

    //Active time of each component in last second in ms
    unsigned long cpu_active_time = (new_cpu_active_time - old_cpu_active_time) * 1000 / RTIMER_SECOND;
    unsigned long led_active_time = (old_led_active_time - new_led_active_time) * 1000 / RTIMER_SECOND;

    // Swap the values to calculate active time and energy consumption for next second 
    old_cpu_active_time = new_cpu_active_time;
    old_led_active_time = new_cpu_active_time;

    printf("Active Time - CPU: %lu ms LED: %lu ms \n", cpu_active_time, led_active_time);

    // Energy consumption E = voltage * current * time 
    // https://stackoverflow.com/questions/45644277/how-to-calculate-total-energy-consumption-using-cooja
    // Voltage and current values are from datasheet of the microcontroller
    unsigned long energy_consumption_cpu = 3 * 0.05 * cpu_active_time;
    unsigned long energy_consumption_led = 1.8 * 20 * led_active_time / 1000;

    printf("Energy Consumed - CPU: %lu mJ LED: %lu mJ \n", energy_consumption_cpu, energy_consumption_led);
    
    etimer_reset(&et);
    }

    PROCESS_END();
}
