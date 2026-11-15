//Header Control
#ifndef CONTROL_H
#define CONTROL_H

#include <iostream>
#include <string>
#include <ctime> //Noise
#include "instrumentos.h" //inheritance for the new instruments here
#include "equipos.h" //Tank and Pump

using namespace std;

//Ulstrasonic Level Sensor LE/LIT-100 (Instrument)
class LevelSensor:public Instrument{
    private:
    double Cread; //Currently read
    double Fnoise; //noise factor

    public:
    //Constructor que usa el constructor de la clase Instrument.h, agrega nombre y inicializa medición.
    LevelSensor(string id, double MaxNoise) : Instrument(id, "Transmiter"), Fnoise(MaxNoise){
        Cread=0.0;
        srand(time(0)); //Random Initialization
    }

    //Reads the level in the tank and add some noise
    double Rlevel(const Tank& tanque){
        double RealLevel=tanque.getLevel();

        //Random Niose +-
        double Osilacion=((rand()%100)/50.0-1.0)*Fnoise;

        Cread=RealLevel+Osilacion;
        if(Cread<0) Cread=0;
        return Cread;
    }

    void ShowState() override{
        cout<<"["<<tag<<"] Nivel Medio: "<<Cread<<" m"<<endl;
    }
};

//Flow Valvel FV-100 (Instrument.h)
class ControlValve:public Instrument{
    private:
    double opening; //%opening
    double Cv; //Max Flow Capacity
    
    public:
    //Constructor nombre, inicializa la apertura de la válvula
    ControlValve(string id, double MaxC):Instrument(id, "Valvula de Globo"), Cv(MaxC){
        opening=0.0;
    }

    //Set Opening
    void SetOpening(double command){
        if(command>100) command=100;
        if(command<0) command=0;
        opening=command;
    }

    //Flow based on the pump
    double GetFout(const Pump& bomba){
        if(!bomba.Rstate()){
            return 0.0;
        }
        return (opening/100.0)*Cv;
    }

    void ShowState() override{
        cout<<"["<<tag<<"] Apertura: "<<opening<<"%"<<endl;
    }

    double getopening() const{return opening;}
};

class PID{
    private:
    double kp, ki, kd; //Gain
    double Cerror; //Error Acumulado
    double Berror; //Error Anterior
    double dt;

    public:
    //Contructor
    PID(double p, double i, double d, double ST): kp(p), ki(i), kd(d), dt(ST){
        Cerror=0;
        Berror=0;
    }

    double Calculus(double SetPoint, double VP){
        double error=SetPoint-VP;

        //P+I+D
        double P=kp*error;
        Cerror+=error*dt;
        double I=ki*Cerror;
        double D=kd*(error-Berror)/dt;

        Berror=error;

        return P+I+D; //Controller Output
    }
};
#endif