#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();
  std::vector <std::string> GetPreviousState() const;
  void SetPreviousState(std::vector<std::string> state);
  std::vector <std::string> GetNextState() const;
  void SetNextState(std::vector<std::string> state);
  std::vector <std::string> GetTransitionState() const;
  void SetTransitionState(std::vector<std::string> state);

 private:
    std::vector<std::string> previousState_{};
    std::vector<std::string> nextState_{};
    std::vector<std::string> transitionState_{};
};

#endif