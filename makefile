#	Makefile for C++ Projects
#	Version 1.0 (beta)
#	Date: 2021-09-28
#	Author: OdinhengeT (https://github.com/OdinhengeT)

#	==========================================================================================================================
#	   :######:   :#####:    :####: :##:    :##: :##:    :##: :########: :##:    :##:   :######:    :########: :############: 
#	  :###::###:  :##::##:.   :##:  :###:   :##: :##:    :##: :##:'''''  :###:   :##:  :###::###:   :##:'''''   '''':##:''''  
#	 :###:  :###: :##:  :##:  :##:  :####:  :##: :##:    :##: :##:       :####:  :##: :###:  :###:  :##:            :##:      
#	 :##:    :##: :##:   :##: :##:  :##:##: :##: :##:....:##: :##:...    :##:##: :##: :##:    '''   :##:...         :##:      
#	 :###:  :###: :##:   :##: :##:  :##::##::##: :##########: :######:   :##::##::##: :##:   .....  :######:        :##:      
#	  :###::###:  :##:   :##: :##:  :##: :##:##: :##:'''':##: :##''''    :##: :##:##: :##:  :#####: :##:'''         :##:      
#	   :######:   :##:  :##:  :##:  :##:  :####: :##:    :##: :##        :##:  :####: :###:  :####: :##:            :##:      
#	  :###::###.  :##::##:'   :##:  :##:   :###: :##:    :##: :##:.....  :##:   :###:  :###::###:'  :##:            :##:      
#	 :###:  :###: :#####:    :####: :##:    :##: :##:    :##: :########: :##:    :##:   :######:    :########:      :##:      
#	==========================================================================================================================

#	Description: 
#		This is a basic makefile created to build small C++ projects, and to support them as they grow larger. Although this
#		makefile is originaly intended to build using the MSYS2 Mingw-w64 toolchain, using other toolchains should work fine.
#		This makefile supports the .cpp file-extension (not .cc), compiling to assembly, Libraries and Testing. 

#	Set Up:
#		- Specify your compiler and Optimization, Version & Warning flags under 'Compiler & Linker'. 	
#		- Add all subdirectories in src-folder to the subDirs variable under 'Subdirectories (in src)'.
#		- Create a variable for each of the subdirectories containing the names of all .cpp files in that directory with
#		  relative path from the src folder and file extension (.cpp).
#		- Specify desired name of program to build, its dependencies and its libraries in appropriate variables under 
#		  'Program Specifics'.

#	Useful Targets:
#		- make (default): Builds specified program 
#		- make asm: Compiles the project to Assembly
#		- make init: Creates necessary directories and user-specified subdirectories in project folder
#		- make test: Builds specified testing program 		 

#	==========================================================================================================================
#	Config Settings ==========================================================================================================
#	==========================================================================================================================

#	Compiler & Flags =========================================================================================================

CXX := g++

CXX_OPTIMIZATION := O2
CXX_VERSION := c++17
CXX_WARNINGS := Wall Wextra pedantic-errors Wold-style-cast

CXXFLAGS := $(addprefix -, std=$(CXX_VERSION) $(CXX_OPTIMIZATION) $(CXX_WARNINGS))

#	Main Directories =========================================================================================================

dASM  := asm
dBIN  := bin
dLIB  := libs
dSRC  := src
dTEST := testing

#	Subdirectories (in src) ==================================================================================================

subDirs := entity graphics graphics/fonts window

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

#	Program Specifics ========================================================================================================

Program_Name := Pong
Program_Deps := pong.cpp $(entity) $(graphics) $(graphics/fonts) $(window)
Program_Libs := gdi32

#	Testing Specifics ========================================================================================================

Test_Name := $(Program_Name)_Test
Test_Deps := $(entity) $(graphics) $(graphics/fonts) $(window)
Test_Libs := gdi32

Test_Files := testMain.cpp

#	==========================================================================================================================
#	Makefile Magic ===========================================================================================================
#	==========================================================================================================================

#	Declaring .PHONY (non-file) ==============================================================================================

.PHONY: asm asm_init default done init log settings test

#	Default (Executable) Build ===============================================================================================

default: init settings log $(Program_Name).exe done

# Building Program Routine
$(Program_Name).exe: $(addprefix $(dBIN)/, $(Program_Deps:.cpp=.o))
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(addprefix -l, $(Program_Libs))
	@echo "- Created $@"

# Compile a .cpp file in dSRC to a .o file in dBIN
$(addprefix $(dBIN)/, %.o): $(addprefix $(dSRC)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -o $@ -c $^
	@echo "- Created $@"

#	Testing Build ============================================================================================================

test: init settings log $(Test_Name).exe done

# Building Test Routine
$(Test_Name).exe: $(addprefix $(dTEST)/$(dBIN)/, $(Test_Files:.cpp=.o)) $(addprefix $(dBIN)/, $(Test_Deps:.cpp=.o))
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(addprefix -l, $(Test_Libs))
	@echo "- Created $@"

# Compile a .cpp file in dTEST to a .o file in dTEST/dBIN
$(addprefix $(dTEST)/$(dBIN)/, %.o): $(addprefix $(dTEST)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -o $@ -c $^
	@echo "- Created $@"

#	Assembly Build ===========================================================================================================

asm: asm_init settings log $(addsuffix .s,$(Programs)) done

# Building Assembly Routine
$(Program_Name).s: $(addprefix $(dASM)/, $(Program_Deps:.cpp=.s))

# Compile a .cpp file in dSRC to a .s file in dASM
$(addprefix $(dASM)/, %.s): $(addprefix $(dSRC)/, %.cpp)
	@$(CXX) $(CXXFLAGS) -S -o $@ $^
	@echo "- Created $@"

#	Extra Targets ============================================================================================================

# Display Settings (Flags ect)
settings:
	@echo "==Settings=="
	@echo "- Version: $(CXX) $(CXX_VERS)"
	@echo "- Optimization: $(CXX_OPTI)"
	@echo "- Warnings: $(CXX_WARN)"

# Initializing Project Folder Routine
init:
	@echo "==Initializing=="
	$(call createDirectory,$(dBIN))
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dBIN)/,$(p))))

	$(call createDirectory,$(dLIB))

	$(call createDirectory,$(dSRC))
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dSRC)/,$(p))))

	$(call createDirectory,$(dTEST)) 
	$(call createDirectory,$(dTEST)/$(dBIN)) 

# Initializing Assembly Folder Routine
asm_init:
	@echo "==Initializing Assembly=="
	$(call createDirectory,$(dASM)) 
	$(foreach p, $(subDirs), $(call createDirectory,$(addprefix $(dASM)/,$(p))))
	@echo "- Done"

# Log Message
log: 
	@echo "==Log=="

# Done Message
done:
	@echo "==Done=="

#	Function =================================================================================================================

# Initialize Directory (Empty Line IS Necessary)
define createDirectory
	@if [ ! -d "$(1)" ]; \
		then echo "- Created $(1)"; \
		mkdir $(1); \
	fi

endef