#include "contiki.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
static process_event_t event_data_ready;
PROCESS(main_thread, "Process that recieves sensor data");
PROCESS(second_thread, "Process that send sensor data");

AUTOSTART_PROCESSES(&main_thread, &second_thread);

PROCESS_THREAD(main_thread, ev, data){
	
	/*Main Pro: Recieve the data from the sensor and prints*/
	PROCESS_BEGIN();
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
        printf("Data Received: %d\r\n", *((int*)data));
	}
	PROCESS_END();

}
PROCESS_THREAD(second_thread, ev, data){
    /*Second_thread: Simulate the sensor and send data to the first p.*/
    PROCESS_BEGIN();
    static int random_data;
    event_data_ready = process_alloc_event();
    static struct etimer timer;
    etimer_set(&timer, CLOCK_SECOND * 10);
    
    while (1){
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        random_data = rand();
        printf("Data Send: %i\n",random_data);
		process_post(&main_thread, event_data_ready, &random_data);
        etimer_reset(&timer);

    }
    PROCESS_END();
}
