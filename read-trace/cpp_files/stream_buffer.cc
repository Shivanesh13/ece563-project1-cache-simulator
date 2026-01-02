#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;
#include "stream_buffer.h"
#include <iostream>
#include <iomanip>


stream_buffer :: stream_buffer(uint32_t size){
    stream_node* temp;
    this->size = size;
    valid = false;
    head = new stream_node();
    tail = head;
    prefetch = 0;
    for(uint32_t i = 1;i < size; i++){
        temp = new stream_node();
        tail->next = temp;
        tail = temp;
    }
}

stream_buffer :: ~stream_buffer(){
    stream_node* temp1 = head;
    stream_node* temp2 = head;
    for(uint32_t i = 0; i < size; i++){
        temp2 = temp1->next;
        delete temp1;
        temp1 = temp2;
    }
}

void stream_buffer :: buffer_print(){
    stream_node* temp = head;
    while(temp){
        cout << hex << setw(8) << temp->get_value() << " ";
        temp = temp->next;
    }
    printf("\n");
}

void stream_buffer :: set_buffer(uint32_t val){
    stream_node* temp = head;
    uint32_t temp_val = val;
    valid = true;
    for(uint32_t i =0; i < size;i++){
        temp->set_value(temp_val);
        temp_val++;
        temp = temp->next;
    }
}
uint32_t stream_buffer :: get_counter(){
    return this->counter;
}

void stream_buffer :: set_counter(uint32_t count){
    this->counter = count;
}

void stream_buffer :: reset_counter(){
    this->counter = 0;
}

void stream_buffer :: increment_counter(){
    this->counter++;
}

bool stream_buffer :: check_hit(uint32_t tag_index){
    if(valid){
        if(tag_index >= head->get_value() && tag_index < (head->get_value() + size)){
            prefetch = prefetch + 1 + (tag_index - head->get_value());
            return true;
        }else {
            return false;
        }
    }
    return false;
}


