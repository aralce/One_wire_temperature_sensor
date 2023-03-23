name_of_test = "ONE_WIRE_TEMPERATURE_SENSOR -> Arduino"

TEST_MAIN_FOLDER_DIR = ./
MAIN_TEST_FOLDER_DIR = ../
PROGRAM_TO_TEST_FOLDER_DIR = $(MAIN_TEST_FOLDER_DIR)../

CPPUTEST_HOME = ../cpputest/

COMPILER_INCLUDE_FLAGS  = -I$(CPPUTEST_HOME)include
COMPILER_INCLUDE_FLAGS  += -I$(MAIN_TEST_FOLDER_DIR)

FLAG_FOR_DEFINE = -D IS_RUNNING_TESTS

CXX = g++
CXXFLAGS  =  -Wall $(COMPILER_INCLUDE_FLAGS) $(FLAG_FOR_DEFINE)

##UNCOMMENT TO TEST MEMORY LEAK
#CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h

LD_LIBRARIES  = -L$(CPPUTEST_HOME)lib -lCppUTest -lCppUTestExt

BUILD_OUTPUT_DIR = build/

all: create_build_folder link_objects_of_tests
	@echo $(name_of_test)
	@$(BUILD_OUTPUT_DIR)tests.out

clean:
	@rm -f -r $(BUILD_OUTPUT_DIR)

create_build_folder:
	@mkdir -p $(BUILD_OUTPUT_DIR)

OBJECT_FILES  = $(patsubst %.cpp, %.o, $(notdir $(wildcard $(TEST_MAIN_FOLDER_DIR)*.cpp)))
OBJECT_FILES  += $(patsubst %.cpp, %.o, $(notdir $(wildcard $(PROGRAM_TO_TEST_FOLDER_DIR)implementation/Arduino/*.cpp)))
OBJECT_FILES_ON_DIR = $(addprefix $(BUILD_OUTPUT_DIR),$(OBJECT_FILES))

link_objects_of_tests: $(OBJECT_FILES_ON_DIR)
	@$(CXX) $(CXXFLAGS) $^ $(LD_LIBRARIES) -o $(BUILD_OUTPUT_DIR)tests.out

vpath %.cpp $(PROGRAM_TO_TEST_FOLDER_DIR)implementation/Arduino/

$(BUILD_OUTPUT_DIR)%.o : %.cpp
	@$(CXX) $(CXXFLAGS) $< $(LD_LIBRARIES) -c -o $@

print:
	@ls $(PROGRAM_TO_TEST_FOLDER_DIR)implementation/Arduino/