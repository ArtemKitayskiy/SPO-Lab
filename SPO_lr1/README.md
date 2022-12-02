### Задание на лабораторную работу
Написать программу, использующую Windows API, которая без запросов к пользователю выводит в терминал сведения о системе, а именно:

1. Скорость вычисления квадратного корня при помощи механизма QPC.
2. Номер версии Windows.
3. Системный каталог, название машины и псевдоним текущего пользователя.
4. Список логических томов с их названиями, первой точкой монтирования размером и количеством свободного места.

### Результат работы программы:
```
result=378206, duration=1471.500 usec
Windows v6.2 (build 35)
System Directory: C:\Windows\system32
Computer name: ARTEM-PC
User name: Artem
Technical name 0 volume:\\?\Volume{62c21e2a-5019-4cfc-b12d-8ac10631397a}\
Mount point:
Error:3
Technical name 1 volume:\\?\Volume{036984a4-1a66-420a-a16c-547c3027dc0b}\
Mount point: C:\
Free bytes : 24704487424
Total bytes: 119388762112
Technical name 2 volume:\\?\Volume{1ba864c1-66f9-11ea-be58-c838e0f8944e}\
Mount point: D:\
Free bytes : 109451710464
Total bytes: 998444605440
Technical name 3 volume:\\?\Volume{3893a47a-41cb-4ace-9bc4-8d267372eba4}\
Mount point:
Error:3
Technical name 4 volume:\\?\Volume{99b735ae-66f6-11ea-be56-806e6f6e6963}\
Mount point: E:\
Error:21
