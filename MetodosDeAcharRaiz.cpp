#include "ResolverdorDeMatrizes3000.cpp"
#include <iomanip> 
#include <cmath>
using matriz = Matriz::matriz<double>;


double abs(double x){
    return (x<0) ? -x : x;
}

double sqrt(double x){
    double y = x;
    for(unsigned long long int i = 0; i < 100; ++i){
        y = y - (y*y - x)/(2*y);
    }
    return y;
}

double cbrt(double x){
    double y = x;
    for(unsigned long long int i = 0; i < 100; ++i){
        y = y - (y*y*y - x)/(3*y*y);
    }
    return y;
}

double Bisecao(double epsilon1, double epsilon2, double a, double b, double (*f)(double)){
    unsigned long long int i = 0;
    double x = a;
    while(abs(x - ((a+b)/2)) > epsilon1 || abs(f(((a+b)/2))) > epsilon2){
        ++i;
        x = ((a+b)/2);
        if((f(a) > 0) ^ (f(x) > 0)){
            b = x;
        }
        else{
            a = x;
        }
    }
    std::cout << i << "\n";
    return x;
}

double FalsaPosicao(double epsilon1, double epsilon2, double a, double b, double (*f)(double)){
    unsigned long long int i = 0;
    double x = a;
    double x1 = (a*f(b) - b*f(a))/(f(b) - f(a));
    while(abs(x - x1) > epsilon1 || abs(f(x1)) > epsilon2){
        ++i;
        x = x1;
        x1 = (a*f(b) - b*f(a))/(f(b) - f(a));
        if((f(a) > 0) ^ (f(x1) > 0)){
            b = x1;
        }
        else{
            a = x1;
        }
    }
    std::cout << i << "\n";
    return x1;
}

double Newton(double epsilon1, double epsilon2, double x0 ,double (*f)(double), double (*fDerivada)(double)){
    unsigned long long int i = 0;
    double x = x0;
    double x1 = x0 - (f(x0)/fDerivada(x0));
    while(abs(x - x1) > epsilon1 || abs(f(x1)) > epsilon2){
        ++i;
        x = x1;
        x1 = x1 -(f(x1)/fDerivada(x1));
    }
    std::cout << i << "\n";
    return x1;
}

double Secante(double epsilon1, double epsilon2, double x0, double x1 ,double (*f)(double)){
    unsigned long long int i = 0;
    double x2 = x0;
    double x3 = x1;
    double x4;
    while(abs(x2 - x3) > epsilon1 || abs(f(x3)) > epsilon2){
        ++i;
        x4 = x3;
        x3 = ((x2*f(x3)) - (x3*f(x2)))/(f(x3)-f(x2));
        x2 = x4;
    }
    std::cout << i << "\n";
    return x3;
}

double PontoFixo(double epsilon1, double epsilon2, double x0 ,double (*f)(double), double (*fIteracao)(double)){
    unsigned long long int i = 0;
    double x = x0;
    while(abs(x - fIteracao(x)) > epsilon1 || abs(f(x)) > epsilon2){
        ++i;
        x = fIteracao(x);
    }
    std::cout << i << "\n";
    return x;
}


double g(double x){
    return x*(std::log10(x)) - 1;
}

double goutro(double x){
    return (std::log(x) + 1)/std::log(10);
}

/*
Exercício 19:
    Bisecção:
        Iterações: 47
        Valor: 1.324718
        F(Valor): 1.998401e-14

    Posição Falsa:
        Iterações: 39
        Valor: 1.324718
        F(Valor): -5.551115e-15

    Método do ponto fixo:
        Iterações: 20
        Valor: 1.324718
        F(Valor): -5.551115e-15

    Newton:
        Iterações: 22
        Valor: 1.324718
        F(Valor): 2.220446e-16

    Secante:
        Iterações: 29
        Valor: 1.324718
        F(Valor): -8.881784e-16

Exercício 21:
    Bisecção:
        Iterações: 46
        Valor: 2.506184
        F(Valor): -6.217249e-15

    Posição Falsa:
        Iterações: 12
        Valor: 2.506184
        F(Valor): 0

    Método do ponto fixo:
        Iterações: 11
        Valor: 2.506184
        F(Valor): 6.661338e-15

    Newton:
        Iterações: 3
        Valor: 2.506184
        F(Valor): 0 

    Secante:
        Iterações: 6
        Valor: 2.506184
        F(Valor): 0
*/

int main(){
    //std::cout << std::log(2);
    std::cout << std::setprecision(7);
    double a = Secante(0.00000000000001, 0.00000000000001, 2.3, 2.7,*g);
    std::cout << a << "\n";
    std::cout << (g(a));
}
