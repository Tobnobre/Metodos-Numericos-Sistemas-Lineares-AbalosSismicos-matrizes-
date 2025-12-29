#include <emscripten/emscripten.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "linear_solver.h"

using namespace std;

extern "C" {

// Função chamada pelo JavaScript
EMSCRIPTEN_KEEPALIVE
const char* calcularSistemaSismico(int n, double* flatA, double* flatB, double precisao, int metodoId) {
    
    // 1. Converter arrays brutos (ponteiros) para vectors C++
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);

    for (int i = 0; i < n; i++) {
        b[i] = flatB[i];
        for (int j = 0; j < n; j++) {
            // A matriz vem como um array único do JS, precisamos "fatiar"
            A[i][j] = flatA[i * n + j];
        }
    }

    // 2. Chamar o Solver
    bool usarSeidel = (metodoId == 1);
    ResultadoSismico res = LinearSolver::analisarOndasSismicas(n, A, b, precisao, usarSeidel);

    // 3. Serializar a resposta para JSON (Manual, para evitar libs externas pesadas)
    stringstream ss;
    ss << "{";
    
    // Inversa
    ss << "\"inversa\": [";
    for (int i = 0; i < n; i++) {
        ss << "[";
        for (int j = 0; j < n; j++) {
            ss << res.inversa[i][j];
            if (j < n - 1) ss << ",";
        }
        ss << "]";
        if (i < n - 1) ss << ",";
    }
    ss << "],";

    // Deslocamentos
    ss << "\"d\": [";
    for (int i = 0; i < n; i++) {
        ss << res.deslocamentos[i];
        if (i < n - 1) ss << ",";
    }
    ss << "],";

    // Metadados
    ss << "\"perigo\": " << (res.perigo ? "true" : "false") << ",";
    ss << "\"metodo\": \"" << res.metodo << "\",";
    ss << "\"iteracoes\": " << res.iteracoes_totais;
    
    ss << "}";

    // 4. Retornar string segura para o JS
    string json_str = ss.str();
    char* c_str = new char[json_str.length() + 1];
    strcpy(c_str, json_str.c_str());
    
    return c_str;
}

// Função auxiliar para liberar memória da string retornada
EMSCRIPTEN_KEEPALIVE
void liberarMemoria(char* ptr) {
    delete[] ptr;
}

} // extern "C"