#pragma once

#include <QString>
namespace Scenario
{
class ProcessModel;
}
namespace Pico
{
std::string defaultIncludesGraph();
std::string defaultIncludesScenario();
std::string scenarioToCPP(const Scenario::ProcessModel& s);
std::string scenarioToCPPStruct(const Scenario::ProcessModel& s);
}
