# 使用变量
# 支持自动推导
# 声明伪目标

OBJS = get.o sum.o val.o \
	main.o
EXE = main

${EXE}:${OBJS}
	gcc -o ${EXE} ${OBJS}

main.o:sum.h get.h
get.o:get.h
sum.o:sum.h
val.o:val.h

.PHONY: clean cleanall
clean:
	rm -rf ${OBJS}
cleanall:clean
	rm -rf ${EXE}