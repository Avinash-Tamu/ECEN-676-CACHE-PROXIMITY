#ifndef MODULE_H_
#define MODULE_H_

#include <assert.h>
#include <iostream>
#include <map>
#include "settings.h"
#include "types.h"

using namespace std;

using IO_PORT = std::multimap<timestamp_t, Mreq *>;

typedef enum {
    NI_M = 0,
    PR_M,
    L1_M,
    L2_M,
    L3_M,
    MC_M,
    INVALID_M
} module_t;

class ModuleID {
public:
    int nodeID;             // The identifier for the module node
    module_t module_index;  // The module index, which identifies the module type

    // Constructor
    ModuleID(int nodeID = -1, module_t module_index = INVALID_M)
        : nodeID(nodeID), module_index(module_index) {}

    // Equality operator to compare ModuleID with an int (based on the 'nodeID')
    bool operator==(int other_id) const;

    // Equality operator to compare two ModuleID objects
    bool operator==(const ModuleID& other) const;

    // Inequality operator to compare ModuleID with an int
    bool operator!=(int other_id) const;

    // Inequality operator to compare two ModuleID objects
    bool operator!=(const ModuleID& other) const;

    // Get the module associated with this ModuleID (Placeholder implementation)
    Module* get_module();
};

class Bus;

class Module {
public:
    const char *name;  // Name should be const, no need for const_cast
    ModuleID moduleID;  // The ModuleID associated with this module

    // Constructor
    Module(ModuleID moduleID, const char *name)
        : moduleID(moduleID), name(name) {}

    // Virtual destructor
    virtual ~Module() {}

    // Read input port function
    Mreq* read_input_port(void);

    // Write output port function
    bool write_output_port(Mreq *mreq);

    // Virtual methods that need to be implemented by derived classes
    virtual void tick(void) = 0;
    virtual void tock(void) = 0;
};

// Function to print ModuleID
void print_id(const char *str, ModuleID mid);

#endif // MODULE_H_
