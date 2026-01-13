#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Estrutura para facilitar o retorno dos dados para a interface Web
struct ResultadoSismico {
    vector<vector<double>> inversa;
    vector<double> deslocamentos;
    bool perigo;
};

// --- MÉTODOS ITERATIVOS ---

// Resolve Ax = b por Jacobi
vector<double> resolverJacobi(int n, const vector<vector<double>>& A, const vector<double>& b, double e) {
    vector<double> x(n, 0.0);
    vector<double> x_novo(n, 0.0);
    double erro = 10.0;
    
    while (erro > e) {
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j];
            }
            x_novo[i] = (b[i] - soma) / A[i][i];
        }
        
        // Cálculo do Erro Máximo
        double max_diff = 0;
        for (int i = 0; i < n; i++) {
            max_diff = max(max_diff, abs(x_novo[i] - x[i]));
            x[i] = x_novo[i];
        }
        erro = max_diff;
    }
    return x;
}

// Resolve Ax = b por Gauss-Seidel
vector<double> resolverSeidel(int n, const vector<vector<double>>& A, const vector<double>& b, double e) {
    vector<double> x(n, 0.0);
    vector<double> x_antigo(n, 0.0);
    double erro = 10.0;
    
    while (erro > e) {
        for (int i = 0; i < n; i++) x_antigo[i] = x[i];
        
        for (int i = 0; i < n; i++) {
            double soma = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) soma += A[i][j] * x[j]; // Usa x[j] que já pode estar atualizado
            }
            x[i] = (b[i] - soma) / A[i][i];
        }
        
        double max_diff = 0;
        for (int i = 0; i < n; i++) max_diff = max(max_diff, abs(x[i] - x_antigo[i]));
        erro = max_diff;
    }
    return x;
}

// --- LÓGICA DO TRABALHO (TEMA 2) ---

ResultadoSismico processarSondagem(int n, vector<vector<double>> A, vector<double> b, double e, bool usarSeidel) {
    ResultadoSismico res;
    res.inversa.assign(n, vector<double>(n, 0.0));
    
    // 1. Calcular A^-1 coluna por coluna
    for (int j = 0; j < n; j++) {
        vector<double> identidade_col(n, 0.0);
        identidade_col[j] = 1.0; // Define a coluna da identidade {1 0 0...}
        
        vector<double> col_inversa;
        if (usarSeidel) 
            col_inversa = resolverSeidel(n, A, identidade_col, e);
        else 
            col_inversa = resolverJacobi(n, A, identidade_col, e);
            
        // Preenche a coluna j da matriz inversa
        for (int i = 0; i < n; i++) {
            res.inversa[i][j] = col_inversa[i];
        }
    }
    
    // 2. Calcular d = A^-1 * b
    res.deslocamentos.assign(n, 0.0);
    res.perigo = false;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res.deslocamentos[i] += res.inversa[i][j] * b[j];
        }
        if (abs(res.deslocamentos[i]) > 0.4) res.perigo = true;
    }
    
    return res;
}

int main() {
    int n = 3;
    double e = 0.0001; // Precisão sugerida para calibração
    vector<vector<double>> A = {{5, 3, 1}, {5, 6, 1}, {1, 6, 7}};
    vector<double> b = {1, 2, 3};

    cout << "--- CALIBRACAO TEMA 2 ---" << endl;
    
    ResultadoSismico r = processarSondagem(n, A, b, e, true); // Usando Seidel para calibrar

    cout << "\nMatriz Inversa A^-1:" << endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++) cout << setw(10) << r.inversa[i][j] << " ";
        cout << endl;
    }

    cout << "\nDeslocamentos {d}:" << endl;
    for(int i=0; i<n; i++) cout << "d" << i+1 << " = " << r.deslocamentos[i] << " cm" << endl;

    if(r.perigo) cout << "\n[ALERTA] RISCO DE DANOS ESTRUTURAIS! (Deslocamento > 0.4)" << endl;
    else cout << "\n[OK] Deslocamentos dentro do limite de seguranca." << endl;

    return 0;
}