//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Scheduler.h"

Define_Module(Scheduler);



Scheduler::Scheduler()
{
    selfMsg = nullptr;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}


void Scheduler::initialize() {

    NrUsers = par("gateSize").intValue();
    NrOfChannels = par("NrOfChannels").intValue();
    selfMsg = new cMessage("selfMsg");

    for(int i = 0; i < NrUsers; i++){
        const char *prefix = "user";

        char numberStr[2];
        snprintf(numberStr, sizeof(numberStr), "%d", i+1);

        char result[6];

        strcpy(result, prefix);
        strcat(result, numberStr);

        cModule* userModule = getParentModule()->getSubmodule(result);
        cPar& userWeightParameter = userModule->par("weight");
        int userWeight = userWeightParameter.intValue();

        userWeights[i] = userWeight;
        EV << "User weight: " << userWeight << endl;
    }
    scheduleAt(simTime(), selfMsg);


}

void Scheduler::handleMessage(cMessage *msg)
{
   // int q[NrUsers];
 //   int NrBlocks[Nrusers];

    /*
    for(int j=0;j<NrUsers; j++){
        q[j]= getParentModule()->getSubmodule("user",j)->getSubmodule("myqq")->par("qlp");
        EV << "q["<<j<<"]= " << q[j] <<endl;
    }
     */


    int userWeights[NrUsers];

        if (msg == selfMsg) {

          //here comes the scheduling algorithm !!!
            for(int i = 0; i < NrUsers; i++) {
                NrBlocks[i] = 1; //you have to change this !!!
             }

            for(int i = 0; i < NrUsers; i++) {

        //finds out the length of each queue !!
                if(NrBlocks[i]>0){
                    cMessage *cmd = new cMessage("cmd");

                    // q[i]= q[i]-NrBlocks[i];
                    // EV << "Decrease: q["<<i<<"]= " << q[i] <<endl;
                    cmd->addPar("nrBlocks");
                    cmd->par("nrBlocks").setLongValue(NrBlocks[i]);
                    //set parameter value, e.g., nr of blocks to be sent from the queue by user i
                    //
                    send(cmd,"txScheduling",i);
                }
          //  double xtime = simTime().dbl();
            }
        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);

    }

}
