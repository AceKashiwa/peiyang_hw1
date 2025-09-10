#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    Matrix m;

    if (a.rows != b.rows || a.cols != b.cols){
        m = create_matrix(0, 0);
        printf("Error: Matrix a and b must have the same rows and cols.\n");
    }else{
        m = create_matrix(a.rows, a.cols);
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < a.cols; j++) {
                m.data[i][j] = a.data[i][j] + b.data[i][j];
            }
        }
    }

    return m;
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    Matrix m;

    if (a.rows != b.rows || a.cols != b.cols){
        m = create_matrix(0, 0);
        printf("Error: Matrix a and b must have the same rows and cols.\n");
    }else{
        m = create_matrix(a.rows, a.cols);
        for (int i = 0; i <a.rows; i ++){
            for (int j = 0; j <a.cols; j ++){
                m.data[i][j] = a.data[i][j] - b.data[i][j];
            }
        }
    }

    return m;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    Matrix m;

    if (a.cols != b.rows){
        m = create_matrix(0, 0);
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
    }else{
        m = create_matrix(a.rows, b.cols);
        for (int i = 0; i < a.rows; i++) {
            for (int j = 0; j < b.cols; j++) {
                m.data[i][j] = 0;
                for (int k = 0; k < a.cols; k++) {
                    m.data[i][j] += a.data[i][k] * b.data[k][j];
                }
            }
        }
    }

    return m;
}

Matrix scale_matrix(Matrix a, double k)
{
    Matrix m = create_matrix(a.rows, a.cols);
    
    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            m.data[i][j] = a.data[i][j] * k;
        }
    }

    return m;
}

Matrix transpose_matrix(Matrix a)
{
    Matrix m = create_matrix(a.cols, a.rows);

    for (int i = 0; i < a.rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            m.data[j][i] = a.data[i][j];
        }
    }

    return m;
}

double det_matrix(Matrix a)
{
    if (a.rows != a.cols || a.rows == 0) {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }

    int n = a.rows;
    Matrix m = a;

    int flag = 1;
    double det = 1.0;

    for (int r = 0; r < n; r++) {
        // 当a[r][r]为0时
        if (m.data[r][r] == 0) {
            int zero = 1;
            for (int i = r + 1; i < n; i++) {
                if (m.data[i][r] != 0) {
                    zero = 0;
                    for (int j = 0; j < n; j++) {
                        double tmp = m.data[r][j];
                        m.data[r][j] = m.data[i][j];
                        m.data[i][j] = tmp;
                    }
                    flag = -flag;
                    break;
                }
            }
            if (zero) return 0; // 若整列均为0，则行列式为0
        }

        // 以a[r][r]为主元，对下面的行进行消元
        for (int i = r + 1; i < n; ++i) {
            double factor = m.data[i][r] / m.data[r][r];
            for (int j = r + 1; j < n; ++j) {
                m.data[i][j] -= factor * m.data[r][j];
            }
            // 思考：此处操作与否不影响行列式的计算，若其余函数需要使用消元法，应考虑是否进行此操作
            // m.data[i][r] = 0;
        }

        det *= m.data[r][r];
    }

    return flag * det;
}

Matrix inv_matrix(Matrix a)
{
    Matrix m;

    if (a.rows != a.cols || a.rows == 0) {
        m = create_matrix(0, 0);
        printf("Error: The matrix must be a square matrix.\n");
    }else if (det_matrix(a) == 0){
        m = create_matrix(0, 0);
        printf("Error: The matrix is singular.\n");
    }else{
        int n = a.rows;
        m = create_matrix(n, n);

        // 构造矩阵 [A | E]
        Matrix tmp = create_matrix(n, 2 * n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                tmp.data[i][j] = a.data[i][j];
                tmp.data[i][j + n] = (i == j) ? 1 : 0;
            }
        }

        // [A | E] 变换为 [E | A^-1]
        for (int r = 0; r < n; r++) {
            // 当tmp[r][r]为0时
            if (tmp.data[r][r] == 0) {
                for (int i = r + 1; i < n; i++) {
                    if (tmp.data[i][r] != 0) {
                        for (int j = 0; j < 2 * n; j++) {
                            double t = tmp.data[r][j];
                            tmp.data[r][j] = tmp.data[i][j];
                            tmp.data[i][j] = t;
                        }
                        break;
                    }
                }
            }

            // 归一化
            double div = tmp.data[r][r];    // Debug: 省略变量div会导致除数变化，函数逻辑错误
            for (int j = 0; j < 2 * n; j++) {
                tmp.data[r][j] /= div;
            }

            // 对其他行进行消元
            for (int i = 0; i < n; i++) {
                if (i != r) {
                    double factor = tmp.data[i][r];
                    for (int j = 0; j < 2 * n; j++) {
                        tmp.data[i][j] -= factor * tmp.data[r][j];
                    }
                }
            }
        }

        // 提取右侧矩阵作为逆矩阵
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                m.data[i][j] = tmp.data[i][j + n];
            }
        }
    }

    return m;
}

int rank_matrix(Matrix a)
{
    int m = a.rows;
    int n = a.cols;
    int rank = (m < n ? m : n);
    Matrix tmp = a;

    // 转化为上三角矩阵
    for (int r = 0; r < rank; r++) {
        // 当tmp[r][r]为0时
        if (tmp.data[r][r] == 0) {
            int zero = 1;
            for (int i = r + 1; i < m; i++) {
                if (tmp.data[i][r] != 0) {
                    zero = 0;
                    for (int j = 0; j < n; j++) {
                        double t = tmp.data[r][j];
                        tmp.data[r][j] = tmp.data[i][j];
                        tmp.data[i][j] = t;
                    }
                    break;
                }
            }
            if (zero) {
                rank--;
                // 将最后一列移到当前列位置
                for (int j = 0; j < m; j++) {
                    tmp.data[r][j] = tmp.data[j][m - 1];
                }
                r--; // 重新处理当前行
                continue;
            }
        }

        // 以tmp[r][r]为主元，对下面的行进行消元
        for (int i = r + 1; i < m; ++i) {
            double factor = tmp.data[i][r] / tmp.data[r][r];
            for (int j = r; j < n; ++j) {
                tmp.data[i][j] -= factor * tmp.data[r][j];
            }
            // 思考：此处操作与否依旧不影响秩的计算
            // tmp.data[i][r] = 0;
        }
    }
    return rank;
}

double trace_matrix(Matrix a)
{
    if (a.rows != a.cols || a.rows == 0) {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }

    double trace = 0;
    for (int i = 0; i < a.rows; i++) {
        trace += a.data[i][i];
    }
    
    return trace;
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}