#ifndef INTERNAL_BITS_HPP
#define INTERNAL_BITS_HPP

#include <cstdint>
#include <string>
#include <algorithm>

typedef signed char data_t;

namespace BitSpace {
    typedef uint8_t bit_value_t;

    const bit_value_t _0 = 0x0;
    const bit_value_t _1 = 0x1;
    const bit_value_t _z = 0x2;
    const bit_value_t _x = 0x3;

    ///////////////////////////
    // 2 bits are used per value
    static data_t ext_val(bit_value_t val_in)
    {
        switch(val_in)
        {
            case _0:    return 0;
            case _1:    return 1;
            default:    return -1;
        }  
    }

    static char to_str(bit_value_t val_in)
    {
        switch(val_in)
        {
            case _0:    return '0';
            case _1:    return '1';
            case _z:    return 'z';
            default:    return 'x';
        }            
    }

    ///////////////////////////
    // 2 bits are used per value
    static bit_value_t int_val(data_t val_in)
    {
        return  (val_in == 0 || val_in == '0')  ? _0:
                (val_in == 1 || val_in == '1')  ? _1:
                (val_in == 2 || val_in == 'z')  ? _z:
                                                _x;
    }

    const uint64_t mask = 0x3;

    template<typename T>
    class BitFields
    {
    private :

        T bits;

        size_t veri_length()
        {
            return (sizeof(bits)<<2);
        }

        size_t get_bit_location(size_t address)
        {
            size_t real_address = ((address%veri_length())<<1);
            return real_address;
        }

        void init_values_internal(T init)
        {
            for(size_t i=0; i < this->veri_length() ; i++)
            {
                this->bits |= init;
                init <<= 2;
            }
        }

        bit_value_t get_bit_val(size_t address)
        {
            size_t real_address = this->get_bit_location(address);
            return (this->bits >> real_address) & mask;
        }
        
        void set_bit_value(size_t address, bit_value_t value)
        {	
            size_t real_address = this->get_bit_location(address);

            T long_value = value;

            T set_value = long_value << real_address;
            T zero_out_location = ~(mask << real_address);

            this->bits &= zero_out_location;
            this->bits |= set_value;
        }


    public :

        void init_values(data_t initial_value)
        {
            this->init_values_internal(int_val(initial_value));
        }

        BitFields()
        {
            this->init_values_internal(_x);
        }

        BitFields(data_t initial_value)
        {
            this->init_values(initial_value);
        }

        data_t get_bit(size_t address)
        {
            return ext_val(get_bit_val(address));
        }

        void set_bit(size_t address, data_t value)
        {	    
            this->set_bit_value(address,int_val(value));
        }

        char get_bit_printable(size_t address)
        {
            return to_str(get_bit_val(address));
        }

        size_t size()
        {
            return this->veri_length();
        }

        std::string get_bit_printable_data(size_t address)
        {
            return std::to_string(get_bit(address));
        }

    };
}

#endif