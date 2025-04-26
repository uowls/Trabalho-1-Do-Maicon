#include "../Trabalho/ResolverdorDeMatrizes3000.cpp"
using matriz = Matriz::matriz<double>;
using shublau = double;
//A1:
//Float: 1.19209e-07
//A2:
//Double: 2.22045e-16

//B:
//Passo 3 é porque A, no fim do programa, é o primeiro valor, multiplo de 1/2 que não satisfaz a condição de 1+A > 1, então 2*A, o valor anterior, é o ultimo tal que 1 + prec > 1 => 2*A é a precisão de máquina, por definição


/*int main(){
    shublau A = 1;
    shublau s = 2;

    while(s > 1){
        A = A/2;
        s = 1 + A;
    }
    shublau prec = 2*A;
    std::cout << prec << std::endl;
}*/ // Programa usado para A1, A2 e B

shublau Precisao(shublau val){
    shublau A = 1;
    shublau s = val + A;
    while(s > val){
        A /= 2;
        s = A + val;
    }
    shublau prec = 2*A;
    return prec;
}

//C1:
/*
10: 1.77636e-15
17: 3.55271e-15
100: 1.42109e-14
184: 2.84217e-14
1000: 1.13687e-13
1575: 2.27374e-13
10000: 1.81899e-12
17893: 3.63798e-12*/

//C2: Porquue, devido à natureza da representação por float, numeros maiores tem uma densidade de valores relativamente menor, que implica em menos numeros serem representáveis, que implica nua diminuição da precisão (aumentando o valor em que dois numeros sao indistinguiveis) a cada ṕotencia de 2

int main(){
    shublau input;
    std::string I = "";
    std::cin >> I;
    input = std::stod(I);
    std::cout << Precisao(input);



    return 0;
}
