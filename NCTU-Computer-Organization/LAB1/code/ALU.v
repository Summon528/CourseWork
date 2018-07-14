//Subject:     CO project 1 - ALU
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      0611262 曾長彥
//----------------------------------------------
//Date:        2018/07/14
//----------------------------------------------
//Description:
//--------------------------------------------------------------------------------

module ALU(
    src1_i,
	src2_i,
	ctrl_i,
	result_o,
	zero_o,
	shamt_i
	);

//I/O ports
input  [32-1:0]  src1_i;
input  [32-1:0]	 src2_i;
input  [4-1:0]   ctrl_i;
input  [4:0] shamt_i;

output [32-1:0]	 result_o;
output           zero_o;

//Internal signals
reg    [32-1:0]  result_o;
wire             zero_o;

//Parameter

//Main function
assign zero_o = (result_o == 0);
always @(*) begin
  case(ctrl_i)
	0: result_o <= src1_i & src2_i;
	1: result_o <= src1_i | src2_i;
	2: result_o <= src1_i + src2_i;
	3: result_o <= $signed(src2_i) >>> shamt_i;
	4: result_o <= $signed(src2_i) >>> src1_i;
	5: result_o <= src2_i << 16;
	6: result_o <= src1_i - src2_i;
	7: result_o <= src1_i < src2_i ? 1 : 0;
	8: result_o <= (src1_i == src2_i) ? 1 : 0;
	12: result_o <= ~(src1_i | src2_i);
	default: result_o <= 0;
  endcase
end
endmodule
