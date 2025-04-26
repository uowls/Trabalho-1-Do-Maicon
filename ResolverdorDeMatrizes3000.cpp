#include <string>
#include <iostream>

namespace Matriz{
    template<typename Type = double, typename Enable = void> class matriz{
        static_assert(std::is_same_v<Enable, void>, "Não pode inicializar com mais de um tipo");
        static_assert(std::is_arithmetic_v<Type>, "Precisa ser um número");
    };



    template<typename Type> class matriz<Type, typename std::enable_if_t<std::is_arithmetic<Type>::value>>{
        private:
            unsigned long long int __sizeY = 0;
            unsigned long long int __sizeX = 0;
            Type**  __data = nullptr;

            void __confereInicializador() const{
                if(this->__data == nullptr){
                    std::__throw_invalid_argument("Matrix não inicializada");
                }
            }

            void __desalocador(){
                if(this->__data){
                    for(unsigned long long int i = 0; i < this->__sizeY; ++i){
                        delete[] this->__data[i];
                    }
                    delete[] this->__data;
                    this->__data = nullptr;
                }
            }
        
            class __proxyParaLinha;

            friend class __proxyParaLinha;

            class __proxyParaLinha{
                private:
                    unsigned long long int __linha;
                    matriz<Type>* __matrizInterna; 
                    Type __multiplicador = 1;

                public:
                    __proxyParaLinha(unsigned long long int inputLinha, matriz<Type>* inputMatriz) noexcept : __linha(inputLinha), __matrizInterna(inputMatriz) {}

                    __proxyParaLinha(const __proxyParaLinha& inputProxy) noexcept : __linha(inputProxy.__linha), __matrizInterna(inputProxy.__matrizInterna), __multiplicador(inputProxy.__multiplicador) {}

                    __proxyParaLinha& operator=(const __proxyParaLinha& inputProxy){ //Seta uma linha para outra
                        if(this->__matrizInterna != inputProxy.__matrizInterna){
                            std::__throw_invalid_argument("Só pode na mesma matriz");
                        }
                        if(this->__linha != inputProxy.__linha){
                            for(unsigned long long int i = 0; i < inputProxy.__matrizInterna->__sizeX; ++i){
                                this->__matrizInterna->__data[this->__linha][i] = inputProxy.__multiplicador*inputProxy.__matrizInterna->__data[inputProxy.__linha][i]; 
                            }
                        }
                        return *this;
                    }

                    __proxyParaLinha& operator=(std::initializer_list<Type> inputInicializador){ //Seta uma linha para uma linha especificada como inicializador
                        if(this->__matrizInterna->__sizeX != inputInicializador.size()){
                            std::__throw_invalid_argument("Tamanho errado de linha");
                        }
                        unsigned long long int coluna = 0;
                        for (typename std::initializer_list<Type>::const_iterator colunaIt = inputInicializador.begin(); colunaIt != inputInicializador.end(); ++colunaIt, ++coluna) {
                            this->__matrizInterna->__data[this->__linha][coluna] = *colunaIt;
                        }
                        return *this;
                    }

                    __proxyParaLinha& operator+=(const __proxyParaLinha& inputProxy){ //Soma uma linha na outra
                        if(this->__matrizInterna != inputProxy.__matrizInterna){
                            std::__throw_invalid_argument("Só pode na mesma matriz");
                        }
                        for(unsigned long long int i = 0; i < inputProxy.__matrizInterna->__sizeX; ++i){
                            this->__matrizInterna->__data[this->__linha][i] += inputProxy.__multiplicador*inputProxy.__matrizInterna->__data[inputProxy.__linha][i]; 
                        }
                        return *this;
                    }

                    __proxyParaLinha& operator-=(const __proxyParaLinha& inputProxy){ //Subtrai uma linha da outra
                        if(this->__matrizInterna != inputProxy.__matrizInterna){
                            std::__throw_invalid_argument("Só pode na mesma matriz");
                        }
                        for(unsigned long long int i = 0; i < inputProxy.__matrizInterna->__sizeX; ++i){
                            this->__matrizInterna->__data[this->__linha][i] -= inputProxy.__multiplicador*inputProxy.__matrizInterna->__data[inputProxy.__linha][i]; 
                        }
                        return *this;
                    }

                    __proxyParaLinha& operator*(const Type& inputEscalar) noexcept{ //Não faz nada até você tentar somar, subtrair ou igualar, mas guarda o multiplicador 
                        __multiplicador *= inputEscalar;
                        return *this;
                    }

                    friend __proxyParaLinha operator*(const Type& inputEscalar, __proxyParaLinha inputProxy) noexcept{ //Não faz nada até você tentar somar, subtrair ou igualar, mas guarda o multiplicador 
                        inputProxy.__multiplicador *= inputEscalar;
                        return inputProxy;
                    }

                    __proxyParaLinha& operator^(__proxyParaLinha inputProxy){ //Troca a posição de duas linhas
                        if(this->__matrizInterna != inputProxy.__matrizInterna){
                            std::__throw_invalid_argument("Só pode na mesma matriz");
                        }
                        std::swap(this->__matrizInterna->__data[this->__linha],this->__matrizInterna->__data[inputProxy.__linha]);
                        return *this;
                    }

                    __proxyParaLinha& operator*=(const Type& inputEscalar) noexcept{ //Multiplica uma linha por um escalar
                        for(unsigned long long int i = 0; i < this->__matrizInterna->__sizeX; ++i){
                            this->__matrizInterna->__data[this->__linha][i] *= inputEscalar;
                        }
                        return *this;
                    }
                    
                    friend std::ostream& operator<<(std::ostream& os, const typename matriz<Type>::__proxyParaLinha& inputProxy) noexcept{ //Printa uma linha
                        for (unsigned long long int i = 0; i < inputProxy.__matrizInterna->__sizeX; ++i) {
                            os << inputProxy.__multiplicador*inputProxy.__matrizInterna->__data[inputProxy.__linha][i];
                            if (i < inputProxy.__matrizInterna->__sizeX - 1) {
                                os << " "; 
                            }
                        }
                        return os;
                    }
            };

        public:

            static matriz<Type> I(unsigned long long int tamanho){
                if (tamanho == 0) {
                    std::__throw_invalid_argument("Tamanho da matriz identidade deve ser maior que zero");
                }
                matriz<Type> identidade;
                identidade.__sizeX = tamanho;
                identidade.__sizeY = tamanho;
                delete[] identidade.__data;
                identidade.__data = new Type*[tamanho];
                for (unsigned long long int i = 0; i < tamanho; ++i) {
                    identidade.__data[i] = new Type[tamanho];
                    for (unsigned long long int j = 0; j < tamanho; ++j) {
                        identidade.__data[i][j] = (Type)(i == j);
                    }
                }
                return identidade;
            }

            static matriz<Type> zero(unsigned long long int linhas, unsigned long long int colunas){
                if (linhas == 0 || colunas == 0) {
                    std::__throw_invalid_argument("Tamanho da matriz zero deve ser maior que zero");
                }
                matriz<Type> zero;
                zero.__sizeX = colunas;
                zero.__sizeY = linhas;
                delete[] zero.__data;
                zero.__data = new Type*[linhas];
                for (unsigned long long int i = 0; i < linhas; ++i) {
                    zero.__data[i] = new Type[colunas];
                    for (unsigned long long int j = 0; j < colunas; ++j) {
                        zero.__data[i][j] = (Type)(0);
                    }
                }
                return zero;
            } 
            
            static matriz<Type> um(unsigned long long int linhas, unsigned long long int colunas){
                if (linhas == 0 || colunas == 0) {
                    std::__throw_invalid_argument("Tamanho da matriz zero deve ser maior que zero");
                }
                matriz<Type> um;
                um.__sizeX = colunas;
                um.__sizeY = linhas;
                delete[] zero.__data;
                um.__data = new Type*[linhas];
                for (unsigned long long int i = 0; i < linhas; ++i) {
                    um.__data[i] = new Type[colunas];
                    for (unsigned long long int j = 0; j < colunas; ++j) {
                        um.__data[i][j] = (Type)(1);
                    }
                }
                return um;
            }

            static matriz<Type> constante(unsigned long long int linhas, unsigned long long int colunas, Type inputConstante){
                if (linhas == 0 || colunas == 0) {
                    std::__throw_invalid_argument("Tamanho da matriz zero deve ser maior que zero");
                }
                matriz<Type> constante;
                constante.__sizeX = colunas;
                constante.__sizeY = linhas;
                delete[] constante.__data;
                uconstantem.__data = new Type*[linhas];
                for (unsigned long long int i = 0; i < linhas; ++i) {
                    constante.__data[i] = new Type[colunas];
                    for (unsigned long long int j = 0; j < colunas; ++j) {
                        constante.__data[i][j] = inputConstante;
                    }
                }
                return constante;
            }

            matriz(){}

            matriz(const unsigned long long int& inputSizeY,  const unsigned long long int& inputSizeX) : __sizeY(inputSizeY), __sizeX(inputSizeX){
                if(inputSizeX == 0 || inputSizeY == 0){
                    std::__throw_invalid_argument("Tamanho de matriz invalido");
                }
                delete[] this->__data;
                this->__data = new Type*[this->__sizeY];
                for(unsigned long long int i = 0; i < this->__sizeY; ++i){
                    this->__data[i] = new Type[this->__sizeX];
                }
            }

            matriz(const matriz<Type>& inputMatriz) : __sizeY(inputMatriz.__sizeY), __sizeX(inputMatriz.__sizeX){
                inputMatriz.__confereInicializador();
                delete[] this->__data;
                this->__data = new Type*[inputMatriz.__sizeY];
                for(unsigned long long int i = 0; i < inputMatriz.__sizeY; ++i){
                    this->__data[i] = new Type[inputMatriz.__sizeX];
                    for(unsigned long long j = 0; j < inputMatriz.__sizeX; ++j){
                        this->__data[i][j] = inputMatriz.__data[i][j];
                    }
                }
            }

            matriz(std::initializer_list<std::initializer_list<Type>> inputInicializador){
                this->__sizeY = inputInicializador.size();
                this->__sizeX = inputInicializador.begin()->size();
                this->__data = new Type*[this->__sizeY];
                unsigned long long int linha = 0;
                for (typename std::initializer_list<std::initializer_list<Type>>::const_iterator linhaIt = inputInicializador.begin(); linhaIt != inputInicializador.end(); ++linhaIt, ++linha) {
                    if (linhaIt->size() != this->__sizeX) {
                        std::__throw_invalid_argument("Cada linha tem que ter o mesmo numero de colunas");
                    }
                    this->__data[linha] = new Type[this->__sizeX];
                    unsigned long long int coluna = 0;
                    for (typename std::initializer_list<Type>::const_iterator colunaIt = linhaIt->begin();
                        colunaIt != linhaIt->end(); ++colunaIt, ++coluna) {
                        this->__data[linha][coluna] = *colunaIt;
                    }
                }
            }

            matriz(matriz<Type>&& inputMatriz) noexcept : __sizeY(inputMatriz.__sizeY), __sizeX(inputMatriz.__sizeX), __data(inputMatriz.__data){
                inputMatriz.__data = nullptr;
                inputMatriz.__sizeX = 0;
                inputMatriz.__sizeY = 0;
            }

            matriz<Type>& operator=(const matriz<Type>& inputMatriz){
                if(this != &inputMatriz){
                    inputMatriz.__confereInicializador();
                    this->__desalocador();
                    this->__sizeX = inputMatriz.__sizeX;
                    this->__sizeY = inputMatriz.__sizeY;
                    this->__data = new Type*[this->__sizeY];
                    for(unsigned long long int i = 0; i < this->__sizeY; ++i){
                        this->__data[i] = new Type[this->__sizeX];
                        for(unsigned long long int j = 0; j < this->__sizeX; ++j){
                            this->__data[i][j] = inputMatriz.__data[i][j];
                        }
                    }
                }
                return *this;
            }

            matriz<Type>& operator=(matriz<Type>&& inputMatriz) noexcept{
                if(this != &inputMatriz){
                    this->__desalocador();
                    this->__sizeX = inputMatriz.__sizeX;
                    this->__sizeY = inputMatriz.__sizeY;
                    this->__data = inputMatriz.__data;
                    inputMatriz.__sizeX = 0;
                    inputMatriz.__sizeY = 0;
                    inputMatriz.__data = nullptr;
                }
                return *this;
            }

            matriz<Type>& operator=(std::initializer_list<std::initializer_list<Type>> inputInicializador){
                this->__desalocador();
                this->__sizeY = inputInicializador.size();
                this->__sizeX = inputInicializador.begin()->size();
                this->__data = new Type*[this->__sizeY];
                unsigned long long int linha = 0;
                for (typename std::initializer_list<std::initializer_list<Type>>::const_iterator linhaIt = inputInicializador.begin(); linhaIt != inputInicializador.end(); ++linhaIt, ++linha) {
                    if (linhaIt->size() != this->__sizeX) {
                        std::__throw_invalid_argument("Cada linha tem que ter o mesmo numero de colunas");
                    }
                    this->__data[linha] = new Type[this->__sizeX];
                    unsigned long long int coluna = 0;
                    for (typename std::initializer_list<Type>::const_iterator colunaIt = linhaIt->begin();
                        colunaIt != linhaIt->end(); ++colunaIt, ++coluna) {
                        this->__data[linha][coluna] = *colunaIt;
                    }
                }
                return *this;
            }

            ~matriz(){
                this->__desalocador();
            }

            Type& operator()(const unsigned long long int& linha, const unsigned long long int& coluna){
                if(linha == 0 || coluna == 0 || linha > this->__sizeY || coluna > this->__sizeX){
                    std::__throw_invalid_argument("Tentou acessar indice inexistente");
                }
                return this->__data[linha - 1][coluna - 1];
            }

            const Type& operator()(const unsigned long long int& linha, const unsigned long long int& coluna) const{
                if(linha == 0 || coluna == 0 || linha > this->__sizeY || coluna > this->__sizeX){
                    std::__throw_invalid_argument("Tentou acessar indice inexistente");
                }
                return this->__data[linha - 1][coluna - 1];
            }

            __proxyParaLinha operator()(const unsigned long long int& linha){
                if(linha == 0 || linha > this->__sizeY){
                    std::__throw_invalid_argument("Tentou acessar indice inexistente");
                }
                return __proxyParaLinha(linha - 1,this);
            }

            friend std::ostream& operator<<(std::ostream& os, const matriz<Type>& inputMatriz) noexcept{
                for (unsigned long long int i = 0; i < inputMatriz.__sizeY; ++i) {
                    for (unsigned long long int j = 0; j < inputMatriz.__sizeX; ++j) {
                        os << inputMatriz.__data[i][j];
                        if (j < inputMatriz.__sizeX - 1) {
                            os << " ";
                        }
                    }
                    os << "\n"; 
                }
                return os;
            }

            matriz<Type> operator+(const matriz<Type>& inputMatriz) const{
                if(this->__sizeX != inputMatriz.__sizeX || this->__sizeY != inputMatriz.__sizeY){
                    std::__throw_invalid_argument("Matrizes não tem tamanho compatível");
                }
                matriz<Type> resultado;
                resultado.__sizeX = this->__sizeX;
                resultado.__sizeY = this->__sizeY;
                delete[] resultado.__data;
                resultado.__data = new Type*[resultado.__sizeY];
                for(unsigned long long int i = 0; i < resultado.__sizeY; ++i){
                    resultado.__data[i] = new Type[resultado.__sizeX];
                    for(unsigned long long int j = 0; j < resultado.__sizeX; ++j){
                        resultado.__data[i][j] = this->__data[i][j] + inputMatriz.__data[i][j];
                    }
                }
                return resultado;
            }

            matriz<Type> operator-(const matriz<Type>& inputMatriz) const{
                if(this->__sizeX != inputMatriz.__sizeX || this->__sizeY != inputMatriz.__sizeY){
                    std::__throw_invalid_argument("Matrizes não tem tamanho compatível");
                }
                matriz<Type> resultado;
                resultado.__sizeX = this->__sizeX;
                resultado.__sizeY = this->__sizeY;
                delete[] resultado.__data;
                resultado.__data = new Type*[resultado.__sizeY];
                for(unsigned long long int i = 0; i < resultado.__sizeY; ++i){
                    resultado.__data[i] = new Type[resultado.__sizeX];
                    for(unsigned long long int j = 0; j < resultado.__sizeX; ++j){
                        resultado.__data[i][j] = this->__data[i][j] - inputMatriz.__data[i][j];
                    }
                }
                return resultado;
            }

            matriz<Type> operator-() const noexcept{
                matriz<Type> resultado;
                resultado.__sizeX = this->__sizeX;
                resultado.__sizeY = this->__sizeY;
                delete[] resultado.__data;
                resultado.__data = new Type*[resultado.__sizeY];
                for(unsigned long long int i = 0; i < resultado.__sizeY; ++i){
                    resultado.__data[i] = new Type[resultado.__sizeX];
                    for(unsigned long long int j = 0; j < resultado.__sizeX; ++j){
                        resultado.__data[i][j] = -this->__data[i][j];
                    }
                }
                return resultado;
            }     

            matriz<Type> operator*(const matriz<Type>& inputMatriz) const{
                if(this->__sizeX != inputMatriz.__sizeY){
                    std::__throw_invalid_argument("Matrizes não tem tamanho compatível");
                }
                matriz<Type> resultado;
                resultado.__sizeX = inputMatriz.__sizeX;
                resultado.__sizeY = this->__sizeY;
                delete[] resultado.__data;
                resultado.__data = new Type*[resultado.__sizeY];
                for(unsigned long long int i = 0; i < resultado.__sizeY; ++i){
                    resultado.__data[i] = new Type[resultado.__sizeX];
                    for(unsigned long long int j = 0; j < resultado.__sizeX; ++j){
                        resultado.__data[i][j] = 0;
                        for(unsigned long long int k = 0; k < this->__sizeX; ++k){
                            resultado.__data[i][j] += this->__data[i][k] * inputMatriz.__data[k][j];
                        }
                        
                    }
                }
                return resultado;
            }
            
            unsigned long long int linhas(){
                return this->__sizeY;
            }

            unsigned long long int colunas(){
                return this->__sizeX;
            }
    
            bool operator==(const matriz<Type>& inputMatriz){
                if(this->__data == inputMatriz.__data){
                    return true;
                }
                if(this->__sizeX != inputMatriz.__sizeX || this->__sizeY != inputMatriz.__sizeY){
                    return false;
                }
                for(unsigned long long int i = 0; i < this->__sizeY; ++i){
                    for(unsigned long long int j = 0; j < this->__sizeX; ++j){
                        if(this->__data[i][j] != inputMatriz.__data[i][j]){
                            return false;
                        }
                    }
                }
                return true;
            }
    
            bool operator!=(const matriz<Type>& inputMatriz){
                return !(*this == inputMatriz);
            }

            matriz<Type> transposta() const noexcept{
                matriz<Type> resultado;
                resultado.__sizeX = this->__sizeY;
                resultado.__sizeY = this->__sizeX;
                delete[] resultado.__data;
                resultado.__data = new Type*[resultado.__sizeY];
                for(unsigned long long int i = 0; i < resultado.__sizeY; ++i){
                    resultado.__data[i] = new Type[resultado.__sizeX];
                    for(unsigned long long int j = 0; j < resultado.__sizeX; ++j){
                        resultado.__data[i][j] = this->__data[j][i];
                    }
                }
                return resultado;
            }

            matriz<Type> operator|(const matriz<Type>& inputMatriz){
                if(this->__sizeY != inputMatriz.__sizeY){
                    std::__throw_invalid_argument("Matrizes não tem tamanho compatível");
                }
                matriz<Type> resultado;
                resultado.__sizeX = this->__sizeX + inputMatriz.__sizeX;
                resultado.__sizeY = this->__sizeY;
                delete[] resultado.__data;
                resultado.__data = new Type*[resultado.__sizeY];
                for(unsigned long long int i = 0; i < resultado.__sizeY; ++i){
                    resultado.__data[i] = new Type[resultado.__sizeX];
                    for(unsigned long long int j = 0; j < resultado.__sizeX;++j){
                        if(j < this->__sizeX){
                            resultado.__data[i][j] = this->__data[i][j];
                        }
                        else{
                            resultado.__data[i][j] = inputMatriz.__data[i][j - inputMatriz.__sizeX];
                        }
                    }
                }
                return resultado;
            }
    };

