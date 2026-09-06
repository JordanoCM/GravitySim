		FORMAT	ELF64
		PUBLIC	aceleracaofast
                PUBLIC  velocidadefast
                PUBLIC  posicaofast
		SECTION	'.text'
velocidadefast:
        movlps xmm0, [RDI+20]
        movhps xmm0, [RSI+20]
        movss xmm1, [RDX]
        movss [RCX], xmm1
        movss [RCX+4], xmm1
        movss [RCX+8], xmm1
        movss [RCX+12], xmm1
        movaps xmm1, [RCX]
        mulps xmm0, xmm1
        movlps xmm1, [RDI+12]
        movhps xmm1, [RSI+12]
        addps xmm0, xmm1
        movlps [RDI+12], xmm0
        movhps [RSI+12], xmm0
        RET
posicaofast:
        movss xmm0, [RDX]
        movss [RCX], xmm0
        movss [RCX+4], xmm0
        movss [RCX+8], xmm0
        movss [RCX+12], xmm0
        movaps xmm0, [RCX]
        mulps xmm0, xmm0
        MOV RAX, 2
        cvtsi2ss xmm1, RAX
        movss [RCX], xmm1
        movss [RCX+4], xmm1
        movss [RCX+8], xmm1
        movss [RCX+12], xmm1
        movaps xmm1, [RCX]
        divps xmm0, xmm1
        movlps xmm1, [RDI+20]
        movhps xmm1, [RSI+20]
        mulps xmm0, xmm1
        movlps xmm1, [RDI+4]
        movhps xmm1, [RSI+4]
        addps xmm0, xmm1 
        movlps xmm1, [RDI+12]
        movhps xmm1, [RSI+12]
        movss xmm2, [RDX]
        movss [RCX], xmm2
        movss [RCX+4], xmm2
        movss [RCX+8], xmm2
        movss [RCX+12], xmm2
        movaps xmm2, [RCX]
        mulps xmm1, xmm2
        addps xmm0, xmm1
        movlps [RDI+4], xmm0
        movhps [RSI+4], xmm0
        RET
aceleracaofast:
        movss xmm0, DWORD[RSI]
        movss [RCX], xmm0
        movss [RCX+4], xmm0
        movss xmm0, DWORD[RDI]
        movss [RCX+8], xmm0
        movss [RCX+12], xmm0
        movss xmm0, [RDX]
        movaps xmm1, [RCX]
        movss [RCX], xmm0
        movss [RCX+4], xmm0
        movss [RCX+8], xmm0
        movss [RCX+12], xmm0
        movaps xmm0, [RCX]
        mulps xmm1, xmm0
        movlps xmm0,[RDI+4]
        movlps xmm2,[RSI+4]
        subps xmm0, xmm2
        movlps [RCX], xmm0
        mulps xmm0, xmm0
        movlps [RCX], xmm0
        movss xmm2, [RCX]
        movss [RCX+8], xmm2
        movss xmm2, [RCX+4]
        movss [RCX+12], xmm2
        movaps xmm0, [RCX]
        divps xmm1, xmm0
        movaps [RCX], xmm1
        movss xmm0, [RDI+4]
        MOV RAX, -1
        comiss xmm0, [RSI+4]
        JE zerar1
        JA negativar1
        JB negativar2
negativado1:
        movss xmm0, [RDI+8]
        comiss xmm0, [RSI+8]
        JE zerar2
        JA negativar3
        JB negativar4
negativado2:
        xorps xmm1, xmm1
        movlps xmm1, [RDI+20]
        movhps xmm1, [RSI+20]
        addps xmm1, [RCX]
        movlps [RDI+20], xmm1
        movhps [RSI+20], xmm1
        RET
negativar1:
        cvtsi2ss xmm0, RAX
        mulss xmm0, DWORD[RCX]
        movss DWORD[RCX], xmm0 
        JMP negativado1
negativar2:
        cvtsi2ss xmm0, RAX
        mulss xmm0, DWORD[RCX+8]
        movss DWORD[RCX+8], xmm0
        JMP negativado1
negativar3:
        cvtsi2ss xmm0, RAX
        mulss xmm0, DWORD[RCX+4]
        movss DWORD[RCX+4], xmm0
        JMP negativado2
negativar4:
        cvtsi2ss xmm0, RAX
        mulss xmm0, DWORD[RCX+12]
        movss DWORD[RCX+12], xmm0
        JMP negativado2
zerar1:
        xorps xmm2, xmm2
        movss [RCX], xmm2
        movss [RCX+8], xmm2
        JMP negativado1
zerar2:
        xorps xmm2, xmm2
        movss [RCX+4], xmm2
        movss [RCX+12], xmm2
        JMP negativado2