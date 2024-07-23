#ifndef NMSDLLINJECTOR_H
#define NMSDLLINJECTOR_H

#include <cstddef>

#ifdef NMSDLLINJECTOR_EXPORTS
#define NMSDLLINJECTOR_API __declspec(dllexport)
#else
#define NMSDLLINJECTOR_API __declspec(dllimport)
#endif

extern "C" {
    NMSDLLINJECTOR_API bool DosyaKopyala(const char* kaynak, const char* hedef);
    NMSDLLINJECTOR_API bool DosyaTasiyici(const char* kaynak, const char* hedef);
    NMSDLLINJECTOR_API bool DosyaSil(const char* dosyaYolu);
    NMSDLLINJECTOR_API bool DosyaOlustur(const char* dosyaYolu);
    NMSDLLINJECTOR_API bool DosyaIzle(const char* klasorYolu);
    NMSDLLINJECTOR_API bool AgIzle();
    NMSDLLINJECTOR_API bool HashOlusturSHA1(const char* dosyaYolu, char* hash, std::size_t hashBoyutu);
    NMSDLLINJECTOR_API bool HashOlusturSHA256(const char* dosyaYolu, char* hash, std::size_t hashBoyutu);
    NMSDLLINJECTOR_API bool HashOlusturSHA512(const char* dosyaYolu, char* hash, std::size_t hashBoyutu);
    NMSDLLINJECTOR_API bool HashOlusturMD5(const char* dosyaYolu, char* hash, std::size_t hashBoyutu);
    NMSDLLINJECTOR_API bool SistemBilgisiGetir(char* bilgi, std::size_t bilgiBoyutu);
    NMSDLLINJECTOR_API bool SistemKullanimiBilgisi(char* bilgi, std::size_t bilgiBoyutu);
    NMSDLLINJECTOR_API bool ZamanBilgisiGetir(char* bilgi, std::size_t bilgiBoyutu);
    NMSDLLINJECTOR_API bool DosyaBoyutuGetir(const char* dosyaYolu, std::size_t* boyut);
    NMSDLLINJECTOR_API bool KlasorBoyutuGetir(const char* klasorYolu, std::size_t* boyut);
    NMSDLLINJECTOR_API bool KriptografikDosyaSifrele(const char* dosyaYolu, const char* sifre, const char* hedefDosyaYolu);
    NMSDLLINJECTOR_API bool KriptografikDosyaCoz(const char* dosyaYolu, const char* sifre, const char* hedefDosyaYolu);
    NMSDLLINJECTOR_API bool RASTgeleSayiUret(int min, int max, int* sonuc);
}

#endif // NMSDLLINJECTOR_H
