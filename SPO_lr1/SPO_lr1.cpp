#include <windows.h>
#include <iostream>
using namespace std;

int main()
{
    //Замеры времени
    LARGE_INTEGER t0;
    QueryPerformanceCounter(&t0); //замер текущего времени
    cout << t0.QuadPart << '\n';
    double result = sqrt(t0.QuadPart); //вычисление квадратного корня из текущего времени
    LARGE_INTEGER t1;
    QueryPerformanceCounter(&t1); //замер текущего времени после вычисления квадратного корня
    cout << t1.QuadPart << '\n';
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency); //замер частоты счетчика временных интервалов
    double ticks = t1.QuadPart - t0.QuadPart; //длительность интервала в тактах
    cout << ticks << '\n';
    double ticks_per_sec = frequency.QuadPart; //отдельная переменная для частоты
    cout << "Frequency: " << ticks_per_sec << '\n';
    cout << "\n";
    double usec_per_sec = 1e6; //отдельная переменная для числа микросекунд в секунде
    double usec = usec_per_sec * ticks / ticks_per_sec; //вычисление времени в микросекундах
    cout << usec << '\n';
    printf("result=%g, duration=%.3f usec\n", result, usec);

    //Получение версии операционной системы
    DWORD info = GetVersion(); //получение номера версии ОС
    DWORD mask = 0x0000ffff; //создание маски
    DWORD version = info & mask; //наложение маски или побитовое «И»
    DWORD platform = info >> 16; //сдвиг вправо на 16 бит для получения старшего слова
    DWORD version_major = version & 0x000000ff; //получение младшего байта младшего слова
    DWORD version_minor = version >> 8; //получение старшего байта младшего слова
    DWORD build = 0;
    if ((info & 0x10000000) == 0) //проверка равенства старшего бита нулю
    {
        build = platform >> 8; //получение самого старшего байта
    }
    cout << "Windows v" << version_major << "." << version_minor << " (build " << build << ")" << '\n';

    //Получение системного каталога
    wchar_t system_dir[MAX_PATH];
    if (GetSystemDirectory(system_dir, MAX_PATH))
        wcout << "System Directory: " << system_dir << '\n';      
    else
        cout << "Error: " << GetLastError() << '\n';

    DWORD buf_size = MAX_PATH;

    //Получение названия компьютера
    wchar_t comp_name[MAX_PATH];
    if (GetComputerName(comp_name, &buf_size))
        wcout << "Computer name: " << comp_name << '\n';
    else 
        cout << "Error: " << GetLastError() << '\n';

    //Получение имени текущего пользователя
    wchar_t user_name[MAX_PATH];
    if (GetUserName(user_name, &buf_size))
        wcout << "User name: " << user_name << '\n';
    else
        cout << "Error: " << GetLastError() << '\n';

    //Получение информации о дисках
    wchar_t buffer[MAX_PATH];
    wchar_t buf_path[MAX_PATH * 4];
    ULARGE_INTEGER free_bytes, total_bytes;

    HANDLE search = FindFirstVolume(buffer, sizeof(buffer)); //определение имени первого тома
    int i = 0; //инициализация переменной для подсчета номера тома
    do {
        wcout << "Technical name " << i << " volume:" << buffer << '\n'; //печать техничекого имени i тома
        i++;
        DWORD buf_size = MAX_PATH * 4;
        GetVolumePathNamesForVolumeName(buffer, buf_path, buf_size, &buf_size); //получение из технического имени тома путь к его каталогу
        wchar_t* path = buf_path; 
        wcout << "Mount point: " << path << '\n'; 
        if (GetDiskFreeSpaceEx(buf_path, &free_bytes, &total_bytes, NULL) == 0) //получение значений количества свободных байт и общего обьема
            cout << "Error:" << GetLastError() << '\n';
        else
        {
            cout << "Free bytes : " << free_bytes.QuadPart << '\n';
            cout << "Total bytes: " << total_bytes.QuadPart << '\n';
        }
    }while (FindNextVolume(search, buffer, sizeof(buffer))); //переход к следующему тому
    if (GetLastError() != ERROR_NO_MORE_FILES) 
        cout << "Error:" << GetLastError() << '\n'; //получение информации об ошибке
    FindVolumeClose(search); //закрытие перебора томов

    return 0;
}