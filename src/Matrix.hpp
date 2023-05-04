#pragma once

#include "float.h"
#include "math.h"

namespace mtrn3100 {

class Matrix {
public:
    Matrix() : Matrix(0, 0, 0) {}

    Matrix(size_t num_rows, size_t num_cols) : Matrix(num_rows, num_cols, 0) {}

    Matrix(size_t num_rows, size_t num_cols, float value) : num_rows(num_rows), num_cols(num_cols) {
        if (size() > max_size) {
            while (1) {
            };
        }
        for (size_t i = 0; i < size(); i++) {
            data[i] = value;
        }
    }

    Matrix(const Matrix& m) = default;

    Matrix(Matrix&& m) = default;

    Matrix& operator=(const Matrix& m) = default;

    Matrix& operator=(Matrix&& m) = default;

    ~Matrix() = default;

    float& operator()(const size_t num_row, const size_t num_col) {
        if (num_row >= num_rows || num_col >= num_cols) {
            while (1) {
            };
        }
        return data[num_row * num_cols + num_col];
    }

    const float& operator()(const size_t num_row, const size_t num_col) const {
        if (num_row >= num_rows || num_col >= num_cols) {
            while (1) {
            };
        }
        return data[num_row * num_cols + num_col];
    }

    float* begin() { return data; }

    const float* begin() const { return data; }

    float* end() { return begin() + size(); }

    const float* end() const { return begin() + size(); }

    size_t size() const { return num_rows * num_cols; }

    size_t capacity() const { return max_size; }

    size_t rows() const { return num_rows; }

    size_t cols() const { return num_cols; }

    float* row(const size_t num_row, size_t& size) {
        if (num_row < 0 || num_row > num_rows) {
            while (1) {
            };
        }
        size = num_cols;
        for (size_t i = 0; i < num_cols; i++) {
            row_cache[i] = operator()(num_row, i);
        }
        return row_cache;
    }

    float* col(const size_t num_col, size_t& size) {
        if (num_col < 0 || num_col > num_cols) {
            while (1) {
            };
        }
        size = num_rows;
        for (size_t i = 0; i < num_rows; i++) {
            col_cache[i] = operator()(i, num_col);
        }
        return col_cache;
    }

    friend bool operator==(const Matrix& lhs, const Matrix& rhs) {
        if (lhs.num_rows != rhs.num_rows || lhs.num_cols != rhs.num_cols) {
            return false;
        }
        for (size_t i = 0; i < lhs.num_rows; i++) {
            for (size_t j = 0; j < lhs.num_cols; j++) {
                if (fabs(lhs(i, j) - rhs(i, j)) > threshold) {
                    return false;
                }
            }
        }
        return true;
    }

    friend bool operator!=(const Matrix& lhs, const Matrix& rhs) { return !(lhs == rhs); }

    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs) {
        Matrix rm(lhs.num_rows, lhs.num_cols);
        if (lhs.num_rows != rhs.num_rows || lhs.num_cols != rhs.num_cols) {
            while (1) {
            };
        }
        for (size_t i = 0; i < lhs.num_rows; i++) {
            for (size_t j = 0; j < lhs.num_cols; j++) {
                rm(i, j) = lhs(i, j) + rhs(i, j);
            }
        }
        return rm;
    }

