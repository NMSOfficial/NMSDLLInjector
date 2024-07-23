#include "NMSDLLInjector.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>

#include <cryptlib.h>
#include <sha.h>
#include <files.h>
#include <modes.h>
#include <aes.h>
#include <osrng.h>
#include <md5.h>

using namespace CryptoPP;

bool DosyaKopyala(const char* kaynak, const char* hedef) {
    std::ifstream src(kaynak, std::ios::binary);
    std::ofstream dest(hedef, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

bool DosyaTasiyici(const char* kaynak, const char* hedef) {
    return (std::rename(kaynak, hedef) == 0);
}

bool DosyaSil(const char* dosyaYolu) {
    return (std::remove(dosyaYolu) == 0);
}

bool DosyaOlustur(const char* dosyaYolu) {
    std::ofstream dosya(dosyaYolu);
    return dosya.is_open();
}

bool DosyaIzle(const char* klasorYolu) {
    return true;
}

bool AgIzle() {
    return true;
}

bool HashOlusturSHA1(const char* dosyaYolu, char* hash, std::size_t hashBoyutu) {
    SHA1 sha1;
    std::ifstream dosya(dosyaYolu, std::ios::binary);
    if (!dosya) return false;

    std::vector<byte> veri(4096);
    while (dosya.read(reinterpret_cast<char*>(veri.data()), veri.size())) {
        sha1.Update(veri.data(), dosya.gcount());
    }
    sha1.Final(reinterpret_cast<byte*>(hash));

    return true;
}

bool HashOlusturSHA256(const char* dosyaYolu, char* hash, std::size_t hashBoyutu) {
    SHA256 sha256;
    std::ifstream dosya(dosyaYolu, std::ios::binary);
    if (!dosya) return false;

    std::vector<byte> veri(4096);
    while (dosya.read(reinterpret_cast<char*>(veri.data()), veri.size())) {
        sha256.Update(veri.data(), dosya.gcount());
    }
    sha256.Final(reinterpret_cast<byte*>(hash));

    return true;
}

bool HashOlusturSHA512(const char* dosyaYolu, char* hash, std::size_t hashBoyutu) {
    SHA512 sha512;
    std::ifstream dosya(dosyaYolu, std::ios::binary);
    if (!dosya) return false;

    std::vector<byte> veri(4096);
    while (dosya.read(reinterpret_cast<char*>(veri.data()), veri.size())) {
        sha512.Update(veri.data(), dosya.gcount());
    }
    sha512.Final(reinterpret_cast<byte*>(hash));

    return true;
}

bool HashOlusturMD5(const char* dosyaYolu, char* hash, std::size_t hashBoyutu) {
    MD5 md5;
    std::ifstream dosya(dosyaYolu, std::ios::binary);
    if (!dosya) return false;

    std::vector<byte> veri(4096);
    while (dosya.read(reinterpret_cast<char*>(veri.data()), veri.size())) {
        md5.Update(veri.data(), dosya.gcount());
    }
    md5.Final(reinterpret_cast<byte*>(hash));

    return true;
}

bool SistemBilgisiGetir(char* bilgi, std::size_t bilgiBoyutu) {
    return true;
}

bool SistemKullanimiBilgisi(char* bilgi, std::size_t bilgiBoyutu) {
    return true;
}

bool ZamanBilgisiGetir(char* bilgi, std::size_t bilgiBoyutu) {
    std::time_t now = std::time(nullptr);
    std::strftime(bilgi, bilgiBoyutu, "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return true;
}

bool DosyaBoyutuGetir(const char* dosyaYolu, std::size_t* boyut) {
    std::ifstream dosya(dosyaYolu, std::ios::binary | std::ios::ate);
    if (!dosya) return false;

    *boyut = dosya.tellg();
    return true;
}

bool KlasorBoyutuGetir(const char* klasorYolu, std::size_t* boyut) {
    return true;
}

bool KriptografikDosyaSifrele(const char* dosyaYolu, const char* sifre, const char* hedefDosyaYolu) {
    AutoSeededRandomPool rng;
    byte anahtar[16];
    rng.GenerateBlock(anahtar, sizeof(anahtar));
    byte iv[AES::BLOCKSIZE];
    rng.GenerateBlock(iv, sizeof(iv));

    CBC_Mode<AES>::Encryption encryptor;
    encryptor.SetKeyWithIV(anahtar, sizeof(anahtar), iv);

    std::ifstream input(dosyaYolu, std::ios::binary);
    std::ofstream output(hedefDosyaYolu, std::ios::binary);
    if (!input || !output) return false;

    std::vector<byte> buffer(4096);
    while (input.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        std::vector<byte> encrypted(buffer.size());
        encryptor.ProcessData(encrypted.data(), buffer.data(), input.gcount());
        output.write(reinterpret_cast<char*>(encrypted.data()), input.gcount());
    }
    return true;
}

bool KriptografikDosyaCoz(const char* dosyaYolu, const char* sifre, const char* hedefDosyaYolu) {
    AutoSeededRandomPool rng;
    byte anahtar[16];
    rng.GenerateBlock(anahtar, sizeof(anahtar));
    byte iv[AES::BLOCKSIZE];
    rng.GenerateBlock(iv, sizeof(iv));

    CBC_Mode<AES>::Decryption decryptor;
    decryptor.SetKeyWithIV(anahtar, sizeof(anahtar), iv);

    std::ifstream input(dosyaYolu, std::ios::binary);
    std::ofstream output(hedefDosyaYolu, std::ios::binary);
    if (!input || !output) return false;

    std::vector<byte> buffer(4096);
    while (input.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        std::vector<byte> decrypted(buffer.size());
        decryptor.ProcessData(decrypted.data(), buffer.data(), input.gcount());
        output.write(reinterpret_cast<char*>(decrypted.data()), input.gcount());
    }
    return true;
}

bool RASTgeleSayiUret(int min, int max, int* sonuc) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    *sonuc = min + std::rand() % (max - min + 1);
    return true;
}
