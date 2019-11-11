#include <stdlib.h>

//holds crew data
typedef struct{
    int number;
    int skill;
    int time_busy; 
    double cost;
}CREW_t; 

//holds aircraft data
typedef struct{
    int ID;
    int maintenance;
    int hours;
} AIRCRAFT_t; 


//scans in all of the crew data
int crew_scan(int crew_total, CREW_t crew[crew_total], int crew_skill_max){
    for(int i = 0; i < crew_total; i++){
        crew[i].number = i;
        printf("Enter crew %d skill level: ", i);
        scanf("%d", &crew[i].skill);
        crew[i].time_busy = 0;
        printf("Enter crew %d cost per hour: ", i);
        scanf("%lf", &crew[i].cost);
        if(crew[i].skill > crew_skill_max){
            crew_skill_max = crew[i].skill; 
        }
    }
    return crew_skill_max;
}

//scans in all of the aircraft data
void aircraft_scan(int aircraft_total, AIRCRAFT_t aircraft[aircraft_total], int  crew_skill_max){
    for(int i = 0; i < aircraft_total; i++){
        printf("Enter aircraft %d ID: ", i);
        scanf("%d", &aircraft[i].ID);
        printf("Enter aircraft %d mainenance level: ", i);
        scanf("%d", &aircraft[i].maintenance);
        printf("Enter aircraft %d hours: ", i);
        scanf("%d", &aircraft[i].hours);
        if(aircraft[i].maintenance > crew_skill_max){
            printf("**ERROR: NO CREW EXPERIENCED ENOUGH FOR MAINTENANCE OF AIRCRAFT %d", aircraft[i].ID );
            exit(0);
        }
    }
}



/* 
Scans in ONE part of the aircraft array, and compares that part with the
entire crew array to see how many employees are available.*/
int match_crew_to_maintanance(AIRCRAFT_t aircraft, int crew_total, CREW_t crew[crew_total]){
    int crew_available = 0;

    //this loop check for minimum crew skill level
    for(int i = 0; i < crew_total; i++){
        if((crew[i].skill == aircraft.maintenance) && (crew[i].time_busy == 0)){
            crew_available++;
        }
    }
    

    //printf("\n%d crew for %d\n", crew_available, aircraft.ID);
    return crew_available;
}

int earliest_available(int crew_available, AIRCRAFT_t aircraft, int crew_total, CREW_t crew[crew_total]){
    int crew_choice = -1; 
    //also compare the cost of multiple crews of the same quallifications just incase one is cheaper than others
    double min_cost = 10000.00;


        /*Use this int to compare the times of the crew members.
        The number set is high becuase all crew will be available sooner than that */
        int time_available = 100;
        
        for(int ii = 0; ii < crew_total; ii++){
            if((crew[ii].skill == aircraft.maintenance) && (crew[ii].time_busy < time_available) && (crew[ii].cost < min_cost)){
                    crew_choice = ii;
                    min_cost = crew[crew_choice].cost;
                    time_available = crew[ii].time_busy;
                    
                
            }
        }
    
    return crew_choice;
}




//this will just apply say the job is getting done by group number i, and
//make group i's time busy go up
int schedule(int crew_choice, AIRCRAFT_t aircraft, int crew_total, CREW_t crew[crew_total], int *Ttime){
    //will return the total cost of the fix
    int cost = 0;

    crew[crew_choice].time_busy = crew[crew_choice].time_busy + aircraft.hours;
   
    //will set the total time = crew with the longest workload.
    if(crew[crew_choice].time_busy > *Ttime){
        *Ttime = crew[crew_choice].time_busy; 
    }

    cost = (crew[crew_choice].cost * aircraft.hours);


    return cost;
}

//this will reset all of the times the crew are busy so we can compare everything

void reset_times(int crew_total, CREW_t crew[crew_total]){
    for(int i = 0; i < crew_total; i++){
        crew[i].time_busy = 0;
    }
}



int fast_earliest_available(int crew_available, AIRCRAFT_t aircraft, int crew_total, CREW_t crew[crew_total]){
    int crew_choice = -1; 

    //if there are multable crews that are free we will just pick one
    if(crew_available > 0){
       for(int i = 0; i < crew_total; i++){
            if((crew[i].skill == aircraft.maintenance) && (crew[i].time_busy == 0)){
                crew_choice = i;
                return crew_choice;
            }
        }   
    }


    //this is the first come first serve part of the program
    //i check who is the least busy by comparing all of there busy times. than i chose that guy for the job
    int least_busy = 100;
    if(crew_available == 0){
        least_busy = 100;
       //this will find the least busy crew members. 
        for(int i = 0; i < crew_total; i++){
            if((crew[i].time_busy < least_busy) && (crew[i].skill >= aircraft.maintenance)){
                least_busy = crew[i].time_busy;
            }
        }

        //this allocates the choice to the first crew memeber who is free
        for(int r = 0; r < crew_total; r++){
            if((crew[r].time_busy == least_busy) && (crew[r].skill >= aircraft.maintenance)){
                crew_choice = r;
                return crew_choice;
            }
        }
    }

    
    //if we return -1 that means error;
    
    if(crew_choice == -1){
        printf("\n\n**ERROR IN FAST_EARLIEST_AVAILABLE**\n\n");
        return crew_choice;
    }
    
    
}

//sorts the list from biggest projects to smallest projects to ensure fastest results
void sort_aircraft(int aircraft_total, AIRCRAFT_t aircraft[aircraft_total]){
    AIRCRAFT_t temp; 
    //this sorts the aircraft by maintenance level
    for(int i = 0; i < aircraft_total; i++){
      for(int z = (i + 1); z < aircraft_total; z++){
          if(aircraft[i].maintenance < aircraft[z].maintenance){
              temp = aircraft[i];
              aircraft[i] = aircraft[z];
              aircraft[z] = temp;
          }
         
      }
    }

    //now that it is ordrered, i need to also put the biggest projects first
    for(int i = 0; i < aircraft_total; i++){
      for(int z = (i + 1); z < aircraft_total; z++){
          if((aircraft[i].hours < aircraft[z].hours) && (aircraft[i].maintenance == aircraft[z].maintenance)){
              temp = aircraft[i];
              aircraft[i] = aircraft[z];
              aircraft[z] = temp;
          }
      }
    }
}


int printSchedule(int cheap_crew, AIRCRAFT_t aircraft, CREW_t crew, int z){
    if(!z){
        printf("%-5d %10d", aircraft.ID, crew.time_busy);
        return 1;
    }else{
        printf(" %10d %10d\n", crew.time_busy, cheap_crew);
        return 0;
    }
    
}

void aircraftCheck(int aircraft_total, AIRCRAFT_t aircraft[aircraft_total]){
    for(int i = 0; i < aircraft_total; i++){
        for(int g = i + 1; g < aircraft_total; g++){
            if(aircraft[i].ID == aircraft[g].ID){
                printf("\n\n\n**ERROR, IDENTICAL AIRCRAFT ID\n\n");
                exit(0);
            }
        }
    }
}
