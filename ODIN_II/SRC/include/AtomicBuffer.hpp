#ifndef VERILOG_BITS_HPP
#define VERILOG_BITS_HPP

#include <atomic>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <string>
#include <memory>
#include <thread>

#include "stdio.h"
#include "InternalBits.hpp"

// used to resize the buffer for simulation. the smaller the less footprint
// Max is 31
#define CONCURENCY_LIMIT 1

typedef uint32_t cycle_t;

class AtomicBuffer
{
private:
    #if   CONCURENCY_LIMIT < 4
        BitSpace::BitFields<uint8_t> bits;
    #elif CONCURENCY_LIMIT < 8
        BitSpace::BitFields<uint16_t> bits;
    #elif CONCURENCY_LIMIT < 16
        BitSpace::BitFields<uint32_t> bits;
    #else
        BitSpace::BitFields<uint64_t> bits;
    #endif

    std::atomic<bool> locked;
	uint32_t cycle;
	
    void lock_it()
	{
        std::atomic_thread_fence(std::memory_order_acquire);
        while(locked.exchange(true, std::memory_order_relaxed))
        {
            std::this_thread::yield();
        }
	}

	void unlock_it()
	{
        locked.exchange(false, std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_relaxed);
	}
	
    size_t get_alloc_size()
    {
        return (sizeof(cycle) + sizeof(bits));
    }
	
public:

    AtomicBuffer()
    {
        this->init_all_values(-1);
    }

    AtomicBuffer(data_t value_in)
    {
        this->init_all_values(value_in);
    }

    void init_all_values( data_t value)
    {
    	this->locked = false;
        this->update_cycle(-1);
        for(int i=0; i<this->bits.size(); i++)
        {
            this->bits.set_bit(i,value);
        }
    }

    bool cycle_is_less_than(int64_t cycle_in)
    {
        lock_it();
		bool value = (this->cycle < cycle_in);
        unlock_it();
        return value;
    }

    void update_cycle( int64_t cycle_in)
    {
		lock_it();
        if(cycle_in > this->cycle)
            this->cycle = cycle_in;
        unlock_it();
    }

    data_t get_value( int64_t cycle_in)
    {
        cycle_t index = cycle_in%this->bits.size();
		lock_it();
    	data_t value = this->bits.get_bit(index);
        unlock_it();
        return value;
    }

    void update_value(int64_t cycle_in, data_t value_in)
    {
        cycle_t index = cycle_in%this->bits.size();
		lock_it();
        this->bits.set_bit(index,value_in);
        if(cycle_in > this->cycle)
            this->cycle = cycle_in;

        unlock_it();

    }

    size_t size()
    {
        return this->bits.size();
    }
    
    std::string to_string(bool big_endian)
    {

        size_t start =      (big_endian)? 0x0: this->size()-1;
        size_t end =        (big_endian)? this->size()-1: 0x0;
        size_t increment =  (big_endian)? 1: -1;

        std::string to_return = "";
        for(size_t address=start; address != end; address += increment)
        {
            to_return += this->bits.get_bit_printable_data(address);
        }

        return to_return;
    }

    void print_type()
    {
        printf("allocated:(%zu) with bits_size(%zu)\n",this->get_alloc_size(), sizeof(bits));
    }
    
};

#endif // VERILOG_BITS_HPP
