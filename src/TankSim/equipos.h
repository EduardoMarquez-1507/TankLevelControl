//Header for the Equipment
#ifndef EQUIPOS_H
#define EQUIPOS_H

#include <iostream>
#include <string>
#include <cmath> //To get the Liters of the tank

using namespace std;

//Pump class
class Pump{
    private:
    string tag;
    bool ONstate; // true=ON, false=OFF (state)

    public:
    Pump(string id) : tag(id), ONstate(false){}
    void TurnON(){ONstate=true;} //turns on the pump
    void TurnOFF(){ONstate=false;} //Turns off the pump
    bool Rstate() const{return ONstate;}
    string getTAG() const{return tag;}

};

class Tank{
    private:
    double areabase; //base in m2
    double height; //height of the tank
    double Clevel; //Currently level on the tank
    const double LtoM3=0.001; //Factor

    public:
    //Constructor
    Tank(double area, double altura) : areabase(area), height(altura){
        Clevel=0.0; //Empty
    }

    //Process: dh/dt = (Qin-Qout)/Area
    void UpdateLevel(double Finlps, double Foutlps, double dt){
        //1. From Lt/seg to M^3
        double Vin=Finlps*LtoM3*dt;
        double Vout=Foutlps*LtoM3*dt;

        //Substraction to find real volume
        double chvol=Vin-Vout;
        double chlevel=chvol/areabase;

        //Update Level
        Clevel+=chlevel;

        //Physical Limitations
        if(Clevel>height) Clevel=height; //Top or Full limit
        if(Clevel<0) Clevel=0; //Empty
    }

    //Getters
    double getLevel() const{return Clevel;}
    double getPLevel() const{
        return (Clevel/height)*100;
    }
};

#endif