#include <iostream>
#include "libmatrix/SpareMatrix.h"


int main() {
    
    SpareMatrix<int, 0> matrix;
    
    for (int i = 0; i < 10; ++i)
    {
        matrix[i][i] = i;    
    }    
    
    for (int i = 0; i < 10; ++i)
    {
        matrix[i][9 - i] = 9 - i;
    }
    
    std::cout << std::endl;
    
    for (int i = 1; i < 9; ++i)
    {
        for (int j = 1; j < 9; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }  
        std::cout << std::endl;  
    }
    
    
    std::cout << std::endl;
    
    std::cout << "Number of cells occupied: " << matrix.size() << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Cells: " << std::endl;
    std::cout << "r c v" << std::endl;
    
    for (const auto &cell : matrix.getXY())
    {
         std::cout << cell.second << " " << cell.first << " " 
                   << matrix[cell.second][cell.first] << std::endl;
    }
    
    return 0;
}