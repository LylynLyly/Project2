#include "GridField.h"
#include <stdexcept>

template<typename T>
GridField<T>::GridField(size_t width,size_t height,T initial) : width_(width), height_(height), data(width* height, initial) { }

template<typename T> T& GridField<T>::at(size_t x, size_t y)
{
    if (x >= width_ || y >= height_)
        throw std::out_of_range("GridField index out of range");

    return data[y * width_ + x];
}

template<typename T> const T& GridField<T>::at(size_t x, size_t y) const
{
    if (x >= width_ || y >= height_)
        throw std::out_of_range("GridField index out of range");

    return data[y * width_ + x];
}

template<typename T> size_t GridField<T>::width() const { return width_; }

template<typename T> size_t GridField<T>::height() const { return height_; }

template class GridField<float>;
template class GridField<int>;