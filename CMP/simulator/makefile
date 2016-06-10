single_cycle: main.o data_io.o output.o CMP.o D_CMP.o
	gcc -o CMP main.o data_io.o output.o CMP.o D_CMP.o
main.o: main.c var.h data_io.h output.h CMP.h  D_CMP.h 
	gcc -c main.c
data_io.o: var.h data_io.h
	gcc -c data_io.c
output.o: output.h var.h
	gcc -c output.c
CMP.o: CMP.h var.h
	gcc -c CMP.c 
D_CMP.o: D_CMP.h var.h
	gcc -c D_CMP.c
clean:
	 rm -f CMP main.o data_io.o output.o CMP.o D_CMP.o
