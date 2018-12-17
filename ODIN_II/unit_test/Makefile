# If the first argument is "run"...
ifeq (build,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "make"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif
ifeq (run,$(firstword $(MAKECMDGOALS)))	
  # use the rest as arguments for "make"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif
ifeq (gdb,$(firstword $(MAKECMDGOALS)))	
  # use the rest as arguments for "make"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif
ifeq (valgrind,$(firstword $(MAKECMDGOALS)))	
  # use the rest as arguments for "make"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif
ifeq (debug,$(firstword $(MAKECMDGOALS)))	
  # use the rest as arguments for "make"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

INCLUDE =-I../SRC/include

BIN = bin/exec.out

C = clang++ -std=c++14 -lpthread

cleanup_flags=\
-fsanitize=thread \
-ferror-limit=1000 \
-Werror \
-Wpedantic \
-Wclobbered \
-Wcast-function-type \
-Wempty-body \
-Wignored-qualifiers \
-Wimplicit-fallthrough=3 \
-Wmissing-field-initializers \
-Woverride-init \
-Wredundant-move \
-Wtype-limits \
-Wuninitialized \
-Wshift-negative-value \
-Waddress \
-Wbool-compare \
-Wbool-operation \
-Wc++11-compat-Wc++14-compat \
-Wcatch-value \
-Wchar-subscripts \
-Wcomment \
-Wformat \
-Wint-in-bool-context \
-Winit-self \
-Wlogical-not-parentheses \
-Wmaybe-uninitialized \
-Wmemset-elt-size \
-Wmemset-transposed-args \
-Wmisleading-indentation \
-Wmissing-attributes \
-Wmultistatement-macros \
-Wnarrowing \
-Wnonnull \
-Wnonnull-compare \
-Wopenmp-simd \
-Wparentheses \
-Wpessimizing-move \
-Wpointer-sign \
-Wreorder \
-Wrestrict \
-Wreturn-type \
-Wsequence-point \
-Wsign-compare \
-Wsizeof-pointer-div \
-Wsizeof-pointer-memaccess \
-Wstrict-aliasing \
-Wstrict-overflow=1 \
-Wswitch \
-Wtautological-compare \
-Wtrigraphs \
-Wuninitialized \
-Wunknown-pragmas \
-Wunused-label \
-Wunused-value \
-Wunused-variable \
-Wvolatile-register-var \
-Wno-unknown-warning-option

PHONY: error

error: 
	echo "can only use 'clean', 'build <testname>.cpp' or 'run <arguments>'"

debug:
	mkdir -p bin
	$(C) -ggdb $(cleanup_flags) $(INCLUDE) $(RUN_ARGS) -o $(BIN)

build:
	mkdir -p bin
	$(C) $(INCLUDE) $(RUN_ARGS) -o $(BIN)

run:
	$(BIN) $(RUN_ARGS) 

valgrind: build
	valgrind --tool=helgrind $(BIN) $(RUN_ARGS) 

gdb:
	gdb --args $(BIN) $(RUN_ARGS)

clean:
	$(RM) -Rf bin

