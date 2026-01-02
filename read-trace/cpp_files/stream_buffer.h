#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "stream_node.h"

using namespace std;

class stream_buffer{
    uint32_t counter;
    bool valid;
    uint32_t size;
    stream_node* head;
    stream_node* tail = nullptr;
    
    public :
        uint32_t prefetch = 0;
        stream_buffer(uint32_t);
        ~stream_buffer();
        void set_buffer(uint32_t);
        void set_counter(uint32_t);
        uint32_t get_counter();
        void reset_counter();
        void increment_counter();
        void buffer_print();
        bool check_hit(uint32_t);
};