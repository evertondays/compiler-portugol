inteiro:idade
inteiro:soma
soma<-idade+5
soma<-idade-5
soma<-idade/5
soma<-idade*5
soma<-1+5*(idade/soma)+10-soma
soma<-1-5
soma<-1/5
soma<-1*5

se idade=soma então escreva(soma) senão escreva("soma") fim_se
se idade>soma então escreva(soma) senão escreva("soma") fim_se
se idade<soma então escreva(soma) senão escreva("soma") fim_se
se idade<=soma então escreva(soma) senão escreva("soma") fim_se
se idade>=soma então escreva(soma) senão escreva(soma) fim_se
se idade<>soma então escreva(soma) senão escreva("soma") fim_se
se idade=soma então escreva(soma) senão escreva("soma") fim_se
se idade=soma e soma<>idade ou soma>idade ou soma<idade então escreva(soma) senão escreva("soma") fim_se

se idade=soma e não soma<>idade ou soma>idade ou soma<idade então escreva(soma) senão escreva("soma") fim_se
se idade=soma e não soma<>idade ou (soma>idade ou soma<idade) então escreva(soma) senão escreva("soma") fim_se

inteiro:valor

para 1 até 10 passo 1
escreva("oi")
fim_para

para soma até 10 passo 1
escreva("oi")
fim_para

para 1 até valor passo 1
escreva("oi")
fim_para

para soma até valor
escreva("oi")
fim_para

para 1 até 10
escreva("oi")
fim_para

para soma até 10
escreva("oi")
fim_para

para 1 até valor
escreva("oi")
fim_para

para soma até valor
escreva("oi")
fim_para

para 1 até valor passo 1
    se idade=soma então escreva(soma) senão escreva("soma") fim_se
    se idade=soma então escreva(soma) senão escreva("soma") fim_se
    se idade=soma então escreva(soma) senão leia(soma) fim_se

    para 1 até valor passo 1
        se idade=soma então escreva(soma) senão escreva("soma") fim_se
        se idade=soma então escreva(soma) senão escreva("soma") escreva("soma") escreva("soma") fim_se
        se idade=soma então escreva(soma) senão escreva("soma") fim_se
        para 1 até 10
            escreva("oi")
            se idade=soma então escreva(soma) senão escreva("soma") fim_se
        fim_para
    fim_para
fim_para