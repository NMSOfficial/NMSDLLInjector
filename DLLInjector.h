#ifndef DLLINJEKTOR_H
#define DLLINJEKTOR_H

#ifdef DLLINJEKTOR_EXPORTS
#define DLLINJEKTOR_API __declspec(dllexport)
#else
#define DLLINJEKTOR_API __declspec(dllimport)
#endif

extern "C" DLLINJEKTOR_API bool DllEnjekteEt(const char* hedefIslem, const char* dllYolu);

#endif // DLLINJEKTOR_H
