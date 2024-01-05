#include <iostream>
#include <iomanip>
#include <fstream>
#include "list.hpp"

#define R 5
#define C 5
#define K_1 3  // koef of sparsity
#define K_2 2  // koef of sparsity

void compress(int **matrix, List *AN, List *D) {
    AN->add(0);
    AN->cur->num = matrix[0][0];
    D->add(0);
    for (int i = 1; i < R; i++) {
        bool flag = false;
        for (int j = 0; j <= i; j++) {
            if (matrix[i][j] != 0)
                flag = true;
            if (flag || i == j) {
                AN->add(AN->get_size());
                AN->cur->num = matrix[i][j];
            }
        }
        D->add(D->get_size());
        D->cur->num = AN->get_size() - 1;
    }
}

void unpack(int **matrix, List *AN, List *D) {
    AN->cur = AN->first;
    D->cur = D->first;
    matrix[0][0] = AN->cur->num;
    AN->cur = AN->cur->next;
    D->cur = D->cur->next;
    for (int i = 1; i < R; i++) {
        for (int j = 0, st = i - D->cur->num + D->cur->prev->num + 1; j <= i; j++) {
            if (j < st) {
                matrix[i][j] = 0;
                matrix[j][i] = matrix[i][j];
            } else {
                matrix[i][j] = AN->cur->num;
                matrix[j][i] = matrix[i][j];
                AN->cur = AN->cur->next;
            }
        }
        D->cur = D->cur->next;
    }
}

void printMatrix(int **matrix, const std::string &name) {
    std::cout << "Matrix " << name << ":\n";
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
//            if (j <= i)
            std::cout << std::setw(2) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
}

void sumMatrices(List *A_AN, List *A_D, List *B_AN, List *B_D, List *S_AN, List *S_D) {
    int Ak, Bk, n = 0, i = 0;
    A_AN->cur = A_AN->first;
    B_AN->cur = B_AN->first;
    S_AN->add(n);
    S_AN->cur->num = A_AN->cur->num + B_AN->cur->num;
    A_AN->cur = A_AN->cur->next;
    B_AN->cur = B_AN->cur->next;

    A_D->cur = A_D->first;
    B_D->cur = B_D->first;
    S_D->add(i);
    S_D->cur->num = n;
    A_D->cur = A_D->cur->next;
    B_D->cur = B_D->cur->next;

    for (i = 1; i < A_D->get_size(); i++) {
        Ak = A_D->cur->num - A_D->cur->prev->num;
        Bk = B_D->cur->num - B_D->cur->prev->num;
        int count = 0, sum;
        bool flag = false;
        if (Ak < Bk) {
            while (Ak < Bk) {
                S_AN->add(++n);
                S_AN->cur->num = B_AN->cur->num;
                B_AN->cur = B_AN->cur->next;
                Bk--;
            }
        } else if (Ak > Bk) {
            while (Ak > Bk) {
                S_AN->add(++n);
                S_AN->cur->num = A_AN->cur->num;
                A_AN->cur = A_AN->cur->next;
                Ak--;
            }
        }
        while (count != Ak) {
            sum = A_AN->cur->num + B_AN->cur->num;
            if (sum != 0) {
                flag = true;
            }
            if (flag || Ak == 1) {
                S_AN->add(++n);
                S_AN->cur->num = sum;
                if (Ak == 1) {
                    S_D->add(i);
                    S_D->cur->num = n;
                }
                A_AN->cur = A_AN->cur->next;
                B_AN->cur = B_AN->cur->next;

            } else {
                A_AN->cur = A_AN->cur->next;
                B_AN->cur = B_AN->cur->next;
            }
            Ak--;
        }
        A_D->cur = A_D->cur->next;
        B_D->cur = B_D->cur->next;
    }

}

int main() {
    int **A = new int *[R];
    int **res_A = new int *[R];
    int **B = new int *[R];
    int **res_B = new int *[R];
    int **S = new int *[R];
    int **res_S = new int *[R];
    for (int i = 0; i < R; ++i) {
        A[i] = new int[C];
        res_A[i] = new int[C];
        B[i] = new int[C];
        res_B[i] = new int[C];
        S[i] = new int[C];
        res_S[i] = new int[C];
    }

    List *A_AN, *A_D, *B_AN, *B_D, *S_AN, *S_D;
    A_AN = new List();
    A_D = new List();
    B_AN = new List();
    B_D = new List();
    S_AN = new List();
    S_D = new List();

//    for (int i = 0; i < R; i++) {
//        for (int j = 0; j < C; j++) {
//            if ((i + j) % K_1 == 0)
//                A[i][j] = i * j + i + j;
//            else A[i][j] = 0;
//        }
//    }
//    for (int i = 0; i < R; i++) {
//        for (int j = 0; j < C; j++) {
//            if ((i + j) % K_2 == 0)
//                B[i][j] = i * j + i + j;
//            else B[i][j] = 0;
//        }
//    }

    std::ifstream inA("A.txt");
    std::ifstream inB("B.txt");
    if (inA.is_open() && inB.is_open()) {
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < R; j++) {
                inA >> A[i][j];
                inB >> B[i][j];
            }
        }
    }
    inA.close();
    inB.close();


    printMatrix(A, "A");
    compress(A, A_AN, A_D);
    print_List(A_AN, "A_AN");
    print_List(A_D, "A_D");
    unpack(res_A, A_AN, A_D);
    printMatrix(res_A, "res_A");

    printMatrix(B, "B");
    compress(B, B_AN, B_D);
    print_List(B_AN, "B_AN");
    print_List(B_D, "B_D");
    unpack(res_B, B_AN, B_D);
    printMatrix(res_B, "res_B");

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            S[i][j] = A[i][j] + B[i][j];
        }
    }
    printMatrix(S, "S = A + B");

    sumMatrices(A_AN, A_D, B_AN, B_D, S_AN, S_D);
    unpack(res_S, S_AN, S_D);
    printMatrix(res_S, "unpacked_S = A + B");

    int unmatch = 0;
    for (int i = 0; i < R; i++) {
        if (unmatch)
            break;
        for (int j = 0; j < C; j++) {
            if (S[i][j] != res_S[i][j]) {
                unmatch++;
                break;
            }
        }
    }
    if (unmatch)
        std::cout << "Error." << std::endl;
    else {
        std::cout << "Matrices are equal." << std::endl;
        std::ofstream outS("S.txt");
        if (outS.is_open()) {
            for (int i = 0; i < R; i++) {
                for (int j = 0; j < R; j++) {
                    outS << std::setw(2) << res_S[i][j] << " ";
                }
                outS << std::endl;
            }
        }
        outS.close();
    }

    for (int i = 0; i < R; ++i) {
        delete[] A[i];
        delete[] res_A[i];
        delete[] B[i];
        delete[] res_B[i];
        delete[] S[i];
        delete[] res_S[i];
    }
    delete[] A;
    delete[] res_A;
    delete[] B;
    delete[] res_B;
    delete[] S;
    delete[] res_S;
    delete A_AN;
    delete A_D;
    delete B_AN;
    delete B_D;
    delete S_AN;
    delete S_D;

    return 0;
}
