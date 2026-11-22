#include <cmath>

double gaussian(double distance, double scale)
{
    return std::exp(-(distance * distance) / (2.0 * scale * scale));
}
