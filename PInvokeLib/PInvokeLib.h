#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#ifdef PINVOKELIB_EXPORTS
#define PINVOKELIB_API __declspec(dllexport)
#else
#define PINVOKELIB_API __declspec(dllimport)
#endif


// Define the test structures

typedef struct _MYPOINT
{
    int x;
    int y;
} MYPOINT;

typedef struct _MYPERSON
{
    char* first;
    char* last;
} MYPERSON;

typedef struct _MYPERSON2
{
    MYPERSON* person;
    int age;
} MYPERSON2;

typedef struct _MYPERSON3
{
    MYPERSON person;
    int age;
} MYPERSON3;

typedef struct _MYARRAYSTRUCT
{
    bool flag;
    int vals[3];
} MYARRAYSTRUCT;

union MYUNION
{
    int i;
    double d;
};

union MYUNION2
{
    int i;
    char str[128];
};

typedef struct _MYSTRSTRUCT
{
    wchar_t* buffer;
    UINT size;
} MYSTRSTRUCT;

typedef struct _MYSTRSTRUCT2
{
    char* buffer;
    UINT size;
} MYSTRSTRUCT2;



// constants and pointer definitions

const int COL_DIM = 5;

typedef bool (CALLBACK* FPTR)(int i);

typedef bool (CALLBACK* FPTR2)(char* str);

// Data type codes
enum DataType
{
    DT_I2 = 1,
    DT_I4,
    DT_R4,
    DT_R8,
    DT_STR
};

// This is an exported class.
class PINVOKELIB_API CTestClass
{
public:
    CTestClass(void);
    int DoSomething(int i);

private:
    int m_member;
};

// Exports for PInvokeLib.dll

#ifdef __cplusplus
extern "C"
{
#endif

    PINVOKELIB_API CTestClass* CreateTestClass();

    PINVOKELIB_API void DeleteTestClass(CTestClass* instance);

    PINVOKELIB_API int TestArrayOfInts(int* pArray, int size);

    PINVOKELIB_API int TestRefArrayOfInts(int** ppArray, int* pSize);

    PINVOKELIB_API int TestMatrixOfInts(int pMatrix[][COL_DIM], int row);

    PINVOKELIB_API int TestArrayOfStrings(char* ppStrArray[], int size);

    PINVOKELIB_API int TestArrayOfStructs(MYPOINT* pPointArray, int size);

    PINVOKELIB_API int TestArrayOfStructs2(MYPERSON* pPersonArray, int size);

    PINVOKELIB_API int TestStructInStruct(MYPERSON2* pPerson2);

    PINVOKELIB_API void TestStructInStruct3(MYPERSON3 person3);

    PINVOKELIB_API void TestUnion(MYUNION u, int type);

    PINVOKELIB_API void TestUnion2(MYUNION2 u, int type);

    PINVOKELIB_API void TestCallBack(FPTR pf, int value);

    PINVOKELIB_API void TestCallBack2(FPTR2 pf2, char* value);

    // buffer is an in/out param
    PINVOKELIB_API void TestStringInStruct(MYSTRSTRUCT* pStruct);

    // buffer is in/out param
    PINVOKELIB_API void TestStringInStructAnsi(MYSTRSTRUCT2* pStruct);

    PINVOKELIB_API void TestOutArrayOfStructs(int* pSize, MYSTRSTRUCT2** ppStruct);

    PINVOKELIB_API char* TestStringAsResult();

    PINVOKELIB_API void SetData(DataType typ, void* object);

    PINVOKELIB_API void TestArrayInStruct(MYARRAYSTRUCT* pStruct);

#ifdef __cplusplus
}
#endif


