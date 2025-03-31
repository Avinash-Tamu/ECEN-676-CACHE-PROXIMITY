/*
#include "MESI_protocol.h"
#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"
#include <cassert>

extern Simulator *Sim;

// Constructor initializes state and proximity
MESI_protocol::MESI_protocol(Hash_table *my_table, Hash_entry *my_entry)
    : Protocol(my_table, my_entry), state(MESI_CACHE_I), proximity(-1), last_accessed_by(-1) {}

// Destructor
MESI_protocol::~MESI_protocol() {}

// Dump the current state of the cache line for debugging
void MESI_protocol::dump(void) {
    const char *block_states[8] = {"X", "I", "S", "E", "M", "IS", "IM", "SM"};
    fprintf(stderr, "MESI_protocol - state: %s\n", block_states[state]);
}

// Process cache requests
void MESI_protocol::process_cache_request(Mreq *request) {
    switch (state) {
        case MESI_CACHE_I:  do_cache_I(request); break;
        case MESI_CACHE_IM: do_cache_IM(request); break;
        case MESI_CACHE_M:  do_cache_M(request); break;
        case MESI_CACHE_S:  do_cache_S(request); break;
        case MESI_CACHE_IS: do_cache_IS(request); break;
        case MESI_CACHE_SM: do_cache_SM(request); break;
        case MESI_CACHE_E:  do_cache_E(request); break;
        default:
            fatal_error("Invalid Cache State for MESI Protocol\n");
    }
}

// Process snoop requests
void MESI_protocol::process_snoop_request(Mreq *request) {
    switch (state) {
        case MESI_CACHE_I:  do_snoop_I(request); break;
        case MESI_CACHE_IM: do_snoop_IM(request); break;
        case MESI_CACHE_M:  do_snoop_M(request); break;
        case MESI_CACHE_S:  do_snoop_S(request); break;
        case MESI_CACHE_IS: do_snoop_IS(request); break;
        case MESI_CACHE_SM: do_snoop_SM(request); break;
        case MESI_CACHE_E:  do_snoop_E(request); break;
        default:
            fatal_error("Invalid Cache State for MESI Protocol\n");
    }
}

// Handle the "I" state for cache request processing
inline void MESI_protocol::do_cache_I(Mreq *request) {
    switch (request->msg) {
        fprintf(stderr, "cache DATA received in I state for address: %lx, this is unexpected!\n", request->src_mid);
        case LOAD:
            if (proximity == -1 || proximity == request->src_mid) {
                send_GETS(request->addr);
                state = MESI_CACHE_IS;
            } else {
                send_GETS(request->addr);
                state = MESI_CACHE_IS;
            }
            proximity = request->src_mid; // Update proximity
            Sim->cache_misses++;
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_IM;
            proximity = request->src_mid;
            Sim->cache_misses++;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: I state shouldn't see this message\n");
    }
}

// Handle the "M" state for cache request processing
inline void MESI_protocol::do_cache_M(Mreq *request) {
    switch (request->msg) {
        fprintf(stderr, "cache DATA received in M state for address: %lx, this is unexpected!\n", request->src_mid);
        case LOAD:
        case STORE:
            send_DATA_to_proc(request->addr);
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: M state shouldn't see this message\n");
    }
}

// Handle the "S" state for cache request processing
inline void MESI_protocol::do_cache_S(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_SM;
            proximity = request->src_mid;
            Sim->cache_misses++;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: S state shouldn't see this message\n");
    }
}

// Handle the "E" state for cache request processing
inline void MESI_protocol::do_cache_E(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_DATA_to_proc(request->addr);
            state = MESI_CACHE_M;
            Sim->silent_upgrades++;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: E state shouldn't see this message\n");
    }
}

// Handle the "IM" state for cache request processing
inline void MESI_protocol::do_cache_IM(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_IM;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: IM state shouldn't see this message\n");
    }
}

// Handle the "IS" state for cache request processing
inline void MESI_protocol::do_cache_IS(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_SM;
            proximity = request->src_mid;
            Sim->cache_misses++;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: IS state shouldn't see this message\n");
    }
}

// Handle the "SM" state for cache request processing
inline void MESI_protocol::do_cache_SM(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_SM;
            proximity = request->src_mid;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: SM state shouldn't see this message\n");
    }
}

// Handle the "E" state for snoop requests
inline void MESI_protocol::do_snoop_E(Mreq *request) {
    switch (request->msg) {
        case GETS:
            set_shared_line();
            break;
        case GETM:
            send_DATA_on_bus(request->addr, request->src_mid);
            state = MESI_CACHE_I;
            break;
        case DATA:
            fprintf(stderr, "Snoop DATA received in E state for address: %lx, this is unexpected!\n", request->addr);
     
           fatal_error("Should not see data for this line! I have the line!");
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: E state shouldn't see this message\n");
    }
}

// Handle snoop requests for the "S" state
inline void MESI_protocol::do_snoop_S(Mreq *request) {
    switch (request->msg) {
        case GETS:
            if (proximity == -1 || proximity == request->src_mid) {
                set_shared_line();
            }
            break;
        case GETM:
            if (proximity != request->src_mid) {
                set_shared_line();
                state = MESI_CACHE_I;
            }
            break;
        case DATA:
        fprintf(stderr, "Snoop DATA received in S state for address: %lx, this is unexpected!\n", request->addr);
           fatal_error("Should not see data for this line! I have the line!");
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: S state shouldn't see this message\n");
    }
}

// Handle snoop requests for the "M" state
inline void MESI_protocol::do_snoop_M(Mreq *request) {
    switch (request->msg) {
        case GETS:
            if (proximity == request->src_mid) {
                send_DATA_on_bus(request->addr, request->src_mid);
                state = MESI_CACHE_S;
            }
            break;
        case GETM:
            send_DATA_on_bus(request->addr, request->src_mid);
            state = MESI_CACHE_I;
            proximity = -1;
            break;
        case DATA:
            //fprintf(stderr, "Unexpected Snoop DATA received in M state for address: %lx\n", request->addr);
            //fprintf(stderr, "State: M, Proximity: %d, Src_mid: %d, Request Type: %d\n", proximity, request->src_mid, request->msg);
            //fatal_error("Should not see data for this line! I have the line!");
             send_DATA_on_bus(request->addr, request->src_mid);
            state = MESI_CACHE_S;
            if (proximity == request->src_mid) {
                fprintf(stderr, "Unexpected Snoop DATA received in M state for address: %lx\n", request->addr);
                fatal_error("Should not see data for this line! I have the line!");
            }
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: M state shouldn't see this message\n");
    }
}

// Handle snoop requests for the "I" state
inline void MESI_protocol::do_snoop_I(Mreq *request) {
    switch (request->msg) {
        fprintf(stderr, "Snoop DATA received in I state for address: %lx, this is unexpected!\n", request->addr);
        case GETS:
        case GETM:
        case DATA:
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: I state shouldn't see this message\n");
    }
}

// Handle snoop requests for the "IS" state
inline void MESI_protocol::do_snoop_IS(Mreq *request) {
    switch (request->msg) {
        case GETS:
        case GETM:
            break;
        case DATA:
            send_DATA_to_proc(request->addr);
            if (get_shared_line()) {
                state = MESI_CACHE_S;
            } else {
                state = MESI_CACHE_E;
            }
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: IS state shouldn't see this message\n");
    }
}

// Handle snoop requests for the "IM" state
inline void MESI_protocol::do_snoop_IM(Mreq *request) {
    switch (request->msg) {
        case GETS:
        case GETM:
            break;
        case DATA:
            send_DATA_to_proc(request->addr);
            state = MESI_CACHE_M;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: IM state shouldn't see this message\n");
    }
}

// Handle snoop requests for the "SM" state
inline void MESI_protocol::do_snoop_SM(Mreq *request) {
    switch (request->msg) {
        case GETS:
            set_shared_line();
            break;
        case GETM:
            break;
        case DATA:
            send_DATA_to_proc(request->addr);
            state = MESI_CACHE_M;
            break;
        default:
            request->print_msg(my_table->moduleID, "ERROR");
            fatal_error("Client: SM state shouldn't see this message\n");
    }
}

// Implementation of the equality operator for ModuleID (comparing nodeID)
bool ModuleID::operator==(int other_id) const {
    return this->nodeID == other_id;
}

// Definition of fatal_error function (just for context)
void fatal_error(const char *message) {
    fprintf(stderr, "Fatal Error: %s\n", message);
    exit(1);
}

*/

