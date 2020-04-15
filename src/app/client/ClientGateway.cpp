#include "gateway.hpp"
#include <cstring>

ClientGateway::ClientGateway(
    std::queue<char*>& requests,
    std::queue<char*>& responses)
: m_requests(requests), m_responses(responses), m_mutex()
{ }

void ClientGateway::operator()() {
    initGateway(this);
    PyImport_AppendInittab("gateway", &PyInit_gateway);
    Py_Initialize();

    FILE* fp = _Py_fopen("client/client.py", "rb");
    if (fp) {
        PyRun_SimpleFileExFlags(fp, "client/client.py", 1, NULL);
    }
    else {
        printf("PnClientError: Can not start client\n");
    }

    Py_Finalize();
}

char* ClientGateway::pollRequests() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_requests.empty()) {
        char* request = m_requests.front();
        m_requests.pop();
        return request;
    }
    else {
        return nullptr;
    }
}

void ClientGateway::transmitResponse(const char* data, int size) {
    std::lock_guard<std::mutex> lock(m_mutex);
    char* response = new char[size+4];
    memcpy(response, &size, 4);
    memcpy(&response[4], data, size);
    m_responses.push(response);
}
