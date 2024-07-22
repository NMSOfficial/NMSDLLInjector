#define DLLINJEKTOR_EXPORTS
#include "DllInjector.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

HANDLE IslemTutamaciniAl(const char* islemAdi) {
    HANDLE islemAnlikGoruntu;
    PROCESSENTRY32 islemGirdisi;
    islemAnlikGoruntu = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (islemAnlikGoruntu == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    islemGirdisi.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(islemAnlikGoruntu, &islemGirdisi)) {
        CloseHandle(islemAnlikGoruntu);
        return NULL;
    }
    do {
        if (strcmp(islemGirdisi.szExeFile, islemAdi) == 0) {
            HANDLE islemTutamaci = OpenProcess(PROCESS_ALL_ACCESS, FALSE, islemGirdisi.th32ProcessID);
            CloseHandle(islemAnlikGoruntu);
            return islemTutamaci;
        }
    } while (Process32Next(islemAnlikGoruntu, &islemGirdisi));
    CloseHandle(islemAnlikGoruntu);
    return NULL;
}

DLLINJEKTOR_API bool DllEnjekteEt(const char* hedefIslem, const char* dllYolu) {
    HANDLE islemTutamaci = IslemTutamaciniAl(hedefIslem);
    if (islemTutamaci == NULL) {
        std::cerr << "İşlem bulunamadı." << std::endl;
        return false;
    }

    LPVOID dllYoluBellek = VirtualAllocEx(islemTutamaci, 0, strlen(dllYolu) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (dllYoluBellek == NULL) {
        std::cerr << "Hedef işlemde bellek ayırma başarısız." << std::endl;
        CloseHandle(islemTutamaci);
        return false;
    }

    BOOL yazildi = WriteProcessMemory(islemTutamaci, dllYoluBellek, (LPVOID)dllYolu, strlen(dllYolu) + 1, 0);
    if (!yazildi) {
        std::cerr << "DLL yolunu hedef işlem belleğine yazma başarısız." << std::endl;
        VirtualFreeEx(islemTutamaci, dllYoluBellek, 0, MEM_RELEASE);
        CloseHandle(islemTutamaci);
        return false;
    }

    HANDLE isParcasi = CreateRemoteThread(islemTutamaci, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, dllYoluBellek, 0, 0);
    if (isParcasi == NULL) {
        std::cerr << "Hedef işlemde uzaktan iş parçacığı oluşturma başarısız." << std::endl;
        VirtualFreeEx(islemTutamaci, dllYoluBellek, 0, MEM_RELEASE);
        CloseHandle(islemTutamaci);
        return false;
    }

    WaitForSingleObject(isParcasi, INFINITE);
    VirtualFreeEx(islemTutamaci, dllYoluBellek, 0, MEM_RELEASE);
    CloseHandle(isParcasi);
    CloseHandle(islemTutamaci);

    return true;
}
