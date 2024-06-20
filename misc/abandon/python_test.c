#include <stdio.h>
#include <python3.10/Python.h>
#include <time.h>
#define DISTANCE_FUNC "distance"
#define EUCLIDEAN "Euclidean_distance"
#define COSINE "Cosine_similarity"

long Euclidean_distance(long a, long b)
{
    long Result;
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyObject *pName = PyUnicode_DecodeFSDefault(DISTANCE_FUNC);
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    if (pModule != NULL)
    {
        PyObject *pFuncEuclidean = PyObject_GetAttrString(pModule, EUCLIDEAN);
        PyObject *pFuncCosine = PyObject_GetAttrString(pModule, COSINE);
        if (pFuncEuclidean && PyCallable_Check(pFuncEuclidean))
        {
            PyObject *pArgs = PyTuple_Pack(2, PyLong_FromLong(a), PyLong_FromLong(b));
            PyObject *pValue = PyObject_CallObject(pFuncEuclidean, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL)
            {
                Result = PyLong_AsLong(pValue);
                Py_DECREF(pValue);
            }
            else
            {
                Py_DECREF(pFuncEuclidean);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr, "Call failed\n");
            }
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", EUCLIDEAN);
        }
    }
    Py_Finalize();
    return Result;
}

int main()
{
    long para1 = 1;
    long para2 = 2;
    long Result;
    while (0)
    {
        Result = Euclidean_distance(para1, para2);
        printf("%ld\n", Result);
    }
    Result = Euclidean_distance(para1, para2);
    printf("%ld\n", Result);
    Result = Euclidean_distance(para1, para2);
    printf("%ld\n", Result);

    return 0;
}
