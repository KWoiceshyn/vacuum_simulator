#include "vacuum_simulator/vac_simulator.h"

using namespace vacuum_simulator;

int main(){
    double minx = 0.0, maxx1 = 10.0, maxx2 = 5.0;
    double miny = 0.0, maxy1 = 10.0, maxy2 = 5.0;
    auto sim = new vacuum_simulator::Simulator(minx, maxx1, maxx2, miny, maxy1, maxy2);
    //double xs = 4.0, ys = 3.0, hs = M_PI/2;
    //int time = 10000;
    double pct = 99.0;
    //sim->RunSimulationTime(xs, ys, hs, time);
    int avg_time_to_pct = 0;
    for(auto i = 0; i < 10; ++i){
        avg_time_to_pct += sim->RunSimulationPercent(pct);
    }
    avg_time_to_pct /= 10;

    delete sim;

    return 0;
}
