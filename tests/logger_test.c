#include "../include/logger.h"
// using this file only for testing purposes
// Erin Dunlap

int main() {
    // Start the logger and open the log file
    init_logger("simulation.log");

    // Pretend some things are happening
    increment_sim_time(1);
    log_event("SERVER: Starting the simulation.");

    increment_sim_time(1);
    log_event("TRAIN1: Sent ACQUIRE request for IntersectionA.");

    increment_sim_time(1);
    log_event("SERVER: GRANTED IntersectionA to Train1.");

    increment_sim_time(2);
    log_event("TRAIN1: Finished traversing IntersectionA.");

    increment_sim_time(1);
    log_event("SIMULATION COMPLETE. All trains reached destination.");

    // Done logging
    close_logger();

    return 0;
}
