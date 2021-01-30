#ifndef VACUUMSIMULATOR_VACSIMULATOR_H
#define VACUUMSIMULATOR_VACSIMULATOR_H

#include "vacuum_simulator/occ_grid.h"

#include <vector>
#include <random>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace vacuum_simulator {

    class Simulator {
    public:
        Simulator(double x_min, double x_max1, double x_max2, double y_min, double y_max1, double y_max2);

        // run simulation until a desired amount of time has elapsed
        // returns the percent swept at the end
        // outputs results to txt file for plotting
        double RunSimulationTime(double x_start, double y_start, double h_start, int time_ms);

        // run simulation until a desired percentage of the room area has been swept
        // returns the time elapsed to sweep the given percent
        // uses a smarter way to choose a new direction when encountering a wall
        int RunSimulationPercent(double desired_pct);

    private:

        // move the robot forward a timestep, at its constant speed
        void RobotMotion();

        // returns true if the robot has hit a wall
        // adjusts its position coordinates to keep it in-bounds
        bool CheckCollision(double& x, double& y, double inflation);

        // when encountering a wall, choose a new direction to travel randomly
        void NewHeading();

        // when encountering a wall, a new direction is chosen with double probability if is unswept than if it is already swept
        void NewSmartHeading();

        // random heading generator
        std::random_device random_device_;
        std::mt19937 random_engine_;
        std::uniform_real_distribution<double> heading_distribution_;
        //random position generator
        std::uniform_real_distribution<double> position_distribution_;
        std::uniform_int_distribution<> start_quadrant_distribution_;

        // occupancy grid of room
        std::unique_ptr<Grid> room_grid_;

        // struct that enables us to keep track of probability indices after sorting
        struct ProbValue{
            double prob;
            int idx;
        };

        // robot state
        double x_state_;
        double y_state_;
        double h_state_;

        // parameters
        const double robot_diameter_m_ = 0.25;
        const double inflation_ = robot_diameter_m_ / 2.0;
        const double robot_speed_mps_ = 0.01;  // meters per tenth of a second (timestep)
        double x_min_;  // left wall
        double x_max1_;  // right wall of lower part of L-shaped room
        double x_max2_;  // right wall of upper part of L-shaped room
        double y_min_;  // bottom wall
        double y_max1_;  // top wall of left part of L-shaped room
        double y_max2_;  // top wall of right part of L-shaped room
    };

} // namespace vacuum_simulator


#endif //VACUUMSIMULATOR_VACSIMULATOR_H
