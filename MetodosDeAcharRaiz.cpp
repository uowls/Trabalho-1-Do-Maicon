#include "../Trabalho/ResolverdorDeMatrizes3000.cpp"
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
    double x = a;
    while(abs(x - ((a+b)/2)) > epsilon1 || abs(f(((a+b)/2))) > epsilon2){
        x = ((a+b)/2);
        if((f(a) > 0) ^ (f(x) > 0)){
            b = x;
        }
        else{
            a = x;
        }
    }
    return x;
}

double FalsaPosicao(double epsilon1, double epsilon2, double a, double b, double (*f)(double)){
    double x = a;
    double x1 = (a*f(b) - b*f(a))/(f(b) - f(a));
    while(abs(x - x1) > epsilon1 || abs(f(x1)) > epsilon2){
        x = x1;
        x1 = (a*f(b) - b*f(a))/(f(b) - f(a));
        if((f(a) > 0) ^ (f(x1) > 0)){
            b = x1;
        }
        else{
            a = x1;
        }
    }
    return x1;
}

double Newton(double epsilon1, double epsilon2, double x0 ,double (*f)(double), double (*fDerivada)(double)){
    double x = x0;
    double x1 = x0 - (f(x0)/fDerivada(x0));
    while(abs(x - x1) > epsilon1 || abs(f(x1)) > epsilon2){
        x = x1;
        x1 = x1 -(f(x1)/fDerivada(x1));
    }
    return x1;
}

double Secante(double epsilon1, double epsilon2, double x0, double x1 ,double (*f)(double)){
    double x2 = x0;
    double x3 = x1;
    double x4;
    while(abs(x2 - x3) > epsilon1 || abs(f(x3)) > epsilon2){
        x4 = x3;
        x3 = ((x2*f(x3)) - (x3*f(x2)))/(f(x3)-f(x2));
        x2 = x4;
    }
    return x3;
}

double PontoFixo(double epsilon1, double epsilon2, double x0 ,double (*f)(double), double (*fIteracao)(double)){
    double x = x0;
    while(abs(x - fIteracao(x)) > epsilon1 || abs(f(x)) > epsilon2){
        x = fIteracao(x);
    }
    return x;
}


double g(double x){
    return x*x*x - x - 1;
}

double glinha(double x){
    return cbrt(x+1);
}

int main(){
    std::cout << PontoFixo(0.00000000000001, 0.000000000000001, 2, *g, *glinha);
}