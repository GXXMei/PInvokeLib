#include "pch.h"
#include "PInvokeLib.h"
#include <strsafe.h>
#include <objbase.h>
#include <stdio.h>

//******************************************************************
// This is the constructor of a class that has been exported.
CTestClass::CTestClass()
{
    m_member = 1;
}

int CTestClass::DoSomething(int i)
{
    return i * i + m_member;
}

PINVOKELIB_API CTestClass* CreateTestClass()
{
    return new CTestClass();
}

PINVOKELIB_API void DeleteTestClass(CTestClass* instance)
{
    delete instance;
}

//******************************************************************
PINVOKELIB_API int TestArrayOfInts(int* pArray, int size)
{
    int result = 0;

    for (int i = 0; i < size; i++)
    {
        result += pArray[i];
        pArray[i] += 100;
    }
    return result;
}

//******************************************************************
PINVOKELIB_API int TestRefArrayOfInts(int** ppArray, int* pSize)
{
    int result = 0;

    //����ʹ��CoTaskMemAlloc������new������
    //��Ϊ�йܶ˵Ĵ�������Marshal.FreeCoTaskMem
    //�ͷŴ��ڴ档
    // 
    //�����ڴ�ռ�
    int* newArray = (int*)CoTaskMemAlloc(sizeof(int) * 5);

    for (int i = 0; i < *pSize; i++)
    {
        result += (*ppArray)[i];
    }

    for (int j = 0; j < 5; j++)
    {
        newArray[j] = (*ppArray)[j] + 100;
    }

    //�ͷ�
    CoTaskMemFree(*ppArray);
    *ppArray = newArray;
    *pSize = 5;

    return result;
}

//******************************************************************
PINVOKELIB_API int TestMatrixOfInts(int pMatrix[][COL_DIM], int row)
{
    int result = 0;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < COL_DIM; j++)
        {
            result += pMatrix[i][j];
            pMatrix[i][j] += 100;
        }
    }
    return result;
}

//******************************************************************
PINVOKELIB_API int TestArrayOfStrings(char* ppStrArray[], int count)
{
    int result = 0;
    STRSAFE_LPSTR temp;
    size_t len;
    const size_t alloc_size = sizeof(char) * 10;

    for (int i = 0; i < count; i++)
    {
        len = 0;
        //ȷ���ַ����Ƿ񳬹�ָ���ĳ��ȣ����ַ�Ϊ��λ��
        StringCchLengthA(ppStrArray[i], STRSAFE_MAX_CCH, &len);
        result += len;

        temp = (STRSAFE_LPSTR)CoTaskMemAlloc(alloc_size);
      //  ��һ���ַ������Ƶ���һ���ַ���(temp)
        StringCchCopyA(temp, alloc_size, (STRSAFE_LPCSTR)"123456789");

        // CoTaskMemFree must be used instead of delete to free memory.

        CoTaskMemFree(ppStrArray[i]);
        ppStrArray[i] = (char*)temp;
    }

    return result;
}

//******************************************************************
PINVOKELIB_API int TestArrayOfStructs(MYPOINT* pPointArray, int size)
{
    int result = 0;
    MYPOINT* pCur = pPointArray;

    for (int i = 0; i < size; i++)
    {
        result += pCur->x + pCur->y;
        pCur->y = 0;
        pCur++;
    }

    return result;
}

//******************************************************************
PINVOKELIB_API int TestStructInStruct(MYPERSON2* pPerson2)
{
    size_t len = 0;

    StringCchLengthA(pPerson2->person->last, STRSAFE_MAX_CCH, &len);
    len = sizeof(char) * (len + 2) + 1;

    STRSAFE_LPSTR temp = (STRSAFE_LPSTR)CoTaskMemAlloc(len);
    StringCchCopyA(temp, len, (STRSAFE_LPSTR)"Mc");

    //��һ���ַ���last���ӵ���һ���ַ���temp temp += last;
    StringCbCatA(temp, len, (STRSAFE_LPSTR)pPerson2->person->last);

    CoTaskMemFree(pPerson2->person->last);
    pPerson2->person->last = (char*)temp;

    return pPerson2->age;
}

//******************************************************************
PINVOKELIB_API int TestArrayOfStructs2(MYPERSON* pPersonArray, int size)
{
    int result = 0;
    MYPERSON* pCur = pPersonArray;
    STRSAFE_LPSTR temp;
    size_t len;

    for (int i = 0; i < size; i++)
    {
        len = 0;
        //����first�ַ����ĳ���
        StringCchLengthA(pCur->first, STRSAFE_MAX_CCH, &len);
        len++;
        result += len;
        len = 0;
        //����last�ַ����ĳ���
        StringCchLengthA(pCur->last, STRSAFE_MAX_CCH, &len);
        len++;
        result += len;

        len = sizeof(char) * (len + 2);
        //�����ڴ��
        temp = (STRSAFE_LPSTR)CoTaskMemAlloc(len);
        //temp  = "Mc"
        StringCchCopyA(temp, len, (STRSAFE_LPCSTR)"Mc");
        // temp = "Mc" + last
        StringCbCatA(temp, len, (STRSAFE_LPCSTR)pCur->last);
        result += 2;

        // CoTaskMemFree must be used instead of delete to free memory.
        //�ͷ�last���ڴ��
        CoTaskMemFree(pCur->last);
        //ָ��temp���ڴ�����
        pCur->last = (char*)temp;
        pCur++;
    }

    return result;
}

