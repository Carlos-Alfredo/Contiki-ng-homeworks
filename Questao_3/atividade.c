#include<stdlib.h>
#include<stdio.h>
#include "contiki.h"
#include <unistd.h>


int even_or_odd(char N)
{
    // check if the last digit
    // is either '0', '2', '4',
    // '6', '8', 'A'(=10),
    // 'C'(=12) or 'E'(=14)
    if(N%2==0){
        return 1;
    }
    else{
        return 0;
    }
}

static void ctimer_event_callback2(void *ptr){
	printf("CALLBACK Values: %s.\r\n", ret);
	//check_update_image();
	ctimer_reset((struct ctimer*) ptr);
	indice2++;
}

static void ctimer_event_callback3(void *ptr){
	printf("CALLBACK Values: %s.\r\n", ret);
	//check_update_image();
	ctimer_reset((struct ctimer*) ptr);
	indice3++;
}

static char ret[100];
static process_event_t event_data_ready_2;
static process_event_t event_data_ready_3;
static process_event_t event_data_ready_4;
static process_event_t event_data_ready_5;
static int value;
static int count=0;
static int indice2=0;
static int indice3=0;

PROCESS(main_thread, "Process that recieves data from buffer");
PROCESS(second_thread, "Process that sums two values");
PROCESS(third_thread, "Process that multiply two values");

AUTOSTART_PROCESSES(&main_thread,&second_thread,&third_thread);

PROCESS_THREAD(main_thread, ev, data){
	

	PROCESS_BEGIN();
    static char b[100];
    b[0]=0x50;
	event_data_ready_2 = process_alloc_event();
    event_data_ready_3 = process_alloc_event();
    static struct etimer timer;
    static int count_2 =0;
    etimer_set(&timer, CLOCK_SECOND * 2);
    while(1){
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        value =even_or_odd(b[count_2]);
        if(value==1){
		    process_post(&second_thread, event_data_ready_2,
                        &b[count_2]);
            PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready_4);
        }
        else{
            process_post(&third_thread, event_data_ready_3, 
                        &b[count_2]);
            PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready_5);
        }
        printf("Process Result: %04x\n",ret[count-1]);
        etimer_reset(&timer);
        count_2 = count_2 +1;
        b[count_2]= ret[count-1];
	}
	PROCESS_END();

}

PROCESS_THREAD(second_thread,ev,data){
    PROCESS_BEGIN();
    event_data_ready_4 = process_alloc_event();
    ctimer_set(&ct, CLOCK_SECOND * 5, ctimer_event_callback2, &ct);
    while(1){
        PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready_2);
        printf("Number send by 1 to 2: %04x\n",*((int*)data));
        indice2=0
        while(indice2<5);
        ret[count]=3**((int*)data)/2;
        count = count +1; 
        process_post(&main_thread, event_data_ready_4,0);       
    }
    PROCESS_END();   
}

PROCESS_THREAD(third_thread,ev,data){
    PROCESS_BEGIN();
    event_data_ready_5 = process_alloc_event();
    ctimer_set(&ct, CLOCK_SECOND * 5, ctimer_event_callback3, &ct);
    while(1){
        PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready_3);
        printf("Number send by 1 to 3: %04x\n",*((char*)data));
        indice3=0
        while(indice3<5);
        ret[count]=5* *((int*)data);
        count = count +1;   
        process_post(&main_thread, event_data_ready_5,0);    
    }
    PROCESS_END();   
}