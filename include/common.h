#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>

struct ResultadoSismico {
    std::vector<std::vector<double>> inversa; 
    std::vector<double> deslocamentos;        
    bool perigo;                              
    std::string metodo;                       
    int iteracoes_totais;  
    double erro_final;
    bool erro_matematico;      
    bool diagonal_dominante;
    std::string mensagem_erro;
};

#endif