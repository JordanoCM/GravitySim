EXECUTAVEL=main

c:  
	gcc mainDefault.c -o $(EXECUTAVEL)
otim: 
	gcc -c mainOtim.c
	gcc -no-pie -o $(EXECUTAVEL) mainOtim.o modulo.o
asm:
	fasm modulo.asm
clean: 
	rm -rf *.o $(EXECUTAVEL)
	
run:
	./$(EXECUTAVEL)