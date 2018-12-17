#ifndef VERILOG_BITS_HPP
#define VERILOG_BITS_HPP

#include <algorithm>
#include <vector>
#include <assert.h>
#include <string>
#include <memory>
#include <atomic>

#include "stdio.h"
#include "InternalBits.hpp"

class VerilogBits
{
private:

    BitSpace::BitFields<uint8_t> *bits;
    int16_t bit_size;

    inline size_t to_index(size_t address)
    {
        return (address >> 2);
    }

    std::string to_string(bool big_endian)
    {
        size_t start =      (big_endian)? 0x0: this->size()-1;
        size_t end =        (big_endian)? this->size()-1: 0x0;
        size_t increment =  (big_endian)? 1: -1;

        std::string to_return = "";
        for(int address=start; (big_endian)? (address <= static_cast<int>(end)): (address >= static_cast<int>(end)); address += increment)
        {
            to_return += this->bits[to_index(address)].get_bit_printable_data(address);
        }
        return to_return;
    }

    size_t get_alloc_size()
    {
        return sizeof(bit_size) + sizeof(bits) + (sizeof(BitSpace::BitFields<uint8_t>) * to_index(this->bit_size));
    }

public:

    VerilogBits()
    {
    }

    VerilogBits(size_t data_size, data_t value_in)
    {
        
        this->init_bits(data_size, value_in);
    }

    void init_bits(size_t data_size, data_t value_in)
    {
        this->bit_size = data_size;
        this->bits = new BitSpace::BitFields<uint8_t>[to_index(this->bit_size)];
        for(size_t i=0; i < to_index(this->bit_size); i++)
        {
            bits[i].init_values(value_in);
        }
    }

    size_t size()
    {
        return bit_size;
    }

    void print_type()
    {
        printf("allocated:(%zu) with bits_size(%zu)\n",this->get_alloc_size(), sizeof(bits));
    }

    void print()
    {
        printf("%s\n",this->to_string(false).c_str());
    }

    data_t get_bit(size_t address)
    {
        assert(address < this->size()
            && "accessing out of bound location for verilog bits");

        return this->bits[to_index(address)].get_bit(address);
    }

    void set_bit(size_t address, data_t value)
    {
        assert(address < this->size()
            && "accessing out of bound location for verilog bits");

        this->bits[to_index(address)].set_bit(address, value);
    }
};


#undef REAL_SIZE

#endif // VERILOG_BITS_HPP
