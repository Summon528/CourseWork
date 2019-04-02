//Subject:     CO project 1 - ALU Controller
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      0611262 曾長彥
//----------------------------------------------
//Date:        2018/07/14
//----------------------------------------------
//Description:
//--------------------------------------------------------------------------------

module ALU_Ctrl(
          funct_i,
          ALUOp_i,
          ALUCtrl_o,
          JR_o
          );

//I/O ports
input      [6-1:0] funct_i;
input      [3-1:0] ALUOp_i;

output     [4-1:0] ALUCtrl_o;
output     JR_o;

//Internal Signals
reg        [4-1:0] ALUCtrl_o;
wire               JR_o;

//Parameter


//Select exact operation

assign JR_o =
!funct_i[5] & !funct_i[4] & funct_i[3] & !funct_i[2] & !funct_i[1] & !funct_i[0] &
!ALUOp_i[0] & !ALUOp_i[1] & !ALUOp_i[2];

always @(*) begin
  case (ALUOp_i)
    3'b000:
      case (funct_i)
        6'b100_001: ALUCtrl_o <= 4'b0010;  //addu
        6'b100_011: ALUCtrl_o <= 4'b0110;  //subu
        6'b100_100: ALUCtrl_o <= 4'b0000;  //and
        6'b100_101: ALUCtrl_o <= 4'b0001;  //or
        6'b101_010: ALUCtrl_o <= 4'b0111;  //slt
        6'b000_011: ALUCtrl_o <= 4'b0011;  //sra
        6'b000_111: ALUCtrl_o <= 4'b0100;  //srav
        6'b011_000: ALUCtrl_o <= 4'b1000;  //mul
        default: ALUCtrl_o <= 4'b0000;
      endcase
    3'b001: ALUCtrl_o <= 4'b0010;  //addi lw sw
    3'b010: ALUCtrl_o <= 4'b0111;  //sltiu
    3'b011: ALUCtrl_o <= 4'b0110;  //beq bne
    3'b100: ALUCtrl_o <= 4'b0101;  //lui
    3'b101: ALUCtrl_o <= 4'b0001;  //ori
    default: ALUCtrl_o <= 4'b0000;
  endcase
end

endmodule
