#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ClientGateway.hpp"

PyObject* PyInit_gateway(void);

void initGateway(ClientGateway* gateway);
