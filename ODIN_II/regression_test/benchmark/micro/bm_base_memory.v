// DEFINES
`define BITS 4
`define MEMORY_WORDS 8

module 	bm_base_memory(clock, 
		we, 
		address_in,
		address_out,
		value_out,
		value_in
		);

// SIGNAL DECLARATIONS
input	clock;
input 	we;
input 	[`MEMORY_WORDS-1:0]value_in;
input 	[1:0]address_in;
input 	[1:0]address_out;

output [`MEMORY_WORDS-1:0] value_out;


reg [`MEMORY_WORDS-1:0] memory [`BITS-1:0]; // 4 memory slots of Bits wide

always @(posedge clock)
begin
	if(we)
		memory[address_in] <= value_in;
	value_out <= memory[address_out];
end

endmodule
