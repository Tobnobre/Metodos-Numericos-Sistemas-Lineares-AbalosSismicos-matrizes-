#include <emscripten/emscripten.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include "sismic.h" // Inclui apenas o controlador principal

using namespace std;

extern "C" {

EMSCRIPTEN_KEEPALIVE
char* calcularSistemaSismico(int n, double* flatA, double* flatB, double precisao, int metodoId, double limiar) {
    
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);

    for (int i = 0; i < n; i++) {
        b[i] = flatB[i];
        for (int j = 0; j < n; j++) {
            A[i][j] = flatA[i * n + j];
        }
    }

    bool usarSeidel = (metodoId == 1);
    
    // Chamada simplificada
    ResultadoSismico res = SismicSystem::analisar(n, A, b, precisao, usarSeidel, limiar);

    stringstream ss;
    ss << "{";
    
    if (res.erro_matematico) {
        ss << "\"erro_critico\": \"Divisão por zero detectada, alterar matriz sem o pivô zero.\",";
        ss << "\"sucesso\": false"; 
    } else {
        ss << "\"sucesso\": true,";
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

        ss << "\"d\": [";
        for (int i = 0; i < n; i++) {
            ss << res.deslocamentos[i];
            if (i < n - 1) ss << ",";
        }
        ss << "],";

        ss << "\"perigo\": " << (res.perigo ? "true" : "false") << ",";
        ss << "\"metodo\": \"" << res.metodo << "\",";
        ss << "\"iteracoes\": " << res.iteracoes_totais << ",";
        ss << "\"diagonal_dominante\": " << (res.diagonal_dominante ? "true" : "false");
    }
    
    ss << "}";

    string json_str = ss.str();
    char* c_str = new char[json_str.length() + 1];
    strcpy(c_str, json_str.c_str());
    
    return c_str;
}

EMSCRIPTEN_KEEPALIVE
void liberarMemoria(char* ptr) {
    delete[] ptr;
}

}