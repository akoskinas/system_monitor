#include <vector>
#include <string>

#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float util{0.0};
    unsigned long long idle{},iowait{};
    unsigned long long previdle{},previowait{};
    unsigned long long user{},nice{},system{},irq{},softirq{},steal{};
    unsigned long long prevuser{},prevnice{},prevsystem{},previrq{},prevsoftirq{},prevsteal{};
    if (Processor::previousState_.size()> 0 && Processor::nextState_.size()>0){
        // extract values from previous state
        prevuser = (unsigned long long)std::stoi(GetPreviousState().at(0));
        prevnice = (unsigned long long)std::stoi(GetPreviousState().at(1));
        prevsystem = (unsigned long long)std::stoi(GetPreviousState().at(2));
        previrq = (unsigned long long)std::stoi(GetPreviousState().at(5));
        prevsoftirq = (unsigned long long)std::stoi(GetPreviousState().at(6));
        prevsteal = (unsigned long long)std::stoi(GetPreviousState().at(7));
        previdle = (unsigned long long)std::stoi(GetPreviousState().at(3));
        previowait = (unsigned long long)std::stoi(GetPreviousState().at(4));

        // extract values from next state
        user = (unsigned long long)std::stoi(GetNextState().at(0));
        nice = (unsigned long long)std::stoi(GetNextState().at(1));
        system = (unsigned long long)std::stoi(GetNextState().at(2));
        irq = (unsigned long long)std::stoi(GetNextState().at(5));
        softirq = (unsigned long long)std::stoi(GetNextState().at(6));
        steal = (unsigned long long)std::stoi(GetNextState().at(7));
        idle = (unsigned long long)std::stoi(GetNextState().at(3));
        iowait = (unsigned long long)std::stoi(GetNextState().at(4));

        // implement alogorithm for utilization
        unsigned long long PrevIdle = previdle + previowait;
        unsigned long long Idle = idle + iowait;

        unsigned long long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
        unsigned long long NonIdle = user + nice + system + irq + softirq + steal;

        unsigned long long PrevTotal = PrevIdle + PrevNonIdle;
        unsigned long long Total = Idle + NonIdle;

        unsigned long long totald = Total - PrevTotal;
        unsigned long long idled = Idle - PrevIdle;
        if (totald > 0){
            util = (float)(totald - idled)/totald;
        }        
    }
    return util;
}

std::vector <std::string> Processor::GetPreviousState() const{
    return Processor::previousState_;
};
void Processor::SetPreviousState(std::vector<std::string> state){
    Processor::previousState_ = state;
}
std::vector <std::string> Processor::GetNextState() const{
    return Processor::nextState_;
};
void Processor::SetNextState(std::vector<std::string> state){
    Processor::nextState_ = state;
}
std::vector <std::string> Processor::GetTransitionState() const{
    return Processor::transitionState_;
}
void Processor::SetTransitionState(std::vector<std::string> state){
    Processor::transitionState_ = state;
}

