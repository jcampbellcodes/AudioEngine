#pragma once
#ifndef HASHTHIS_H
#define HASHTHIS_H

#include <xaudio2.h>

unsigned int HashThis(const char* input);
unsigned int HashThis(WAVEFORMATEXTENSIBLE& wfx);

#endif