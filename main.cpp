//
//  main.cpp
//  MakeCS164GreatAgain
//  Matt Doherty, Nicholas Fong, Julia Marcondes, Christina Matera
//

#include <iostream>
#include <vector>       //For dynamically sized arrays
#include <time.h>       //For randomize the random seed initialization
#include <math.h>       //For log

using namespace std;

double generateU(){
    return ((double) rand() / (double)(RAND_MAX));
    //returns random number from 0 to 1 so can be used for inverse transform method
}
double generateWeibell(double alpha, double beta){
    return beta * exp(((1/alpha)*log(-log(generateU()))));
    //returns value of inverse transform Weibull distribution
}
int indexOfSmallestElement(std::vector<double> array, int size){       //Find the index of the smallest element in an array
    if(size <= 1){      //Catch cases where array size is too small for array[1] to work
        return 0;
    }
    int index = 0;
    for(int i = 1; i < size; i++){ //loop runs through all elements in array
        if(array[i] < array[index]) //checks if current element is less than min
            index = i; //if so, replace index
    }
    return index;
}
int indexOfElement(std::vector<double> array, int size, double element){    //find the first occurence of element in the vector and return its index
    for(int k = 0; k < size; k++){
        if(array[k] == element){
            return k;
        }
    }
    return -1;      //-1 means it isn't in the vector
}

double generateProduction(double time, double workingSlow, double workingFast){     //Tells us how much was produced
    return time * (18*workingSlow + 30*workingFast);
}

