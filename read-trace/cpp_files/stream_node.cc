#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;
#include "stream_node.h"

stream_node :: stream_node(){
    value = 0;
}

void stream_node :: set_value(uint32_t in){
    value = in;
}
uint32_t stream_node :: get_value(){
    return value;
}