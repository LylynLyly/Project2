#pragma once
#include <cstddef>
#include <vector>
template<typename T>
class GridField
{
private:
	std::size_t width_;
	std::size_t height_;
	std::vector<T> data;
public:
	GridField(std::size_t width, std::size_t height, T initial);
	T& at(std::size_t x, std::size_t y);
	const T& at(std::size_t x, std::size_t y) const;
	std::size_t width() const;
	std::size_t height() const;
};

