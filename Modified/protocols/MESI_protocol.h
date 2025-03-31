/*

#ifndef MESI_PROTOCOL_H
#define MESI_PROTOCOL_H

#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"
//#include "ModuleID.h"

extern Simulator *Sim;



class MESI_protocol : public Protocol {
public:
    // Constructor and Destructor
    MESI_protocol(Hash_table *my_table, Hash_entry *my_entry);
    ~MESI_protocol();

    // Dump state for debugging
    void dump(void);

    // Cache Request Processing
    void process_cache_request(Mreq *request);
    
    // Snoop Request Processing
    void process_snoop_request(Mreq *request);

private:
    // MESI Cache States
    enum { MESI_CACHE_I, MESI_CACHE_IS, MESI_CACHE_IM, MESI_CACHE_S, MESI_CACHE_SM, MESI_CACHE_E, MESI_CACHE_M };

    int state;          // Current state of the cache line
    ModuleID proximity;      // Proximity of the processor to the cache line (representing "closeness")
    int last_accessed_by;  // Last processor to access the cache line(int)

    // Cache line operations based on cache state
    inline void do_cache_I(Mreq *request);
    inline void do_cache_S(Mreq *request);
    inline void do_cache_E(Mreq *request);
    inline void do_cache_M(Mreq *request);
    inline void do_cache_IS(Mreq *request);
    inline void do_cache_IM(Mreq *request);
    inline void do_cache_SM(Mreq *request);

    // Snoop operations based on snoop state
    inline void do_snoop_I(Mreq *request);
    inline void do_snoop_S(Mreq *request);
    inline void do_snoop_E(Mreq *request);
    inline void do_snoop_M(Mreq *request);
    inline void do_snoop_IS(Mreq *request);
    inline void do_snoop_IM(Mreq *request);
    inline void do_snoop_SM(Mreq *request);
};

#endif // MESI_PROTOCOL_H

*/

#ifndef MESI_PROTOCOL_H
#define MESI_PROTOCOL_H

#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"
//#include "ModuleID.h"

extern Simulator *Sim;

class MESI_protocol : public Protocol {
public:
    // Constructor and Destructor
    MESI_protocol(Hash_table *my_table, Hash_entry *my_entry);
    ~MESI_protocol();

    // Dump state for debugging
    void dump(void);

    // Cache Request Processing
    void process_cache_request(Mreq *request);
    
    // Snoop Request Processing
    void process_snoop_request(Mreq *request);

    // Declare the new function
    void update_proximity_and_last_accessed(Mreq *request);

private:
    // MESI Cache States
    enum { MESI_CACHE_I, MESI_CACHE_IS, MESI_CACHE_IM, MESI_CACHE_S, MESI_CACHE_SM, MESI_CACHE_E, MESI_CACHE_M };

    int state;          // Current state of the cache line
    ModuleID proximity;      // Proximity of the processor to the cache line (representing "closeness")
    ModuleID last_accessed_by;  // Last processor to access the cache line (int)

    // Cache line operations based on cache state
    inline void do_cache_I(Mreq *request);
    inline void do_cache_S(Mreq *request);
    inline void do_cache_E(Mreq *request);
    inline void do_cache_M(Mreq *request);
    inline void do_cache_IS(Mreq *request);
    inline void do_cache_IM(Mreq *request);
    inline void do_cache_SM(Mreq *request);

    // Snoop operations based on snoop state
    inline void do_snoop_I(Mreq *request);
    inline void do_snoop_S(Mreq *request);
    inline void do_snoop_E(Mreq *request);
    inline void do_snoop_M(Mreq *request);
    inline void do_snoop_IS(Mreq *request);
    inline void do_snoop_IM(Mreq *request);
    inline void do_snoop_SM(Mreq *request);
};

#endif // MESI_PROTOCOL_H

