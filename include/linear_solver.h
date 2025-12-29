#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include <vector>
#include <string>

// Estrutura para devolver tudo de uma vez
struct ResultadoSismico {
    std::vector<std::vector<double>> inversa; // A matriz inversa calculada
    std::vector<double> deslocamentos;        // O vetor d final
    bool perigo;                              // Se algum |d| > 0.4
    std::string metodo;                       // Nome do método usado
    int iteracoes_totais;                     // Soma das iterações (para análise)
};

class LinearSolver {
public:
    // Resolve Ax = b (retorna o vetor x e conta iterações)
    static std::pair<std::vector<double>, int> resolverJacobi(int n, const std::vector<std::vector<double>>& A, const std::vector<double>& b, double erro_max);
    
    static std::pair<std::vector<double>, int> resolverSeidel(int n, const std::vector<std::vector<double>>& A, const std::vector<double>& b, double erro_max);

    // Função principal do Tema 2: Calcula inversa e analisa risco
    static ResultadoSismico analisarOndasSismicas(int n, const std::vector<std::vector<double>>& A, const std::vector<double>& b, double erro_max, bool usarSeidel);
};

#endif