    friend Matrix operator-(const Matrix& m) {
        Matrix rm = m;
        for (size_t i = 0; i < rm.num_rows; i++) {
            for (size_t j = 0; j < rm.num_cols; j++) {
                rm(i, j) = -rm(i, j);
            }
        }
        return rm;
    }

    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs) { return lhs + (-rhs); }

    friend Matrix operator*(Matrix& lhs, Matrix& rhs) {
        Matrix rm(lhs.num_rows, rhs.num_cols);
        if (lhs.num_cols != rhs.num_rows) {
            while (1) {
            };
        }
        for (size_t i = 0; i < lhs.num_rows; i++) {
            for (size_t j = 0; j < rhs.num_cols; j++) {
                size_t size;
                float* r = lhs.row(i, size);
                float* c = rhs.col(j, size);
                rm(i, j) = inner_product(r, r + size, c, 0.0);
            }
        }
        return rm;
    }

    friend Matrix operator*(const Matrix& m, const float s) {
        Matrix rm(m.num_rows, m.num_cols);
        for (size_t i = 0; i < m.num_rows; i++) {
            for (size_t j = 0; j < m.num_cols; j++) {
                rm(i, j) = m(i, j) * s;
            }
        }
        return rm;
    }

    friend Matrix operator*(const float s, const Matrix& m) { return m * s; }

    friend Matrix operator/(const Matrix& m, const float s) { return m * (1 / s); }

    friend Matrix operator/(const float s, const Matrix& m) { return s * inverse(m); }

    Matrix& operator+=(const Matrix& m) {
        *this = *this + m;
        return *this;
    }

    Matrix& operator-=(const Matrix& m) {
        *this = *this - m;
        return *this;
    }

    Matrix& operator*=(const Matrix& m) {
        *this = *this * m;
        return *this;
    }

    Matrix& operator*=(const float s) {
        *this = *this * s;
        return *this;
    }

    Matrix& operator/=(const float s) {
        *this = *this / s;
        return *this;
    }

    static Matrix tranpose(const Matrix& m) {
        Matrix rm(m.num_cols, m.num_rows);
        for (size_t i = 0; i < m.num_rows; i++) {
            for (size_t j = 0; j < m.num_cols; j++) {
                rm(j, i) = m(i, j);
            }
        }
        return rm;
    }

    static Matrix inverse(const Matrix& m) {
        if (m.num_rows != m.num_cols) {
            while (1) {
            };
        }
        float determinant = 0;
        Matrix rm = m;
        switch (m.rows()) {
            case 0: {
                break;
            }
            case 1: {
                rm(0, 0) = 1 / m(0, 0);
                break;
            }
            case 2: {
                // Author: Cornstalks
                // License: https://github.com/willnode/N-Matrix-Programmer/blob/master/LICENSE
                // Link: https://github.com/willnode/N-Matrix-Programmer/blob/master/Info/Matrix_2x2.txt
                // Changes: Code adapted to C syntax.

                determinant = m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
                determinant = 1 / determinant;

                rm(0, 0) = determinant * (m(1, 1));
                rm(0, 1) = determinant * -(m(0, 1));
                rm(1, 0) = determinant * -(m(1, 0));
                rm(1, 1) = determinant * (m(0, 0));
                break;
            }
            case 3: {
                // Author: Cornstalks
                // License: https://github.com/willnode/N-Matrix-Programmer/blob/master/LICENSE
                // Link: https://github.com/willnode/N-Matrix-Programmer/blob/master/Info/Matrix_3x3.txt
                // Changes: Code adapted to C syntax.

                determinant = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
                              m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
                              m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
                determinant = 1 / determinant;

                rm(0, 0) = (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) * determinant;
                rm(0, 1) = (m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2)) * determinant;
                rm(0, 2) = (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * determinant;
                rm(1, 0) = (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * determinant;
                rm(1, 1) = (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * determinant;
                rm(1, 2) = (m(1, 0) * m(0, 2) - m(0, 0) * m(1, 2)) * determinant;
                rm(2, 0) = (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * determinant;
                rm(2, 1) = (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) * determinant;
                rm(2, 2) = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * determinant;
                break;
            }
            case 4: {
                // Author: willnode
                // License: https://github.com/willnode/N-Matrix-Programmer/blob/master/LICENSE
                // Link: https://github.com/willnode/N-Matrix-Programmer/blob/master/Info/Matrix_4x4.txt
                // Changes: Code adapted to C syntax.

                const float A2323 = m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2);
                const float A1323 = m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1);
                const float A1223 = m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1);
                const float A0323 = m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0);
                const float A0223 = m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0);
                const float A0123 = m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0);
                const float A2313 = m(1, 2) * m(3, 3) - m(1, 3) * m(3, 2);
                const float A1313 = m(1, 1) * m(3, 3) - m(1, 3) * m(3, 1);
                const float A1213 = m(1, 1) * m(3, 2) - m(1, 2) * m(3, 1);
                const float A2312 = m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2);
                const float A1312 = m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1);
                const float A1212 = m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1);
                const float A0313 = m(1, 0) * m(3, 3) - m(1, 3) * m(3, 0);
                const float A0213 = m(1, 0) * m(3, 2) - m(1, 2) * m(3, 0);
                const float A0312 = m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0);
                const float A0212 = m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0);
                const float A0113 = m(1, 0) * m(3, 1) - m(1, 1) * m(3, 0);
                const float A0112 = m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0);

                determinant = m(0, 0) * (m(1, 1) * A2323 - m(1, 2) * A1323 + m(1, 3) * A1223) -
                              m(0, 1) * (m(1, 0) * A2323 - m(1, 2) * A0323 + m(1, 3) * A0223) +
                              m(0, 2) * (m(1, 0) * A1323 - m(1, 1) * A0323 + m(1, 3) * A0123) -
                              m(0, 3) * (m(1, 0) * A1223 - m(1, 1) * A0223 + m(1, 2) * A0123);
                determinant = 1 / determinant;

                rm(0, 0) = determinant * (m(1, 1) * A2323 - m(1, 2) * A1323 + m(1, 3) * A1223);
                rm(0, 1) = determinant * -(m(0, 1) * A2323 - m(0, 2) * A1323 + m(0, 3) * A1223);
                rm(0, 2) = determinant * (m(0, 1) * A2313 - m(0, 2) * A1313 + m(0, 3) * A1213);
                rm(0, 3) = determinant * -(m(0, 1) * A2312 - m(0, 2) * A1312 + m(0, 3) * A1212);
                rm(1, 0) = determinant * -(m(1, 0) * A2323 - m(1, 2) * A0323 + m(1, 3) * A0223);
                rm(1, 1) = determinant * (m(0, 0) * A2323 - m(0, 2) * A0323 + m(0, 3) * A0223);
                rm(1, 2) = determinant * -(m(0, 0) * A2313 - m(0, 2) * A0313 + m(0, 3) * A0213);
                rm(1, 3) = determinant * (m(0, 0) * A2312 - m(0, 2) * A0312 + m(0, 3) * A0212);
                rm(2, 0) = determinant * (m(1, 0) * A1323 - m(1, 1) * A0323 + m(1, 3) * A0123);
                rm(2, 1) = determinant * -(m(0, 0) * A1323 - m(0, 1) * A0323 + m(0, 3) * A0123);
                rm(2, 2) = determinant * (m(0, 0) * A1313 - m(0, 1) * A0313 + m(0, 3) * A0113);
                rm(2, 3) = determinant * -(m(0, 0) * A1312 - m(0, 1) * A0312 + m(0, 3) * A0112);
                rm(3, 0) = determinant * -(m(1, 0) * A1223 - m(1, 1) * A0223 + m(1, 2) * A0123);
                rm(3, 1) = determinant * (m(0, 0) * A1223 - m(0, 1) * A0223 + m(0, 2) * A0123);
                rm(3, 2) = determinant * -(m(0, 0) * A1213 - m(0, 1) * A0213 + m(0, 2) * A0113);
                rm(3, 3) = determinant * (m(0, 0) * A1212 - m(0, 1) * A0212 + m(0, 2) * A0112);
                break;
            }
            case 5: {
                // Author: willnode
                // License: https://github.com/willnode/N-Matrix-Programmer/blob/master/LICENSE
                // Link: https://github.com/willnode/N-Matrix-Programmer/blob/master/Info/Matrix_5x5.txt
                // Changes: Code adapted to C syntax.

                const float A3434 = m(3, 3) * m(4, 4) - m(3, 4) * m(4, 3);
                const float A2434 = m(3, 2) * m(4, 4) - m(3, 4) * m(4, 2);
                const float A2334 = m(3, 2) * m(4, 3) - m(3, 3) * m(4, 2);
                const float A1434 = m(3, 1) * m(4, 4) - m(3, 4) * m(4, 1);
                const float A1334 = m(3, 1) * m(4, 3) - m(3, 3) * m(4, 1);
                const float A1234 = m(3, 1) * m(4, 2) - m(3, 2) * m(4, 1);
                const float A0434 = m(3, 0) * m(4, 4) - m(3, 4) * m(4, 0);
                const float A0334 = m(3, 0) * m(4, 3) - m(3, 3) * m(4, 0);
                const float A0234 = m(3, 0) * m(4, 2) - m(3, 2) * m(4, 0);
                const float A0134 = m(3, 0) * m(4, 1) - m(3, 1) * m(4, 0);
                const float A3424 = m(2, 3) * m(4, 4) - m(2, 4) * m(4, 3);
                const float A2424 = m(2, 2) * m(4, 4) - m(2, 4) * m(4, 2);
                const float A2324 = m(2, 2) * m(4, 3) - m(2, 3) * m(4, 2);
                const float A1424 = m(2, 1) * m(4, 4) - m(2, 4) * m(4, 1);
                const float A1324 = m(2, 1) * m(4, 3) - m(2, 3) * m(4, 1);
                const float A1224 = m(2, 1) * m(4, 2) - m(2, 2) * m(4, 1);
                const float A3423 = m(2, 3) * m(3, 4) - m(2, 4) * m(3, 3);
                const float A2423 = m(2, 2) * m(3, 4) - m(2, 4) * m(3, 2);
                const float A2323 = m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2);
                const float A1423 = m(2, 1) * m(3, 4) - m(2, 4) * m(3, 1);
                const float A1323 = m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1);
                const float A1223 = m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1);
                const float A0424 = m(2, 0) * m(4, 4) - m(2, 4) * m(4, 0);
                const float A0324 = m(2, 0) * m(4, 3) - m(2, 3) * m(4, 0);
                const float A0224 = m(2, 0) * m(4, 2) - m(2, 2) * m(4, 0);
                const float A0423 = m(2, 0) * m(3, 4) - m(2, 4) * m(3, 0);
                const float A0323 = m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0);
                const float A0223 = m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0);
                const float A0124 = m(2, 0) * m(4, 1) - m(2, 1) * m(4, 0);
                const float A0123 = m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0);

                const float B234234 = m(2, 2) * A3434 - m(2, 3) * A2434 + m(2, 4) * A2334;
                const float B134234 = m(2, 1) * A3434 - m(2, 3) * A1434 + m(2, 4) * A1334;
                const float B124234 = m(2, 1) * A2434 - m(2, 2) * A1434 + m(2, 4) * A1234;
                const float B123234 = m(2, 1) * A2334 - m(2, 2) * A1334 + m(2, 3) * A1234;
                const float B034234 = m(2, 0) * A3434 - m(2, 3) * A0434 + m(2, 4) * A0334;
                const float B024234 = m(2, 0) * A2434 - m(2, 2) * A0434 + m(2, 4) * A0234;
                const float B023234 = m(2, 0) * A2334 - m(2, 2) * A0334 + m(2, 3) * A0234;
                const float B014234 = m(2, 0) * A1434 - m(2, 1) * A0434 + m(2, 4) * A0134;
                const float B013234 = m(2, 0) * A1334 - m(2, 1) * A0334 + m(2, 3) * A0134;
                const float B012234 = m(2, 0) * A1234 - m(2, 1) * A0234 + m(2, 2) * A0134;
                const float B234134 = m(1, 2) * A3434 - m(1, 3) * A2434 + m(1, 4) * A2334;
                const float B134134 = m(1, 1) * A3434 - m(1, 3) * A1434 + m(1, 4) * A1334;
                const float B124134 = m(1, 1) * A2434 - m(1, 2) * A1434 + m(1, 4) * A1234;
                const float B123134 = m(1, 1) * A2334 - m(1, 2) * A1334 + m(1, 3) * A1234;
                const float B234124 = m(1, 2) * A3424 - m(1, 3) * A2424 + m(1, 4) * A2324;
                const float B134124 = m(1, 1) * A3424 - m(1, 3) * A1424 + m(1, 4) * A1324;
                const float B124124 = m(1, 1) * A2424 - m(1, 2) * A1424 + m(1, 4) * A1224;
                const float B123124 = m(1, 1) * A2324 - m(1, 2) * A1324 + m(1, 3) * A1224;
                const float B234123 = m(1, 2) * A3423 - m(1, 3) * A2423 + m(1, 4) * A2323;
                const float B134123 = m(1, 1) * A3423 - m(1, 3) * A1423 + m(1, 4) * A1323;
                const float B124123 = m(1, 1) * A2423 - m(1, 2) * A1423 + m(1, 4) * A1223;
                const float B123123 = m(1, 1) * A2323 - m(1, 2) * A1323 + m(1, 3) * A1223;
                const float B034134 = m(1, 0) * A3434 - m(1, 3) * A0434 + m(1, 4) * A0334;
                const float B024134 = m(1, 0) * A2434 - m(1, 2) * A0434 + m(1, 4) * A0234;
                const float B023134 = m(1, 0) * A2334 - m(1, 2) * A0334 + m(1, 3) * A0234;
                const float B034124 = m(1, 0) * A3424 - m(1, 3) * A0424 + m(1, 4) * A0324;
                const float B024124 = m(1, 0) * A2424 - m(1, 2) * A0424 + m(1, 4) * A0224;
                const float B023124 = m(1, 0) * A2324 - m(1, 2) * A0324 + m(1, 3) * A0224;
                const float B034123 = m(1, 0) * A3423 - m(1, 3) * A0423 + m(1, 4) * A0323;
                const float B024123 = m(1, 0) * A2423 - m(1, 2) * A0423 + m(1, 4) * A0223;
                const float B023123 = m(1, 0) * A2323 - m(1, 2) * A0323 + m(1, 3) * A0223;
                const float B014134 = m(1, 0) * A1434 - m(1, 1) * A0434 + m(1, 4) * A0134;
                const float B013134 = m(1, 0) * A1334 - m(1, 1) * A0334 + m(1, 3) * A0134;
                const float B014124 = m(1, 0) * A1424 - m(1, 1) * A0424 + m(1, 4) * A0124;
                const float B013124 = m(1, 0) * A1324 - m(1, 1) * A0324 + m(1, 3) * A0124;
                const float B014123 = m(1, 0) * A1423 - m(1, 1) * A0423 + m(1, 4) * A0123;
                const float B013123 = m(1, 0) * A1323 - m(1, 1) * A0323 + m(1, 3) * A0123;
                const float B012134 = m(1, 0) * A1234 - m(1, 1) * A0234 + m(1, 2) * A0134;
                const float B012124 = m(1, 0) * A1224 - m(1, 1) * A0224 + m(1, 2) * A0124;
                const float B012123 = m(1, 0) * A1223 - m(1, 1) * A0223 + m(1, 2) * A0123;

                determinant =
                    m(0, 0) * (m(1, 1) * B234234 - m(1, 2) * B134234 + m(1, 3) * B124234 - m(1, 4) * B123234) -
                    m(0, 1) * (m(1, 0) * B234234 - m(1, 2) * B034234 + m(1, 3) * B024234 - m(1, 4) * B023234) +
                    m(0, 2) * (m(1, 0) * B134234 - m(1, 1) * B034234 + m(1, 3) * B014234 - m(1, 4) * B013234) -
                    m(0, 3) * (m(1, 0) * B124234 - m(1, 1) * B024234 + m(1, 2) * B014234 - m(1, 4) * B012234) +
                    m(0, 4) * (m(1, 0) * B123234 - m(1, 1) * B023234 + m(1, 2) * B013234 - m(1, 3) * B012234);
                determinant = 1 / determinant;

                rm(0, 0) =
                    determinant * (m(1, 1) * B234234 - m(1, 2) * B134234 + m(1, 3) * B124234 - m(1, 4) * B123234);
                rm(0, 1) =
                    determinant * -(m(0, 1) * B234234 - m(0, 2) * B134234 + m(0, 3) * B124234 - m(0, 4) * B123234);
                rm(0, 2) =
                    determinant * (m(0, 1) * B234134 - m(0, 2) * B134134 + m(0, 3) * B124134 - m(0, 4) * B123134);
                rm(0, 3) =
                    determinant * -(m(0, 1) * B234124 - m(0, 2) * B134124 + m(0, 3) * B124124 - m(0, 4) * B123124);
                rm(0, 4) =
                    determinant * (m(0, 1) * B234123 - m(0, 2) * B134123 + m(0, 3) * B124123 - m(0, 4) * B123123);
                rm(1, 0) =
                    determinant * -(m(1, 0) * B234234 - m(1, 2) * B034234 + m(1, 3) * B024234 - m(1, 4) * B023234);
                rm(1, 1) =
                    determinant * (m(0, 0) * B234234 - m(0, 2) * B034234 + m(0, 3) * B024234 - m(0, 4) * B023234);
                rm(1, 2) =
                    determinant * -(m(0, 0) * B234134 - m(0, 2) * B034134 + m(0, 3) * B024134 - m(0, 4) * B023134);
                rm(1, 3) =
                    determinant * (m(0, 0) * B234124 - m(0, 2) * B034124 + m(0, 3) * B024124 - m(0, 4) * B023124);
                rm(1, 4) =
                    determinant * -(m(0, 0) * B234123 - m(0, 2) * B034123 + m(0, 3) * B024123 - m(0, 4) * B023123);
                rm(2, 0) =
                    determinant * (m(1, 0) * B134234 - m(1, 1) * B034234 + m(1, 3) * B014234 - m(1, 4) * B013234);
                rm(2, 1) =
                    determinant * -(m(0, 0) * B134234 - m(0, 1) * B034234 + m(0, 3) * B014234 - m(0, 4) * B013234);
                rm(2, 2) =
                    determinant * (m(0, 0) * B134134 - m(0, 1) * B034134 + m(0, 3) * B014134 - m(0, 4) * B013134);
                rm(2, 3) =
                    determinant * -(m(0, 0) * B134124 - m(0, 1) * B034124 + m(0, 3) * B014124 - m(0, 4) * B013124);
                rm(2, 4) =
                    determinant * (m(0, 0) * B134123 - m(0, 1) * B034123 + m(0, 3) * B014123 - m(0, 4) * B013123);
                rm(3, 0) =
                    determinant * -(m(1, 0) * B124234 - m(1, 1) * B024234 + m(1, 2) * B014234 - m(1, 4) * B012234);
                rm(3, 1) =
                    determinant * (m(0, 0) * B124234 - m(0, 1) * B024234 + m(0, 2) * B014234 - m(0, 4) * B012234);
                rm(3, 2) =
                    determinant * -(m(0, 0) * B124134 - m(0, 1) * B024134 + m(0, 2) * B014134 - m(0, 4) * B012134);
                rm(3, 3) =
                    determinant * (m(0, 0) * B124124 - m(0, 1) * B024124 + m(0, 2) * B014124 - m(0, 4) * B012124);
                rm(3, 4) =
                    determinant * -(m(0, 0) * B124123 - m(0, 1) * B024123 + m(0, 2) * B014123 - m(0, 4) * B012123);
                rm(4, 0) =
                    determinant * (m(1, 0) * B123234 - m(1, 1) * B023234 + m(1, 2) * B013234 - m(1, 3) * B012234);
                rm(4, 1) =
                    determinant * -(m(0, 0) * B123234 - m(0, 1) * B023234 + m(0, 2) * B013234 - m(0, 3) * B012234);
                rm(4, 2) =
                    determinant * (m(0, 0) * B123134 - m(0, 1) * B023134 + m(0, 2) * B013134 - m(0, 3) * B012134);
                rm(4, 3) =
                    determinant * -(m(0, 0) * B123124 - m(0, 1) * B023124 + m(0, 2) * B013124 - m(0, 3) * B012124);
                rm(4, 4) =
                    determinant * (m(0, 0) * B123123 - m(0, 1) * B023123 + m(0, 2) * B013123 - m(0, 3) * B012123);
                break;
            }
            case 6: {
                // Author: willnode
                // License: https://github.com/willnode/N-Matrix-Programmer/blob/master/LICENSE
                // Link: https://github.com/willnode/N-Matrix-Programmer/blob/master/Info/Matrix_6x6.txt
                // Changes: Code adapted to C syntax.

                const float A4545 = m(4, 4) * m(5, 5) - m(4, 5) * m(5, 4);
                const float A3545 = m(4, 3) * m(5, 5) - m(4, 5) * m(5, 3);
                const float A3445 = m(4, 3) * m(5, 4) - m(4, 4) * m(5, 3);
                const float A2545 = m(4, 2) * m(5, 5) - m(4, 5) * m(5, 2);
                const float A2445 = m(4, 2) * m(5, 4) - m(4, 4) * m(5, 2);
                const float A2345 = m(4, 2) * m(5, 3) - m(4, 3) * m(5, 2);
                const float A1545 = m(4, 1) * m(5, 5) - m(4, 5) * m(5, 1);
                const float A1445 = m(4, 1) * m(5, 4) - m(4, 4) * m(5, 1);
                const float A1345 = m(4, 1) * m(5, 3) - m(4, 3) * m(5, 1);
                const float A1245 = m(4, 1) * m(5, 2) - m(4, 2) * m(5, 1);
                const float A0545 = m(4, 0) * m(5, 5) - m(4, 5) * m(5, 0);
                const float A0445 = m(4, 0) * m(5, 4) - m(4, 4) * m(5, 0);
                const float A0345 = m(4, 0) * m(5, 3) - m(4, 3) * m(5, 0);
                const float A0245 = m(4, 0) * m(5, 2) - m(4, 2) * m(5, 0);
                const float A0145 = m(4, 0) * m(5, 1) - m(4, 1) * m(5, 0);
                const float A4535 = m(3, 4) * m(5, 5) - m(3, 5) * m(5, 4);
                const float A3535 = m(3, 3) * m(5, 5) - m(3, 5) * m(5, 3);
                const float A3435 = m(3, 3) * m(5, 4) - m(3, 4) * m(5, 3);
                const float A2535 = m(3, 2) * m(5, 5) - m(3, 5) * m(5, 2);
                const float A2435 = m(3, 2) * m(5, 4) - m(3, 4) * m(5, 2);
                const float A2335 = m(3, 2) * m(5, 3) - m(3, 3) * m(5, 2);
                const float A1535 = m(3, 1) * m(5, 5) - m(3, 5) * m(5, 1);
                const float A1435 = m(3, 1) * m(5, 4) - m(3, 4) * m(5, 1);
                const float A1335 = m(3, 1) * m(5, 3) - m(3, 3) * m(5, 1);
                const float A1235 = m(3, 1) * m(5, 2) - m(3, 2) * m(5, 1);
                const float A4534 = m(3, 4) * m(4, 5) - m(3, 5) * m(4, 4);
                const float A3534 = m(3, 3) * m(4, 5) - m(3, 5) * m(4, 3);
                const float A3434 = m(3, 3) * m(4, 4) - m(3, 4) * m(4, 3);
                const float A2534 = m(3, 2) * m(4, 5) - m(3, 5) * m(4, 2);
                const float A2434 = m(3, 2) * m(4, 4) - m(3, 4) * m(4, 2);
                const float A2334 = m(3, 2) * m(4, 3) - m(3, 3) * m(4, 2);
                const float A1534 = m(3, 1) * m(4, 5) - m(3, 5) * m(4, 1);
                const float A1434 = m(3, 1) * m(4, 4) - m(3, 4) * m(4, 1);
                const float A1334 = m(3, 1) * m(4, 3) - m(3, 3) * m(4, 1);
                const float A1234 = m(3, 1) * m(4, 2) - m(3, 2) * m(4, 1);
                const float A0535 = m(3, 0) * m(5, 5) - m(3, 5) * m(5, 0);
                const float A0435 = m(3, 0) * m(5, 4) - m(3, 4) * m(5, 0);
                const float A0335 = m(3, 0) * m(5, 3) - m(3, 3) * m(5, 0);
                const float A0235 = m(3, 0) * m(5, 2) - m(3, 2) * m(5, 0);
                const float A0534 = m(3, 0) * m(4, 5) - m(3, 5) * m(4, 0);
                const float A0434 = m(3, 0) * m(4, 4) - m(3, 4) * m(4, 0);
                const float A0334 = m(3, 0) * m(4, 3) - m(3, 3) * m(4, 0);
                const float A0234 = m(3, 0) * m(4, 2) - m(3, 2) * m(4, 0);
                const float A0135 = m(3, 0) * m(5, 1) - m(3, 1) * m(5, 0);
                const float A0134 = m(3, 0) * m(4, 1) - m(3, 1) * m(4, 0);

                const float B345345 = m(3, 3) * A4545 - m(3, 4) * A3545 + m(3, 5) * A3445;
                const float B245345 = m(3, 2) * A4545 - m(3, 4) * A2545 + m(3, 5) * A2445;
                const float B235345 = m(3, 2) * A3545 - m(3, 3) * A2545 + m(3, 5) * A2345;
                const float B234345 = m(3, 2) * A3445 - m(3, 3) * A2445 + m(3, 4) * A2345;
                const float B145345 = m(3, 1) * A4545 - m(3, 4) * A1545 + m(3, 5) * A1445;
                const float B135345 = m(3, 1) * A3545 - m(3, 3) * A1545 + m(3, 5) * A1345;
                const float B134345 = m(3, 1) * A3445 - m(3, 3) * A1445 + m(3, 4) * A1345;
                const float B125345 = m(3, 1) * A2545 - m(3, 2) * A1545 + m(3, 5) * A1245;
                const float B124345 = m(3, 1) * A2445 - m(3, 2) * A1445 + m(3, 4) * A1245;
                const float B123345 = m(3, 1) * A2345 - m(3, 2) * A1345 + m(3, 3) * A1245;
                const float B045345 = m(3, 0) * A4545 - m(3, 4) * A0545 + m(3, 5) * A0445;
                const float B035345 = m(3, 0) * A3545 - m(3, 3) * A0545 + m(3, 5) * A0345;
                const float B034345 = m(3, 0) * A3445 - m(3, 3) * A0445 + m(3, 4) * A0345;
                const float B025345 = m(3, 0) * A2545 - m(3, 2) * A0545 + m(3, 5) * A0245;
                const float B024345 = m(3, 0) * A2445 - m(3, 2) * A0445 + m(3, 4) * A0245;
                const float B023345 = m(3, 0) * A2345 - m(3, 2) * A0345 + m(3, 3) * A0245;
                const float B015345 = m(3, 0) * A1545 - m(3, 1) * A0545 + m(3, 5) * A0145;
                const float B014345 = m(3, 0) * A1445 - m(3, 1) * A0445 + m(3, 4) * A0145;
                const float B013345 = m(3, 0) * A1345 - m(3, 1) * A0345 + m(3, 3) * A0145;
                const float B012345 = m(3, 0) * A1245 - m(3, 1) * A0245 + m(3, 2) * A0145;
                const float B345245 = m(2, 3) * A4545 - m(2, 4) * A3545 + m(2, 5) * A3445;
                const float B245245 = m(2, 2) * A4545 - m(2, 4) * A2545 + m(2, 5) * A2445;
                const float B235245 = m(2, 2) * A3545 - m(2, 3) * A2545 + m(2, 5) * A2345;
                const float B234245 = m(2, 2) * A3445 - m(2, 3) * A2445 + m(2, 4) * A2345;
                const float B145245 = m(2, 1) * A4545 - m(2, 4) * A1545 + m(2, 5) * A1445;
                const float B135245 = m(2, 1) * A3545 - m(2, 3) * A1545 + m(2, 5) * A1345;
                const float B134245 = m(2, 1) * A3445 - m(2, 3) * A1445 + m(2, 4) * A1345;
                const float B125245 = m(2, 1) * A2545 - m(2, 2) * A1545 + m(2, 5) * A1245;
                const float B124245 = m(2, 1) * A2445 - m(2, 2) * A1445 + m(2, 4) * A1245;
                const float B123245 = m(2, 1) * A2345 - m(2, 2) * A1345 + m(2, 3) * A1245;
                const float B345235 = m(2, 3) * A4535 - m(2, 4) * A3535 + m(2, 5) * A3435;
                const float B245235 = m(2, 2) * A4535 - m(2, 4) * A2535 + m(2, 5) * A2435;
                const float B235235 = m(2, 2) * A3535 - m(2, 3) * A2535 + m(2, 5) * A2335;
                const float B234235 = m(2, 2) * A3435 - m(2, 3) * A2435 + m(2, 4) * A2335;
                const float B145235 = m(2, 1) * A4535 - m(2, 4) * A1535 + m(2, 5) * A1435;
                const float B135235 = m(2, 1) * A3535 - m(2, 3) * A1535 + m(2, 5) * A1335;
                const float B134235 = m(2, 1) * A3435 - m(2, 3) * A1435 + m(2, 4) * A1335;
                const float B125235 = m(2, 1) * A2535 - m(2, 2) * A1535 + m(2, 5) * A1235;
                const float B124235 = m(2, 1) * A2435 - m(2, 2) * A1435 + m(2, 4) * A1235;
                const float B123235 = m(2, 1) * A2335 - m(2, 2) * A1335 + m(2, 3) * A1235;
                const float B345234 = m(2, 3) * A4534 - m(2, 4) * A3534 + m(2, 5) * A3434;
                const float B245234 = m(2, 2) * A4534 - m(2, 4) * A2534 + m(2, 5) * A2434;
                const float B235234 = m(2, 2) * A3534 - m(2, 3) * A2534 + m(2, 5) * A2334;
                const float B234234 = m(2, 2) * A3434 - m(2, 3) * A2434 + m(2, 4) * A2334;
                const float B145234 = m(2, 1) * A4534 - m(2, 4) * A1534 + m(2, 5) * A1434;
                const float B135234 = m(2, 1) * A3534 - m(2, 3) * A1534 + m(2, 5) * A1334;
                const float B134234 = m(2, 1) * A3434 - m(2, 3) * A1434 + m(2, 4) * A1334;
                const float B125234 = m(2, 1) * A2534 - m(2, 2) * A1534 + m(2, 5) * A1234;
                const float B124234 = m(2, 1) * A2434 - m(2, 2) * A1434 + m(2, 4) * A1234;
                const float B123234 = m(2, 1) * A2334 - m(2, 2) * A1334 + m(2, 3) * A1234;
                const float B045245 = m(2, 0) * A4545 - m(2, 4) * A0545 + m(2, 5) * A0445;
                const float B035245 = m(2, 0) * A3545 - m(2, 3) * A0545 + m(2, 5) * A0345;
                const float B034245 = m(2, 0) * A3445 - m(2, 3) * A0445 + m(2, 4) * A0345;
                const float B025245 = m(2, 0) * A2545 - m(2, 2) * A0545 + m(2, 5) * A0245;
                const float B024245 = m(2, 0) * A2445 - m(2, 2) * A0445 + m(2, 4) * A0245;
                const float B023245 = m(2, 0) * A2345 - m(2, 2) * A0345 + m(2, 3) * A0245;
                const float B045235 = m(2, 0) * A4535 - m(2, 4) * A0535 + m(2, 5) * A0435;
                const float B035235 = m(2, 0) * A3535 - m(2, 3) * A0535 + m(2, 5) * A0335;
                const float B034235 = m(2, 0) * A3435 - m(2, 3) * A0435 + m(2, 4) * A0335;
                const float B025235 = m(2, 0) * A2535 - m(2, 2) * A0535 + m(2, 5) * A0235;
                const float B024235 = m(2, 0) * A2435 - m(2, 2) * A0435 + m(2, 4) * A0235;
                const float B023235 = m(2, 0) * A2335 - m(2, 2) * A0335 + m(2, 3) * A0235;
                const float B045234 = m(2, 0) * A4534 - m(2, 4) * A0534 + m(2, 5) * A0434;
                const float B035234 = m(2, 0) * A3534 - m(2, 3) * A0534 + m(2, 5) * A0334;
                const float B034234 = m(2, 0) * A3434 - m(2, 3) * A0434 + m(2, 4) * A0334;
                const float B025234 = m(2, 0) * A2534 - m(2, 2) * A0534 + m(2, 5) * A0234;
                const float B024234 = m(2, 0) * A2434 - m(2, 2) * A0434 + m(2, 4) * A0234;
                const float B023234 = m(2, 0) * A2334 - m(2, 2) * A0334 + m(2, 3) * A0234;
                const float B015245 = m(2, 0) * A1545 - m(2, 1) * A0545 + m(2, 5) * A0145;
                const float B014245 = m(2, 0) * A1445 - m(2, 1) * A0445 + m(2, 4) * A0145;
                const float B013245 = m(2, 0) * A1345 - m(2, 1) * A0345 + m(2, 3) * A0145;
                const float B015235 = m(2, 0) * A1535 - m(2, 1) * A0535 + m(2, 5) * A0135;
                const float B014235 = m(2, 0) * A1435 - m(2, 1) * A0435 + m(2, 4) * A0135;
                const float B013235 = m(2, 0) * A1335 - m(2, 1) * A0335 + m(2, 3) * A0135;
                const float B015234 = m(2, 0) * A1534 - m(2, 1) * A0534 + m(2, 5) * A0134;
                const float B014234 = m(2, 0) * A1434 - m(2, 1) * A0434 + m(2, 4) * A0134;
                const float B013234 = m(2, 0) * A1334 - m(2, 1) * A0334 + m(2, 3) * A0134;
                const float B012245 = m(2, 0) * A1245 - m(2, 1) * A0245 + m(2, 2) * A0145;
                const float B012235 = m(2, 0) * A1235 - m(2, 1) * A0235 + m(2, 2) * A0135;
                const float B012234 = m(2, 0) * A1234 - m(2, 1) * A0234 + m(2, 2) * A0134;

                const float C23452345 = m(2, 2) * B345345 - m(2, 3) * B245345 + m(2, 4) * B235345 - m(2, 5) * B234345;
                const float C13452345 = m(2, 1) * B345345 - m(2, 3) * B145345 + m(2, 4) * B135345 - m(2, 5) * B134345;
                const float C12452345 = m(2, 1) * B245345 - m(2, 2) * B145345 + m(2, 4) * B125345 - m(2, 5) * B124345;
                const float C12352345 = m(2, 1) * B235345 - m(2, 2) * B135345 + m(2, 3) * B125345 - m(2, 5) * B123345;
                const float C12342345 = m(2, 1) * B234345 - m(2, 2) * B134345 + m(2, 3) * B124345 - m(2, 4) * B123345;
                const float C03452345 = m(2, 0) * B345345 - m(2, 3) * B045345 + m(2, 4) * B035345 - m(2, 5) * B034345;
                const float C02452345 = m(2, 0) * B245345 - m(2, 2) * B045345 + m(2, 4) * B025345 - m(2, 5) * B024345;
                const float C02352345 = m(2, 0) * B235345 - m(2, 2) * B035345 + m(2, 3) * B025345 - m(2, 5) * B023345;
                const float C02342345 = m(2, 0) * B234345 - m(2, 2) * B034345 + m(2, 3) * B024345 - m(2, 4) * B023345;
                const float C01452345 = m(2, 0) * B145345 - m(2, 1) * B045345 + m(2, 4) * B015345 - m(2, 5) * B014345;
                const float C01352345 = m(2, 0) * B135345 - m(2, 1) * B035345 + m(2, 3) * B015345 - m(2, 5) * B013345;
                const float C01342345 = m(2, 0) * B134345 - m(2, 1) * B034345 + m(2, 3) * B014345 - m(2, 4) * B013345;
                const float C01252345 = m(2, 0) * B125345 - m(2, 1) * B025345 + m(2, 2) * B015345 - m(2, 5) * B012345;
                const float C01242345 = m(2, 0) * B124345 - m(2, 1) * B024345 + m(2, 2) * B014345 - m(2, 4) * B012345;
                const float C01232345 = m(2, 0) * B123345 - m(2, 1) * B023345 + m(2, 2) * B013345 - m(2, 3) * B012345;
                const float C23451345 = m(1, 2) * B345345 - m(1, 3) * B245345 + m(1, 4) * B235345 - m(1, 5) * B234345;
                const float C13451345 = m(1, 1) * B345345 - m(1, 3) * B145345 + m(1, 4) * B135345 - m(1, 5) * B134345;
                const float C12451345 = m(1, 1) * B245345 - m(1, 2) * B145345 + m(1, 4) * B125345 - m(1, 5) * B124345;
                const float C12351345 = m(1, 1) * B235345 - m(1, 2) * B135345 + m(1, 3) * B125345 - m(1, 5) * B123345;
                const float C12341345 = m(1, 1) * B234345 - m(1, 2) * B134345 + m(1, 3) * B124345 - m(1, 4) * B123345;
                const float C23451245 = m(1, 2) * B345245 - m(1, 3) * B245245 + m(1, 4) * B235245 - m(1, 5) * B234245;
                const float C13451245 = m(1, 1) * B345245 - m(1, 3) * B145245 + m(1, 4) * B135245 - m(1, 5) * B134245;
                const float C12451245 = m(1, 1) * B245245 - m(1, 2) * B145245 + m(1, 4) * B125245 - m(1, 5) * B124245;
                const float C12351245 = m(1, 1) * B235245 - m(1, 2) * B135245 + m(1, 3) * B125245 - m(1, 5) * B123245;
                const float C12341245 = m(1, 1) * B234245 - m(1, 2) * B134245 + m(1, 3) * B124245 - m(1, 4) * B123245;
                const float C23451235 = m(1, 2) * B345235 - m(1, 3) * B245235 + m(1, 4) * B235235 - m(1, 5) * B234235;
                const float C13451235 = m(1, 1) * B345235 - m(1, 3) * B145235 + m(1, 4) * B135235 - m(1, 5) * B134235;
                const float C12451235 = m(1, 1) * B245235 - m(1, 2) * B145235 + m(1, 4) * B125235 - m(1, 5) * B124235;
                const float C12351235 = m(1, 1) * B235235 - m(1, 2) * B135235 + m(1, 3) * B125235 - m(1, 5) * B123235;
                const float C12341235 = m(1, 1) * B234235 - m(1, 2) * B134235 + m(1, 3) * B124235 - m(1, 4) * B123235;
                const float C23451234 = m(1, 2) * B345234 - m(1, 3) * B245234 + m(1, 4) * B235234 - m(1, 5) * B234234;
                const float C13451234 = m(1, 1) * B345234 - m(1, 3) * B145234 + m(1, 4) * B135234 - m(1, 5) * B134234;
                const float C12451234 = m(1, 1) * B245234 - m(1, 2) * B145234 + m(1, 4) * B125234 - m(1, 5) * B124234;
                const float C12351234 = m(1, 1) * B235234 - m(1, 2) * B135234 + m(1, 3) * B125234 - m(1, 5) * B123234;
                const float C12341234 = m(1, 1) * B234234 - m(1, 2) * B134234 + m(1, 3) * B124234 - m(1, 4) * B123234;
                const float C03451345 = m(1, 0) * B345345 - m(1, 3) * B045345 + m(1, 4) * B035345 - m(1, 5) * B034345;
                const float C02451345 = m(1, 0) * B245345 - m(1, 2) * B045345 + m(1, 4) * B025345 - m(1, 5) * B024345;
                const float C02351345 = m(1, 0) * B235345 - m(1, 2) * B035345 + m(1, 3) * B025345 - m(1, 5) * B023345;
                const float C02341345 = m(1, 0) * B234345 - m(1, 2) * B034345 + m(1, 3) * B024345 - m(1, 4) * B023345;
                const float C03451245 = m(1, 0) * B345245 - m(1, 3) * B045245 + m(1, 4) * B035245 - m(1, 5) * B034245;
                const float C02451245 = m(1, 0) * B245245 - m(1, 2) * B045245 + m(1, 4) * B025245 - m(1, 5) * B024245;
                const float C02351245 = m(1, 0) * B235245 - m(1, 2) * B035245 + m(1, 3) * B025245 - m(1, 5) * B023245;
                const float C02341245 = m(1, 0) * B234245 - m(1, 2) * B034245 + m(1, 3) * B024245 - m(1, 4) * B023245;
                const float C03451235 = m(1, 0) * B345235 - m(1, 3) * B045235 + m(1, 4) * B035235 - m(1, 5) * B034235;
                const float C02451235 = m(1, 0) * B245235 - m(1, 2) * B045235 + m(1, 4) * B025235 - m(1, 5) * B024235;
                const float C02351235 = m(1, 0) * B235235 - m(1, 2) * B035235 + m(1, 3) * B025235 - m(1, 5) * B023235;
                const float C02341235 = m(1, 0) * B234235 - m(1, 2) * B034235 + m(1, 3) * B024235 - m(1, 4) * B023235;
                const float C03451234 = m(1, 0) * B345234 - m(1, 3) * B045234 + m(1, 4) * B035234 - m(1, 5) * B034234;
                const float C02451234 = m(1, 0) * B245234 - m(1, 2) * B045234 + m(1, 4) * B025234 - m(1, 5) * B024234;
                const float C02351234 = m(1, 0) * B235234 - m(1, 2) * B035234 + m(1, 3) * B025234 - m(1, 5) * B023234;
                const float C02341234 = m(1, 0) * B234234 - m(1, 2) * B034234 + m(1, 3) * B024234 - m(1, 4) * B023234;
                const float C01451345 = m(1, 0) * B145345 - m(1, 1) * B045345 + m(1, 4) * B015345 - m(1, 5) * B014345;
                const float C01351345 = m(1, 0) * B135345 - m(1, 1) * B035345 + m(1, 3) * B015345 - m(1, 5) * B013345;
                const float C01341345 = m(1, 0) * B134345 - m(1, 1) * B034345 + m(1, 3) * B014345 - m(1, 4) * B013345;
                const float C01451245 = m(1, 0) * B145245 - m(1, 1) * B045245 + m(1, 4) * B015245 - m(1, 5) * B014245;
                const float C01351245 = m(1, 0) * B135245 - m(1, 1) * B035245 + m(1, 3) * B015245 - m(1, 5) * B013245;
                const float C01341245 = m(1, 0) * B134245 - m(1, 1) * B034245 + m(1, 3) * B014245 - m(1, 4) * B013245;
                const float C01451235 = m(1, 0) * B145235 - m(1, 1) * B045235 + m(1, 4) * B015235 - m(1, 5) * B014235;
                const float C01351235 = m(1, 0) * B135235 - m(1, 1) * B035235 + m(1, 3) * B015235 - m(1, 5) * B013235;
                const float C01341235 = m(1, 0) * B134235 - m(1, 1) * B034235 + m(1, 3) * B014235 - m(1, 4) * B013235;
                const float C01451234 = m(1, 0) * B145234 - m(1, 1) * B045234 + m(1, 4) * B015234 - m(1, 5) * B014234;
                const float C01351234 = m(1, 0) * B135234 - m(1, 1) * B035234 + m(1, 3) * B015234 - m(1, 5) * B013234;
                const float C01341234 = m(1, 0) * B134234 - m(1, 1) * B034234 + m(1, 3) * B014234 - m(1, 4) * B013234;
                const float C01251345 = m(1, 0) * B125345 - m(1, 1) * B025345 + m(1, 2) * B015345 - m(1, 5) * B012345;
                const float C01241345 = m(1, 0) * B124345 - m(1, 1) * B024345 + m(1, 2) * B014345 - m(1, 4) * B012345;
                const float C01251245 = m(1, 0) * B125245 - m(1, 1) * B025245 + m(1, 2) * B015245 - m(1, 5) * B012245;
                const float C01241245 = m(1, 0) * B124245 - m(1, 1) * B024245 + m(1, 2) * B014245 - m(1, 4) * B012245;
                const float C01251235 = m(1, 0) * B125235 - m(1, 1) * B025235 + m(1, 2) * B015235 - m(1, 5) * B012235;
                const float C01241235 = m(1, 0) * B124235 - m(1, 1) * B024235 + m(1, 2) * B014235 - m(1, 4) * B012235;
                const float C01251234 = m(1, 0) * B125234 - m(1, 1) * B025234 + m(1, 2) * B015234 - m(1, 5) * B012234;
                const float C01241234 = m(1, 0) * B124234 - m(1, 1) * B024234 + m(1, 2) * B014234 - m(1, 4) * B012234;
                const float C01231345 = m(1, 0) * B123345 - m(1, 1) * B023345 + m(1, 2) * B013345 - m(1, 3) * B012345;
                const float C01231245 = m(1, 0) * B123245 - m(1, 1) * B023245 + m(1, 2) * B013245 - m(1, 3) * B012245;
                const float C01231235 = m(1, 0) * B123235 - m(1, 1) * B023235 + m(1, 2) * B013235 - m(1, 3) * B012235;
                const float C01231234 = m(1, 0) * B123234 - m(1, 1) * B023234 + m(1, 2) * B013234 - m(1, 3) * B012234;

                determinant = m(0, 0) * (m(1, 1) * C23452345 - m(1, 2) * C13452345 + m(1, 3) * C12452345 -
                                         m(1, 4) * C12352345 + m(1, 5) * C12342345) -
                              m(0, 1) * (m(1, 0) * C23452345 - m(1, 2) * C03452345 + m(1, 3) * C02452345 -
                                         m(1, 4) * C02352345 + m(1, 5) * C02342345) +
                              m(0, 2) * (m(1, 0) * C13452345 - m(1, 1) * C03452345 + m(1, 3) * C01452345 -
                                         m(1, 4) * C01352345 + m(1, 5) * C01342345) -
                              m(0, 3) * (m(1, 0) * C12452345 - m(1, 1) * C02452345 + m(1, 2) * C01452345 -
                                         m(1, 4) * C01252345 + m(1, 5) * C01242345) +
                              m(0, 4) * (m(1, 0) * C12352345 - m(1, 1) * C02352345 + m(1, 2) * C01352345 -
                                         m(1, 3) * C01252345 + m(1, 5) * C01232345) -
                              m(0, 5) * (m(1, 0) * C12342345 - m(1, 1) * C02342345 + m(1, 2) * C01342345 -
                                         m(1, 3) * C01242345 + m(1, 4) * C01232345);
                determinant = 1 / determinant;

                rm(0, 0) = determinant * (m(1, 1) * C23452345 - m(1, 2) * C13452345 + m(1, 3) * C12452345 -
                                          m(1, 4) * C12352345 + m(1, 5) * C12342345);
                rm(0, 1) = determinant * -(m(0, 1) * C23452345 - m(0, 2) * C13452345 + m(0, 3) * C12452345 -
                                           m(0, 4) * C12352345 + m(0, 5) * C12342345);
                rm(0, 2) = determinant * (m(0, 1) * C23451345 - m(0, 2) * C13451345 + m(0, 3) * C12451345 -
                                          m(0, 4) * C12351345 + m(0, 5) * C12341345);
                rm(0, 3) = determinant * -(m(0, 1) * C23451245 - m(0, 2) * C13451245 + m(0, 3) * C12451245 -
                                           m(0, 4) * C12351245 + m(0, 5) * C12341245);
                rm(0, 4) = determinant * (m(0, 1) * C23451235 - m(0, 2) * C13451235 + m(0, 3) * C12451235 -
                                          m(0, 4) * C12351235 + m(0, 5) * C12341235);
                rm(0, 5) = determinant * -(m(0, 1) * C23451234 - m(0, 2) * C13451234 + m(0, 3) * C12451234 -
                                           m(0, 4) * C12351234 + m(0, 5) * C12341234);
                rm(1, 0) = determinant * -(m(1, 0) * C23452345 - m(1, 2) * C03452345 + m(1, 3) * C02452345 -
                                           m(1, 4) * C02352345 + m(1, 5) * C02342345);
                rm(1, 1) = determinant * (m(0, 0) * C23452345 - m(0, 2) * C03452345 + m(0, 3) * C02452345 -
                                          m(0, 4) * C02352345 + m(0, 5) * C02342345);
                rm(1, 2) = determinant * -(m(0, 0) * C23451345 - m(0, 2) * C03451345 + m(0, 3) * C02451345 -
                                           m(0, 4) * C02351345 + m(0, 5) * C02341345);
                rm(1, 3) = determinant * (m(0, 0) * C23451245 - m(0, 2) * C03451245 + m(0, 3) * C02451245 -
                                          m(0, 4) * C02351245 + m(0, 5) * C02341245);
                rm(1, 4) = determinant * -(m(0, 0) * C23451235 - m(0, 2) * C03451235 + m(0, 3) * C02451235 -
                                           m(0, 4) * C02351235 + m(0, 5) * C02341235);
                rm(1, 5) = determinant * (m(0, 0) * C23451234 - m(0, 2) * C03451234 + m(0, 3) * C02451234 -
                                          m(0, 4) * C02351234 + m(0, 5) * C02341234);
                rm(2, 0) = determinant * (m(1, 0) * C13452345 - m(1, 1) * C03452345 + m(1, 3) * C01452345 -
                                          m(1, 4) * C01352345 + m(1, 5) * C01342345);
                rm(2, 1) = determinant * -(m(0, 0) * C13452345 - m(0, 1) * C03452345 + m(0, 3) * C01452345 -
                                           m(0, 4) * C01352345 + m(0, 5) * C01342345);
                rm(2, 2) = determinant * (m(0, 0) * C13451345 - m(0, 1) * C03451345 + m(0, 3) * C01451345 -
                                          m(0, 4) * C01351345 + m(0, 5) * C01341345);
                rm(2, 3) = determinant * -(m(0, 0) * C13451245 - m(0, 1) * C03451245 + m(0, 3) * C01451245 -
                                           m(0, 4) * C01351245 + m(0, 5) * C01341245);
                rm(2, 4) = determinant * (m(0, 0) * C13451235 - m(0, 1) * C03451235 + m(0, 3) * C01451235 -
                                          m(0, 4) * C01351235 + m(0, 5) * C01341235);
                rm(2, 5) = determinant * -(m(0, 0) * C13451234 - m(0, 1) * C03451234 + m(0, 3) * C01451234 -
                                           m(0, 4) * C01351234 + m(0, 5) * C01341234);
                rm(3, 0) = determinant * -(m(1, 0) * C12452345 - m(1, 1) * C02452345 + m(1, 2) * C01452345 -
                                           m(1, 4) * C01252345 + m(1, 5) * C01242345);
                rm(3, 1) = determinant * (m(0, 0) * C12452345 - m(0, 1) * C02452345 + m(0, 2) * C01452345 -
                                          m(0, 4) * C01252345 + m(0, 5) * C01242345);
                rm(3, 2) = determinant * -(m(0, 0) * C12451345 - m(0, 1) * C02451345 + m(0, 2) * C01451345 -
                                           m(0, 4) * C01251345 + m(0, 5) * C01241345);
                rm(3, 3) = determinant * (m(0, 0) * C12451245 - m(0, 1) * C02451245 + m(0, 2) * C01451245 -
                                          m(0, 4) * C01251245 + m(0, 5) * C01241245);
                rm(3, 4) = determinant * -(m(0, 0) * C12451235 - m(0, 1) * C02451235 + m(0, 2) * C01451235 -
                                           m(0, 4) * C01251235 + m(0, 5) * C01241235);
                rm(3, 5) = determinant * (m(0, 0) * C12451234 - m(0, 1) * C02451234 + m(0, 2) * C01451234 -
                                          m(0, 4) * C01251234 + m(0, 5) * C01241234);
                rm(4, 0) = determinant * (m(1, 0) * C12352345 - m(1, 1) * C02352345 + m(1, 2) * C01352345 -
                                          m(1, 3) * C01252345 + m(1, 5) * C01232345);
                rm(4, 1) = determinant * -(m(0, 0) * C12352345 - m(0, 1) * C02352345 + m(0, 2) * C01352345 -
                                           m(0, 3) * C01252345 + m(0, 5) * C01232345);
                rm(4, 2) = determinant * (m(0, 0) * C12351345 - m(0, 1) * C02351345 + m(0, 2) * C01351345 -
                                          m(0, 3) * C01251345 + m(0, 5) * C01231345);
                rm(4, 3) = determinant * -(m(0, 0) * C12351245 - m(0, 1) * C02351245 + m(0, 2) * C01351245 -
                                           m(0, 3) * C01251245 + m(0, 5) * C01231245);
                rm(4, 4) = determinant * (m(0, 0) * C12351235 - m(0, 1) * C02351235 + m(0, 2) * C01351235 -
                                          m(0, 3) * C01251235 + m(0, 5) * C01231235);
                rm(4, 5) = determinant * -(m(0, 0) * C12351234 - m(0, 1) * C02351234 + m(0, 2) * C01351234 -
                                           m(0, 3) * C01251234 + m(0, 5) * C01231234);
                rm(5, 0) = determinant * -(m(1, 0) * C12342345 - m(1, 1) * C02342345 + m(1, 2) * C01342345 -
                                           m(1, 3) * C01242345 + m(1, 4) * C01232345);
                rm(5, 1) = determinant * (m(0, 0) * C12342345 - m(0, 1) * C02342345 + m(0, 2) * C01342345 -
                                          m(0, 3) * C01242345 + m(0, 4) * C01232345);
                rm(5, 2) = determinant * -(m(0, 0) * C12341345 - m(0, 1) * C02341345 + m(0, 2) * C01341345 -
                                           m(0, 3) * C01241345 + m(0, 4) * C01231345);
                rm(5, 3) = determinant * (m(0, 0) * C12341245 - m(0, 1) * C02341245 + m(0, 2) * C01341245 -
                                          m(0, 3) * C01241245 + m(0, 4) * C01231245);
                rm(5, 4) = determinant * -(m(0, 0) * C12341235 - m(0, 1) * C02341235 + m(0, 2) * C01341235 -
                                           m(0, 3) * C01241235 + m(0, 4) * C01231235);
                rm(5, 5) = determinant * (m(0, 0) * C12341234 - m(0, 1) * C02341234 + m(0, 2) * C01341234 -
                                          m(0, 3) * C01241234 + m(0, 4) * C01231234);
                break;
            }
            default: {
                while (1) {
                };
            }
        }
        return rm;
    }

    operator float() {
        if (num_rows != 1 || num_cols != 1) {
            while (1) {
            };
        }
        return data[0];
    }

    static Matrix eye(size_t size) { return Matrix(size); }

private:
    Matrix(size_t size) : num_rows(size), num_cols(size) {
        for (size_t i = 0; i < num_rows; i++) {
            for (size_t j = 0; j < num_cols; j++) {
                operator()(i, j) = i == j ? 1 : 0;
            }
        }
    }

    template <typename InputIt1, typename InputIt2, typename T>
    static T inner_product(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init) {
        while (first1 != last1) {
            init += *first1++ * *first2++;
        }
        return init;
    }

    size_t num_rows;
    size_t num_cols;
    static constexpr size_t max_size = 36;
    static constexpr float threshold = 0.00001;
    float data[max_size];
    float row_cache[max_size];
    float col_cache[max_size];
};

}  // namespace mtrn3100
