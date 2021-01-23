#include "vacuum_simulator/vac_simulator.h"

namespace vacuum_simulator {

    Simulator::Simulator(double x_min, double x_max1, double x_max2, double y_min, double y_max1, double y_max2):
    x_min_(x_min),
    x_max1_(x_max1),
    x_max2_(x_max2),
    y_min_(y_min),
    y_max1_(y_max1),
    y_max2_(y_max2),
    x_state_(0.0),
    y_state_(0.0),
    h_state_(0.0),
    random_device_(),
    random_engine_(random_device_()),
    heading_distribution_(0.0, 2 * M_PI),
    start_quadrant_(0, 2),
    position_distribution_(x_min, x_max2)
    {

    }

    void Simulator::RunSimulationTime(double x_start, double y_start, double h_start, int time_ds) {
        x_state_ = x_start; y_state_ = y_start; h_state_ = h_start;
        std::ofstream ofs;
        ofs.open("../path.txt");
        room_grid_= std::make_unique<Grid>(x_min_, x_max1_, x_max2_, y_min_, y_max1_, y_max2_);
        for (auto t = 0; t < time_ds; ++t) {
            RobotMotion();
            if (CheckCollision(x_state_, y_state_, kInflation))
                NewHeading();
            ofs << x_state_ << " " << y_state_ << std::endl;
            room_grid_->Sweep(x_state_, y_state_, kRobotDiameterM);
        }
        ofs.close();
        std::cout << "Percent swept: " << room_grid_->GetPercentSwept() << std::endl;
        room_grid_->WriteGridToFile();
    }

    int Simulator::RunSimulationPercent(double desired_pct) {
        //random starting pose
        x_state_ = position_distribution_(random_engine_);
        y_state_ = position_distribution_(random_engine_);
        NewHeading();
        int start_quadrant = start_quadrant_(random_engine_);
        switch (start_quadrant) {
            case 0:
                // bottom left
                break;
            case 1:
                // top left
                y_state_ += y_max2_;
                break;
            case 2:
                // bottom right
                x_state_ += x_max2_;
                break;
        }
        std::ofstream ofs;
        ofs.open("../path.txt");
        room_grid_= std::make_unique<Grid>(x_min_, x_max1_, x_max2_, y_min_, y_max1_, y_max2_);
        int elapsed_time = 0;
        while (room_grid_->GetPercentSwept() < desired_pct) {
            ++elapsed_time;
            RobotMotion();
            if (CheckCollision(x_state_, y_state_, kInflation))
                NewSmartHeading();
            ofs << x_state_ << " " << y_state_ << std::endl;
            room_grid_->Sweep(x_state_, y_state_, kRobotDiameterM);
        }
        ofs.close();
        std::cout << "Time elapsed: " << elapsed_time/10 << " seconds." << std::endl;
        room_grid_->WriteGridToFile();
        return elapsed_time;
    }

    void Simulator::RobotMotion() {
        x_state_ += kRobotSpeedMpts * cos(h_state_);
        y_state_ += kRobotSpeedMpts * sin(h_state_);
    }

    bool Simulator::CheckCollision(double& x, double& y, double inflation) {
        bool collided = false;
        const double buf = 0.02;
        if (x < x_min_ + inflation || y < y_min_ + inflation) {
            x = std::max(x, x_min_ + inflation);
            y = std::max(y, y_min_ + inflation);
            collided = true;  // hitting left or bottom wall
        }
        if (x > x_max1_ - inflation || y > y_max1_ - inflation) {
            x = std::min(x, x_max1_ - inflation);
            y = std::min(y, y_max1_ - inflation);
            collided = true;  // top most or right most wall
        }
        if (x > x_max2_ - inflation && x < x_max2_ - inflation + buf && y > y_max2_ - inflation) {
            x = std::min(x, x_max2_ - inflation);
            collided = true;  // upper left corner, right wall
        }
        if (x > x_max2_ - inflation && y > y_max2_ - inflation && y < y_max2_ - inflation + buf) {
            y = std::min(y, y_max2_ - inflation);
            collided = true;  // lower right corner, upper wall
        }
        return collided;
    }

    double wrap_angle(double a){
        while (a > M_PI)
            a -= 2.0 * M_PI;
        while (a <= -M_PI)
            a += 2.0 * M_PI;
        return a;
    }

    void Simulator::NewHeading() {
        h_state_ = heading_distribution_(random_engine_);
    }

    struct ProbValue{
        double prob;
        int idx;
    };

    // Twice as likely to choose a direction that is 'unswept' than one that is swept
    void Simulator::NewSmartHeading() {
        const int kAngleIntervals = 360 / 5; // discretize circle into 5 degree intervals
        const double kAngleIncrement = 5.0 * M_PI / 180.0;
        // check a point 10cm in front of robot to see if it's "dirty"
        const double heading_vec = kInflation + 0.1;
        std::vector<ProbValue> probabilities (kAngleIntervals);
        int count = 0, count_dirty = 0;
        for (int i = 0; i < kAngleIntervals; ++i) {
            double h = kAngleIncrement * i;
            double x = x_state_ + heading_vec * cos(h);
            double y = y_state_ + heading_vec * sin(h);
            if (CheckCollision(x, y, 0.0)) { // probability is zero if not on map
                probabilities[i].prob = 0.0;
                probabilities[i].idx = i;
            } else {
                // probability is double if cell is unoccupied
                probabilities[i].prob = room_grid_->CellOccupied(x, y);
                probabilities[i].idx = i;
                if (probabilities[i].prob > 1.5)
                    ++count_dirty;
                else
                    ++count;
            }
        }
        // compute the height of a cell in the discrete distribution
        auto distro_prob = 1.0 / static_cast<double>(count + 2 * count_dirty);
        std::transform(probabilities.begin(), probabilities.end(), probabilities.begin(), [&distro_prob](ProbValue& v){return ProbValue{v.prob * distro_prob, v.idx};});
        // sort probabilities in descending order
        std::sort(probabilities.begin(), probabilities.end(), [](ProbValue& a, ProbValue& b){return a.prob > b.prob;});
        // calculate cumulative probabilities
        for(auto i = 1; i < kAngleIntervals; ++i){
            probabilities[i].prob += probabilities[i-1].prob;
        }
        // draw from distribution
        auto sample = heading_distribution_(random_engine_) / (2.0 * M_PI); // random btw 0.0 and 1.0
        for(auto i = 0; i < kAngleIntervals; ++i){
            if (sample <= probabilities[i].prob){
                h_state_ = static_cast<double>(probabilities[i].idx) * kAngleIncrement;
                break;
            }
        }

    }
}

