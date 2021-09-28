#	Author: OdinhengeT 
#	Date: 2021-09-28

#	Description:
#	This is a basic makefile created to build small C++ projects (with the MSYS2 Mingw-w64 toolchain), and to support them as they grow larger.
#	Supports: .cpp format (not .cc), compiling to assembly, Libraries and Subdirectories, testing

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Compiler & Linker

# Compiler
CXX := g++

# Flags
CXX_VERSION := c++17
CXX_OPTIMIZATION := O2
CXX_WARNINGS := Wall Wextra pedantic-errors Wold-style-cast

CXXFLAGS := $(addprefix -, std=$(CXX_VERSION) $(CXX_OPTIMIZATION) $(CXX_WARNINGS))

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Directory Structure

# Main Directorues 
dASM  := asm
dBIN  := bin
dLIB  := libs
dSRC  := src
dTEST := testing

# Subdirectories (in src)
subDirs := entity graphics graphics/fonts window

# Files contained in Subdirectories

entity := $(addprefix entity/, \
	entity.cpp \
)

graphics/fonts := $(addprefix graphics/fonts/, \
	bitFont.cpp \
)

graphics := $(addprefix graphics/, \
	frame.cpp \
)

window := $(addprefix window/, \
	window.cpp \
)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Programs to Build

Programs := Pong

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Default & PHONY Targets

# Default Routine Run by Make
default: init settings log $(addsuffix .exe,$(Programs)) done

# Declaration of PHONY (non-file) Targets
.PHONY: asm asm_init default done init log settings test test_init

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Pong

Pong_deps := pong.cpp $(entity) $(graphics) $(graphics/fonts) $(window)
Pong_libs := gdi32

# Executable Build
Pong.exe: $(addprefix $(dBIN)/, $(Pong_deps:.cpp=.o))
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(addprefix -l, $(Pong_libs))
	@echo "- Created $@"

# Assembly Build 
Pong.s: $(addprefix $(dASM)/, $(Pong_deps:.cpp=.s))

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Testing

test: test_init settings log testMain.exe done

# Test-cases to be run by testMain
Tests := testMain.cpp

# Dependencies & Libraries of tests to be run (from dSRC)
Test_deps := $(entity) $(graphics) $(graphics/fonts) $(window)
Test_libs := gdi32

# Executable Build
testMain.exe: $(addprefix $(dTEST)/bin/, $(Tests:.cpp=.o)) $(addprefix $(dBIN)/, $(Test_deps:.cpp=.o))
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(addprefix -l, $(Test_libs))
	@echo "- Created $@"

test_init:
	@echo "==Initializing Testing=="
	$(call createDirectory,$(dTEST)) 
	$(call createDirectory,$(dTEST)/bin) 
	@echo "- Done"

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Assembly

asm: asm_init settings log $(addsuffix .s,$(Programs)) done

asm_init:
	@echo "==Initializing Assembly=="
	$(call createDirectory,$(dASM)) 
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dASM)/,$(p))))
	@echo "- Done"

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Standardized Compile-routines

$(addprefix $(dBIN)/, %.o): $(addprefix $(dSRC)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -o $@ -c $^
	@echo "- Created $@"

$(addprefix $(dTEST)/bin/, %.o): $(addprefix $(dTEST)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -g -o $@ -c $^
	@echo "- Created $@"

$(addprefix $(dASM)/, %.s): $(addprefix $(dSRC)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -S -o $@ $^
	@echo "- Created $@"

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Extra Targets

# Display Settings (Flags ect)
settings:
	@echo "==Settings=="
	@echo "- Version: $(CXX) $(CXX_VERS)"
	@echo "- Optimization: $(CXX_OPTI)"
	@echo "- Warnings: $(CXX_WARN)"

# Install Routine
init:
	@echo "==Initializing=="
	$(call createDirectory,$(dBIN))
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dBIN)/,$(p))))
	$(call createDirectory,$(dSRC))
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dSRC)/,$(p))))
	$(call createDirectory,$(dLIB))
	@echo "- Done"

# Log Message
log: 
	@echo "==Log=="

# Done Message
done:
	@echo "==Done=="

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Functions

# Initialize Directory (Empty Line IS Necessary)
define createDirectory
	@if [ ! -d "$(1)" ]; \
		then echo "- Created $(1)"; \
		mkdir $(1); \
	fi

endef