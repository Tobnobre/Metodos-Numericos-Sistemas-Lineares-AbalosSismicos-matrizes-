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
    bool erro_matematico;      // True se houver divisão por zero
    bool diagonal_dominante;   // True se a matriz garante convergência
};

#endif