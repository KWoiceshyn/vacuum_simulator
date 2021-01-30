#include "vacuum_simulator/vac_simulator.h"

using namespace vacuum_simulator;

int main(int argc, char** argv){

    if(argc < 4){
        std::cerr << "Usage: " << argv[0] << "--time x_start y_start heading_start time_desired \n" <<
                  "or: --percent pct_desired num_simulations" << std::endl;
        return 1;
    }
    double minx = 0.0, maxx1 = 10.0, maxx2 = 5.0;
    double miny = 0.0, maxy1 = 10.0, maxy2 = 5.0;
    auto sim = new vacuum_simulator::Simulator(minx, maxx1, maxx2, miny, maxy1, maxy2);

    if(std::string(argv[1]) == "--time"){
        double x_s = atof(argv[2]);
        double y_s = atof(argv[3]);
        double h_s = atof(argv[4]);
        int t = atoi(argv[5]) * 10; // seconds to tenths of a second
        double pct_swept = sim->RunSimulationTime(x_s, y_s, h_s, t);
        std::cout << "Desired time: " << t << " Percent swept: " << pct_swept << std::endl;
    }
    else if(std::string(argv[1]) == "--percent") {
        double pct = atof(argv[2]);
        int n_s = atoi(argv[3]);
        int avg_time_to_pct = 0;
        for (auto i = 0; i < n_s; ++i) {
            avg_time_to_pct += sim->RunSimulationPercent(pct);
        }
        avg_time_to_pct /= n_s;
        std::cout << "Average time to " << pct << " percent across " << n_s << " trials: " << avg_time_to_pct << std::endl;
    }
    delete sim;

    return 0;
}
