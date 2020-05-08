all: sandbox.so

sandbox.so: sandbox.c
	gcc  -Wall -o sandbox.so -shared -fPIC sandbox.c -ldl

clean:
	rm *.so