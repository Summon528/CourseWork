//Subject:     CO project 1 - Simple Single CPU
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      0611262 曾長彥
//----------------------------------------------
//Date:        2018/07/14
//----------------------------------------------
//Description:
//--------------------------------------------------------------------------------
module Simple_Single_CPU(
        clk_i,
        rst_i
        );

//I/O port
input         clk_i;
input         rst_i;

//Internal Signles
wire [31:0] pc_in;
wire [31:0] pc_out;

//Greate componentes
ProgramCounter PC(
        .clk_i(clk_i),
        .rst_i (rst_i),
        .pc_in_i(pc_in) ,
        .pc_out_o(pc_out)
        );

wire[31:0] pc_out_add;
Adder Adder1(
        .src1_i(32'd4),
        .src2_i(pc_out),
        .sum_o(pc_out_add)
        );

wire[31:0] instr;
Instr_Memory IM(
        .pc_addr_i(pc_out),
        .instr_o(instr)
        );

wire reg_dst;
wire[4:0] write_reg_1;
MUX_2to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(instr[20:16]),
        .data1_i(instr[15:11]),
        .select_i(reg_dst),
        .data_o(write_reg_1)
        );

wire[31:0] write_data;
wire reg_write;
wire[31:0] read_data_1;
wire[31:0] read_data_2;
Reg_File RF(
        .clk_i(clk_i),
        .rst_i(rst_i) ,
        .RSaddr_i(instr[25:21]),
        .RTaddr_i(instr[20:16]),
        .RDaddr_i(write_reg_1),
        .RDdata_i(write_data),
        .RegWrite_i (reg_write),
        .RSdata_o(read_data_1),
        .RTdata_o(read_data_2)
        );

wire[2:0] alu_op;
wire alu_src;
wire branch;
Decoder Decoder(
        .instr_op_i(instr[31:26]),
        .RegWrite_o(reg_write),
        .ALU_op_o(alu_op),
        .ALUSrc_o(alu_src),
        .RegDst_o(reg_dst),
        .Branch_o(branch)
        );

wire[3:0] alu_ctl;
ALU_Ctrl AC(
        .funct_i(instr[5:0]),
        .ALUOp_i(alu_op),
        .ALUCtrl_o(alu_ctl)
        );

wire [31:0] sign_extended;
Sign_Extend SE(
        .data_i(instr[15:0]),
        .data_o(sign_extended)
        );

wire [31:0] read_data_2_mux;
MUX_2to1 #(.size(32)) Mux_ALUSrc(
        .data0_i(read_data_2),
        .data1_i(sign_extended),
        .select_i(alu_src),
        .data_o(read_data_2_mux)
        );

wire zero;
ALU ALU(
        .src1_i(read_data_1),
        .src2_i(read_data_2_mux),
        .ctrl_i(alu_ctl),
        .result_o(write_data),
        .zero_o(zero),
        .shamt_i(instr[10:6])
        );

wire [31:0] shifted;
wire [31:0] pc_jumped;
Adder Adder2(
        .src1_i(pc_out_add),
        .src2_i(shifted),
        .sum_o(pc_jumped)
        );

Shift_Left_Two_32 Shifter(
        .data_i(sign_extended),
        .data_o(shifted)
        );

MUX_2to1 #(.size(32)) Mux_PC_Source(
        .data0_i(pc_out_add),
        .data1_i(pc_jumped),
        .select_i(zero & branch),
        .data_o(pc_in)
        );

Data_Memory  DM(
        .clk_i(),
	.addr_i(),
	.data_i(),
	.MemRead_i(),
	.MemWrite_i(),
	.data_o()
);

endmodule
