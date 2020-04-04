#pragma once

#include <queue>
#include <mutex>

class ClientGateway {
public:
    ClientGateway(std::queue<char*>& requests,
                  std::queue<char*>& responses);

    void operator()();
    char* pollRequests();
    void transmitResponse(const char* data, int size);

private:
    std::queue<char*>& m_requests;
    std::queue<char*>& m_responses;
    std::mutex m_mutex;
};
