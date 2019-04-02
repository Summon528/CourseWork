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
        .addr_i(pc_out),
        .instr_o(instr)
        );

wire[1:0] reg_dst;
wire[4:0] write_reg_1;
MUX_4to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(instr[20:16]),
        .data1_i(instr[15:11]),
        .data2_i(5'b11111),
        .select_i(reg_dst),
        .data_o(write_reg_1)
        );

wire[31:0] alu_result;
wire reg_write;
wire[31:0] read_data_1;
wire[31:0] read_data_2;
wire[31:0] write_data;
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
wire[1:0] mem_to_reg;
wire jump;
wire mem_read;
wire mem_write;
wire[1:0] branch_type;

Decoder Decoder(
        .instr_op_i(instr[31:26]),
        .RegWrite_o(reg_write),
        .ALU_op_o(alu_op),
        .ALUSrc_o(alu_src),
        .RegDst_o(reg_dst),
        .Branch_o(branch),
        .MemToReg_o(mem_to_reg),
        .Jump_o(jump),
        .MemRead_o(mem_read),
        .MemWrite_o(mem_write),
        .BranchType_o(branch_type)
        );

wire[3:0] alu_ctl;
wire      jr_ctl;
ALU_Ctrl AC(
        .funct_i(instr[5:0]),
        .ALUOp_i(alu_op),
        .ALUCtrl_o(alu_ctl),
        .JR_o(jr_ctl)
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
        .result_o(alu_result),
        .zero_o(zero),
        .shamt_i(instr[10:6])
        );

wire [31:0] shifted;
wire [31:0] pc_offsetted;
Adder Adder2(
        .src1_i(pc_out_add),
        .src2_i(shifted),
        .sum_o(pc_offsetted)
        );

Shift_Left_Two_32 Shifter(
        .data_i(sign_extended),
        .data_o(shifted)
        );

wire branch_mux_o;
wire [31:0] pc_mux_o;
MUX_2to1 #(.size(32)) Mux_PC_Source(
        .data0_i(pc_out_add),
        .data1_i(pc_offsetted),
        .select_i(branch_mux_o & branch),
        .data_o(pc_mux_o)
);

wire [31:0] dm_out;
Data_Memory  Data_Memory(
        .clk_i(clk_i),
	.addr_i(alu_result),
	.data_i(read_data_2),
	.MemRead_i(mem_read),
	.MemWrite_i(mem_write),
	.data_o(dm_out)
);


wire [31:0] j_shift_2;

Shift_Left_Two_32 Shifter2(
        .data_i({6'b000000, instr[25:0]}),
        .data_o(j_shift_2)
);


wire [31:0] mux_pc_src2_o;
MUX_2to1 #(.size(32)) Mux_PC_Source2(
        .data0_i({pc_out_add[31:27], j_shift_2[26:0]}),
        .data1_i(pc_mux_o),
        .select_i(jump),
        .data_o(mux_pc_src2_o)
);

MUX_2to1 #(.size(32)) Mux_PC_Source3(
        .data0_i(mux_pc_src2_o),
        .data1_i(read_data_1),
        .select_i(jr_ctl),
        .data_o(pc_in)
);

MUX_4to1 #(.size(1)) Mux_Branch_Type(
        .data0_i(zero),
        .data1_i(!zero),
        .data2_i(zero | alu_result[31]),
        .data3_i(alu_result[31]),
        .select_i(branch_type),
        .data_o(branch_mux_o)
);

MUX_4to1 #(.size(32)) Mux_Write_Data(
        .data0_i(alu_result),
        .data1_i(dm_out),
        .data2_i(sign_extended),
        .data3_i(pc_out_add),
        .select_i(mem_to_reg),
        .data_o(write_data)
);

endmodule
