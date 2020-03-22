#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ClientGateway.hpp"
#include <iostream>

PyObject* PyInit_litequarks(void);

void initGateway(ClientGateway* gateway);
