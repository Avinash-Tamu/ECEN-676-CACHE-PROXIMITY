#include <assert.h>
#include <string.h>
#include "bus.h"
#include "module.h"
#include "mreq.h"
#include "sim.h"
#include "types.h"

extern Simulator *Sim;

// Equality operator definition for ModuleID
bool ModuleID::operator==(const ModuleID& other) const {
    return this->nodeID == other.nodeID && this->module_index == other.module_index;
}

// Inequality operator definition for ModuleID
bool ModuleID::operator!=(const ModuleID& other) const {
    return this->nodeID != other.nodeID || this->module_index != other.module_index;
}

// Implementation of the get_module function for ModuleID
Module* ModuleID::get_module() {
    // Ensure the module is valid by asserting it's not NULL
    Module *ret = Sim->Nd[nodeID]->mod[module_index];
    assert(ret != nullptr);  // Ensure the module is not NULL
    return ret;
}

// Module's read_input_port function: retrieves snooped requests from the bus
Mreq* Module::read_input_port(void) {
    // Call bus_snoop function of the simulator's bus object to retrieve Mreq
    return Sim->bus->bus_snoop();
}

// Module's write_output_port function: sends requests to the bus
bool Module::write_output_port(Mreq *mreq) {
    // Call bus_request function of the simulator's bus object to write Mreq to bus
    return Sim->bus->bus_request(mreq);
}

// Function to print the ModuleID information in a human-readable format
void print_id(const char *str, ModuleID mid) {
    switch (mid.module_index) {
        case NI_M:
            fprintf(stderr, "%4s:%3d/NI  ", str, mid.nodeID);
            break;
        case PR_M:
            fprintf(stderr, "%4s:%3d/PR  ", str, mid.nodeID);
            break;
        case L1_M:
            fprintf(stderr, "%4s:%3d/L1  ", str, mid.nodeID);
            break;
        case L2_M:
            fprintf(stderr, "%4s:%3d/L2  ", str, mid.nodeID);
            break;
        case L3_M:
            fprintf(stderr, "%4s:%3d/L3  ", str, mid.nodeID);
            break;
        case MC_M:
            fprintf(stderr, "%4s:%3d/MC  ", str, mid.nodeID);
            break;
        case INVALID_M:
            fprintf(stderr, "%4s:  None ", str);
            break;
        default:
            fprintf(stderr, "%4s:%3d/Unknown ", str, mid.nodeID); // Added case for unknown module index
            break;
    }
}