//******************************************************************
PINVOKELIB_API void TestStructInStruct3(MYPERSON3 person3)
{
    printf("\n\nperson passed by value:\n");
    printf("first = %s last = %s age = %i\n\n",
        person3.person.first,
        person3.person.last,
        person3.age);
}

//*********************************************************************
PINVOKELIB_API void TestUnion(MYUNION u, int type)
{
    if ((type != 1) && (type != 2))
    {
        return;
    }
    if (type == 1)
    {
        printf("\n\ninteger passed: %i", u.i);
    }
    else if (type == 2)
    {
        printf("\n\ndouble passed: %f", u.d);
    }
}

//******************************************************************
PINVOKELIB_API void TestUnion2(MYUNION2 u, int type)
{
    if ((type != 1) && (type != 2))
    {
        return;
    }
    if (type == 1)
    {
        printf("\n\ninteger passed: %i", u.i);
    }
    else if (type == 2)
    {
        printf("\n\nstring passed: %s", u.str);
    }
}

//******************************************************************
PINVOKELIB_API void TestCallBack(FPTR pf, int value)
{
    printf("\nReceived value: %i", value);
    printf("\nPassing to callback...");
    bool res = (*pf)(value);

    if (res)
    {
        printf("Callback returned true.\n");
    }
    else
    {
        printf("Callback returned false.\n");
    }
}

//******************************************************************
PINVOKELIB_API void TestCallBack2(FPTR2 pf2, char* value)
{
    printf("\nReceived value: %s", value);
    printf("\nPassing to callback...");
    bool res = (*pf2)(value);

    if (res)
    {
        printf("Callback2 returned true.\n");
    }
    else
    {
        printf("Callback2 returned false.\n");
    }
}

//******************************************************************
PINVOKELIB_API void TestStringInStruct(MYSTRSTRUCT* pStruct)
{
    wprintf(L"\nUnicode buffer content: %s\n", pStruct->buffer);

    // Assuming that the buffer is big enough.
    StringCbCatW(pStruct->buffer, pStruct->size, (STRSAFE_LPWSTR)L"++");
}

//******************************************************************
PINVOKELIB_API void TestStringInStructAnsi(MYSTRSTRUCT2* pStruct)
{
    printf("\nAnsi buffer content: %s\n", pStruct->buffer);

    // Assuming that the buffer is big enough.
    StringCbCatA((STRSAFE_LPSTR)pStruct->buffer, pStruct->size, (STRSAFE_LPSTR)"++");
}

//******************************************************************
PINVOKELIB_API void TestOutArrayOfStructs(int* pSize, MYSTRSTRUCT2** ppStruct)
{
    const int cArraySize = 5;
    *pSize = 0;
    *ppStruct = (MYSTRSTRUCT2*)CoTaskMemAlloc(cArraySize * sizeof(MYSTRSTRUCT2));

    if (ppStruct != NULL)
    {
        MYSTRSTRUCT2* pCurStruct = *ppStruct;
        LPSTR buffer;
        *pSize = cArraySize;

        STRSAFE_LPCSTR teststr = "***";
        size_t len = 0;
        StringCchLengthA(teststr, STRSAFE_MAX_CCH, &len);
        len++;

        for (int i = 0; i < cArraySize; i++, pCurStruct++)
        {
            pCurStruct->size = len;
            buffer = (LPSTR)CoTaskMemAlloc(len);
            StringCchCopyA(buffer, len, teststr);
            pCurStruct->buffer = (char*)buffer;
        }
    }
}

//************************************************************************
PINVOKELIB_API char* TestStringAsResult()
{

    const size_t alloc_size = 64;
    STRSAFE_LPSTR result = (STRSAFE_LPSTR)CoTaskMemAlloc(alloc_size);
    STRSAFE_LPCSTR teststr = "This is return value";
    StringCchCopyA(result, alloc_size, teststr);

    return (char*)result;
}

//************************************************************************
PINVOKELIB_API void SetData(DataType typ, void* object)
{
    switch (typ)
    {
    case DT_I2: printf("Short %i\n", *((short*)object)); break;
    case DT_I4: printf("Long %i\n", *((long*)object)); break;
    case DT_R4: printf("Float %f\n", *((float*)object)); break;
    case DT_R8: printf("Double %f\n", *((double*)object)); break;
    case DT_STR: printf("String %s\n", (char*)object); break;
    default: printf("Unknown type"); break;
    }
}

//************************************************************************
PINVOKELIB_API void TestArrayInStruct(MYARRAYSTRUCT* pStruct)
{
    pStruct->flag = true;
    pStruct->vals[0] += 100;
    pStruct->vals[1] += 100;
    pStruct->vals[2] += 100;
}
