/*

* Author: Jared Blanco, jblanco2018@my.fit.edu
* Course: CSE 1001, Section 01
* Project: maintenance.c


**program walkthrough**

    my program takes in all input from STDIN.
    I ask the user for the ammount of crew/aircraft in order to create static arrays

    I have two algorithems in my program, the 'cheap way' and 'fast way'

    The cheap way will use the cheapest repair man per aircraft possible. 

    The 'fast way' works on a first come, first serve basis. This is efficient becuase we sortted
    the aircrafts in the beggining of the program from big project to small,
    I  also sort the aircrafts by time(withing their maintenance level) for best results
    If there is an open man quallified than I will just hand him the aircraft. 
    This way of doing things will be faster than the cheap way


    i print out the schedule in my print functioin by printSchedule funtion.
    I print out the crew time busy before and after i schedule them to get a percise schedule
    printed percisly and effecently instead of creating another array just for sceduling




*/
#include <stdio.h>
#include "Scheduling.h"

int main(){
    //these are neccesary ints
    int crew_total, aircraft_total, crew_available, crew_skill_max = 0;

    //these are int's neccesary for comparison
    int cheap_cost = 0, cheap_time = 0, cheap_crew;
    int fast_cost = 0, fast_time = 0, fast_crew;


    printf("enter number of crew: ");
    scanf("%d", &crew_total);
    
    //data type CREW_t is from crew.h, it's a struct 
    CREW_t crew[crew_total];

    //this function will fill up my crew array with data
    crew_skill_max = crew_scan(crew_total, crew, crew_skill_max);



    printf("Enter number of aircraft: ");
    scanf("%d", &aircraft_total);

    //data type Aircraft_t is in crew.h, it's a struct
    AIRCRAFT_t aircraft[aircraft_total];

    //this function will fill up my aircraft array with data
    aircraft_scan(aircraft_total, aircraft, crew_skill_max);

    //checks to make sure there is only one of each aircraft
    aircraftCheck(aircraft_total, aircraft);






    printf("There are %d crew's and %d airfrafts\n", crew_total, aircraft_total);

    //sorts the aircraft from hardest to easiest, helps insure the fastest result
    sort_aircraft(aircraft_total, aircraft);

    //CEAP WAY of scheduling
    printf("\n\nLOWEST COST SCHEDULE:\n");

    printf("%-5s %10s %10s %10s\n", "JOB", "START_TIME", "END_TIME", "CREW_ID");
    int flag = 0;
    for(int i = 0; i < aircraft_total; i++){
        
        //return how many crew members have 0 work load. earliest available function uses this data
        crew_available = match_crew_to_maintanance(aircraft[i], crew_total, crew);
    
        //picks a crew member, based on skill level. if the employee is full than we just add onto 
        //the workload becuase we do not care about time, just about cost. 
        cheap_crew = earliest_available(crew_available, aircraft[i], crew_total, crew);

        //prints out the time before scuduleing and than later prints out after sceduling to simulate start and end time

        flag = printSchedule(cheap_crew, aircraft[i], crew[cheap_crew], flag);


        //scedule function keeps track of who is doing what job and for how long.returns the cost of the job
        //and adds it to the total cost of cheap_cost
        cheap_cost = cheap_cost + schedule(cheap_crew, aircraft[i], crew_total, crew, &cheap_time);

        flag = printSchedule(cheap_crew, aircraft[i], crew[cheap_crew], flag);
        


    } 
    printf("\nALL JOBS FINISHED IN %d HOURS AND COST $%d\n", cheap_time,cheap_cost);


    //need to reset all of the crew busy times
    reset_times(crew_total, crew);


    
     
    //FAST WAY of scheduling
    printf("\n\nFASTEST SCHEDULE:\n");

    printf("%-5s %10s %10s %10s\n", "JOB", "START_TIME", "END_TIME", "CREW_ID");

    for(int i = 0; i < aircraft_total; i++){
        

        //calling this function to see how many crew members are availalbe
        crew_available = match_crew_to_maintanance(aircraft[i], crew_total, crew);

        //i will give out aircafts on a "first come, first serve" basis. this works becuase my 
        //aircraft are sorted, started with bigger projects
        fast_crew = fast_earliest_available(crew_available, aircraft[i], crew_total, crew);

        flag = printSchedule(fast_crew, aircraft[i], crew[fast_crew], flag);



        fast_cost = fast_cost + schedule(fast_crew, aircraft[i], crew_total, crew, &fast_time);

        flag = printSchedule(fast_crew, aircraft[i], crew[fast_crew], flag);

    } 
    

    printf("\nALL JOBS FINISHED IN %d HOURS AND COST $%d\n", fast_time, fast_cost);









    return 0;
}