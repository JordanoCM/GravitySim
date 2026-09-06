EXECUTAVEL=main

c:  
	gcc mainDefault.c -o $(EXECUTAVEL)

otim: asm
	gcc -O3 -c mainOtim.c
	gcc -no-pie -o $(EXECUTAVEL) mainOtim.o modulo.o

asm:
	fasm modulo.asm

clean: 
	rm -rf *.o $(EXECUTAVEL)
	
run:
	./$(EXECUTAVEL) < entrada.txt