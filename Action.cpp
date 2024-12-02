#include "Action.h"
#include "Simulation.h"
#include <iostream>
#include <stdexcept>

using namespace std;
BaseAction:: BaseAction(): status(ActionStatus::COMPLETED), errorMsg("")){}


void BaseAction::complete(){
    this->status = ActionStatus::COMPLETED;
}

void BaseAction::error (string errorMsg){
    this->errorMsg = std::move(errorMsg);
    this->status = ActionStatus::ERROR;
}

ActionStatus BaseAction :: getStatus() const{
    return status;
}

const string& BaseAction::getErrorMsg () const{
    return errorMsg;
}


 AddFacility:: AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price,
  const int lifeQualityScore, const int economyScore, const int environmentScore)
  : facilityName(facilityName),facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore),
   economyScore(economyScore), environmentScore(environmentScore) {}

 AddFacility::AddFacility(const AddFacility& other)
    : facilityName(other.facilityName),
      facilityCategory(other.facilityCategory),
      price(other.price),
      lifeQualityScore(other.lifeQualityScore),
      economyScore(other.economyScore),
      environmentScore(other.environmentScore) {} 



AddFacility::AddFacility(AddFacility&& other) noexcept
    : facilityName(std::move(other.facilityName)),
      facilityCategory(other.facilityCategory),
      price(other.price),
      lifeQualityScore(other.lifeQualityScore),
      economyScore(other.economyScore),
      environmentScore(other.environmentScore) {
}


   void AddFacility:: act(Simulation &simulation) {
    try{
        simulation.addFacility(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
    complete();
    }
    catch (const std::exception& m){
        error(m.what());
    }
   }

AddFacility* AddFacility:: clone() const{
    return new AddFacility(*this);
}

const string AddFacility::toString() const {
    // Convert FacilityCategory to string
    string categoryStr;
    switch (facilityCategory) {
        case FacilityCategory::LIFE_QUALITY:
            categoryStr = "Life Quality";
            break;
        case FacilityCategory::ECONOMY:
            categoryStr = "Economy";
            break;
        case FacilityCategory::ENVIRONMENT:
            categoryStr = "Environment";
            break;
        default:
            categoryStr = "Unknown";
            break;
    }
     return "AddFacility: [Facility Name: " + facilityName +
           ", Category: " + categoryStr  +
           ", Price: " + to_string(price) +
           ", Life Quality Impact: " + to_string(lifeQualityScore) +
           ", Economy Impact: " + to_string(economyScore) +
           ", Environment Impact: " + to_string(environmentScore) +
           "]";
}




SimulateStep:: SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep:: act(Simulation &simulation){
    for(int i = 0; i< numOfSteps; i++){
        simulation.step();
    }
}
const string SimulateStep:: toString() const{
    return "Number of steps: " + to_string(numOfSteps);    
        }
SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
    
}



 AddPlan:: AddPlan(const string &settlementName, const string &selectionPolicy)
 :settlementName(settlementName),selectionPolicy(selectionPolicy){}

void AddPlan:: act(Simulation &simulation){
    simulation.addPlan(settlementName,*selectionPolicy);
}
const string AddPlan::  toString() const{
    return "Settlement Name: " + settlementName + "\nSelection Policy: " 
    + selectionPolicy;
    
}
AddPlan* AddPlan::clone() const{
    return new AddPlan(*this);
}


AddSettlement::AddSettlement (const string &settlementName,SettlementType settlementType)
:settlementName(settlementName),settlementType(settlementType){}


void AddSettlement:: act(Simulation &simulation){
    if(simulation.isSettlementExists(settlementName)){
        error(settlementName + "is already exists");
        return;
    }
    Settlement* newSettlement = new Settlement(settlementName,settlementType);

    if(simulation.addSettlement(newSettlement)){
        complete();
    } else {
        delete newSettlement;
        error("Could'nt add" + settlementName);
    }




//         AddSettlement *clone() const override;
//         const string toString() const override;
//     private:
//         const string settlementName;
//         const SettlementType settlementType;
// };
