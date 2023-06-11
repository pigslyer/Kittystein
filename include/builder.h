/*
* Compiler which compiles Water Arrow projects to C and allows running those.
* 
* 
* 
* Named after Hammerite God from Thief and Garrett's arrows respectively.
* */

#ifndef BUILDER_H
#define BUILDER_H

#include <delight.h>

#ifndef BUILDER_INTERNAL

#endif

typedef struct
{
	MessageType errorLevel;
	const char* const message;
} CompilerMessage;

bool builder_build(const char* const rootFolder, CompilerMessage* warningsErrors, uint maxLengh, uint* count);


#endif