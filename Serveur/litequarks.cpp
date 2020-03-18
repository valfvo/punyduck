#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <vector>

class Demande {
public:
    static char* pollRequest() {
        return s_requests1.empty() ? nullptr : s_requests1[0];
    }

    static void sendData(char* data) {
        s_requests2.push_back(data);
    }

    static void addData(char* data) {
        s_requests1.push_back(data);
    }

private:
    static std::vector<char*> s_requests1;
    static std::vector<char*> s_requests2;
};

std::vector<char*> Demande::s_requests1;
std::vector<char*> Demande::s_requests2;

static PyObject* LQ_poll_request(PyObject* self) {
    char* request = Demande::pollRequest();
    if (request) {
        return PyMemoryView_FromMemory(request, strlen(request), PyBUF_READ);
    }
    else {
        Py_RETURN_NONE;
    }
}

static PyObject* LQ_send_data(PyObject* self, PyObject* args) {
    char* data;
    if(!PyArg_ParseTuple(args, "y", &data)) {
        return NULL;
    }
    std::cout << "data = " << std::string(data) << std::endl;

    Py_RETURN_NONE;
}

static PyMethodDef LQMethods[] = {
    {"poll_request", (PyCFunction)LQ_poll_request, METH_NOARGS, "Return request"},
    {"send_data", LQ_send_data, METH_VARARGS, "Transfer Data"},
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

int main(int argc, char* argv[]) {

    std::string query("test poll_request");
    char* cstr = new char[query.length() + 1];
    strcpy(cstr, query.c_str());

    Demande::addData(cstr);

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

    return EXIT_SUCCESS;
}