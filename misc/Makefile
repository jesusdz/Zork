OUTPUT = zork
SOURCES = main.cpp
COMPILER_FLAGS =
LINKER_FLAGS =

ROOT_DIR = ..\\

default:
	msbuild /nologo /verbosity:m $(ROOT_DIR)Zork.sln

rundebug:
	devenv /nologo /debugexe $(ROOT_DIR)x64\Debug\Zork.exe

clean:
	if exist $(ROOT_DIR).vs\ rmdir $(ROOT_DIR).vs /s /q
	if exist $(ROOT_DIR)x64\ rmdir $(ROOT_DIR)x64 /s /q
	if exist $(ROOT_DIR)x86\ rmdir $(ROOT_DIR)x86 /s /q

