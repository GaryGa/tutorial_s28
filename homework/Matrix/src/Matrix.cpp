// C++ 作业，矩阵类的实现
// author: hamocat (github: GaryGa)
// 关注永雏塔菲谢谢喵
// 鉴于给定的函数接口，矩阵结构通过最简单的二维数组实现，析构也用了手动delete，
// 很遗憾没有用到vector和智能指针等c++特性。
// 希望能抽出时间重写第二版，但这样的话原来很多接口就要修改了。
// 2026.11.22
#include "Matrix.hpp"
#include <iostream>
#include <cmath>

// 默认构造函数
Matrix::Matrix() {
    rows_ = 0;
    cols_ = 0;
    data_ = nullptr;
}

// 构造函数
// new需要做未正常分配内存的错误处理吗？
Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
    data_ = new double *[rows_];
    for (int i = 0; i < rows_; i++) {
        *(data_ + i)  = new double [cols]();
    }
}

// 拷贝构造函数
Matrix::Matrix(const Matrix &other)
    : rows_(other.getRows()), cols_(other.getCols()) {
    data_ = new double *[rows_];
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            data_[i][j] = other.get(i, j);
        }
    }
}

// 移动构造函数
Matrix::Matrix(Matrix &&other) noexcept
    : rows_(other.getRows()), cols_(other.getCols()), data_(other.getData()) {
    other.setRows(0);
    other.setCols(0);
    other.setData(nullptr);
}

// 析构函数
Matrix::~Matrix(){
    if (data_){
        for (int i = 0; i < rows_; i ++){delete [] *(data_ + i);}
    }
}

// (链式)设置矩阵元素
Matrix &Matrix::set(int row, int col, double value){
    data_[row][col] = value;
    return *this;
}

// 获取矩阵元素
double Matrix::get(int row, int col) const{
    double value = data_[row][col];
    return value;
}

// 获取矩阵行
double *Matrix::getRow(int row) const{
    double* specificRow = data_[row];
    return specificRow;
}

// 设置矩阵行（hamocat新增numValues，原因见main.cpp）
Matrix &Matrix::setRow(int row, double *values, int numValues){
    for (int i = 0; i < cols_ && i < numValues; i ++){
        data_[row][i] = values[i];
    }

    return *this;
}

// 获取矩阵列
double *Matrix::getCol(int col) const{
    double* specificCol;
    for (int i = 0; i < rows_; i ++){
        specificCol[i] = data_[i][col];
    }
    return specificCol;
}

// 设置矩阵列（hamocat新增numValues，原因同理见main.cpp）
Matrix &Matrix::setCol(int col, double *values, int numValues){
    for (int i = 0; i < rows_ && i < numValues; i ++){
        data_[i][col] = values[i];
    }
    return *this;
}

// 获取矩阵行数
int Matrix::getRows() const{
    return rows_;
}

// 设置矩阵行数（hamocat新增）
void Matrix::setRows(int rows){
    rows_ = rows;
}

// 获取矩阵列数
int Matrix::getCols() const{
    return cols_;
}

// 设置矩阵列数（hamocat新增）
void Matrix::setCols(int cols){
    cols_ = cols;
}

// 获取矩阵数据
double **Matrix::getData() const{
    return data_;
}

// 设置矩阵数据（hamocat新增）
void Matrix::setData(double **data){
    data_ = data;
}

// 打印矩阵（hamocat修改返回值为void，鉴于打印矩阵不需要返回原矩阵）
void Matrix::print() const{
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            std::cout << data_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

// 矩阵加法
// 还不会做错误处理真是抱歉
Matrix Matrix::add(const Matrix &other) const{
    if (other.cols_ != cols_ || other.rows_ != rows_){
        std::cout << "Error: can't add two matrices with different rows and cols" << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            newMatrix.set(i, j, data_[i][j] + other.data_[i][j]);
        }
    }
    return newMatrix;
}

// 矩阵乘法
Matrix Matrix::multiply(const Matrix &other) const{
    if (cols_ != other.rows_){
        std::cout << "Error: can't multiply two matrices with unmatched rows and cols" << std::endl;
        Matrix newMatrix;
        return newMatrix;
    }

    Matrix newMatrix(rows_, other.cols_);
    for (int i = 0; i < rows_; i ++){
        Matrix tempMat(other);
        for (int j = 0; j < other.rows_; j ++){
            for (int p = 0; p < other.cols_; p ++){
                tempMat.data_[j][p] = other.data_[j][p] * data_[i][j];
            }
        }
        for (int q = 0; q < other.cols_; q ++){
            double* tempCol = tempMat.getCol(q);
            for (int t = 0; t < other.rows_; t ++){
                newMatrix.data_[q][i] += tempCol[t];
            }
        }
    }

    return  newMatrix;
}

// 矩阵转置
Matrix Matrix::transpose() const{
    Matrix transMat(cols_, rows_);
    for (int i = 0; i < cols_; i ++){
        double* tempB4Col = getCol(i);
        transMat.setRow(i, tempB4Col, rows_);
    }

    return transMat;
}

// 矩阵是否为方阵
bool Matrix::isSquare() const{
    return (rows_ == cols_) ? true: false;
}

// bomb!!!(hamocat)
Matrix Matrix::bomb(int row, int col)const{
    Matrix tempMat(rows_ - 1, cols_ - 1);
    for (int it = 0, im = 0; it < rows_ - 1; it ++, im++){
        if (it == row){im ++;}
        double* tempArr = getRow(im);
        for (int jt = 0, jm = 0; jt < cols_ - 1; jt ++, jm ++){
            if (jt == col){jm ++;}
            tempMat.set(it, jt, tempArr[jm]);
        }
    }
    return tempMat;
}

// 矩阵行列式
// 这样的错误处理会和非满秩矩阵冲突
double Matrix::determinant() const{
    if (!isSquare() || cols_ == 0){
        std::cout << "Error: not a square matrix or is a zero matrix" << std::endl;
        return 0;
    }
    double det = 0;
    if (cols_ > 2){
        for (int i = 0; i < cols_; i ++){
            Matrix tempMat = bomb(1,i);
            det += pow(-1, i) * data_[1][i] * tempMat.determinant();
        }
    }
    if (cols_ == 2){
        double a = get(1,1), b = get(1,2), c = get(2,1), d = get(2,2);
        det = a * d - b * c;
    }
    if (cols_ == 1){
        det = get(1,1);
    }

    return det;
}

// 矩阵求逆
Matrix Matrix::inverse() const{
    if (determinant()){
        std::cout << "Error: not a full rank matrix" << std::endl;
        Matrix invMat;
        return invMat;
    }
    Matrix invMat(rows_, cols_);
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            Matrix tempMat = bomb(i,j);
            invMat.set(i, j, pow(-1, j) * tempMat.determinant());
        }
    }

    return invMat;
}