#include "MESI_protocol.h"
#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"
#include <cassert>

extern Simulator *Sim;

// Destructor
MESI_protocol::~MESI_protocol() {}

void MESI_protocol::dump(void) {
    const char *block_states[8] = {"X","I", "S", "E", "M", "IS", "IM", "SM"};
    fprintf(stderr, "MESI_protocol - state: %s\n", block_states[state]);
}

void MESI_protocol::process_cache_request(Mreq *request) {
    switch (state) {
        case MESI_CACHE_I:  do_cache_I(request); break;
        case MESI_CACHE_IM: do_cache_IM(request); break;
        case MESI_CACHE_M:  do_cache_M(request); break;
        case MESI_CACHE_S:  do_cache_S(request); break;
        case MESI_CACHE_IS: do_cache_IS(request); break;
        case MESI_CACHE_SM: do_cache_SM(request); break;
        case MESI_CACHE_E:  do_cache_E(request); break;
        default:
            fatal_error("Invalid Cache State for MESI Protocol\n");
    }
}

void MESI_protocol::process_snoop_request(Mreq *request) {
    switch (state) {
        case MESI_CACHE_I:  do_snoop_I(request); break;
        case MESI_CACHE_IM: do_snoop_IM(request); break;
        case MESI_CACHE_M:  do_snoop_M(request); break;
        case MESI_CACHE_S:  do_snoop_S(request); break;
        case MESI_CACHE_IS: do_snoop_IS(request); break;
        case MESI_CACHE_SM: do_snoop_SM(request); break;
        case MESI_CACHE_E:  do_snoop_E(request); break;
        default:
            fatal_error("Invalid Cache State for MESI Protocol\n");
    }
}

