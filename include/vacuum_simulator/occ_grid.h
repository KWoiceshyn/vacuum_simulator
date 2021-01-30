#ifndef VACUUMSIMULATOR_OCCGRID_H
#define VACUUMSIMULATOR_OCCGRID_H

#include <vector>
#include <cmath>
#include <fstream>

namespace vacuum_simulator{
    class Grid{
    public:
        // create an occupancy grid for an L-shaped room
        // x_max1 and y_max1 are the larger dimensions
        // x_max2 and y_max2 are the smaller dimensions
        Grid(double x_min, double x_max1, double x_max2, double y_min, double y_max1, double y_max2);

        // given the robot's current position and diameter, fill in all cells inside of the circle of the robot
        void Sweep(double x_pos, double y_pos, double diameter);

        // compute the percent area of the room swept
        double GetPercentSwept();

        // writes the occupancy grid cell values {0,1} to a text file
        void WriteGridToFile();

        // returns 2.0 if a cell has not been swept, 1.0 otherwise
        double CellOccupied(double x, double y);

    private:
        std::vector<std::vector<uint8_t>> grid_;
        int total_cells_;
        int cells_filled_ = 0;
        const double resolution_ = 0.01; // 1 cm cell
    };

} // namespace vacuum_simulator

#endif //VACUUMSIMULATOR_OCCGRID_H
