#	Author: OdinhengeT 
#	Date: 2021-09-10

#	Description:
#	This is a basic makefile created to build small C++ projects (with the MSYS2 Mingw-w64 toolchain), and to support them as they grow larger.
#	Supports: .cpp format (not .cc), compiling to assembly, Libraries and Subdirectories, 

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
dASM := asm
dBIN := bin
dLIB := libs
dSRC := src

# Subdirectories (in src)
subDirs := fonts graphics qShapes window

# Files contained in Subdirectories
fonts := $(addprefix fonts/, \
	thorFont.cpp \
)

graphics := $(addprefix graphics/, \
	frame.cpp ThorColor.cpp \
)

qShapes := $(addprefix qShapes/, \
	qShapes.cpp \
)

window := $(addprefix window/, \
	window.cpp \
)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Programs to Build

Programs := Pong

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Phony & Main Targets

.PHONY: assembly default done install install_assembly log settings

# Default Routine Run by Make
default: install settings log $(addsuffix .exe,$(Programs)) done

# Assembly Routine
assembly: install_assembly settings log $(addsuffix .s,$(Programs)) done

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Pong

Pong_deps := pong.cpp $(fonts) $(graphics) $(qShapes) $(window)
Pong_libs := gdi32

# Executable Build
Pong.exe: $(addprefix $(dBIN)/, $(Pong_deps:.cpp=.o))
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(addprefix -l, $(Pong_libs))
	@echo "- Created $@"

# Assembly Build 
Pong.s: $(addprefix $(dASM)/, $(Pong_deps:.cpp=.s))

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Standardized Compile-routines

$(addprefix $(dBIN)/, %.o): $(addprefix $(dSRC)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -o $@ -c $^
	@echo "- Created $@"

$(addprefix $(dASM)/, %.s): $(addprefix $(dSRC)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -S -o $@ $^
	@echo "- Created $@"

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Functions

# Initialize Directory (Empty Line IS Necessary)
define createDirectory
	@if [ ! -d "$(1)" ]; \
		then echo "- Created $(1)"; \
		mkdir $(1); \
	fi

endef

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#	Extra Targets

# Display Settings (Flags ect)
settings:
	@echo "==Settings=="
	@echo "- Version: $(CXX) $(CXX_VERS)"
	@echo "- Optimization: $(CXX_OPTI)"
	@echo "- Warnings: $(CXX_WARN)"

# Install Routine
install:
	@echo "==Installing=="
	$(call createDirectory,$(dBIN))
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dBIN)/,$(p))))
	$(call createDirectory,$(dSRC))
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dSRC)/,$(p))))
	$(call createDirectory,$(dLIB))
	@echo "- Done"

# Assembly Install Routine
install_assembly:
	@echo "==Installing Assembly=="
	$(call createDirectory,$(dASM)) 
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dASM)/,$(p))))
	@echo "- Done"

# Log Message
log: 
	@echo "==Log=="

# Done Message
done:
	@echo "==Done=="