#
Documentação

##
- codificadores.c: Tem como objetivo pegar as instuções e converter para o código binario da mips32
Exemplo add $t1, $t2, $t3 em uma sequência de 32 bits (zeros e uns) que o processador pode entender diretamente. 
O código faz isso para os três formatos principais de instrução MIPS: Tipo-R, Tipo-I e Tipo-J.

- executores.c: A sua função é ler instruções já decodificadas e executar as operações que elas representam, modificando o estado dos registradores e da memória, exatamente como um processador real faria.

- label.c: Tem como objetivo armazenar os nomes dos rótulos e os endereços de memória correspondentes para que possam ser encontrados posteriormente.

- memoria.c: Ele gerencia duas áreas : a memória de dados, onde o programa armazena variáveis e strings, e a memória de instruções, onde o código a ser executado é guardado. O código define funções para ler e escrever dados nessas memórias de forma segura.
