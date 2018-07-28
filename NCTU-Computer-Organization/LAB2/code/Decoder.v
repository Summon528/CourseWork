//Subject:     CO project 1 - Decoder
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      0611262 曾長彥
//----------------------------------------------
//Date:        2018/07/14
//----------------------------------------------
//Description:
//--------------------------------------------------------------------------------

module Decoder(
    instr_op_i,
    RegWrite_o,
    ALU_op_o,
    ALUSrc_o,
    RegDst_o,
    Branch_o,
    MemToReg_o,
    Jump_o,
    MemRead_o,
    MemWrite_o,
    BranchType_o
  );

//I/O ports
input  [6-1:0] instr_op_i;

output         RegWrite_o;
output [3-1:0] ALU_op_o;
output         ALUSrc_o;
output         RegDst_o;
output         Branch_o;
output [2-1:0] MemToReg_o;
output         Jump_o;
output         MemRead_o;
output         MemWrite_o;
output [2-1:0] BranchType_o;

//Internal Signals
reg    [3-1:0] ALU_op_o;
reg            ALUSrc_o;
reg            RegWrite_o;
reg            RegDst_o;
reg            Branch_o;
reg    [2-1:0] MemToReg_o;
reg            Jump_o;
reg            MemRead_o;
reg            MemWrite_o;
reg    [2-1:0] BranchType_o;

//Parameter


//Main function
always @(*) begin
  case (instr_op_i)
    6'b000_000: begin //r type
      ALU_op_o <= 3'b000;
      ALUSrc_o <= 0;
      Branch_o <= 0;
      BranchType_o <= 2'b00;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemToReg_o <= 0;
      MemWrite_o <= 0;
      RegDst_o <= 1;
      RegWrite_o <= 1;
    end

    6'b001_000: begin //addi
      ALU_op_o <= 3'b001;
      ALUSrc_o <= 1;
      Branch_o <= 0;
      BranchType_o <= 2'b00;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemToReg_o <= 0;
      MemWrite_o <= 0;
      RegDst_o <= 0;
      RegWrite_o <= 1;
    end

    6'b001_011: begin //sltiu
	    RegWrite_o <= 1;
      ALU_op_o <= 3'b010;
      ALUSrc_o <= 1;
      Branch_o <= 0;
      BranchType_o <= 2'b00;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemToReg_o <= 0;
      MemWrite_o <= 0;
      RegDst_o <= 0;
	  end

	  6'b000_100: begin //beq
	    RegWrite_o <= 0;
      ALU_op_o <= 3'b011;
      ALUSrc_o <= 0;
      Branch_o <= 1;
      BranchType_o <= 2'b00;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemToReg_o <= 0;
      MemWrite_o <= 0;
      RegDst_o <= 0;
	  end

    6'b001_111: begin //lui
      ALU_op_o <= 3'b100;
      ALUSrc_o <= 1;
      Branch_o <= 0;
      BranchType_o <= 2'b00;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemToReg_o <= 0;
      MemWrite_o <= 0;
      RegDst_o <= 0;
      RegWrite_o <= 1;
    end

    6'b001_101: begin //ori
      ALU_op_o <= 3'b101;
      ALUSrc_o <= 1;
      Branch_o <= 0;
      BranchType_o <= 2'b00;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemToReg_o <= 0;
      MemWrite_o <= 0;
      RegDst_o <= 0;
      RegWrite_o <= 1;
    end

    6'b000_101: begin //bne
      RegWrite_o <= 0;
      ALU_op_o <= 3'b011;
      ALUSrc_o <= 0;
      RegDst_o <= 0;
      Branch_o <= 1;
      MemToReg_o <= 0;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemWrite_o <= 0;
      BranchType_o <= 2'b01;
	  end

    6'b100_011: begin //lw
      RegWrite_o <= 0;
      ALU_op_o <= 3'b001;
      ALUSrc_o <= 1;
      RegDst_o <= 0;
      Branch_o <= 0;
      MemToReg_o <= 1;
      Jump_o <= 1;
      MemRead_o <= 1;
      MemWrite_o <= 0;
      BranchType_o <= 2'b00;
	  end

    default: begin
      RegWrite_o <= 0;
      ALU_op_o <= 3'b000;
      ALUSrc_o <= 0;
      RegDst_o <= 0;
      Branch_o <= 0;
      MemToReg_o <= 0;
      Jump_o <= 1;
      MemRead_o <= 0;
      MemWrite_o <= 0;
      BranchType_o <= 2'b00;
    end

  endcase
end
endmodule
