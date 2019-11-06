OUTPUT = zork
SOURCES = main.cpp
COMPILER_FLAGS =
LINKER_FLAGS =

default:
	msbuild /nologo /verbosity:m Zork.sln

rundebug:
	devenv /nologo /debugexe x64\Debug\Zork.exe