    template <typename T> bool EDiagonal(matriz<T> inputMatriz){
        if(inputMatriz.linhas() != inputMatriz.colunas()){
            return false;
        }
        for(unsigned long long int i = 0; i < inputMatriz.linhas(); ++i){
            for(unsigned long long int j = 0; j < inputMatriz.colunas(); ++j){
                if(i != j && inputMatriz(i+1,j+1) != 0){
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T> bool ETriangularInferior(matriz<T> inputMatriz){
        if(inputMatriz.linhas() != inputMatriz.colunas()){
            return false;
        }
        for(unsigned long long int i = 0; i < inputMatriz.linhas(); ++i){
            for(unsigned long long int j = 0; j < inputMatriz.colunas(); ++j){
                if(i < j && inputMatriz(i+1,j+1) != 0){
                    return false;
                }
            }
        }
        return true;
    }
    
    template <typename T> bool ETriangularSuperior(matriz<T> inputMatriz){
        if(inputMatriz.linhas() != inputMatriz.colunas()){
            return false;
        }
        for(unsigned long long int i = 0; i < inputMatriz.linhas(); ++i){
            for(unsigned long long int j = 0; j < inputMatriz.colunas(); ++j){
                if(i > j && inputMatriz(i+1,j+1) != 0){
                    return false;
                }
            }
        }
        return true;
    }
}