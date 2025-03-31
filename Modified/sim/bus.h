#ifndef BUS_H_
#define BUS_H_

#include "types.h"
#include "mreq.h"  // Include the definition of the Mreq class

// Forward declaration of Mreq class if needed
class Mreq;

// Bus class definition
class Bus {
public:
    // Constructor and Destructor
    Bus();
    ~Bus();

    // Public member variables
    Mreq *current_request;        // The current request being processed on the bus
    LIST<Mreq *> pending_requests;  // List of pending requests waiting to be processed
    Mreq *data_reply;             // Holds the data reply request if it's pending

    bool request_in_progress;     // Flag indicating whether a request is currently being processed
    bool shared_line;             // Flag indicating whether the shared line is active

    // Public member functions
    void tick();                  // Main tick function that handles request processing

    bool is_shared_active() const { return shared_line; }  // Check if the shared line is active
    bool bus_request(Mreq *request);  // Function to request sending a message to the bus
    Mreq* bus_snoop();                  // Function to snoop the current bus request and return a copy
};

#endif // BUS_H_
