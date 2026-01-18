#ifndef SEIDEL_H
#define SEIDEL_H

#include <vector>

class SeidelSolver {
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