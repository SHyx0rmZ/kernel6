#include "cpustate.hpp"

extern "C" cpustate *isr_handler(cpustate *state)
{
    return state;
}
