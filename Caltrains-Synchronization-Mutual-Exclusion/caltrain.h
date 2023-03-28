#include <pthread.h>

struct station {

    int emptySeats;//number of emptyseats in the train
    int passenger_onWait;//no. of waiting passengers
    int passenger_onBoard;//no. of passengers on board
    pthread_mutex_t mutex;
    //condition variables
    pthread_cond_t trainArrive;//condition variable "Passenger can move to the train board".
    pthread_cond_t trainLeave;//condition variable "Train is ready to depart".
	
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);