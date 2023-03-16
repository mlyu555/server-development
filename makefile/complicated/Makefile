.PHONY: all clean plog

MKDIR = mkdir
RM = rm
RMFLAGS = -rf
CC = gcc

DIR_OBJ = objs
DIR_DEP = deps
DIR_EXE = exes
DIRS = $(DIR_OBJ) $(DIR_DEP) $(DIR_EXE)
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(DIR_OBJ)/, $(OBJS))
DEPS = $(SRCS:.c=.dep)
DEPS := $(addprefix $(DIR_DEP)/, $(DEPS))
EXE = complicated
EXE := $(addprefix $(DIR_EXE)/, $(EXE))

all: $(EXE)

ifeq ("$(wildcard $(DEP_DIR))", "")
     DEP_DIR_DEPS:=$(DEP_DIR)
endif
-include $(DEPS)

$(DIRS):
	$(MKDIR) $@

$(EXE): $(EXE_DIR) $(OBJS)
	$(CC) -o $@ $(filter %.o, $^)

$(OBJ_DIR)/%.o: $(OBJ_DIR) %.c
	$(CC) -o $@ -c $(filter %.c, $^)

$(DEP_DIR)/%.dep: $(DEP_DIR_DEPS) %.c
	@echo "Making $@ ..."
	@set -e; \
	$(RM) $(RMFLAGS) $@.tmp; \
	$(CC) -E -MM $(filter %.c, $^) > $@.tmp; \
	sed 's,\(.*\)\.o[ :]*,objs/\1.o: ,g' < $@.tmp > $@; \
	$(RM) $(RMFLAGS) $@.tmp

clean:
	$(RM) $(RMFLAGS) $(DIRS)

# plog:
# 	@echo "DIRS: $(DIRS)"
# 	@echo "OBJ_DIR: $(OBJ_DIR)"
# 	@echo "EXE_DIR: $(EXE_DIR)"
# 	@echo "DEP_DIR: $(DEP_DIR)"
# 	@echo "SRCS: $(SRCS)"
# 	@echo "OBJS: $(OBJS)"
# 	@echo "DEPS: $(DEPS)"
# 	@echo "EXE: $(EXE)"

# ------------首次make------------报错（include先于目标all构建）
# Making deps/main.dep ...
# /bin/sh: line 2: deps/main.dep.tmp: No such file or directory
# Makefile:24: deps/main.dep: No such file or directory
# make: *** [Makefile:31: deps/main.dep] Error 1

# ------------重复make------------

# ------------clean后make------------



# sed 's,\(.*\)\.o[ :]*,objs/\1.o $@: ,g' < $@.tmp > $@ ; \