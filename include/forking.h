//Erin Dunlap
#ifndef FORKING_H
#define FORKING_H


typedef struct {
    char name[MAX_NAME_LEN];
    char route[MAX_ROUTE_LEN][MAX_NAME_LEN];  // Each intersection name (example, "A", "B")
    int route_len;
} Train;

// Called from main to fork and simulate trains
void fork_trains(Train *train_list, int train_count);

#endif