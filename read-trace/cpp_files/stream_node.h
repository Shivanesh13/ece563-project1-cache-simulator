#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;

class stream_node{
    private:
        uint32_t value;
    public:
        stream_node* next = nullptr; 
        stream_node();
        void set_value(uint32_t );
        uint32_t get_value();
};