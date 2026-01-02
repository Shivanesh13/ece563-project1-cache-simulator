#ifndef SIM_CACHE_H
#define SIM_CACHE_H

typedef struct {
   uint32_t BLOCKSIZE;
   uint32_t L1_SIZE;
   uint32_t L1_ASSOC;
   uint32_t L2_SIZE;
   uint32_t L2_ASSOC;
   uint32_t PREF_N;
   uint32_t PREF_M;
} cache_params_t;

typedef struct {
   uint32_t SIZE;
   uint32_t BLOCKSIZE;
   uint32_t ASSOC;
   uint32_t INDEX;
   uint32_t PREF_N;
   uint32_t PREF_M;
} cache_config_t;

typedef struct {
   uint32_t reads;
   uint32_t read_misses;
   uint32_t writes;
   uint32_t write_misses;
   uint32_t miss_rate;
   uint32_t writebacks;
   uint32_t prefetch;
} cache_stats_t;

typedef struct {
   uint32_t SIZE_BITS;
   uint32_t OFFSET_BITS;   
   uint32_t INDEX_BITS;
   uint32_t TAG_BITS;
} cache_bit_info_t;

typedef struct {
   uint32_t tag;
   bool is_dirty;
} dirty_state;

const uint32_t ADDRESS_LENGTH = 32; // in bits

// Put additional data structures here as per your requirement.

#endif
