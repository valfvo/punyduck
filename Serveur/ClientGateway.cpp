#include "litequarks.hpp"

ClientGateway::ClientGateway(
    std::vector<char*>& requests,
    std::vector<char*>& responses)
: m_requests(requests), m_responses(responses), m_mutex()
{ }

void ClientGateway::operator()() {
    initGateway(this);
    PyImport_AppendInittab("litequarks", &PyInit_litequarks);
    Py_Initialize();

    FILE* fp = _Py_fopen("client.py", "rb");
    if (fp) {
        PyRun_SimpleFile(fp, "client.py");
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

void ClientGateway::transmitResponse(char* data) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_responses.push_back(data);
}
