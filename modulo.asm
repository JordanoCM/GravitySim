FORMAT ELF64
PUBLIC aceleracaofast
PUBLIC velocidadefast
PUBLIC posicaofast

SECTION '.text'

; ---------------------------------------------------------------------
; void aceleracaofast(float *p_i, float *p_j, float *G, float *aux)
; Utiliza floats empacotados (SSE) para calcular a força gravitacional 2D
; ---------------------------------------------------------------------
aceleracaofast:
        ; Carrega posições r_i [x_i, y_i] e r_j [x_j, y_j] usando movlps (empacotados)
        movlps  xmm1, [RDI+4]       ; xmm1 = [x_i, y_i]
        movlps  xmm2, [RSI+4]       ; xmm2 = [x_j, y_j]

        ; Vetor distância dr = r_j - r_i em uma única instrução vetorial (subps)
        subps   xmm2, xmm1          ; xmm2 = [dx, dy]

        ; Eleva dx e dy ao quadrado em paralelo (mulps)
        movaps  xmm3, xmm2          
        mulps   xmm3, xmm3          ; xmm3 = [dx^2, dy^2]

        ; Soma dx^2 + dy^2 para obter r^2
        movaps  xmm4, xmm3
        shufps  xmm4, xmm4, 1       ; move dy^2 para a parte baixa
        addss   xmm3, xmm4          ; xmm3 (baixo) = r^2

        ; Evita divisão por zero
        xorps   xmm5, xmm5
        ucomiss xmm3, xmm5
        je      .skip_accel

        ; r = sqrt(r^2)
        sqrtss  xmm5, xmm3          
        movaps  xmm6, xmm3          
        mulss   xmm6, xmm5          ; r^3 = r^2 * r

        ; Fator escalar = (G * m_j) / r^3
        movss   xmm0, [RDX]         ; G
        movss   xmm1, [RSI]         ; m_j
        mulss   xmm0, xmm1          ; G * m_j
        divss   xmm0, xmm6          ; fator

        ; Replicar o fator em ambas as pistas do registrador (empacotado)
        shufps  xmm0, xmm0, 0       ; xmm0 = [fator, fator]

        ; Aceleração vetorial acumulada = fator * dr (usando mulps empacotado)
        mulps   xmm2, xmm0          ; xmm2 = [fator * dx, fator * dy]

        ; Acumula na aceleração da partícula i ([RDI+20] contendo [acc_x, acc_y])
        movlps  xmm7, [RDI+20]      
        addps   xmm7, xmm2          ; addps empacotado
        movlps  [RDI+20], xmm7      

.skip_accel:
        ret

; ---------------------------------------------------------------------
; void velocidadefast(float *p1, float *p2, float *ms, float *aux)
; Atualiza velocidades usando floats empacotados [v_x, v_y] e [a_x, a_y]
; ---------------------------------------------------------------------
velocidadefast:
        movss   xmm0, [RDX]         ; dt
        shufps  xmm0, xmm0, 0       ; empacota dt nas duas pistas

        ; Atualiza p1
        movlps  xmm1, [RDI+20]      ; [a_x, a_y]
        mulps   xmm1, xmm0          ; a * dt (empacotado)
        movlps  xmm2, [RDI+12]      ; [v_x, v_y]
        addps   xmm2, xmm1          ; v + a*dt (empacotado)
        movlps  [RDI+12], xmm2

        ; Atualiza p2
        movlps  xmm1, [RSI+20]      ; [a_x, a_y]
        mulps   xmm1, xmm0          ; a * dt (empacotado)
        movlps  xmm2, [RSI+12]      ; [v_x, v_y]
        addps   xmm2, xmm1          ; v + a*dt (empacotado)
        movlps  [RSI+12], xmm2
        ret

; ---------------------------------------------------------------------
; void posicaofast(float *p1, float *p2, float *ms, float *aux)
; Atualiza posições usando floats empacotados [pos_x, pos_y], [v_x, v_y], [a_x, a_y]
; ---------------------------------------------------------------------
posicaofast:
        movss   xmm0, [RDX]         ; dt
        movaps  xmm9, xmm0          
        mulss   xmm9, xmm9          ; dt^2
        movss   xmm8, [.half]       
        mulss   xmm9, xmm8          ; 0.5 * dt^2
        
        shufps  xmm0, xmm0, 0       ; dt empacotado
        shufps  xmm9, xmm9, 0       ; 0.5*dt^2 empacotado

        ; Atualiza p1
        movlps  xmm1, [RDI+12]      ; [v_x, v_y]
        mulps   xmm1, xmm0          ; v * dt
        movlps  xmm2, [RDI+20]      ; [a_x, a_y]
        mulps   xmm2, xmm9          ; 0.5 * a * dt^2
        addps   xmm1, xmm2          
        movlps  xmm3, [RDI+4]       ; [pos_x, pos_y]
        addps   xmm3, xmm1          ; pos + v*dt + 0.5*a*dt^2 (empacotado)
        movlps  [RDI+4], xmm3

        ; Atualiza p2
        movlps  xmm1, [RSI+12]      ; [v_x, v_y]
        mulps   xmm1, xmm0          ; v * dt
        movlps  xmm2, [RSI+20]      ; [a_x, a_y]
        mulps   xmm2, xmm9          ; 0.5 * a * dt^2
        addps   xmm1, xmm2          
        movlps  xmm3, [RSI+4]       ; [pos_x, pos_y]
        addps   xmm3, xmm1          ; pos + v*dt + 0.5*a*dt^2 (empacotado)
        movlps  [RSI+4], xmm3
        ret

SECTION '.data'
.half   dd 0.5