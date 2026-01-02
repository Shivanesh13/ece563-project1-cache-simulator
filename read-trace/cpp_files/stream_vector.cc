#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;
#include "stream_vector.h"



stream_vector :: stream_vector(uint32_t N, uint32_t M){
    stream_size = M;
    total_stream = N;
    prefetch = 0;
    for(uint32_t i = 0; i< total_stream; i++){
        stream_buffer* temp = new stream_buffer(stream_size);
        temp->set_counter(i);
        vec.push_back(temp);
    }
}

void stream_vector :: stream_print(){
     cout << "\n===== Stream Buffer(s) contents =====" << endl;
    for(uint32_t i = 0; i < total_stream; i++){
        for(uint32_t j = 0; j < total_stream; j++){
            if(vec[j]->get_counter() == i){
                vec[j]->buffer_print();
                break;
            }
        }
    }
}

uint32_t stream_vector :: get_prefetch(){
    uint32_t t = 0;
    for(auto temp : vec){
        t = t + temp->prefetch;
    }
    return (t + prefetch);
}

stream_vector :: ~stream_vector(){
    for(auto& temp : vec){
        delete temp;
    }
}

void stream_vector :: update_counters(uint32_t count){
    for(auto& temp : vec){
        if(temp->get_counter() == count){
            temp->set_counter(0);
        } else if(temp->get_counter() <= count){
            temp->set_counter(temp->get_counter()+1);
        }
    }
}


bool stream_vector :: check_tag(uint32_t tag, bool hit){
    for(uint32_t i = 0; i < total_stream; i++){
        for(auto& temp : vec){
            if(i == temp->get_counter()){
                if(temp->check_hit(tag)){
                    temp->set_buffer(tag+1);
                    update_counters(i);
                    return true;
                }
            }
        }
    }
    if(hit == false){
        for(auto& temp : vec){
            if(temp->get_counter() == total_stream -1){
                temp->set_buffer(tag+1);
                prefetch = prefetch + stream_size;
                update_counters(temp->get_counter());
                break;
            }
        }
    }
    return false;
}

