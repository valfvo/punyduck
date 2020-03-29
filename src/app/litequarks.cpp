#include "litequarks.hpp"

static ClientGateway* p_gateway;

static PyObject* LQ_poll_request(PyObject* self) {
    char* request = p_gateway->pollRequest();
    if (request) {
        return PyMemoryView_FromMemory(request, strlen(request), PyBUF_READ);
    }
    else {
        Py_RETURN_NONE;
    }
}

static PyObject* LQ_transmit_response(PyObject* self, PyObject* args) {
    // char* data;
    Py_buffer buffer;
    // std::cout << (void*)data << " fini !" << std::endl;
    // int size;
    // if (!PyArg_ParseTuple(args, "s", &data, &size)) {
    if (!PyArg_ParseTuple(args, "y*", &buffer)) {
        return NULL;
    }
    // else {
    //     std::cout << "transmit_response_result : " << data << ", type : " << typeid(data).name() << std::endl;
    // }
    // std::cout << "toujours la, " << (char*)buffer.buf << " fini !" << buffer.len << std::endl;
    p_gateway->transmitResponse((char*)buffer.buf, buffer.len);
    PyBuffer_Release(&buffer);
    std::cout << "encore oui" << std::endl;
    Py_RETURN_NONE;
}

static PyMethodDef LQMethods[] = {
    {"poll_request", (PyCFunction)LQ_poll_request, METH_NOARGS, "Return request"},
    {"transmit_response", LQ_transmit_response, METH_VARARGS, "Transfer Data"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef LitequarksModule = {
    PyModuleDef_HEAD_INIT,
    "litequarks",
    "Interface the Python Client on C++ Client",
    -1,
    LQMethods
};

PyObject* PyInit_litequarks(void) {
    return PyModule_Create(&LitequarksModule);
}

void initGateway(ClientGateway* gateway) {
    p_gateway = gateway;
}