#define INVALID_NODE_ID -1
#define INVALID_M (module_t)-1  // Ensure INVALID_M matches module_t type

// Helper function to check if ModuleID is valid
inline bool is_valid_module_id(const ModuleID& module) {
    return module.nodeID != INVALID_NODE_ID && module.module_index != INVALID_M;
}

// Helper function to update proximity and last accessed
inline void MESI_protocol::update_proximity_and_last_accessed(Mreq *request) {
    if (is_valid_module_id(request->src_mid)) {
        proximity = request->src_mid;  // Store the entire ModuleID
        last_accessed_by = request->src_mid;  // Store the entire ModuleID
    } else {
        fatal_error("Invalid ModuleID in update_proximity_and_last_accessed");
    }
}

// Constructor initializes state and proximity
MESI_protocol::MESI_protocol(Hash_table *my_table, Hash_entry *my_entry)
    : Protocol(my_table, my_entry), state(MESI_CACHE_I), proximity(INVALID_NODE_ID, INVALID_M), last_accessed_by(INVALID_NODE_ID, INVALID_M) {
    // Initialize proximity and last_accessed_by to invalid ModuleID if required
    proximity = ModuleID(INVALID_NODE_ID, INVALID_M);
    last_accessed_by = ModuleID(INVALID_NODE_ID, INVALID_M);
}

// Handle the "I" state for cache request processing
inline void MESI_protocol::do_cache_I(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_GETS(request->addr);
            state = MESI_CACHE_IS;
            update_proximity_and_last_accessed(request);
            Sim->cache_misses++;
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_IM;
            update_proximity_and_last_accessed(request);
            Sim->cache_misses++;
            break;
        default:
            fatal_error("I Invalid message");
    }
}

// Handle the "M" state for cache request processing
inline void MESI_protocol::do_cache_M(Mreq *request) {
    switch (request->msg) {
        case LOAD:
        case STORE:
            send_DATA_to_proc(request->addr);
            break;
        default:
            fatal_error("M Invalid message");
    }
}

// Handle the "S" state for cache request processing
inline void MESI_protocol::do_cache_S(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_SM;
            update_proximity_and_last_accessed(request);
            Sim->cache_misses++;
            break;
        default:
            fatal_error("S Invalid message");
    }
}

// Handle the "E" state for cache request processing
inline void MESI_protocol::do_cache_E(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_DATA_to_proc(request->addr);
            state = MESI_CACHE_M;
            Sim->silent_upgrades++;
            break;
        default:
            fatal_error("E Invalid message");
    }
}

