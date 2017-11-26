#pragma once

#ifdef cplusplus
#define TEXPORT extern "c" _declspec(dllexport)
#else
#define TEXPORT _declspec(dllexport)
#endif