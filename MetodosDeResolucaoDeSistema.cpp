#include "../Trabalho/ResolverdorDeMatrizes3000.cpp"
using matriz = Matriz::matriz<double>;

double abs(double x){
    return x < 0 ? -x : x;
}

unsigned long long int abs(long long int x){
    return x < 0 ? (unsigned long long int)-x : (unsigned long long int)x;
}

matriz* fatoracaoLU(matriz Entrada){
    if(Entrada.linhas() != Entrada.colunas()){
        std::__throw_invalid_argument("A matriz não é quadrada");
    }
    matriz* LU = new matriz[2];
    unsigned long long int tamanho = Entrada.colunas();
    LU[0] = matriz::I(tamanho);
    LU[1] = Entrada;
    double mult;
    for(unsigned long long int i = 1; i <= tamanho; ++i){
        for(unsigned long long int k = i + 1; k <= tamanho; ++k){
            mult = LU[1](k,i)/LU[1](i,i);
            LU[1](k) -= (mult)*LU[1](i);
            LU[0](k,i) = (mult);
        }
    }
    return LU;
}

matriz resolvedorDeMatrizesTriangularesSuperiores(matriz A, matriz b){
    if(!Matriz::ETriangularSuperior(A)){
        std::__throw_invalid_argument("A matriz não é triangular superior");
    }
    if(b.colunas() != 1){
        std::__throw_invalid_argument("O vetor não é um vetor");
    }
    matriz x(b.linhas(),1);
    for(unsigned long long int i = b.linhas(); i >= 1; --i){
        double acc = b(i,1);
        for(unsigned long long int j = b.linhas(); j > i; --j){
            acc -= x(j,1)*A(i,j); 
        }
        x(i,1) = acc/A(i,i);
    }
    return x;
}

matriz resolvedorDeMatrizesTriangularesInferiores(matriz A, matriz b){
    if(!Matriz::ETriangularInferior(A)){
        std::__throw_invalid_argument("A matriz não é triangular inferior");
    }
    if(b.colunas() != 1){
        std::__throw_invalid_argument("O vetor não é um vetor");
    }
    matriz x(b.linhas(),1);
    for(unsigned long long int i = 1; i <= b.linhas(); ++i){
        double acc = b(i,1);
        for(unsigned long long int j = 1; j <= i; ++j){
            acc -= x(j,1)*A(i,j); 
        }
        x(i,1) = acc/A(i,i);
    }
    return x;
}

matriz resolvedorDeSistemasPorFatoracaoLU(matriz A, matriz b){
    matriz* temp = fatoracaoLU(A);
    return resolvedorDeMatrizesTriangularesSuperiores(temp[1],resolvedorDeMatrizesTriangularesInferiores(temp[0],b));
}

bool ETridiagonal(matriz inputMatriz){
    if(inputMatriz.linhas() != inputMatriz.colunas()){
        return false;
    }
    for(unsigned long long int i = 0; i < inputMatriz.linhas(); ++i){
        for(unsigned long long int j = 0; j < inputMatriz.colunas(); ++j){
            if(abs((long long int)((long long int)i - (long long int)j)) > 1 && inputMatriz(i+1,j+1) != 0){
                return false;
            }
        }
    }
    return true;
}

matriz reoslvedorDeSistemasTridiagonais(matriz A, matriz b){ //Assumindo que isso de fato resolva sistemas tridiagonais, isso é pra rodar na ordem de 10n passos, onde n é o tamanho da matriz
    if(!ETridiagonal(A)){
        std::__throw_invalid_argument("A matriz não é tridiagonal");
    }
    if(b.colunas() != 1){
        std::__throw_invalid_argument("O vetor não é um vetor");
    }
    matriz x(b.linhas(),1);
    matriz tempMatriz = A;
    matriz tempVetor = b;
    for(unsigned long long int i = 1; i < b.linhas(); ++i){
        tempVetor(i+1,1) -= tempVetor(i,1)*(tempMatriz(i+1,i)/tempMatriz(i,i));
        tempMatriz(i + 1 , i + 1) -= tempMatriz(i,i+1)*(tempMatriz(i+1,i)/tempMatriz(i,i));
    }
    for(unsigned long long int i = b.linhas(); i > 1; --i){
        tempVetor(i - 1,1) -= tempVetor(i,1)*(tempMatriz(i-1,i)/tempMatriz(i,i));
        x(i,1) = tempVetor(i,1)/tempMatriz(i,i);
    }
    x(1,1) = tempVetor(1,1)/tempMatriz(1,1);
    return x;
}

matriz TridiagonalDoMaicon(unsigned long long int n){
    if(n == 0){
        std::__throw_invalid_argument("n não pode ser 0");
    }
    matriz out = matriz::zero(n,n);
    for(unsigned long long int i = 1; i <= n; ++i){
        if(i - 1 > 0){
            out(i,i-1) = -1;
        }
        out(i,i) = 2;
        if(i + 1 <= n){
            out(i,i+1) = -1;
        }
    }
    return out;
}

matriz VetorDoMaicon(unsigned long long int n){
    if(n == 0){
        std::__throw_invalid_argument("n não pode ser 0");
    }
    matriz out = matriz::constante(n,1,(double)((double)8)/((double)((n+1)*(n+1))));
    return out;
}

int main(){
    matriz a = TridiagonalDoMaicon(50);
    matriz b = VetorDoMaicon(50);
    matriz temp = reoslvedorDeSistemasTridiagonais(a,b);
    std::cout << (b|(a*temp)) << "\n" << temp;
    //std::cout << a;
    
}