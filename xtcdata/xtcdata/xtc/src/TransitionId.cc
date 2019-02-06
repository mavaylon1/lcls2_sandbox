#include "xtcdata/xtc/TransitionId.hh"

using namespace XtcData;

const char* TransitionId::name(TransitionId::Value id)
{
    static const char* _names[] = { "Unknown",         "Reset",         "Map",      "Unmap",
                                    "Configure",       "Unconfigure",   "BeginRun", "EndRun",
                                    "BeginCalibCycle", "EndCalibCycle", "Enable",   "Disable",
                                    "L1Accept",        "ConfigUpdate",  "BeginRecord", "EndRecord" };
    return (id < TransitionId::NumberOf ? _names[id] : "-Invalid-");
};
