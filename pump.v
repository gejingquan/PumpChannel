`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12/03/2020 03:39:42 PM
// Design Name: 
// Module Name: pump
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module pump(
	input wire clk,
	input wire rstn,
	(*mark_debug = "true"*)output wire wr_en,
	(*mark_debug = "true"*)output wire [31:0] wr_addr,
	(*mark_debug = "true"*)output wire [31:0] wr_data,
	(*mark_debug = "true"*)input wire wr_done,
    
	(*mark_debug = "true"*)output wire rd_en,
	(*mark_debug = "true"*)output wire [31:0] rd_addr,
	(*mark_debug = "true"*)input wire rd_valid,
	(*mark_debug = "true"*)input wire [31:0] rd_data,
	(*mark_debug = "true"*)input wire rd_done,
     
	(*mark_debug = "true"*)input wire [31:0] exchannel_id,
	(*mark_debug = "true"*)input wire [31:0] pump_in_addr,
	(*mark_debug = "true"*)input wire [31:0] pump_in_size,
	(*mark_debug = "true"*)input wire [31:0] pump_out_addr,
	(*mark_debug = "true"*)input wire [31:0] pump_out_size,
	(*mark_debug = "true"*)input wire [31:0] pump_controller
	);

	(*mark_debug = "true"*)reg [31:0]  read_in_base_addr;
	(*mark_debug = "true"*)reg [31:0]  read_in_max_addr;
	(*mark_debug = "true"*)reg [31:0]  write_in_base_addr; 
	(*mark_debug = "true"*)reg [31:0]  write_in_max_addr;
	(*mark_debug = "true"*)reg [31:0]  read_out_base_addr;
	(*mark_debug = "true"*)reg [31:0]  read_out_max_addr;
	(*mark_debug = "true"*)reg [31:0]  write_out_base_addr;
	(*mark_debug = "true"*)reg [31:0]  write_out_max_addr; 

	
	
	(*mark_debug = "true"*)reg [31:0] rd_addr_reg;
	(*mark_debug = "true"*)reg [31:0] wr_addr_reg;
	(*mark_debug = "true"*)reg [31:0] wr_addr_next_reg;
	(*mark_debug = "true"*)reg rd_en_reg;
	(*mark_debug = "true"*)reg wr_en_reg;
	(*mark_debug = "true"*)reg [31:0] wr_data_reg;
	(*mark_debug = "true"*)reg [31:0] wr_data_next_reg;

	
	assign rd_addr = rd_addr_reg;
	assign wr_addr = wr_addr_reg;
	assign rd_en   = rd_en_reg;
	assign wr_en   = wr_en_reg;
	assign wr_data = wr_data_reg;


	//state machine
	(*mark_debug = "true"*)reg [12:0] state;
	parameter [12:0]  IDLE           = 13'b0_0000_0000_0001;
	parameter [12:0]  WR_IN          = 13'b0_0000_0000_0010;
	parameter [12:0]  WR_IN_DONE     = 13'b0_0000_0000_0100;
	parameter [12:0]  RD_IN          = 13'b0_0000_0000_1000;
	parameter [12:0]  RD_IN_DONE     = 13'b0_0000_0001_0000;
    
	parameter [12:0]  WR_OUT         = 13'b0_0000_0010_0000;
	parameter [12:0]  WR_OUT_DONE    = 13'b0_0000_0100_0000;
	parameter [12:0]  RD_OUT         = 13'b0_0000_1000_0000;
	parameter [12:0]  RD_OUT_DONE    = 13'b0_0001_0000_0000; 
      
	parameter [12:0]  FLUSH_IN       = 13'b0_0010_0000_0000;
	parameter [12:0]  FLUSH_IN_DONE  = 13'b0_0100_0000_0000;    
	parameter [12:0]  FLUSH_OUT      = 13'b0_1000_0000_0000;
	parameter [12:0]  FLUSH_OUT_DONE = 13'b1_0000_0000_0000;	

	
	//exchannel0 base address
	parameter [31:0] EXCHANNEL0_BASE_ADDR = 32'hA000_0000;

	
	//pump_controller        
	parameter [31:0] PUMP_IN   = 32'hF0F0F0F0;
	parameter [31:0] PUMP_OUT  = 32'h0F0F0F0F;
	


	
	//====================================================
	//state machine and base registers
	//====================================================
	always @(posedge clk) 
		begin
		if (rstn == 1'b0) 
			begin
			state <= IDLE;
			end	
		else
			begin
			case(state)
				IDLE:
					begin
					if(pump_controller == PUMP_IN || pump_controller == PUMP_OUT) 
						begin
						state <= RD_IN;
						end
					else 
						begin
                        state <= IDLE;
						end
					
					end
				RD_IN: 
					begin
					state <= RD_IN_DONE;
					end 
				RD_IN_DONE:
					begin
					if (rd_done == 1'b1) 
						begin
						state <= FLUSH_IN;
						end    
					else 
						begin
						state <= state;
						end
					end				
				FLUSH_IN:
					begin
					state <= FLUSH_IN_DONE;
					end 				
				FLUSH_IN_DONE:
					begin
					if (wr_done == 1'b1) 
						begin
						state <= WR_IN;
						end
					else 
						begin
						state <= state;
						end
					end				
				WR_IN:
					begin
					state <= WR_IN_DONE;
					end				
				WR_IN_DONE:
					begin
					if ((wr_done == 1'b1) && (wr_addr_reg != write_in_max_addr)) 
						begin
						state <= RD_IN;
						end
					else if ((wr_done == 1'b1) && (wr_addr_reg == write_in_max_addr)) 
						begin
						state <= RD_OUT;
						end 
					else 
						begin
						state <= state;
						end
					end 				
				RD_OUT:
					begin
					state <= RD_OUT_DONE;                                                    
					end				
				RD_OUT_DONE:
					begin
					if(rd_done == 1'b1)
						begin
						state <= FLUSH_OUT;
						end
					else
						begin
						state <= state;
						end                                                               
					end				
				FLUSH_OUT:
					begin
					state <= FLUSH_OUT_DONE;
					end				
				FLUSH_OUT_DONE:
					begin
					if(wr_done == 1'b1)
						begin
						state <= WR_OUT;
						end
					end				
				WR_OUT:
					begin
					state <= WR_OUT_DONE;
					end				
				WR_OUT_DONE:
					begin
					if ((wr_done == 1'b1) && (wr_addr_reg != write_out_max_addr)) 
						begin
						state <= RD_OUT;
						end
					else if ((wr_done == 1'b1) && (wr_addr_reg == write_out_max_addr)) 
						begin
						state <= IDLE;
						end 
					else 
						begin
						state <= state;
						end
					end
				default: 
					begin
					state = IDLE;
					end					
			endcase
			end
		end

	//====================================================
	//read and write address and data
	//====================================================  
	always @(posedge clk) 	
		begin
		if (rstn == 1'b0) 
			begin
			rd_en_reg <= 1'b0;
			wr_en_reg <= 1'b0;
			rd_addr_reg <= 32'h0;
			wr_addr_reg <= 32'h0;
			wr_data_reg <= 32'h0;
			wr_addr_next_reg <= 32'h0;
			wr_data_next_reg <= 32'h0;
			end			
		else 
			begin
            case (state)
				IDLE:
					begin
					rd_en_reg    <= 1'b0;
					wr_en_reg    <= 1'b0;					
					wr_data_reg  <= 32'h0;
					wr_data_next_reg <= 32'h0;
					if(pump_controller == PUMP_IN) 
						begin
						read_in_base_addr    <= pump_in_addr;
						rd_addr_reg          <= pump_in_addr;
						wr_addr_reg          <= pump_in_addr;						
						read_in_max_addr     <= pump_in_addr+pump_in_size;
						read_out_base_addr   <= pump_out_addr;
						read_out_max_addr    <= pump_out_addr+pump_out_size;                       
						write_in_base_addr   <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000;
						wr_addr_next_reg     <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000;
						write_in_max_addr    <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000 + pump_in_size;
						write_out_base_addr  <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000 + 32'h1000000;
						write_out_max_addr   <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000 + 32'h1000000 + pump_out_size;                                                                       						
						end
					else if(pump_controller == PUMP_OUT)
						begin
                        read_in_base_addr    <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000;
						rd_addr_reg          <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000;
						wr_addr_reg          <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000;
                        read_in_max_addr     <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000 + pump_in_size;
                        read_out_base_addr   <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000 + 32'h1000000;
                        read_out_max_addr    <= EXCHANNEL0_BASE_ADDR + exchannel_id*32'h2000000 + 32'h1000000 + pump_out_size;                         
                        write_in_base_addr   <= pump_in_addr;
						wr_addr_next_reg     <= pump_in_addr;
                        write_in_max_addr    <= pump_in_addr+pump_in_size;
                        write_out_base_addr  <= pump_out_addr;
                        write_out_max_addr   <= pump_out_addr+pump_out_size; 						
						end
					else
						begin
						read_in_base_addr    <= 32'h0;
						rd_addr_reg          <= 32'h0;
						wr_addr_reg          <= 32'h0;
						read_in_max_addr     <= 32'h0;
						read_out_base_addr   <= 32'h0;
						read_out_max_addr    <= 32'h0;                         
						write_in_base_addr   <= 32'h0;
						wr_addr_next_reg     <= 32'h0;
						write_in_max_addr    <= 32'h0;
						write_out_base_addr  <= 32'h0;
						write_out_max_addr   <= 32'h0; 
						end
					end
				RD_IN:
					begin
					rd_en_reg <= 1'b1;
					rd_addr_reg  <= rd_addr_reg;
					end
				RD_IN_DONE:
					begin
					rd_en_reg <= 1'b0;
					if (rd_done == 1'b1) 
						begin
						wr_data_next_reg <= rd_data;
						wr_data_reg <= 32'h0;
						wr_addr_reg <= rd_addr_reg;						
						end
					else 
						begin
						wr_data_next_reg <= wr_data_next_reg;
						end					
					end
				FLUSH_IN:
					begin
					wr_en_reg   <= 1'b1;
					end				
				FLUSH_IN_DONE:
					begin
					wr_en_reg   <= 1'b0;
					if(wr_done == 1'b1)
						begin
						wr_data_reg <= wr_data_next_reg;
						wr_addr_reg <= wr_addr_next_reg;						
						if(rd_addr_reg == read_in_max_addr)
							begin
							rd_addr_reg <= read_out_base_addr;
							end
						else
							begin
							rd_addr_reg <= rd_addr_reg + 32'h4;
							end
						end
					else
						begin
						rd_addr_reg <= rd_addr_reg;
						end
					end				
				WR_IN:
					begin
					wr_en_reg   <= 1'b1;
					end				
				WR_IN_DONE:
					begin
					wr_en_reg   <= 1'b0;
					if(wr_done == 1'b1)
						begin
						if(wr_addr_reg == write_in_max_addr)
							begin
							wr_addr_next_reg <= write_out_base_addr;
							end
						else
							begin
							wr_addr_next_reg <= wr_addr_next_reg + 32'h4;
							end
						end
					else
						begin
						wr_addr_reg <= wr_addr_reg;
						end
					end
				RD_OUT:
					begin
					rd_en_reg <= 1'b1;
					rd_addr_reg <= rd_addr_reg;                   
					end				
				RD_OUT_DONE:
					begin
					rd_en_reg <= 1'b0;
					if (rd_done == 1'b1) 
						begin
						wr_data_next_reg <= rd_data;
						wr_data_reg <= 32'h0;
						wr_addr_reg <= rd_addr_reg;						
						end
					else 
						begin
						wr_data_next_reg <= wr_data_next_reg;
						end 
					end				
				FLUSH_OUT:
					begin
					wr_en_reg   <= 1'b1;
					end					
				FLUSH_OUT_DONE:
					begin
					wr_en_reg   <= 1'b0;
					if(wr_done == 1'b1)
						begin
						wr_data_reg <= wr_data_next_reg;
						wr_addr_reg <= wr_addr_next_reg;
						if(rd_addr_reg == read_out_max_addr)
							begin
							rd_addr_reg <= read_in_base_addr;
							end
						else
							begin
							rd_addr_reg <= rd_addr_reg + 32'h4;
							end
						end
					else
						begin
						rd_addr_reg <= rd_addr_reg;
						end
					end					
				WR_OUT:
					begin
					wr_en_reg   <= 1'b1;
					end				
				WR_OUT_DONE:
					begin
					wr_en_reg   <= 1'b0;
					if(wr_done == 1'b1)
						begin
						if(wr_addr_reg == write_out_max_addr)
							begin
							wr_addr_next_reg <= write_in_base_addr;
							end
						else
							begin
							wr_addr_next_reg <= wr_addr_next_reg +32'h4;
							end
						end
					else
						begin
						wr_addr_reg <= wr_addr_reg;
						end
					end					
				default:
					begin
					rd_en_reg <= 1'b0;
					wr_en_reg <= 1'b0;
					rd_addr_reg <= 32'h0;
					wr_addr_reg <= 32'h0;
					wr_data_reg <= 32'h0;
					wr_addr_next_reg <= 32'h0;
					wr_data_next_reg <= 32'h0;					
					end
				
			endcase		
			end
		end
			
endmodule