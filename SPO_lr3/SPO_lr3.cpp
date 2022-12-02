#include <iostream>
#include <windows.h>
#include <iomanip>
#define PAGE(addr, size) ((uintptr_t)addr & ~((size_t)size - 1))

using namespace std;


uintptr_t address_fun(uintptr_t least, uintptr_t largest)
{
    uintptr_t mem_addr;

    while (true)
    {
        cin >> hex >> mem_addr; //Ввод крайнего значения адреса
        bool flag = (mem_addr >= least && mem_addr <= largest); //Переменная для проверки вхождения адреса в диапазон

        if (flag)
        {
            break;
        }

        else
        {
            cout << "The entered value is out of range! Enter again:"; 
        }
    }
    return mem_addr;
}

void mem_inf_fun(MEMORY_BASIC_INFORMATION info)
{
    cout << dec << info.RegionSize << "\t"; //Размер в байтах
    
    //Тип выделения памяти и был ли использован регион
    switch (info.State)
    {
    case MEM_COMMIT:
        cout << "Yes" << "\t" << "C" << "\t";
        break;
    case MEM_RESERVE:
        cout << "Yes" << "\t" << "R" << "\t";
        break;
    case MEM_FREE:
        cout << "No" << "\t" << "F" << "\t";
        break;
    }

    //Опция защиты памяти
    switch (info.AllocationProtect)
    {
    case PAGE_EXECUTE:
        cout << "--X-";
        break;
    case PAGE_EXECUTE_READ:
        cout << "R-X-";
        break;
    case PAGE_EXECUTE_READWRITE:
        cout << "RWX-";
        break;
    case PAGE_EXECUTE_WRITECOPY:
        cout << "R-XC";
        break;
    case PAGE_NOACCESS:
        cout << "----";
        break;
    case PAGE_READONLY:
        cout << "R---";
        break;
    case PAGE_READWRITE:
        cout << "RW--";
        break;
    case PAGE_WRITECOPY:
        cout << "R--C";
        break;
    default:
        cout << "    ";
        break;
    }
    cout << setw(10);

    //Тип страниц в регионе
    switch (info.Type)
    {
    case MEM_IMAGE:
        cout << "image" << "\n";
        break;
    case MEM_MAPPED:
        cout << "mapped" << "\n";
        break;
    case MEM_PRIVATE:
        cout << "private" << "\n";
        break;
    default:
        cout << "unused" << "\n";
        break;

        cout << "\n";
    }
}

int main()
{
    SYSTEM_INFO sys_inf; //Указатель на структуру, которая содержит информацию о системе
    MEMORY_BASIC_INFORMATION info{}; //Указатель на структуру, которая содержит информацию о диапазоне страниц

    GetSystemInfo(&sys_inf); //Извлекает информацию о текущей системе

    auto least_address = reinterpret_cast<uintptr_t>(sys_inf.lpMinimumApplicationAddress); //Наименьший адрес памяти
    auto largest_address = reinterpret_cast<uintptr_t>(sys_inf.lpMaximumApplicationAddress); //Наибольший адрес памяти

    //Вывод диапазона адресов в шестнадцатеричном виде
    cout << "Addresses range:";
    cout << "[" << hex << showbase << least_address << ", " << largest_address << "]\n";

    //Ввод нижней границы диапазона
    cout << "Enter lower limit:";
    auto least = address_fun(least_address, largest_address); 

    //Ввод верхней границы диапазона
    cout << "Enter upper limit:";
    auto largest = address_fun(least, largest_address);

    //Выравнивание по границе страницы
    least = PAGE(least, sys_inf.dwPageSize);
    largest = PAGE(largest, sys_inf.dwPageSize);

    //Отображение легенды
    cout << "\nLegend:\n";
    cout << "\tState:\tFree, Reserved, Committed;\n";
    cout << "\tAccess:\tReadable, Writable, eXecutable, Copy-on-write, Guard,\n";
    cout << "\t\tNo cache, write combine (Q), no access (-------);\n";
    cout << "\t\tempty space indicates no access to page protection info.\n\n";

    //Вывод таблицы регионов
    cout << "   Address \tSize \tUsed \tState \tAccess \tType\n";
    for (uintptr_t num = least; num < largest; num += info.RegionSize)
    {
        if (VirtualQuery((void*)num, &info, sizeof info) != 0) //Извлекает информацию о диапазоне страниц
        {
            cout << setw(10) << hex << showbase << num << "\t"; //Вывод адреса в шестнадцатеричном виде
            mem_inf_fun(info);
        }

        else
        {
            cout << "Error:" << GetLastError();
            break;
        }
    }
}
