// ==========================================
// AUTOR: Eduardo Márquez
// DESCRIPCIÓN: Tank level control simulation in C++ with a PID controller, noisy level sensor and control valve. Exports data to CSV and plots results with Python (pandas + matplotlib).
// ==========================================

#include <iostream>
#include <fstream> //To create the CSV doc
#include <iomanip> //nums format
#include "instrumentos.h" // Header for the instruments
#include "equipos.h"
#include "control.h"

using namespace std;

int main() {
    cout<<"\tStarting Simulation"<<endl;

    //1. Creating the CSV doc
    ofstream Data; //Object
    Data.open("simulation_data3.csv"); //the object creates the doc
    //Creating the columns
    Data<<"Time(s), SetPoint(m), Real_Level(m), Avg_Level(m), Valve_Openign(%), Input_Flow(L/s), Output_Flow(L/s)\n";

    //Creating the objects (Instanciacion)
    Tank Tanque(0.5, 2.0); //Tank: area of the base=1m2, Height=3m, volume=3,000 liters 
    Pump Bomba("P-100"); //Pump at the output, needed for the flow
    Bomba.TurnON(); //We need the pump ON to be able of controlling the level. 

    //Instrumets
    LevelSensor Transmiter("LIT-100", 0.05); //Transmiter: TAG and Random Noise
    ControlValve Valvula("LV-100", 50.0); //Level Controlled Valve; TAG, (Max Capacity)Cv=50.0 L/s

    //-----------PID CONTROLLER---------
    //Heuristic Tuning (kp, ki, kd)
    //dt=0.1 seg. (Sampling Time)
    PID Controller(150, 20.0, 5.0, 0.1); //kp, ki, kd, sampling time

    //Simulation Variables
    double TotalTime=300.0; //300 seg (5 minutes)
    double dt=0.1; //Step time
    double Ctime=0.0; //Starting time
    double setpoint=1.0; //Goal, we want 1.5m in the tank
    double Qin=20.0; //Input Flow (Supply) 15L/s

    //Simulation Loop
    while (Ctime<=TotalTime)
    {
        //Step 1: Adding a disturbance after 100 seg (Arpox 1.5 mins)
        if(Ctime>100.0) Qin=25.0;
        if(Ctime>200.0) setpoint=0.5;

        //Step 2: Snesor Reads
        double avglevel=Transmiter.Rlevel(Tanque);

        //Step 3: PID Controller
        double PIDcommand=Controller.Calculus(avglevel, setpoint);

        //Step 4: Control Valve
        Valvula.SetOpening(PIDcommand);
        double Qout=Valvula.GetFout(Bomba);

        //Step 5: Updating Level in the Tank
        Tanque.UpdateLevel(Qin, Qout, dt);

        //Step 6: Saving Data
        Data<<fixed<<setprecision(2)<<Ctime<<", "<<setpoint<<", "<<Tanque.getLevel()<<", "<<avglevel<<", "<<Valvula.getopening()<<", "<<Qin<<", "<<Qout<<"\n";

        //Step 7: FeedBack, we print every 10s in order to know if everything going well
        if((int)Ctime%1==0 && (Ctime-(int)Ctime)<0.01){
            cout<<"T: "<<Ctime<<"s | Nivel: "<<avglevel<<"m | Valvula: "<<Valvula.getopening()<<"%"<<endl;
        }

        Ctime+=dt;
    }

    //Close the doc
    Data.close(); //Close and save data

    cout<<"\tEnd of the Simulation"<<endl;
    cout<<"\tData saved in 'Simulation_data.csv'"<<endl;
    

    return 0;
}
