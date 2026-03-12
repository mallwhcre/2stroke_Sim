#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//yz125 2005
const double bore = 54.0;
const double stroke = 54.5;
const double conRod = 102.0;// connecting rod length from center to center
const double throw = stroke / 2.0; //connecting rod radius
const double compRatio = 10.7;

double get_piston_pos(double crankAngle);

int main(){

    //printf("Piston Position at TDC: %f\n", get_piston_pos(0.0));
    //printf("Piston Position at BDC: %f\n", get_piston_pos(180.0));

    int revs=5;

    double totalAngle=0.0;
    double angleStep=10.0;
    double maxAngle=360.0*100;

    while(totalAngle< maxAngle){
        double curAngle= fmod(totalAngle,360.0);
        double pistonPos=get_piston_pos(curAngle);
        printf("%.2f %.4f\n", curAngle, pistonPos);
        totalAngle+=angleStep;
    }
}

double get_piston_pos(double crankAngle){
    double h;

    double rad = crankAngle * M_PI / 180.0; // convert to radians

    h=conRod+throw*(1-cos(rad)) - sqrt( pow(conRod,2)-pow(throw*sin(rad),2) ); 
    
    return h;
}
