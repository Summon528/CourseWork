TARGET = Single_Cycle_CPU

all: $(TARGET)

$(TARGET): *.v
	iverilog -o $(TARGET) *.v
	vvp $(TARGET)

.PHONY: clean

clean:
	$(RM) $(TARGET) $(TARGET).vcd