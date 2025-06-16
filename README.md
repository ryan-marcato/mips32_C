#Documentação

##codificadores.c
Tem como objetivo pegar as instuções e converter para o código binario da mips32
Exemplo add $t1, $t2, $t3 em uma sequência de 32 bits (zeros e uns) que o processador pode entender diretamente. 
O código faz isso para os três formatos principais de instrução MIPS: Tipo-R, Tipo-I e Tipo-J.
