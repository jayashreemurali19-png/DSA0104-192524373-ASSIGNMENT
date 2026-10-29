#include <iostream>
#include <algorithm>

int& highestMark(int marks[], int n)
{
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return marks[index];
}

// Deliberately unsafe demonstration. Do not call/use the returned reference.
// Compilers commonly warn that this returns a reference to a local variable.
int& danglingReference()
{
    int local = 42;
    return local; // undefined behaviour if the returned reference is used
}

int* highestMarkPointer(int marks[], int n)
{
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return &marks[index];
}

int highestMarkIndex(const int marks[], int n)
{
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return index;
}

int main()
{
    int marks[] = {78, 91, 85, 88};
    highestMark(marks, 4) += 2;
    std::cout << "Moderated highest mark: " << highestMark(marks, 4) << '\n';

    int idx = highestMarkIndex(marks, 4);
    std::cout << "Index result: " << idx << ", mark: " << marks[idx] << '\n';
    int* ptr = highestMarkPointer(marks, 4);
    std::cout << "Pointer result: " << *ptr << '\n';
}
