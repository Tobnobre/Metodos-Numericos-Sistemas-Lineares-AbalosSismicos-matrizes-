#include "sismic.h"
#include "utils.h"
#include "jacobi.h"
#include "seidel.h"
#include <cmath>
#include <algorithm>

using namespace std;

ResultadoSismico SismicSystem::analisar(int n, const vector<vector<double>>& A, const vector<double>& b, double erro_max, bool usarSeidel, double limiar) {
    ResultadoSismico res;
    res.inversa.assign(n, vector<double>(n, 0.0));
    res.metodo = usarSeidel ? "Gauss-Seidel" : "Gauss-Jacobi";
    res.iteracoes_totais = 0;
    res.erro_matematico = false;
    res.erro_final = 0.0;
    
    if (usarSeidel) {
        res.diagonal_dominante = Utils::verificarSassenfeld(n, A);
    } else {
        res.diagonal_dominante = Utils::verificarDiagonalDominante(n, A);
    }

    for (int j = 0; j < n; j++) {
        vector<double> identidade_col(n, 0.0);
        identidade_col[j] = 1.0; 
        
        pair<vector<double>, int> resultado_coluna;
        double erro_desta_coluna = 0.0;
        
        if (usarSeidel) 
            resultado_coluna = SeidelSolver::resolver(n, A, identidade_col, erro_max, erro_desta_coluna);
        else 
            resultado_coluna = JacobiSolver::resolver(n, A, identidade_col, erro_max, erro_desta_coluna);
        
        if (resultado_coluna.second < 0) {
            res.erro_matematico = true;
            res.erro_final = erro_desta_coluna;
            
            if (resultado_coluna.second == -1) {
                res.mensagem_erro = "Erro Matemático: Pivô nulo (divisão por zero) detectado na coluna " + to_string(j+1);
            } else {
                res.mensagem_erro = "Falha de Convergência: O método atingiu o limite de iterações ou divergiu.";
            }
            
            return res;
        }

        res.iteracoes_totais += resultado_coluna.second;
        if (erro_desta_coluna > res.erro_final) {
            res.erro_final = erro_desta_coluna;
        }

        for (int i = 0; i < n; i++) {
            res.inversa[i][j] = resultado_coluna.first[i];
        }
    }
    
    res.deslocamentos.assign(n, 0.0);
    res.perigo = false;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res.deslocamentos[i] += res.inversa[i][j] * b[j];
        }
        if (abs(res.deslocamentos[i]) > limiar) res.perigo = true;
    }
    
    return res;
}