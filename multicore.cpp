#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include <stdint.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdio.h>

typedef struct
{
    int32_t (*func)(std::string);
    std::string data;
} queue_entry_t;

queue_t call_queue;
queue_t results_queue;

queue_entry_t entry;

void core1_entry() {
    while (1) {
        // Function pointer is passed to us via the queue_entry_t which also
        // contains the function parameter.
        // We provide an int32_t return value by simply pushing it back on the
        // return queue which also indicates the result is ready.
        printf("Inside core1....\n");
        queue_entry_t entry;

        queue_remove_blocking(&call_queue, &entry);
        std::cout << "Entry is:" << entry.data << std::endl;
        int32_t (*func)(std::string){entry.func};
        int32_t result = func(entry.data);
        queue_add_blocking(&results_queue, &result);
    }
}

int32_t displayString(std::string received){
    std::cout << "In function: " << received << std::endl;

    sleep_ms(3000);
    return 0;
}

std::string rcv;

int main() {
    const uint LED_PIN = 25;
    int32_t res;
    bool first = false;

    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    printf("Starting....\n");
    printf("Setup queues...\n");
    //setup queue
    queue_init(&call_queue, sizeof(queue_entry_t), 2);
    queue_init(&results_queue, sizeof(int32_t), 2);
    printf("Launching core1\n");
    multicore_launch_core1(core1_entry);

    while (true) {

        if(first==false){
            rcv = "A long string to test with..............";
            first=true;
            entry.func = &displayString;
            entry.data = rcv;
            queue_add_blocking(&call_queue, &entry);
        }

        if(queue_try_remove(&results_queue, &res)){
            std::cout << "rcv is now:" << rcv << std::endl;
            queue_add_blocking(&call_queue, &entry);
            printf("Added to queue remove\n");
        }
    }
}
