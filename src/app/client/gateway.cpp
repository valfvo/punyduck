#include "gateway.hpp"

static ClientGateway* p_gateway;

static PyObject*
gateway_poll_requests(PyObject* self) {
    char* request = p_gateway->pollRequests();
    if (request) {
        return PyMemoryView_FromMemory(request, strlen(request), PyBUF_READ);
    }
    else {
        Py_RETURN_NONE;
    }
}

static PyObject*
gateway_transmit_response(PyObject* self, PyObject* args) {
    Py_buffer buffer;
    if (!PyArg_ParseTuple(args, "y*", &buffer)) {
        return NULL;
    }
    p_gateway->transmitResponse((char*)buffer.buf, buffer.len);
    PyBuffer_Release(&buffer);
    Py_RETURN_NONE;
}

static PyMethodDef GatewayMethods[] = {
    {"poll_request", (PyCFunction)gateway_poll_requests, METH_NOARGS,
     "Return request"},
    {"transmit_response", gateway_transmit_response, METH_VARARGS,
     "Transfer Data"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef GatewayModule = {
    PyModuleDef_HEAD_INIT,
    "gateway",
    "Interface the Python Client on C++ Client",
    -1,
    GatewayMethods
};

PyObject* PyInit_gateway(void) {
    return PyModule_Create(&GatewayModule);
}

void initGateway(ClientGateway* gateway) {
    p_gateway = gateway;
}
