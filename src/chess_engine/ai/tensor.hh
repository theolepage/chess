#pragma once

#include <ostream>
#include <memory>
#include <functional>
#include <cassert>
#include <cstdarg>
#include <algorithm>
#include <iostream>
#include <vector>

enum class fill_type
{
    RANDOM,
    SEQUENCE,
    ZEROS,
    ONES
};

enum class transpose
{
    LEFT,
    RIGHT,
    NO_IMPLICIT
};

template <typename T>
class Tensor
{
public:
    /**
    * Constructors
    */

    Tensor()
        : size_(0), shape_(0), data_(nullptr), transposed_(false)
    {}

    Tensor(const std::vector<int>& shape) 
    {
        shape_ = std::make_shared<std::vector<int>>(shape);
        size_ = compute_size(shape_);
        data_ = std::shared_ptr<T[]>(new T[size_]);
        offset_ = 0L;
        transposed_ = false;
    }

    Tensor(const Tensor<T>& t)
    {
        size_ = t.size_;
        shape_ = t.shape_;
        data_ = t.data_;
        offset_ = 0L;
        transposed_ = t.transposed_;
    }

    Tensor& operator=(const Tensor<T>& t)
    {
        size_ = t.size_;
        shape_ = t.shape_;
        data_ = t.data_;
        offset_ = 0L;
        transposed_ = t.transposed_;
        return *this;
    }

    virtual ~Tensor() = default;

    bool operator==(const Tensor<T>& t)
    {
        if (*shape_ != t.get_shape())
            return false;
        for (int i = 0; i < size_; ++i)
        {
            if (data_[i] != t.data_[i])
                return false;
        }
        return true;
    }

    /**
    * Getters / setters
    */

    std::vector<int> get_shape() const
    {
        return *shape_;
    }

    int get_size(void) const
    {
        return size_;
    }

    template<typename ...Ts>
    T operator()(Ts&& ...coords) const
    {
        const std::vector<int> vec = {coords ...};
        return (*this)(vec);
    }

    template<typename ...Ts>
    T& operator()(Ts&& ...coords)
    {
        const std::vector<int> vec = {coords ...};
        return (*this)(vec);
    }

    T operator()(const std::vector<int>& coords) const
    {
        return data_[coord_to_index(coords)];
    }

    T& operator()(const std::vector<int>& coords)
    {
        return data_[coord_to_index(coords)];
    }

    Tensor<T>& reshape(std::vector<int> shape)
    {
        auto new_shape = std::make_shared<std::vector<int>>(shape);
        if (compute_size(new_shape) != compute_size(shape_))
            throw "New shape must be of same size.";

        shape_ = new_shape;
        return *this;
    }

    /**
    * Operations (fill, map, op)
    */

    template <typename FUNCTOR_TYPE>
    void fill(FUNCTOR_TYPE& func)
    {
        fill(func());
    }

    void fill(std::function<T(void)> value_initializer)
    {
        fill(value_initializer());
    }

    void fill(T value)
    {
        assert(data_ != nullptr);

        #pragma omp parallel for
        for (long long i = offset_; i < size_; i++)
            data_[i] = value;
    }

    void fill(fill_type type)
    {
        assert(data_ != nullptr);

        switch(type)
        {#pragma omp parallel for
            case fill_type::RANDOM:
                for (long long i = offset_; i < size_; i++)
                    data_[i] = get_random_float();
                break;
            case fill_type::SEQUENCE:
                for (long long i = offset_; i < size_; i++)
                    data_[i] = static_cast<T>(i);
                break;
            case fill_type::ZEROS:
                for (long long i = offset_; i < size_; i++)
                    data_[i] = static_cast<T>(0);
                break;
            case fill_type::ONES:
                for (long long i = offset_; i < size_; i++)
                    data_[i] = static_cast<T>(1);
                break;
        }
    }

    Tensor<T>& map_inplace(std::function<T(T)> value_initializer)
    {
        assert(data_ != nullptr);

        #pragma omp parallel for
        for (long long i = offset_; i < size_; i++)
            data_[i] = value_initializer(data_[i]);

        return *this;
    }

    Tensor<T> map(std::function<T(T)> value_initializer) const
    {
        assert(data_ != nullptr);

        Tensor<T> res(*shape_);

        #pragma omp parallel for
        for (long long i = offset_; i < size_; i++)
            res.data_[i] = value_initializer(data_[i]);

        return res;
    }

    Tensor<T> op(const Tensor &right, std::function<T(T, T)> fn) const
    {
        assert(data_ != nullptr);
        if (*shape_ != *right.shape_)
            throw std::invalid_argument("Operations requires the two tensors to have the same shape.");

        Tensor<T> res(*shape_);

        #pragma omp parallel for
        for (long long i = offset_; i < size_; i++)
            res.data_[i] = fn(data_[i], right.data_[i]);
        
        return res;
    }

    Tensor<T>& op_inplace(const Tensor &right, std::function<T(T, T)> fn)
    {
        assert(data_ != nullptr);
        if (*shape_ != *right.shape_)
            throw std::invalid_argument("Operations requires the two tensors to have the same shape.");

        #pragma omp parallel for
        for (long long i = offset_; i < size_; i++)
            data_[i] = fn(data_[i], right.data_[i]);

        return *this;
    }

    Tensor<T>& op_inplace(T val, std::function<T(T, T)> fn)
    {
        assert(data_ != nullptr);

        #pragma omp parallel for
        for (int i = offset_; i < size_; i++)
            data_[i] = fn(data_[i], val);

        return *this;
    }

