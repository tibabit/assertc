CC=gcc

all:assert.c runner.c spec_reporter.c main.c
	@echo Building assert...
	@${CC} $^ -o assert
	@echo Done.

test:assert.c runner.c spec_reporter.c
	@echo Building test...
	@${CC} $^ -o $@
	@echo Done.
