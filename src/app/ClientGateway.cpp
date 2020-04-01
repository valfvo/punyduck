#include "litequarks.hpp"
#include <cstring>

ClientGateway::ClientGateway(
    std::vector<char*>& requests,
    std::vector<char*>& responses)
: m_requests(requests), m_responses(responses), m_mutex()
{ }

void ClientGateway::operator()() {
    initGateway(this);
    PyImport_AppendInittab("litequarks", &PyInit_litequarks);
    Py_Initialize();

    FILE* fp = _Py_fopen("../server/client.py", "rb");
    if (fp) {
        PyRun_SimpleFile(fp, "../server/client.py");
    }
    else {
        printf("invalid file\n");
    }

    Py_Finalize();
}

char* ClientGateway::pollRequest() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_requests.empty()) {
        char* request = m_requests[0];
        m_requests.erase(m_requests.begin());
        return request;
    }
    else {
        return nullptr;
    }
}

void ClientGateway::transmitResponse(const char* data, int size) {
    std::lock_guard<std::mutex> lock(m_mutex);
    char* response = new char[size+4];
    char* p_size = ((char*)&size);
    memcpy(response, p_size, 4);
    memcpy(&response[4], data, size);
    m_responses.push_back(response);
}
