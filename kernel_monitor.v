module kernel_monitor(
    (*keep = "TRUE"*) input clk,
    (*keep = "TRUE"*) input rstn,
    (*keep = "TRUE"*) output wr_en,
    (*keep = "TRUE"*) output [31:0] wr_addr,
    (*keep = "TRUE"*) output [31:0] wr_data,
    (*keep = "TRUE"*) input wr_done,
    
    (*keep = "TRUE"*) output rd_en,
    (*keep = "TRUE"*) output [31:0] rd_addr,
    (*keep = "TRUE"*) input rd_valid,
    (*keep = "TRUE"*) input [31:0] rd_data,
    (*keep = "TRUE"*) input rd_done
    );
    
    parameter [31:0] BASE_ADDR = 32'h81000;
    parameter [31:0] MAX_ADDR = 32'h17FF00;
    
    reg wr_en = 1'b0;
    reg [31:0] wr_addr = 32'hA800_1000;
    assign wr_data = rd_data;
    
    reg rd_en = 1'b0;
    reg [31:0] rd_addr = BASE_ADDR;
    
    reg [31:0] cnt = 32'd0;//等待时间2秒
    
    
    //状态机
    parameter [4:0] IDLE = 5'b00001;
    parameter [4:0] WR = 5'b00010;
    parameter [4:0] WR_DONE = 5'b00100;
    parameter [4:0] RD = 5'b01000;
    parameter [4:0] RD_DONE = 5'b10000;
    
    reg [4:0] state = IDLE;
    always @(posedge clk) begin
        if (rstn == 1'b0) begin
            state = IDLE;
        end    
        else begin
            case (state)
                IDLE: begin
                    if (cnt == 32'd200_000_000) begin
                        state <= RD;
                    end
                    else begin
                        state <= state;
                    end
                end
                
                WR: begin
                    state <= WR_DONE;
                end
                
                WR_DONE: begin
                    if (wr_done == 1'b1) begin
                        state <= RD;
                    end
                    else begin
                        state <= state;
                    end
                end
                
                RD: begin
                    state <= RD_DONE;
                end
                
                RD_DONE: begin
                    if (rd_done == 1'b1) begin
                        state <= WR;
                    end
                    else begin
                        state <= state;
                    end
                end
                
                default: begin
                    state = IDLE;
                end
            endcase
        end
    end
    
    always @(posedge clk) begin
        if (rstn == 1'b0) begin
            cnt <= 32'd0;
        end
        else begin
            if (state == IDLE) begin
                cnt <= cnt+32'd1;
            end
            else begin
                cnt <= 32'd0;
            end
        end
    end
    
    
    always @(posedge clk) begin
        if (rstn == 1'b0) begin
            wr_en <= 1'b0;
            wr_addr <= 32'hA800_1000;
        end
        else begin
            case (state)
                WR: begin
                    wr_en <= 1'b1;
                    wr_addr <= wr_addr;
                end
                
                WR_DONE: begin
                    wr_en <= 1'b0;
                    
                    if (wr_done == 1'b1) begin
                        //更新写地址
                        if (wr_addr == 32'hA80F_FF00) begin
                            wr_addr <= 32'hA800_1000;
                        end
                        else begin
                            wr_addr <= wr_addr+32'd4;
                        end
                    end
                    else begin
                        wr_addr <= wr_addr;
                    end
                end
                
                default: begin
                    wr_en <= 1'b0;
                    wr_addr <= wr_addr;
                end
            endcase
        end
    end    
        
                    
    always @(posedge clk) begin
        if (rstn == 1'b0) begin
            rd_en <= 1'b0;
            rd_addr <= BASE_ADDR;
        end
        else begin
            case (state)
                RD: begin
                    rd_en <= 1'b1;
                    rd_addr <= rd_addr;
                end
                
                RD_DONE: begin
                    rd_en <= 1'b0;
                    
                    if (rd_done == 1'b1) begin
                        //更新读地址
                        if (rd_addr == MAX_ADDR) begin
                            rd_addr <= BASE_ADDR;
                        end
                        else begin
                            rd_addr <= rd_addr+32'd4;
                        end
                    end
                    else begin
                        rd_addr <= rd_addr;
                    end
                end
                
                default: begin
                    rd_en <= 1'b0;
                    rd_addr <= rd_addr;
                end
            endcase
        end
    end                    



    
endmodule    
    
