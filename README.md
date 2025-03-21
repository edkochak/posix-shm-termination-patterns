# Взаимодействие через разделяемую память POSIX
## Компиляция программ
Для компиляции всех программ используйте команду:
```bash
make all
```
## Запуск программ
Для запуска различных вариантов программ используйте:
```bash
# Вариант 1: Завершение с использованием сигналов
make run_signal
# Вариант 2: Завершение с использованием флага
make run_flag
# Вариант 3: Завершение с использованием семафора
make run_sem
```
## Компиляция отчета
Для компиляции отчета необходим установленный LaTeX. Используйте следующие команды:
```bash
# Генерация PDF из .tex файла
pdflatex report.tex
# Генерация оглавления
pdflatex report.tex
```
## Структура проекта
- `shared_memory.h` - Общие определения и структуры
- `client_signal.c` и `server_signal.c` - Вариант 1 (с сигналами)
- `client_flag.c` и `server_flag.c` - Вариант 2 (с флагом в памяти)
- `client_sem.c` и `server_sem.c` - Вариант 3 (с семафором)
- `report.tex` - Исходный код отчета в формате LaTeX
- `Makefile` - Файл сборки и запуска программ