int main(){
    srand(time(NULL)); //Randomize seed initialization
    //arrays to hold overall results for each configuration of machines to compare efficiency
    double totalCosts[8];
    double totalRevenue[8];
    double machineDownTime[8];
    //constants
    double slowMachineBreakTimeAlpha = 3.1;
    double slowMachineBreakTimeBeta = 50.8;
    double fastMachineBreakTimeAlpha = 3.64;
    double fastMachineBreakTimeBeta = 41;
    double slowMachineFixTimeAlpha = 5.5;
    double slowMachineFixTimeBeta = 1.5;
    double fastMachineFixTimeAlpha = 3.1;
    double fastMachineFixTimeBeta = 4.5;
    double slowMachineFixCosts = 2;
    double fastMachineFixCosts = 7;
    double endTime = 100;
    double infinity = 9999999;
    
    for(int i = 0; i <= 7; i++){    //go through each possible combination of types of machines where i is the number of fast machines. Beginning with 0 fast machines and 7 slow machines
        //counter variables
        double time = 0;
        double production = 0;
        double downTime = 0;
        double costs = 0;
        //state variables
        double deltaT = 0;
        double slowRepairQueueLength = 0;
        double fastRepairQueueLength = 0;
        double nextSlowRepairTime = infinity; //Initiated to infinity since there exist no repair times to begin with
        double nextFastRepairTime = infinity;
        double nextFastMachineBreakTime = infinity;      //To know when the next machine will break
        double nextFastMachineBreakNumber = 0;          //To locate the machine that will break next
        double nextSlowMachineBreakTime = infinity;
        double nextSlowMachineBreakNumber = 0;
        std::vector<double> slowMachineBreakdownTimes;  //List of times of when each slow machine will break down
        std::vector<double> fastMachineBreakdownTimes;  //List of times of when each fast machine will break down
        int numWorkingFastMachines = i;
        int numWorkingSlowMachines = (7-i);
        int index;
        for(int j = 0; j < (7-i); j++){     //Initialize when each slow machine will break down
            slowMachineBreakdownTimes.push_back(generateWeibell(slowMachineBreakTimeAlpha, slowMachineBreakTimeBeta));
            if(slowMachineBreakdownTimes[j] < nextSlowMachineBreakTime){     //Finds the soonest event
                nextSlowMachineBreakTime = slowMachineBreakdownTimes[j];
                nextSlowMachineBreakNumber = j;
            }
        }
        for(int l = 0; l < i; l++){     //Initialize when each fast machine will break down
            fastMachineBreakdownTimes.push_back(generateWeibell(fastMachineBreakTimeAlpha, fastMachineBreakTimeBeta));
            if(fastMachineBreakdownTimes[l] < nextFastMachineBreakTime){    //Finds the soonest event
                nextFastMachineBreakTime = fastMachineBreakdownTimes[l];
                nextFastMachineBreakNumber = l;
            }
        }
        if(slowMachineBreakdownTimes.size() <= 0){
            slowMachineBreakdownTimes.push_back(infinity); //Just in case no slow machines break down
        }
        if(fastMachineBreakdownTimes.size() <= 0){
            fastMachineBreakdownTimes.push_back(infinity); //Just in case no fast machines break down
        }
        
        
        while(time < endTime){      //Run the simulation after everything has been initialized
            //A fast machine breaking is the first thing on the agenda
            if(nextFastMachineBreakTime <= nextSlowMachineBreakTime && nextFastMachineBreakTime <= nextFastRepairTime && nextFastMachineBreakTime <= nextSlowRepairTime){
                deltaT = (nextFastMachineBreakTime - time);
                production += generateProduction(deltaT, numWorkingSlowMachines, numWorkingFastMachines);  //update counter variables
                downTime += deltaT * (slowRepairQueueLength + fastRepairQueueLength);
                time = nextFastMachineBreakTime;        //progress time
                fastRepairQueueLength++;        //adjust appropriate state variables
                numWorkingFastMachines--;
                fastMachineBreakdownTimes[nextFastMachineBreakNumber] = infinity;        //Machine is broken, it can't break again
                nextFastMachineBreakNumber = indexOfSmallestElement(fastMachineBreakdownTimes,fastMachineBreakdownTimes.size());    //Reset next fast machine break index
                nextFastMachineBreakTime = fastMachineBreakdownTimes[nextFastMachineBreakNumber];       //Reset next fast machine break time
                if(fastRepairQueueLength == 1){ //If theres only one repair that needs to be done, generate its break time
                    nextFastRepairTime = time + generateWeibell(fastMachineFixTimeAlpha, fastMachineFixTimeBeta);
                }
            }
            
            //A slow machine breaks
            else if(nextSlowMachineBreakTime < nextFastMachineBreakTime && nextSlowMachineBreakTime < nextFastRepairTime && nextSlowMachineBreakTime < nextSlowRepairTime){
                deltaT = (nextSlowMachineBreakTime - time);
                production += generateProduction(deltaT, numWorkingSlowMachines, numWorkingFastMachines);  //update counter variables
                downTime += deltaT * (slowRepairQueueLength + fastRepairQueueLength);
                time = nextSlowMachineBreakTime;        //progress time
                slowRepairQueueLength++;                //adjust appropriate state variables
                numWorkingSlowMachines--;
                slowMachineBreakdownTimes[nextSlowMachineBreakNumber] = infinity;        //Machine is broken, it can't break again
                nextSlowMachineBreakNumber = indexOfSmallestElement(slowMachineBreakdownTimes, slowMachineBreakdownTimes.size());
                nextSlowMachineBreakTime = slowMachineBreakdownTimes[nextSlowMachineBreakNumber];
                if(slowRepairQueueLength == 1){
                    nextSlowRepairTime = time + generateWeibell(slowMachineFixTimeAlpha, slowMachineFixTimeBeta);
                }
            }
            
            //A fast machine is fixed
            else if(nextFastRepairTime < nextFastMachineBreakTime && nextFastRepairTime < nextSlowMachineBreakTime && nextFastRepairTime < nextSlowRepairTime){
                deltaT = (nextFastRepairTime - time);
                production += generateProduction(deltaT, numWorkingSlowMachines, numWorkingFastMachines);  //update counter variables
                downTime += deltaT * (slowRepairQueueLength + fastRepairQueueLength);
                costs += deltaT * fastMachineFixCosts;
                time = nextFastRepairTime;        //progress time
                fastRepairQueueLength--;          //adjust appropriate state variables
                numWorkingFastMachines++;
                index = indexOfElement(fastMachineBreakdownTimes, fastMachineBreakdownTimes.size(), infinity);  //find a broken machine and let it be fixed
                fastMachineBreakdownTimes[index] = time + generateWeibell(fastMachineBreakTimeAlpha, fastMachineBreakTimeBeta);
                nextFastMachineBreakNumber = indexOfSmallestElement(fastMachineBreakdownTimes,fastMachineBreakdownTimes.size());    //Reset next fast machine break index
                nextFastMachineBreakTime = fastMachineBreakdownTimes[nextFastMachineBreakNumber];       //Reset next fast machine break time
                if(fastRepairQueueLength >= 1){
                    nextFastRepairTime = time + generateWeibell(fastMachineFixTimeAlpha, fastMachineFixTimeBeta);
                    //generate next repair time and add it to the current time
                }
                else{
                    nextFastRepairTime = infinity; //nothing is left to repair
                }
            }
            
            //A slow machine is fixed
            else if(nextSlowRepairTime < nextFastMachineBreakTime && nextSlowRepairTime < nextSlowMachineBreakTime && nextSlowRepairTime < nextFastRepairTime){
                deltaT = (nextSlowRepairTime - time);
                production += generateProduction(deltaT, numWorkingSlowMachines, numWorkingFastMachines);  //update counter variables
                downTime += deltaT * (slowRepairQueueLength + fastRepairQueueLength);
                costs += deltaT * slowMachineFixCosts;
                time = nextSlowRepairTime;        //progress time
                slowRepairQueueLength--;
                numWorkingSlowMachines++;
                index = indexOfElement(slowMachineBreakdownTimes, slowMachineBreakdownTimes.size(), infinity);  //find a broken machine and let it be fixed
                slowMachineBreakdownTimes[index] = time + generateWeibell(slowMachineBreakTimeAlpha, slowMachineBreakTimeBeta);
                nextSlowMachineBreakNumber = indexOfSmallestElement(slowMachineBreakdownTimes,slowMachineBreakdownTimes.size());    //Reset next slow machine break index
                nextSlowMachineBreakTime = slowMachineBreakdownTimes[nextSlowMachineBreakNumber];       //Reset next slow machine break time
                if(slowRepairQueueLength >= 1){
                    nextSlowRepairTime = time + generateWeibell(slowMachineFixTimeAlpha, slowMachineFixTimeBeta);
                }
                else{
                    nextSlowRepairTime = infinity;
                }
            }
            else{
                cout<<"Error: None of the cases occured\n -Fast Break = "<<nextFastMachineBreakTime<<"\n -Slow Break = "
                <<nextSlowMachineBreakTime<<"\n -Fast Fix = "<<nextFastRepairTime<<"\n -Slow Fix = "<<nextSlowRepairTime<<"\n";
            }
            
        }
        totalCosts[i] = costs;
        totalRevenue[i] = production;
        machineDownTime[i] = downTime;
    }
    
    //Prints out every combination of slow and fast machines
    for(int i = 0; i <= 7; i++){ //i represents the number of fast machines
        cout<<"With "<<i<<" fast machines and "<<(7-i)<<" slow machines, over a time length of "<<endTime<<", we had\n"
        <<" -Total Costs = "<<totalCosts[i]<<"\n"
        <<" -TotalRevenue = "<<totalRevenue[i]<<"\n"
        <<" -Net Profit = "<<totalRevenue[i]-totalCosts[i]<<"\n"
        <<" -Machine Down Time = "<<machineDownTime[i]<<"\n\n";
    }
    return 0;
}

