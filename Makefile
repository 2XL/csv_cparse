# fugoki-emulator-daemon Makefile
# Useful tooling for complex commands
# Specs: http://www.gnu.org/software/make/manual/make.html

# Env vars
export PROJECT_NAME=csv_parser
export NAMESPACE=meteorologicas
export PATH := .scripts:$(PATH)


# COLORS
GREEN  := $(shell tput -Txterm setaf 2)
YELLOW := $(shell tput -Txterm setaf 3)
WHITE  := $(shell tput -Txterm setaf 7)
RESET  := $(shell tput -Txterm sgr0)

VERSION_FILE=VERSION
VERSION=`cat $(VERSION_FILE)`

TARGET_MAX_CHAR_NUM=25

## Show help
help:
	@echo ''
	@echo 'Usage:'
	@echo '  ${YELLOW}make${RESET} ${GREEN}<target>${RESET}'
	@echo ''
	@echo 'Targets:'
	@awk '/^[a-zA-Z\-\_0-9]+:/ { \
		helpMessage = match(lastLine, /^## (.*)/); \
		if (helpMessage) { \
			helpCommand = substr($$1, 0, index($$1, ":")-1); \
			helpMessage = substr(lastLine, RSTART + 3, RLENGTH); \
			printf "  ${YELLOW}%-$(TARGET_MAX_CHAR_NUM)s${RESET} ${GREEN}%s${RESET}\n", helpCommand, helpMessage; \
		} \
	} \
	{ lastLine = $$0 }' $(MAKEFILE_LIST)


## Clean compiled files
clean:
	@echo CLEAN
	@rm -f main
	@echo ...

## Compile Source code
compile:
	@echo COMPILE
	@gcc -o main main.c && chmod u+x main
	@echo ...

## Run
run:
	@./main
	@echo ...

## Test
test:
	@# do something
	@echo ...

## Retry
retry: clean compile run


.PHONY: