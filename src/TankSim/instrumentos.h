//Define the class Instruments. 
#ifndef INSTRUMENTOS_H
#define INSTRUMENTOS_H

#include <iostream>
#include <string>

using namespace std;

//Father Class for the instruments
class Instrument{
    protected:
    string tag; //instument TAG
    string description; //Instrument's full name

    public:
    //Constructor of the class.
    Instrument(string id, string desc): tag(id), description(desc){}

    //Virtual Destructor of the class; Free Memory
    virtual ~Instrument() {}

    //Show state
    virtual void ShowState()=0; //has to be defined by the inheritence (Son class)

    //get TAG
    string getTAG() const{return tag;}

};

#endif
