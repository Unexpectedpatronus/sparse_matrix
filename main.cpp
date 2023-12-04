#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

#define R 5
#define C 5
#define K_1 3  // koef of sparsity
#define K_2 2  // koef of sparsity

void compress(int matrix[][C], std::list<int> &AN, std::vector<int> &D) {
    AN.push_back(matrix[0][0]);
    D.push_back(0);
    for (int i = 1; i < R; i++) {
        bool flag = true;
        for (int j = 0; j <= i; j++) {
            if (matrix[i][j] != 0)
                flag = false;
            if (!flag || i == j)
                AN.push_back(matrix[i][j]);
            if (flag && matrix[i][j] == 0)
                continue;
        }
        D.push_back(AN.size() - 1);
    }
}

void unpack(int matrix[][C], std::list<int> &AN, std::vector<int> &D) {
    matrix[0][0] = AN.front();
    auto cur = AN.begin();
    for (int i = 1; i < R; i++) {
        for (int j = 0, st = i - D[i] + D[i - 1] + 1; j <= i; j++) {
            if (j < st) {
                matrix[i][j] = 0;
                matrix[j][i] = matrix[i][j];
            } else {
                matrix[i][j] = *++cur;
                matrix[j][i] = matrix[i][j];
            }
        }
    }
}

void printMatrix(int matrix[][C], const std::string &name) {
    std::cout << "Matrix " << name << ":\n";
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            std::cout << std::setw(2) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
}

template<typename Container>
void printArray(const Container &arr, const std::string &name) {
    std::cout << name << ":\t";
    for (auto elem: arr) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {

    int A[R][C];
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if ((i + j) % K_1 == 0)
                A[i][j] = i * j + i + j;
            else A[i][j] = 0;
        }
    }

    printMatrix(A, "A");

    std::list<int> A_AN;
    std::vector<int> A_D;
    compress(A, A_AN, A_D);

    printArray(A_AN, "A_AN");
    printArray(A_D, "A_D");


    int newMatrix[R][C];
    unpack(newMatrix, A_AN, A_D);

    printMatrix(newMatrix, "Result");

    int B[R][C];
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if ((i + j) % K_2 == 0)
                B[i][j] = i * j + i + j;
            else B[i][j] = 0;
        }
    }

    printMatrix(B, "B");


    return 0;
}