    Tensor<T>& operator+=(const Tensor &right)
    {
        return op_inplace(right, [](const T a, const T b) { return a + b; });
    }

    Tensor<T>& operator+=(const T v)
    {
        return op_inplace(v, [](const T a, const  T b) { return a + b; });
    }

    Tensor<T> operator+(const Tensor &right) const
    {
        return op(right, [](const T a, const T b) { return a + b; });
    }

    Tensor<T>& operator-=(const Tensor &right)
    {
        return op_inplace(right, [](const T a, const T b) { return a - b; });
    }

    Tensor<T>& operator-=(const T v)
    {
        return op_inplace(v, [](const T a, const  T b) { return a - b; });
    }

    Tensor<T> operator-(const Tensor &right) const
    {
        return op(right, [](const T a, const  T b) { return a - b; });
    }

    Tensor<T>& operator*=(const Tensor &right)
    {
        return op_inplace(right, [](const T a, const T b) { return a * b; });
    }

    Tensor<T>& operator*=(const T v)
    {
        return op_inplace(v, [](const T a, const  T b) { return a * b; });
    }

    Tensor<T> operator*(const Tensor &right) const
    {
        return op(right, [](const T a, const T b) { return a * b; });
    }

    Tensor<T>& operator/=(const Tensor &right)
    {
        return op_inplace(right, [](const T a, const T b) { return a / b; });
    }

    Tensor<T>& operator/=(const T v)
    {
        return op_inplace(v, [](const T a, const  T b) { return a / b; });
    }

    Tensor<T> operator/(const Tensor &right) const
    {
        return op(right, [](const T a, const T b) { return a / b; });
    }


    /**
    * Matrix operations (transpose, matmul)
    */

    Tensor<T> transpose(void) const
    {
        assert(data_ != nullptr);
        if (shape_->size() != 2)
            throw "Invalid shape for matrix transpose.";

        Tensor<T> res({shape_->at(1), shape_->at(0)});
        int rows = shape_->at(0);
        int cols = shape_->at(1);

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                res.data_[offset_ + j * rows + i] = data_[offset_ + i * cols + j];

        return res;
    }

    Tensor<T> matmul(const Tensor<T>& right) const
    {
        assert(data_ != nullptr);
        if (shape_->size() != 2)
            throw "Invalid shape for matrix multiplication.";

        int l_rows = shape_->at(0);
        int l_cols = shape_->at(1);
        int r_rows = right.shape_->at(0);
        int r_cols = right.shape_->at(1);

        if (r_rows != l_cols)
            throw std::invalid_argument("Invalid shapes for matrix multiplication.");

        Tensor<T> res({l_rows, r_cols});
        for (int r = 0; r < l_rows; r++)
        {
            for (int c = 0; c < r_cols; c++)
            {
                T tmp = 0;
                for (int k = 0; k < l_cols; k++)
                    tmp += data_[r * l_cols + k] * right.data_[k * r_cols + c];
                res.data_[r * r_cols + c] = tmp;
            }
        }
        return res;
    }

    /**
    * Miscellaneous (print)
    */

    friend std::ostream& operator<<(std::ostream& os, Tensor& t)
    {
        std::vector<int> shape = t.get_shape();
        std::vector<int> coord(shape.size());
        int l = shape.size() - 1;

        int new_line = shape.size();
        for (long long i = 0; i < t.size_; i++)
        {
            // Opening
            if (i != 0 && new_line > 0)
            {
                for (int i = 0; i < new_line; i++)
                    os << std::endl;
                for (int i = 0; i < l - new_line + 1; i++)
                    os << " ";
            }
            while (new_line > 0)
            {
                os << "[";
                new_line--;
            }

            os << " " << t(coord);
            coord[l] += 1;

            // Closing
            for (int j = coord.size() - 1; j >= 0; j--)
            {
                if (coord[j] == shape[j])
                {
                    new_line++;
                    coord[j] = 0;
                    if (j > 0)
                        coord[j - 1]++;
                    os << (j == l ? " ]" : "]");
                }
            }
        }
        os << std::endl;

        return os;
    }

private:
    long long size_;
    std::shared_ptr<std::vector<int>> shape_;
    std::shared_ptr<T[]> data_;
    long long offset_;
    bool transposed_;

    Tensor(std::vector<int> shape, std::shared_ptr<T[]> data)
    {
        shape_ = std::make_shared<std::vector<int>>(shape);
        size_ = compute_size(shape_);
        data_ = data;
        offset_ = 0;
        transposed_ = false;
    }

    long long compute_size(std::shared_ptr<std::vector<int>> shape)
    {
        long long res = 1;
        for (auto v : *shape)
            res *= v;
        return res;
    }

    long long coord_to_index(std::vector<int> coords) const
    {
        assert(coords.size() == shape_->size());
        for (unsigned i = 0; i < shape_->size(); i++)
            assert(coords[i] < shape_->at(i));

        long long res = 0;
        long long step = 1;

        for (int dim = shape_->size() - 1; dim >= 0; dim--)
        {
            res += coords[dim] * step;
            step *= shape_->at(dim);
        }

        return res + offset_;
    }

    float get_random_float(void) const
    {
        static constexpr float min = -1.0f;
        static constexpr float max = 1.0f;

        float d = static_cast<float>(RAND_MAX / (max - min));

        return min + static_cast<float>(rand() / d);
    }
};