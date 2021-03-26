#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

int GetN();
void NewMatr(double**& M, int n, int m);
void DelMatr(double**& M, int n, int m);
void PrintMatr(double** M, int n, int m, const char* namematr);
void PrintVect(double* x, int n, const char* namematr);
bool Minor(double**& M, int n, int m, double& Determinant);
void Reshuffle(double**& M, int n, int m, int k, double& Determinant);
bool Solve(double**& M, double* x, int n, int m, double& Determinant);
double Determinant(double** M, int n, int m);
void Test(double**& M, int n, int m);
void Clone(double**& M, int n, int m, double** M1);
double Check(double** M1, int n, int m, double* x);
bool Reverb(double** A, int n, int m, double& Determinant, double**& rev);
void Proverka(double** M1, double** M2, int n1, int m1, int n2, int m2);

int main() {
    setlocale(LC_ALL, "Russian");
    int n;
    double det = 1;
    std::cout << "Пожалуйста, введите размер матрицы: ";
    n = GetN();
    int m = n + 1;
    double** A;
    double** A1;
    double** rev;
    double* x = new double[n];

    NewMatr(A, n, m);
    Test(A, n, m);
    NewMatr(A1, n, m);
    Clone(A, n, m, A1);
    NewMatr(rev, n, n);
    PrintMatr(A, n, m, "A");

    if (Solve(A, x, n, m, det)) {
        PrintVect(x, n, "x");
        std::cout << "Введите значение точности численного типа данных: " << Check(A1, n, m, x) << std::endl << std::endl;
    }
    else std::cout << "Нет решений!" << std::endl;

    std::cout << "Определитель матрицы: " << det * Determinant(A, n, m) << std::endl;

    if (Reverb(A1, n, m, det, rev)) {
        PrintMatr(rev, n, n, "rev");
        Proverka(A1, rev, n, m - 1, n, n);
    }

    else std::cout << "Нет единственного решения!" << std::endl;
    DelMatr(A, n, m);
    DelMatr(A1, n, m);
    DelMatr(rev, n, n);
    delete[] x;
    x = NULL;
    return 0;
}

int GetN() {
    float n;
    do {
        std::cin >> n;
        if (n <= 0 || (n - (int)n))
            std::cout << "Ошибка, попробуйте ещё раз!" << std::endl;
    } while (n <= 0 || (n - (int)n));
    return n;
}

void NewMatr(double**& M, int n, int m) {
    M = new double* [n];
    for (int i = 0; i < n; i++) {
        M[i] = new double[m];
    }
}

void DelMatr(double**& M, int n, int m) {
    for (int i = 0; i < n; i++) {
        delete[] M[i];
    }
    delete[] M;
}

void PrintMatr(double** M, int n, int m, const char* namematr) {
    std::cout << std::endl << " " << namematr << ":" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << std::setw(15) << M[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void PrintVect(double* x, int n, const char* namematr) {
    std::cout << std::endl << " " << namematr << ":" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << std::setw(5) << x[i];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Reshuffle(double**& M, int n, int m, int k, double& Determinant) {
    for (int i = k + 1; i < n; i++) {
        if (M[i][k] != 0) {
            double temp;
            for (int i1 = 0; i1 < m; i1++) {
                temp = M[k][i1];
                M[k][i1] = M[i][i1];
                M[i][i1] = temp;
            }
            Determinant *= -1;
            break;
        }
    }
}

bool Minor(double**& M, int n, int m, double& Determinant) {
    for (int i = 0; i < n; i++) {
        if (M[i][i] == 0) {
            Reshuffle(M, n, m, i, Determinant);
        }
        if (M[i][i] == 0) {
            return false;
        }
        for (int i1 = i + 1; i1 < n; i1++) {
            for (int i2 = i + 1; i2 < m; i2++) {
                M[i1][i2] = M[i][i] * M[i1][i2] - M[i][i2] * M[i1][i];
            }
        }
        for (int j = i + 1; j < n; j++) {
            M[j][i] = 0;
        }
    }
    return true;
}

bool Solve(double**& M, double* x, int n, int m, double& Determinant) {
    double res = 0;
    if (!Minor(M, n, m, Determinant)) {
        return false;
    }
    for (int i = n - 1; i >= 0; i--) {
        res = 0;
        for (int j = i + 1; j <= n - 1; j++)
            res = res - x[j] * M[i][j];
        res += M[i][n];
        x[i] = res / M[i][i];
    }
    return true;
}

double Determinant(double** M, int n, int m) {
    double Determinant = 1;
    for (int i = 0; i < n; i++) {
        if (M[i][i] == 0) return 0;
        Determinant *= M[i][i] / pow(M[i][i], n - 1 - i);
    }
    return Determinant;
}

void Test(double**& M, int n, int m) {
    int k;
    std::cout << "1. Матрица из файла. \n2. Случайная матрица.\n3. Единичная матрица.\n4. Нулевая матрица.\n5. Матрица Гильберта.\nВведите требуемое условие: ";
    std::cin >> k;
    switch (k) {
        case 1: {
            std::fstream A("data_array.txt", std::ios::in);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    A >> M[i][j];
            A.close();
            break;
        }
        case 2: {
            double a, b;
            std::srand(time(0));
            a = -10;
            b = 10;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++) {
                    M[i][j] = a + ((double)rand() / RAND_MAX) * (b - a);
                }
            break;
        }
        case 3: {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (i == j) {
                        M[i][j] = 1;
                    } else {
                        M[i][j] = 0;
                    }
                }
            }
            for (int i = 0; i < n; i++) {
                break;
            }
        }
        case 4: {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    M[i][j] = 0;
            break;
        }
        case 5: {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    M[i][j] = 1. / (i + j + 1);
            break;
        }
    }
}

void Clone(double**& M, int n, int m, double** M1) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            M1[i][j] = M[i][j];
}

double Check(double** M1, int n, int m, double* x) {
    double eps = 0;
    double s;
    for (int i = 0; i < n; i++) {
        s = 0;
        for (int k = 0; k < m - 1; k++)
            s += M1[i][k] * x[k];
        if (fabs(M1[i][m - 1] - s) > eps) {
            eps = fabs(M1[i][m - 1] - s);
        }
    }
    return eps;
}

bool Reverb(double** M, int n, int m, double& Determinant, double**& rev) {
    double* x = new double[n];
    bool state = true;
    for (int i = 0; i < n; i++) {
        double** buff;
        NewMatr(buff, n, m);
        Clone(M, n, m, buff);
        for (int j = 0; j < n; j++) {
            if (i == j) {
                buff[j][m - 1] = 1;
            } else {
                buff[j][m - 1] = 0;
            }
        }
        if (!Solve(buff, x, n, m, Determinant)) {
            state = false;
        }
        for (int j = 0; j < n; j++)
            rev[j][i] = x[j];
        DelMatr(buff, n, m);
    }
    delete[] x;
    x = NULL;
    return state;
}

void Proverka(double** M1, double** M2, int n1, int m1, int n2, int m2) {
    double** buff;
    NewMatr(buff, n1, m2);
    double S;
    if (m1 != n2) {
        std::cout << "Ошибка!" << std::endl;
        DelMatr(buff, n1, m2);
    }

    for (int i = 0; i < n1; i++)
        for (int j = 0; j < m2; j++) {
            S = 0;
            for (int k = 0; k < m1; k++)
                S += M1[i][k] * M2[k][j];
            buff[i][j] = S;
        }

    PrintMatr(buff, n1, m2, "Проверка");
    DelMatr(buff, n1, m2);
}