# Курсовая работа по ОС. 
# Малков не разрешил делать на джаве :(

# Server-Client Task Dispatcher

**Server-Client Task Dispatcher** - это пример приложений клиента и сервера на C++, которые могут взаимодействовать друг с другом через сокетное соединение. Клиентское приложение отправляет информацию о задаче (в данном случае, все активные процессы) серверу, после чего сервер обрабатывает эту информацию и отсылает команду на убийство процессора клиенту. PID пользователь сервера выбирает сам.

## Структура проекта

- `Server.hpp`: Исходный код серверного приложения.
- `Client.hpp`: Исходный код клиентского приложения.
- `TaskManager.hpp`: Исходный код клиентского приложения.
- `Main.cpp`: Исходный код клиентского приложения.
- `README.md`: Описание проекта и инструкции по его запуску.

## Использование

1. **Сборка**: Для сборки приложений используйте команду `make`.

2. **Запуск сервера**: После сборки запустите серверное приложение с указанием IP-адреса или хоста.

3. **Запуск клиента**: После сборки запустите клиентское приложение с указанием IP-адреса или хоста сервера.

4. **Отправка задачи**: После запуска клиента серверу автоматически отправятся все текущие процессы.

5. **Убийство процесса**: Сервер обрабатывает введенный PID и отсылает SIGKILL для убийства процесса у клиента.

## Примечания

- В коде использованы стандартные библиотеки для работы с сокетами, а также для чтения и записи файлов.
- Примеры кода и функциональность основаны на обсуждениях и взаимодействии с помощью комментариев.
