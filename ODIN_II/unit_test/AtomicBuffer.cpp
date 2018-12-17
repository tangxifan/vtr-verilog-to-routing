#include "AtomicBuffer.hpp"
#include <atomic>
#include <thread>

void update_the_values(signed char value, AtomicBuffer *my_bits, std::atomic_flag *lock) {
	for(int i=-10; i<32; i++)
	{
		my_bits->update_value(i,value);
		std::string print_out = my_bits->to_string(false);
		while (lock->test_and_set(std::memory_order_acquire));
		printf("(%hhd)[%d] : %s\n",value,i, print_out.c_str());
		
		lock->clear(std::memory_order_release);
	}
}

int main(int argc, char **argv)
{
	std::atomic_flag lock = ATOMIC_FLAG_INIT;

	AtomicBuffer other_bits;
    other_bits.print_type();
    printf("array_size(%zu) \n\n================\n", other_bits.size());
	std::string print_out = other_bits.to_string(false);
	printf("intialized : %s\n",print_out.c_str());

	printf("============ Sequential Test ==============\n");
	for(signed char value=1 ; value>= -1 ; value--)
	{
		for(size_t i=0; i<100; i++)
		{
			other_bits.update_value(i,value);

			print_out = other_bits.to_string(false);
			printf("(%hhd)[%zu] : %s\n",value,i, print_out.c_str());
		}
	}

	AtomicBuffer my_bits;
	printf("============ Multithreaded Test ==============\n");
	std::vector<std::thread> workers;
	for (int id =0; id < 4; id++)
	{
		workers.push_back(std::thread(update_the_values,(id%4)-1,&my_bits, &lock));
	}


	for (auto &worker: workers)	
		worker.join();
}
