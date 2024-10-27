SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
DIFFERENT_FLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla
DEBUG_SET = -DDEBUG -DTURN_ON_CANARIES -DTURN_ON_HASH
OBJECTS = o_files/main.o o_files/getNumber.o o_files/stepByStep.o o_files/getData_of_BinaryFile.o o_files/getCommands.o o_files/stackPushPop.o o_files/stackInitDestroy.o o_files/stackDump.o o_files/stackAssertFunction.o o_files/stackError.o o_files/countHash.o o_files/runCode.o

ifeq ($(sbs), 1)
	DEBUG_SET= -DDEBUG -DTURN_ON_CANARIES -DTURN_ON_HASH -DSTEPBYSTEP
endif

all: init out

out: $(OBJECTS)
	g++ $(OBJECTS) -o out $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/main.o: cpp_files/main.cpp
	g++ -c cpp_files/main.cpp -o o_files/main.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/getData_of_BinaryFile.o: cpp_files/getData_of_BinaryFile.cpp
	g++ -c cpp_files/getData_of_BinaryFile.cpp -o o_files/getData_of_BinaryFile.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/getNumber.o: cpp_files/getNumber.cpp
	g++ -c cpp_files/getNumber.cpp -o o_files/getNumber.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/runCode.o: cpp_files/runCode.cpp
	g++ -c cpp_files/runCode.cpp -o o_files/runCode.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/getCommands.o: cpp_files/getCommands.cpp
	g++ -c cpp_files/getCommands.cpp -o o_files/getCommands.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/stepByStep.o: cpp_files/stepByStep.cpp
	g++ -c cpp_files/stepByStep.cpp -o o_files/stepByStep.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)




o_files/stackPushPop.o: ../workWithStack/cpp_files/stackPushPop.cpp
	g++ -c ../workWithStack/cpp_files/stackPushPop.cpp -o o_files/stackPushPop.o $(DEBUG_SET) -lncurses -DDEBUG $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/stackInitDestroy.o: ../workWithStack/cpp_files/stackInitDestroy.cpp
	g++ -c ../workWithStack/cpp_files/stackInitDestroy.cpp -o o_files/stackInitDestroy.o $(DEBUG_SET) -lncurses -DDEBUG $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/stackDump.o: ../workWithStack/cpp_files/stackDump.cpp
	g++ -c ../workWithStack/cpp_files/stackDump.cpp -o o_files/stackDump.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/stackAssertFunction.o: ../workWithStack/cpp_files/stackAssertFunction.cpp
	g++ -c ../workWithStack/cpp_files/stackAssertFunction.cpp -o o_files/stackAssertFunction.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/stackError.o: ../workWithStack/cpp_files/stackError.cpp
	g++ -c ../workWithStack/cpp_files/stackError.cpp -o o_files/stackError.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

o_files/countHash.o: ../workWithStack/cpp_files/countHash.cpp
	g++ -c ../workWithStack/cpp_files/countHash.cpp -o o_files/countHash.o $(DEBUG_SET) -lncurses $(DIFFERENT_FLAGS) $(SANITIZER)

clean:
	rm -rf o_files/*.o out o_files

init:
	mkdir -p o_files