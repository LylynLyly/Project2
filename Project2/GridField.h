#pragma once
template<typename T>
class GridField
{
private:
	size_t width_;
	size_t height_;
	std::vector<T> data;
public:
	GridField(int x, int y, int z, T init);
	T& at(int x, int y);
	const T& at(int x, int y) const;
	size_t width() const;
	size_t height() const;
};

