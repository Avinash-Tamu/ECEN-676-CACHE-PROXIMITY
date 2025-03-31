#ifndef MREQ_H_
#define MREQ_H_

#include <assert.h>
#include <bitset>
#include <iostream>

#include "module.h"
#include "node.h"
#include "sharers.h"
#include "types.h"
#include "../protocols/messages.h"

using namespace std;

class Mreq {
public:
    // Constructor with improved initialization
    Mreq(message_t msg = MREQ_INVALID,
         paddr_t addr = (paddr_t)0x0,
         ModuleID src_id = ModuleID(-1, INVALID_M),
         ModuleID dest_id = ModuleID(-1, INVALID_M));

    // Destructor implementation (or leave empty if not needed)
    ~Mreq();

    // Member variables
    message_t msg;
    paddr_t pc;
    paddr_t addr;
    Preq* preq;
    ModuleID src_mid;
    ModuleID dest_mid;
    ModuleID fwd_mid;
    int INV_ACK_count;
    timestamp_t req_time;
    bool stalled;

    // Static message string array (ensure correct number of messages)
    static const char* message_t_str[MREQ_MESSAGE_NUM];

    // Debug functions
    void print_msg(ModuleID mid, const char* add_msg);
    void dump(void);
};

#endif /*MREQ_H_*/
