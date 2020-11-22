#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"

PROCESS(protothread1, "Protothread Process 1");
PROCESS(protothread2, "Protothread Process 2");
PROCESS(protothread3, "Protothread Process 3");
AUTOSTART_PROCESSES(&protothread1, &protothread2);

PROCESS_THREAD(protothread1, ev, data) {
	PROCESS_BEGIN();

	printf("Protothread 1!\n");

	SENSORS_ACTIVATE(button_sensor);
	// SENSORS_ACTIVATE(light_sensor);

	// while (1) {
	// 	PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
	// 	printf("Light sensor reads value %d\n", light_sensor.value(0));
	// 	leds_toggle(LEDS_ALL);
	// }
	PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
	process_start(&protothread, NULL);

	PROCESS_END();
}

PROCESS_THREAD(protothread2, ev, data) {
	PROCESS_BEGIN();

	while (1) {
		printf("Hey, I'm the protothread 2. I executed automatically\n");
		PROCESS_PAUSE();
	}

	PROCESS_END();
}

PROCESS_THREAD(protothread3, ev, data) {
	PROCESS_BEGIN();

	while (1) {
		printf("Hey, I'm the protothread 3. I executed after pressing the button\n");
		PROCESS_PAUSE();
	}

	PROCESS_END();
}
