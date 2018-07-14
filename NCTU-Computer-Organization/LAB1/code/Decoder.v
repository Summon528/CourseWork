//Subject:     CO project 1 - Decoder
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:
//----------------------------------------------
//Date:
//----------------------------------------------
//Description:
//--------------------------------------------------------------------------------

module Decoder(
    instr_op_i,
  RegWrite_o,
  ALU_op_o,
  ALUSrc_o,
  RegDst_o,
  Branch_o
  );

//I/O ports
input  [6-1:0] instr_op_i;

output         RegWrite_o;
output [3-1:0] ALU_op_o;
output         ALUSrc_o;
output         RegDst_o;
output         Branch_o;

//Internal Signals
reg    [3-1:0] ALU_op_o;
reg            ALUSrc_o;
reg            RegWrite_o;
reg            RegDst_o;
reg            Branch_o;

//Parameter


//Main function
always @(*) begin
  case (instr_op_i)
    6'b000_000: begin
      RegWrite_o <= 1;
      ALU_op_o <= 3'b000;
      ALUSrc_o <= 0;
      RegDst_o <= 1;
      Branch_o <= 0;
    end

    6'b001_000: begin
      RegWrite_o <= 1;
      ALU_op_o <= 3'b001;
      ALUSrc_o <= 1;
      RegDst_o <= 0;
      Branch_o <= 0;
    end

    6'b001_011: begin
	  RegWrite_o <= 1;
      ALU_op_o <= 3'b010;
      ALUSrc_o <= 1;
      RegDst_o <= 0;
      Branch_o <= 0;
	end

	6'b000_100: begin
	  RegWrite_o <= 0;
      ALU_op_o <= 3'b011;
      ALUSrc_o <= 0;
      RegDst_o <= 0; //doesnt matter
      Branch_o <= 1;
	end
  endcase
end
endmodule
