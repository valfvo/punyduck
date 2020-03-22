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
    char* data;
    if (!PyArg_ParseTuple(args, "y", &data)) {
        return NULL;
    }
    p_gateway->transmitResponse(data);

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
