#include <pthread.h>
#include "caltrain.h"
//to initialize the station object when CalTrain boots in the file caltrain.c.
void station_init(struct station *station){
    pthread_mutex_init( &station->mutex, NULL);
    pthread_cond_init( &station->trainArrive, NULL);
    pthread_cond_init( &station->trainLeave, NULL);
    station->emptySeats=0;
    station->passenger_onWait=0;
    station->passenger_onBoard=0;
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void station_load_train(struct station *station, int count){
pthread_mutex_lock(&station->mutex);
if(station->passenger_onWait>0 && count>0 ){ //if there are available seats and waiting passengers
      station->emptySeats=count; //to continue with this count
      pthread_cond_broadcast(&station->trainArrive);
      pthread_cond_wait(&station->trainLeave,&station->mutex);
}
pthread_mutex_unlock(&station->mutex);//leave the station & unlock the mutex 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void station_wait_for_train(struct station *station){
pthread_mutex_lock(&station->mutex);
station->passenger_onWait+=1;
while(station->emptySeats==0){
    pthread_cond_wait(&station->trainArrive,&station->mutex);
}
    station->passenger_onBoard+=1;
    station->emptySeats-=1;
    station->passenger_onWait-=1;
pthread_mutex_unlock(&station->mutex);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void station_on_board(struct station * station){
pthread_mutex_lock(&station->mutex);    
station->passenger_onBoard-=1;
if((station->emptySeats==0  && station->passenger_onBoard==0)|| (station->passenger_onWait==0 && station->passenger_onBoard==0)){
        pthread_cond_signal(&station->trainLeave);
    }
pthread_mutex_unlock(&station->mutex);
}
////////////////////////////////////////////////////////////////////////////////////////////////////