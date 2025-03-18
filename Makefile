CC = gcc
CFLAGS = -Wall -pthread
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS = -lrt -pthread
else
	LDFLAGS = -pthread
endif
all: client_signal server_signal client_flag server_flag client_sem server_sem
client_signal: client_signal.c
	$(CC) $(CFLAGS) client_signal.c -o client_signal $(LDFLAGS)
server_signal: server_signal.c
	$(CC) $(CFLAGS) server_signal.c -o server_signal $(LDFLAGS)
client_flag: client_flag.c
	$(CC) $(CFLAGS) client_flag.c -o client_flag $(LDFLAGS)
server_flag: server_flag.c
	$(CC) $(CFLAGS) server_flag.c -o server_flag $(LDFLAGS)
client_sem: client_sem.c
	$(CC) $(CFLAGS) client_sem.c -o client_sem $(LDFLAGS)
server_sem: server_sem.c
	$(CC) $(CFLAGS) server_sem.c -o server_sem $(LDFLAGS)
clean:
	rm -f client_signal server_signal client_flag server_flag client_sem server_sem
clean-tex:
	rm -f *.aux *.log *.toc *.out *.synctex.gz
clean-all: clean clean-tex
	rm -f report.pdf
report: report.tex
	pdflatex -interaction=nonstopmode report.tex || true
	pdflatex -interaction=nonstopmode report.tex
	rm -f *.aux *.log *.toc *.out *.synctex.gz
run_signal:
	./server_signal & sleep 1 && ./client_signal
run_flag:
	./server_flag & sleep 1 && ./client_flag
run_sem:
	./server_sem & sleep 1 && ./client_sem
clean_resources:
	-rm -f /dev/shm/my_shared_memory
	-rm -f /dev/shm/sem.sem_producer
	-rm -f /dev/shm/sem.sem_consumer
	-rm -f /dev/shm/sem.sem_termination
	-rm -f /tmp/my_shared_memory
	-rm -f /tmp/sem.sem_producer
	-rm -f /tmp/sem.sem_consumer
	-rm -f /tmp/sem.sem_termination
rerun_signal: clean_resources
	$(MAKE) run_signal
rerun_flag: clean_resources
	$(MAKE) run_flag
rerun_sem: clean_resources
	$(MAKE) run_sem
