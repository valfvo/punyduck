#pragma once

#include <vector>
#include <mutex>

class ClientGateway {
public:
    ClientGateway(std::vector<char*>& requests,
                  std::vector<char*>& responses);

    void operator()();
    char* pollRequest();
    void transmitResponse(const char* data, int size);

private:
    std::vector<char*>& m_requests;
    std::vector<char*>& m_responses;
    std::mutex m_mutex;
};
