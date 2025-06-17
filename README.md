#
Documentação

##
- codificadores.c: Tem como objetivo pegar as instuções e converter para o código binario da mips32
Exemplo add $t1, $t2, $t3 em uma sequência de 32 bits (zeros e uns) que o processador pode entender diretamente. 
O código faz isso para os três formatos principais de instrução MIPS: Tipo-R, Tipo-I e Tipo-J.

- executores.c: A sua função é ler instruções já decodificadas e executar as operações que elas representam, modificando o estado dos registradores e da memória, exatamente como um processador real faria.

-label.c: Tem como objetivo armazenar os nomes dos rótulos e os endereços de memória correspondentes para que possam ser encontrados posteriormente.
