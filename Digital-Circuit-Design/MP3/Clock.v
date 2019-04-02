`timescale 100ms/10ms
module BCD_counter(count,TC,clr_, enb, clk) ;
output reg [3:0] count ;
output reg TC ;
input clr_, enb, clk ;

always @ (*)
	if(enb && count >= 9)
		TC = 1;
	else
		TC = 0;
	
always @ (posedge clk or negedge clr_) // combinational+sequential
	if (~clr_)
		count = 0 ;
	else if (enb)
		if (TC)
			count = 0;
		else
			count = count + 1;
endmodule

module Hex_counter(count,TC,clr_, enb, clk) ;
output reg [2:0] count ;
output reg TC ;
input clr_, enb, clk ;

always @ (*)
	if(enb && count >= 5)
		TC = 1;
	else
		TC = 0;
		
always @ (posedge clk or negedge clr_) // combinational+sequential
	if (~clr_)
		count = 0 ;
	else if (enb)
		if (TC)
			count = 0;
		else
			count = count + 1 ;
endmodule

module H1_counter(count,TC,clr_, enb, clk) ;
output reg [1:0] count ;
output reg TC ;
input clr_, enb, clk ;

always @ (*)
	if(enb && count >= 2)
		TC = 1;
	else
		TC = 0;
		
always @ (posedge clk or negedge clr_) // combinational+sequential
	if (~clr_)
		count = 0 ;
	else if (enb)
		if (TC)
			count = 0;
		else
			count = count + 1 ;
endmodule

module H0_counter(count,TC,clr_, enb, clk, h11) ;
output reg [3:0] count ;
output reg TC ;
input clr_, enb, clk, h11 ;

always @ (*)
	if(enb && count >= (h11 ? 3 : 9))
		TC = 1;
	else
		TC = 0;
	
always @ (posedge clk or negedge clr_) // combinational+sequential
	if (~clr_)
		count = 0 ;
	else if (enb)
		if (TC)
			count = 0;
		else
			count = count + 1;
endmodule


module Testfixture ;
wire Vdd = 1'b1;
reg clk, clr_;
wire [3:0] m0 ;
wire [2:0] m1 ;
wire [3:0] h0 ;
wire [1:0] h1 ;

BCD_counter M0 (m0,m0_tc,clr_, Vdd, clk) ;
Hex_counter M1 (m1,m1_tc,clr_, m0_tc, clk) ;
H1_counter H1 (h1,h1_tc,clr_, h0_tc, clk) ;
H0_counter H0 (h0,h0_tc,clr_, m1_tc, clk, h1[1]) ;


//Stimulus
initial
begin
	clk = 1'b0 ;
	repeat(4800)
		#5 clk = ~clk ;
	$finish ;
end
	
initial
begin
  clr_ = 0;
  #6  clr_ = 1'b1;  
end

initial
begin
$dumpfile("Clock.vcd");
$dumpvars;
end

endmodule