// Handle the "IM" state for cache request processing
inline void MESI_protocol::do_cache_IM(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_IM;
            break;
        default:
            fatal_error("IM Invalid message");
    }
}

// Handle the "IS" state for cache request processing
inline void MESI_protocol::do_cache_IS(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_SM;
            update_proximity_and_last_accessed(request);
            Sim->cache_misses++;
            break;
        default:
            fatal_error("IS Invalid message");
    }
}

// Handle the "SM" state for cache request processing
inline void MESI_protocol::do_cache_SM(Mreq *request) {
    switch (request->msg) {
        case LOAD:
            send_DATA_to_proc(request->addr);
            break;
        case STORE:
            send_GETM(request->addr);
            state = MESI_CACHE_SM;
            update_proximity_and_last_accessed(request);
            break;
        default:
            fatal_error("SM Invalid message");
    }
}

// Handle snoop requests for the "E" state
inline void MESI_protocol::do_snoop_E(Mreq *request) {
    switch (request->msg) {
        case GETS:
            set_shared_line();
            break;
        case GETM:
            send_DATA_on_bus(request->addr, request->src_mid);
            state = MESI_CACHE_I;
            break;
        case DATA:
            fatal_error("Unexpected data in E state");
            break;
        default:
            fatal_error("E Invalid snoop message");
    }
}

// Handle snoop requests for the "S" state
inline void MESI_protocol::do_snoop_S(Mreq *request) {
    switch (request->msg) {
        case GETS:
            if (proximity.nodeID == INVALID_NODE_ID || proximity.nodeID == request->src_mid.nodeID) {
                set_shared_line();
            }
            break;
        case GETM:
            if (proximity.nodeID != request->src_mid.nodeID) {
                set_shared_line();
                state = MESI_CACHE_I;
            }
            break;
        case DATA:
            fatal_error("Unexpected data in S state");
            break;
        default:
            fatal_error("S Invalid snoop message");
    }
}

// Handle snoop requests for the "M" state
inline void MESI_protocol::do_snoop_M(Mreq *request) {
    switch (request->msg) {
        case GETS:
            if (proximity.nodeID == request->src_mid.nodeID) {
                send_DATA_on_bus(request->addr, request->src_mid);
                state = MESI_CACHE_S;
            }
            break;
        case GETM:
            send_DATA_on_bus(request->addr, request->src_mid);
            state = MESI_CACHE_I;
            proximity = ModuleID(INVALID_NODE_ID, INVALID_M);
            break;
        case DATA:
            send_DATA_on_bus(request->addr, request->src_mid);
            proximity = ModuleID(INVALID_NODE_ID, INVALID_M);
            state = MESI_CACHE_I;
            break;
        default:
            fatal_error("M Invalid snoop message");
    }
}

// Handle snoop requests for the "I" state
inline void MESI_protocol::do_snoop_I(Mreq *request) {
    switch (request->msg) {
        case GETS:
        case GETM:
        case DATA:
            break;
        default:
            fatal_error("I Invalid snoop message");
    }
}

// Handle snoop requests for the "IS" state
inline void MESI_protocol::do_snoop_IS(Mreq *request) {
    switch (request->msg) {
        case GETS:
        case GETM:
            break;
        case DATA:
            send_DATA_to_proc(request->addr);
            state = get_shared_line() ? MESI_CACHE_S : MESI_CACHE_E;
            break;
        default:
            fatal_error("IS Invalid snoop message");
    }
}

// Handle snoop requests for the "IM" state
inline void MESI_protocol::do_snoop_IM(Mreq *request) {
    switch (request->msg) {
        case GETS:
        case GETM:
            break;
        case DATA:
            send_DATA_to_proc(request->addr);
            state = MESI_CACHE_M;
            break;
        default:
            fatal_error("IM Invalid snoop message");
    }
}

// Handle snoop requests for the "SM" state
inline void MESI_protocol::do_snoop_SM(Mreq *request) {
    switch (request->msg) {
        case GETS:
            set_shared_line();
            break;
        case GETM:
            break;
        case DATA:
            send_DATA_to_proc(request->addr);
            state = MESI_CACHE_M;
            break;
        default:
            fatal_error("SM Invalid snoop message");
    }
}
