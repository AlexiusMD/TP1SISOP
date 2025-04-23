# Scheduler in C - SISOP T1

Esse é o trabalho 1 da disciplina de Sistemas Operacionais, ministrada pelo professor Fabiano Hessel. Se trata de uma implementação em C de um escalonador seguindo a regra de EDF (Earliest Deadline First).

## Parser/Compiler

Foi disponibilizado pelo professor dois programas em uma linguagem "pseudo-assembly" (disponíveis na pasta programs), simulando um sistema com dois registradores, sendo estes o `Program Counter` e um `Accumulator`.

Para a implementação, existem os seguintes tipos de instrução:

> Operações aritméticas
- ADD -> Soma o operando ao acumulador - ADD op
- SUB -> Subtrai o valor do acumulador - SUB op
- DIV -> Divide o acumulador pelo operando - DIV op
- MULT -> Multiplica o acumulador pelo operando - MULT op

> Operações de memória
- LOAD -> Carrega um valor de variável no acumulador - LOAD op
- STORE -> Salva o valor do acumulador em uma variável - STORE op

> Operações de salto
- BRZERO -> Salta para um rótulo se o acumulador for = 0 - BRZERO op
- BRPOS -> Salta para um rótulo se o acumulador for > 0 - BRPOS op
- BRNEG -> Salta para um rótulo se o acumulador for < 0 - BRNEG op
- BRANY -> Salta incondicionalmente para um rótulo - BRANY op

> Chamadas de sistema
- SYSCALL -> Faz uma chamada de sistema, podendo incluir as operações: - SYSCALL op
    - 0 -> Finaliza o programa
    - 1 -> Imprime um inteiro na tela
    - 2 -> Lê um inteiro do usuário


O programa é dividido em uma seção de instruções/labels e outra de dados, bastante parecido com o funcionamento de uma linguagem de montagem tradicional, mas principalmente inspirada por Assembly MIPS.

## Scheduler

A implementação do escalonador em si funciona a partir do carregamento/instanciamento de uma tarefa em memória, sendo sempre executada a tarefa com menor *deadline* no instante *t*, sendo ordenadas a partir de uma fila de prioridade. O *deadline* da tarefa, neste caso, é exatamente igual ao seu *período*, sendo que o *deadline absoluto* de uma tarefa é igual ao seu **tempo de chegada + período** (ambos são definidos pelo usuário).