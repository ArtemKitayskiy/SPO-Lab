#include <iostream>
#include <windows.h>

using namespace std;

HANDLE input_pipe_read_end; 
HANDLE input_pipe_write_end;
HANDLE output_pipe_read_end; 
HANDLE output_pipe_write_end; 
DWORD bytes_read;

void CreateChildProcess();
void ReadInvite();

int main()
{
    SECURITY_ATTRIBUTES attributes;

    // настройка параметров структуры SECURITY_ATTRIBUTES
    attributes.nLength = sizeof(SECURITY_ATTRIBUTES); // размер структуры в байтах
    attributes.bInheritHandle = TRUE; // при создании нового процесса дескриптор канала наследуется
    attributes.lpSecurityDescriptor = NULL; // присвоение объекту дескриптора по умолчанию

    // создание анонимного канала ввода
    CreatePipe(&input_pipe_read_end, &input_pipe_write_end, &attributes, 0); 
    
    // создание анонимного канала вывода
    CreatePipe(&output_pipe_read_end, &output_pipe_write_end, &attributes, 0); 
    
    // разрешение наследования дескрипторов-концов, которые остаются в текущем процессе
    SetHandleInformation(input_pipe_write_end, HANDLE_FLAG_INHERIT, 0); 
    SetHandleInformation(output_pipe_read_end, HANDLE_FLAG_INHERIT, 0);

    CreateChildProcess(); 
    ReadInvite(); 
    
    while (true)
    {
        const char PLEASE[] = "please";
        char* input = NULL;
        char buffer[256];
        
        while (!input)
        {
            fgets(buffer, sizeof(buffer), stdin); // получает строку из потока
            if (!strncmp(buffer, "thanks", 6)) // сравнивает символы buffer с "thanks"
            {
                break; //завершает цикл в случае равенства строк
            }
            else if (strncmp(buffer, "please", 6)) // сравнивает символы buffer с "please" 
            {
                fprintf(stderr, "Please ask politely!\n>"); // печатает строку в поток
            }
            else
            {
                input = buffer + sizeof(PLEASE); //input указывает на начало непосредственно команды
            }
        }

        if (input != NULL)
        {
            WriteFile(input_pipe_write_end, input, strlen(input), NULL, NULL); // записывает данные в канал дочернего процесса
            ReadInvite();
        }
        else
        {
            break;
        }
    }
 
    return 0;
}

void CreateChildProcess()
// создание дочернего процесса, который использует ранее созданные каналы
{
    wchar_t cmd[] = L"cmd.exe";
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;

    // настройка параметров структуры STARTUPINFO
    ZeroMemory(&siStartInfo, sizeof(siStartInfo)); // заполняет блок памяти нулями, заданный указателем на начало и длиной
    siStartInfo.cb = sizeof(siStartInfo); // размер структуры в байтах
    siStartInfo.hStdInput = input_pipe_read_end; // дескриптор ввода для процесса
    siStartInfo.hStdOutput = output_pipe_write_end; // дескриптор вывода для процесса
    siStartInfo.hStdError = output_pipe_write_end; // дескриптор ошибки для процесса
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES; // дескрипторы наследуются

    // запуск дочернего процесса 
    if (!CreateProcess(
        NULL, // нет имени модуля
        cmd, // командная строка
        NULL, // дескриптор процесса не наследуется
        NULL, // дескриптор потока не наследуется
        TRUE, // дескриптор наследуется
        0, // нет флагов создания
        NULL, // используется родительский блок конфигурации
        NULL, // используется начальный родительский каталог
        &siStartInfo, // указатель на структуру STARTUPINFO
        &piProcInfo)) // указатель на структуру PROCESS_INFORMATION
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
    }

    // закрытие дескрипторов дочернего процесса и его потока
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    
    // закрытие дескрипторов каналов
    CloseHandle(input_pipe_read_end);
    CloseHandle(output_pipe_write_end);
}

void ReadInvite()
// отображения приглашений командной строки, чтения команд и выдачи результатов
{
    char chBuf[64];
    do
    {
        ReadFile(output_pipe_read_end, chBuf, sizeof(chBuf), &bytes_read, NULL); //считывает данные из канала дочернего процесса
        fwrite(chBuf, bytes_read, 1, stdout); // записывает данные в поток 
    } while ((bytes_read != 0) && (chBuf[bytes_read - 1] != '>'));
}
