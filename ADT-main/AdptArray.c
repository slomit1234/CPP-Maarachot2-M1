/*
Based on:https://moodlearn.ariel.ac.il/mod/resource/view.php?id=1723497
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "AdptArray.h"

#define INITIAL_SIZE 4

typedef struct AdptArray_
{
	int ArrSize;
	PElement* pElemArr;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
	PRINT_FUNC printFunc;
	
}AdptArray;




PAdptArray CreateAdptArray(COPY_FUNC copyFunc_, DEL_FUNC delFunc_, PRINT_FUNC printFunc_)
{
	PAdptArray pArr = (PAdptArray)malloc(sizeof(AdptArray));
	if (pArr == NULL)
		return NULL;

	pArr->ArrSize = 0;
	pArr->pElemArr = NULL;
	pArr->delFunc = delFunc_;
	pArr->copyFunc = copyFunc_;
	pArr->printFunc = printFunc_;
	return pArr;
}

Result SetAdptArrayAt(PAdptArray pArr, int idx, PElement pNewElem)
{
	int negIdx = 0;
	int flag = 0;
	PElement* newpElemArr;
	if (pArr == NULL)
		return FAIL;

	if (idx >= pArr->ArrSize)
	{
		// Extend Array
		if ((newpElemArr = (PElement*)calloc((idx + 1), sizeof(PElement))) == NULL)
			return FAIL;
		memcpy(newpElemArr, pArr->pElemArr, (pArr->ArrSize) * sizeof(PElement));
		free(pArr->pElemArr);
		pArr->pElemArr = newpElemArr;
	}
	else if (idx < 0)
	{
		flag = 1;
		negIdx = idx * -1;
		if ((newpElemArr = (PElement*)calloc((negIdx), sizeof(PElement))) == NULL)
			return FAIL;
		memcpy(&(*(newpElemArr+negIdx)), pArr->pElemArr, (pArr->ArrSize) * sizeof(PElement));
		free(pArr->pElemArr);
		pArr->pElemArr = newpElemArr;
		(pArr->pElemArr)[0] = pArr->copyFunc(pNewElem);
		//idx = negIdx - 1;
	}
	else
	{
		pArr->delFunc((pArr->pElemArr)[idx]);
	}

	if (flag != 1)
	{
		(pArr->pElemArr)[idx] = pArr->copyFunc(pNewElem);
	}

	// Update Array Size
	if (idx >= pArr->ArrSize)
	{
		pArr->ArrSize = idx + 1;
	}
	else if (idx < 0)
	{
		pArr->ArrSize = negIdx + pArr->ArrSize;
	}

	return SUCCESS;
}


void DeleteAdptArray(PAdptArray pArr)
{
	int i;
	if (pArr == NULL)
		return;
	for (i = 0; i < pArr->ArrSize; ++i)
	{
		if (pArr->pElemArr[i] != NULL)
		{
			pArr->delFunc((pArr->pElemArr)[i]);
		}
	}

	free(pArr->pElemArr);
	free(pArr);
}

PElement GetAdptArrayAt(PAdptArray pArr, int index) 
{
	if (pArr == NULL || index < 0 || index >= pArr->ArrSize)
	{
		return NULL;
	}
	PElement i = pArr->pElemArr[index];

	if (i == NULL)
		return NULL;

	return pArr->copyFunc(i);
}

	
int GetAdptArraySize(PAdptArray arr)
 {
	if (arr == NULL) {
		return 1;
	}
	return arr->ArrSize;
}

void PrintDB(PAdptArray arr) 
{
	if (arr == NULL) {
		printf("Database is empty.\n");
		return;
	}
	if (arr->ArrSize == 0) {
		printf("Database is empty.\n");
		return;
	}
	for (int i = 0; i < arr->ArrSize; i++) {
		if (arr->pElemArr[i] != NULL)
		{
			arr->printFunc(arr->pElemArr[i]);
			//printf("\n");
		}
	}
}


/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "AdptArray.h"

#define INITIAL_SIZE 4

struct AdptArray_ {
    PElement* arr;
    int size;
    int capacity;
    COPY_FUNC copy_func;
    DEL_FUNC del_func;
    PRINT_FUNC print_func;
};

PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func) {
    PAdptArray arr = (PAdptArray)malloc(sizeof(struct AdptArray_));
    if (arr == NULL) {
        return NULL;
    }
    arr->arr = (PElement*)malloc(INITIAL_SIZE * sizeof(PElement));
    if (arr->arr == NULL) {
        free(arr);
        return NULL;
    }
    arr->size = 0;
    arr->capacity = INITIAL_SIZE;
    arr->copy_func = copy_func;
    arr->del_func = del_func;
    arr->print_func = print_func;
    return arr;
}

void DeleteAdptArray(PAdptArray arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < arr->size; i++) {
        if (arr->arr[i] != NULL) {
            arr->del_func(arr->arr[i]);
        }
    }
    free(arr->arr);
    free(arr);
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement element) {
    if (arr == NULL || index < 0) {
        return FAIL;
    }
    if (index >= arr->capacity) {
        int new_capacity = arr->capacity;
        while (index >= new_capacity) {
            new_capacity *= 2;
        }
        PElement* new_arr = (PElement*)realloc(arr->arr, new_capacity * sizeof(PElement));
        if (new_arr == NULL) {
            return FAIL;
        }
        arr->arr = new_arr;
        arr->capacity = new_capacity;
    }
    if (arr->arr[index] != NULL) {
        arr->del_func(arr->arr[index]);
    }
    arr->arr[index] = arr->copy_func(element);
    if (arr->arr[index] == NULL) {
        return FAIL;
    }
    if (index >= arr->size) {
        arr->size = index + 1;
    }
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray arr, int index) {
    if (arr == NULL || index < 0 || index >= arr->size) {
        return NULL;
    }
    return arr->copy_func(arr->arr[index]);
}

int GetAdptArraySize(PAdptArray arr) {
    if (arr == NULL) {
        return 1;
    }
    return arr->size;
}

void PrintDB(PAdptArray arr) {
    if (arr == NULL) {
        printf("Database is empty.\n");
        return;
    }
    if (arr->size == 0) {
        printf("Database is empty.\n");
        return;
    }
    printf("Database contents:\n");
    for (int i = 0; i < arr->size; i++) {
        printf("Record %d:\n", i);
        arr->print_func(arr->arr[i]);
        printf("\n");
    }
}
*/
