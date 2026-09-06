EXECUTAVEL=main

# Detecta automaticamente o ambiente virtual (local ou no diretório home)
PYTHON = python3
ifneq ($(wildcard .venv/bin/python),)
    PYTHON = .venv/bin/python
else ifneq ($(wildcard venv/bin/python),)
    PYTHON = venv/bin/python
else ifneq ($(wildcard $(HOME)/gravity_env/bin/python),)
    PYTHON = $(HOME)/gravity_env/bin/python
endif

c:  
	gcc mainDefault.c -o $(EXECUTAVEL) -lm

otim: asm
	gcc -O3 -c mainOtim.c
	gcc -no-pie -o $(EXECUTAVEL) mainOtim.o modulo.o

asm:
	fasm modulo.asm

clean: 
	rm -rf *.o $(EXECUTAVEL)
	
run:
	./$(EXECUTAVEL) < entrada.txt
	$(PYTHON) animar.py