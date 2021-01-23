#ifndef VACUUMSIMULATOR_OCCGRID_H
#define VACUUMSIMULATOR_OCCGRID_H

#include <vector>
#include <cmath>
#include <fstream>

namespace vacuum_simulator{
    class Grid{
    public:
        Grid(double x_min, double x_max1, double x_max2, double y_min, double y_max1, double y_max2);
        void Sweep(double x_pos, double y_pos, double diameter);
        double GetPercentSwept();
        void WriteGridToFile();
        double CellOccupied(double x, double y);

    private:
        std::vector<std::vector<uint8_t>> grid_;
        int total_cells_;
        int cells_filled_ = 0;
        const double resolution_ = 0.01; // 1 cm cell
    };

} // namespace vacuum_simulator

#endif //VACUUMSIMULATOR_OCCGRID_H
