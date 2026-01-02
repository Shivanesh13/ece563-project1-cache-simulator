#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
using namespace std;

class cache_block{
    bool valid ;
    bool dirty ;
    uint32_t tag;
    uint32_t counter;

public:
    cache_block(uint32_t );  
    bool check_tag(uint32_t );
    void evict();
    void mark_dirty();  
    bool is_dirty();
    bool is_valid();
    uint32_t get_tag();
    void set_tag(uint32_t  );
    uint32_t get_counter();
    void set_counter(uint32_t ); 
    cache_block copy();
    void mark_clean();
    void mark_valid();

};