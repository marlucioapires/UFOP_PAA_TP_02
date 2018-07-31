# UFOP_PAA_TP_02
Segundo trabalho prático da disciplina Projeto e Análise de Algoritmos, semestre 2018/01, do mestrado acadêmico em Ciência da Computação da UFOP.

Compilar o código: <br />
    gcc -std=c99 -Wall -g *.c -lm -o binario
    
Exemplo de execução: <br />
    ./binario bpinstances/da-BPWC_1_9_9.txt bpinstances/da-BPWC_1_9_9_02.sol 60 m=1 d=1 s=1 t=1000000 a=0.99999 e=0.00001
    
onde os parâmetros são: <br />
     m=1|2|3        (Informe 1 para First-Fit, 2 para Best-Fit ou 3 para Worst-Fit.)<br />
     s=0|1          (Informe 0 para não executar o simulated annealing ou 1 para executá-lo.)<br />
     d=0|1          (Informe 0 para não ordenar os itens ou 1 para ordenar.)<br />
     a=0..1         (O valor de alpha, entre 0 e 1. Valor default: 0,999.)<br />
     e=0..infinito  (O valor de epsilon, entre 0 e infinito. Valor defult: 0,001.)<br />
     t=0..infinito  (O valor da temperatura, entre 0 e infinito. Valor defult: (Lower Bound)².)<br />
