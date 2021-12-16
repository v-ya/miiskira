package		:= package/
task-list	:= $(patsubst %/Makefile, %/, $(wildcard */Makefile))
kiya-list	= $(patsubst %.kiya, %, $(wildcard */*.kiya))

%/:
	@mkdir -p $@

build: ${package} target
	@$(foreach kiya, ${kiya-list}, cp ${kiya}.kiya $<$(notdir ${kiya}) && echo -e "build $<\e[032;1m$(notdir ${kiya})\e[0m";)
target: ${task-list}
	@$(foreach task, $^, make --no-print-directory -C ${task} $@;)
clear: ${task-list}
	@$(foreach task, $^, make --no-print-directory -C ${task} $@;)
	@rm -rf ${package}

.PHONY: build target clear
