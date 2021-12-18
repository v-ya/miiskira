render-list	= $(patsubst %/Makefile, %/%.render, $(wildcard */Makefile))
clear-me	= clear-impl

%/%.render: %/
	@make --no-print-directory -C $<

target: miiskira.render.kiiyaa
miiskira.render.kiiyaa: render.builder ${render-list}
include ../miiskira.in
.PHONY: clear-impl
clear-impl:
	@$(foreach task, $(patsubst %/Makefile, %/, $(wildcard */Makefile)), make --no-print-directory -C ${task} clear;)
