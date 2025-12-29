#include "linear_solver.h"
#include <cmath>
#include <algorithm>

using namespace std;

// Implementação do Gauss-Jacobi
pair<vector<double>, int> LinearSolver::resolverJacobi(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max) {
    vector<double> x(n, 0.0);
    vector<double> x_novo(n, 0.0);
    double erro = 10.0;
    int iter = 0;
    
    // Limite de segurança de 1000 iterações para não travar o navegador
    while (erro > erro_max && iter < 1000) {
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            // Evita divisão por zero
            if (abs(A[i][i]) < 1e-9) return {x, iter}; 
            x_novo[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        for (int i = 0; i < n; i++) {
            max_diff = max(max_diff, abs(x_novo[i] - x[i]));
            x[i] = x_novo[i];
        }
        erro = max_diff;
        iter++;
    }
    return {x, iter};
}

// Implementação do Gauss-Seidel
pair<vector<double>, int> LinearSolver::resolverSeidel(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max) {
    vector<double> x(n, 0.0);
    vector<double> x_antigo(n, 0.0);
    double erro = 10.0;
    int iter = 0;
    
    while (erro > erro_max && iter < 1000) {
        for (int i = 0; i < n; i++) x_antigo[i] = x[i];
        
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            if (abs(A[i][i]) < 1e-9) return {x, iter};
            x[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        for (int i = 0; i < n; i++) max_diff = max(max_diff, abs(x[i] - x_antigo[i]));
        erro = max_diff;
        iter++;
    }
    return {x, iter};
}

// Lógica de Negócio do Tema 2
ResultadoSismico LinearSolver::analisarOndasSismicas(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max, bool usarSeidel) {
    ResultadoSismico res;
    res.inversa.assign(n, vector<double>(n, 0.0));
    res.metodo = usarSeidel ? "Gauss-Seidel" : "Gauss-Jacobi";
    res.iteracoes_totais = 0;

    // 1. Calcular A^-1 coluna por coluna 
    for (int j = 0; j < n; j++) {
        vector<double> identidade_col(n, 0.0);
        identidade_col[j] = 1.0; 
        
        pair<vector<double>, int> resultado_coluna;
        
        if (usarSeidel) 
            resultado_coluna = resolverSeidel(n, A, identidade_col, erro_max);
        else 
            resultado_coluna = resolverJacobi(n, A, identidade_col, erro_max);
            
        res.iteracoes_totais += resultado_coluna.second;

        for (int i = 0; i < n; i++) {
            res.inversa[i][j] = resultado_coluna.first[i];
        }
    }
    
    // 2. Calcular d = A^-1 * b [cite: 369]
    res.deslocamentos.assign(n, 0.0);
    res.perigo = false;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res.deslocamentos[i] += res.inversa[i][j] * b[j];
        }
        // Verifica o limite de 0.4 cm 
        if (abs(res.deslocamentos[i]) > 0.4) res.perigo = true;
    }
    
    return res;
}