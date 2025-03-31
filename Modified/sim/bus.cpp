#include "bus.h"
#include "mreq.h"

Bus::Bus()
{
    current_request = NULL;
    data_reply = NULL;
    request_in_progress = false;
    shared_line = false;
}

Bus::~Bus()
{
}

void Bus::tick()
{
    // If there is a current request, delete it to free resources
    if (current_request)
        delete current_request;

    // Process the request if there's one in progress
    if (request_in_progress)
    {
        // If there is a data reply, complete the current request
        if (data_reply)
        {
            current_request = data_reply;
            data_reply = NULL;  // Clear data_reply after processing
            request_in_progress = false;  // Mark request as no longer in progress
        }
        else
        {
            current_request = NULL;
        }
    }
    else if (!pending_requests.empty())
    {
        // If there are pending requests, start processing the next one
        shared_line = false;  // Reset shared line for this tick
        current_request = pending_requests.front();
        pending_requests.pop_front();
        request_in_progress = true;
    }
    else
    {
        current_request = NULL;  // No pending requests, reset current request
    }
}

bool Bus::bus_request(Mreq *request)
{
    if (request->msg == DATA)
    {
        // If there is already a data_reply, this means the bus is still processing
        if (data_reply != NULL) {
            // Check if the current data request should be replaced or queued
            printf("Warning: Previous data reply not processed yet. Queuing the new request.\n");
            pending_requests.push_back(request);  // Add the new request to the queue
            return false;  // Skip processing this new request for now
        }

        // If there is no active data reply, accept the new data request
        data_reply = request;
    }
    else
    {
        // For other messages (GETS, GETM, etc.), simply add them to the pending requests
        pending_requests.push_back(request);
    }

    return true;
}

Mreq* Bus::bus_snoop()
{
    Mreq *request;

    // If there is a current request, return a copy of it for snooping
    if (current_request)
    {
        request = new Mreq();
        *request = *current_request;  // Create a copy of the current request
        return request;
    }
    else
    {
        return NULL;  // No active request to snoop
    }
}
