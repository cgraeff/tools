SHELL := /bin/bash # Use bash as shell
TARGET = intersec

.PHONY: all run graph tests tgraph clean

all:
	@echo "[Building ...]"
	@cd src; make
	@echo "[done.]"
run:
	@bash run.sh
clean:
	@echo "[Cleaning ...]"
	@-rm -f $(TARGET)
	@cd src; make clean
	@cd hadrons; rm -rf *
	@cd quarks; rm -rf *
	@cd graph; rm -rf *
	@echo "[done.]"
