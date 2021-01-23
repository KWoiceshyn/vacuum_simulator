#include "vacuum_simulator/occ_grid.h"

namespace vacuum_simulator{

    Grid::Grid(double x_min, double x_max1, double x_max2, double y_min, double y_max1, double y_max2) {
        auto x_range = static_cast<int> ((x_max1 - x_min) / resolution_);
        auto y_range = static_cast<int> ((y_max1 - y_min) / resolution_);
        for (auto i = 0; i < x_range; ++i) {
            grid_.emplace_back(std::vector<uint8_t> (y_range, 0));
        }
        auto x_block = static_cast<int> ((x_max1 - x_max2) / resolution_);
        auto y_block = static_cast<int> ((y_max1 - y_max2) / resolution_);
        total_cells_ = (x_range * y_range) - (x_block * y_block);
    }

    void Grid::Sweep(double x_pos, double y_pos, double diameter) {
        auto cx = static_cast<int>(x_pos/resolution_);
        auto cy = static_cast<int>(y_pos/resolution_);
        double radius = diameter / 2.0;
        auto radius_int = static_cast<int>(radius/resolution_);
        double r_squared = radius * radius;
        for (int row = cy - radius_int; row < cy + radius_int; ++row) {
            for (int col = cx - radius_int; col < cx + radius_int; ++col) {
                if (grid_[row][col] == 0) {
                    double dx = std::fabs(x_pos - col * resolution_);
                    double dy = std::fabs(y_pos - row * resolution_);
                    double dist_squared = dx * dx + dy * dy;
                    if (dist_squared <= r_squared) {
                        // inside the circle, fill in this cell (col, row)
                        grid_[row][col] = 1;
                        ++cells_filled_;
                    }
                }
            }
        }
    }

    double Grid::GetPercentSwept() {
        return (static_cast<double>(cells_filled_) / total_cells_) * 100.0;
    }

    void Grid::WriteGridToFile() {
        std::ofstream ofsg;
        ofsg.open("../grid.txt");
        for (const auto& row : grid_) {
            for (const auto& val : row) {
                ofsg << (int)val <<" ";
            }
            ofsg << std::endl;
        }
        ofsg.close();
    }

    double Grid::CellOccupied(double x, double y) {
        auto cx = static_cast<int>(x / resolution_);
        auto cy = static_cast<int>(y / resolution_);
        // assumes bounds have been checked
        if(grid_[cy][cx] == 0)
            return 2.0; // double probability if direction is 'dirty'
        else
            return 1.0;
    }

} // namespace vacuum_simulator

