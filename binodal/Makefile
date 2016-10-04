SHELL := /bin/bash # Use bash as shell

.PHONY: all clean

all:
	@echo “[Generating graphs…]”
	@bash ./script.sh
	@echo "[done.]"
clean:
	@echo "[Cleaning...]"
	@cd graph; rm -rf *
	@cd hadrons; rm -rf *
	@cd quarks; rm -rf *
	@echo "[done.]"
