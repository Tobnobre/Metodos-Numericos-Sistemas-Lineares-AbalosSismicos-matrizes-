#ifndef JACOBI_H
#define JACOBI_H

#include <vector>

class JacobiSolver {
public:
    static std::pair<std::vector<double>, int> resolver(
        int n, 
        const std::vector<std::vector<double>>& A, 
        const std::vector<double>& b, 
        double erro_max,
        double &erro_final
    );
};

#endif