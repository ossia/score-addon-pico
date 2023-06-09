#pragma once

#include <QString>
namespace Scenario
{
class ProcessModel;
}
namespace Pico
{
std::string defaultIncludes();
std::string scenarioToCPP(const Scenario::ProcessModel& s);
}
