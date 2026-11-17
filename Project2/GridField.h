#pragma once
template<typename T>
class GridField
{
private:
	size_t width_;
	size_t height_;
	std::vector<T> data;
public:
	GridField(size_t width, size_t height, T initial);
	T& at(size_t x, size_t y);
	const T& at(size_t x, size_t y) const;
	size_t width() const;
	size_t height() const;
};

