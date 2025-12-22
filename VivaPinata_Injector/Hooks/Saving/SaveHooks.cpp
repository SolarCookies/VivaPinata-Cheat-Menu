#pragma once
#include "SaveHooks.h"

void SaveHooks::Setup() {
	std::cout << "Setting up Save hooks..." << std::endl;

	// Hook the various print functions.

	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x007F0F80),
			&Hooks::CreateSaveAndInitZlib_7F0F80,
			reinterpret_cast<void**>(&Hooks::CreateSaveAndInitZlib_7F0F80_Original)
		)) throw std::runtime_error("Unable to hook CreateSaveAndInitZlib_7F0F80()");
	}

	//Enable and Disable by setting the if(0) to if(1)
	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x00686300),
			&Hooks::CreateAES128KeyFromSHA1Hash_686300,
			reinterpret_cast<void**>(&Hooks::CreateAES128KeyFromSHA1Hash_686300_Original)
		)) throw std::runtime_error("Unable to hook CreateAES128KeyFromSHA1Hash_686300()");
	}

	if(0){
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x00685A00),
			&Hooks::GenerateHash_685A00,
			reinterpret_cast<void**>(&Hooks::GenerateHash_685A00_Original)
		)) throw std::runtime_error("Unable to hook GenerateHash_685A00()");
	}

	if (0) {
		if (MH_CreateHook(
			reinterpret_cast<void*>(0x00686550),
			&Hooks::CryptDecrypt_686550,
			reinterpret_cast<void**>(&Hooks::CryptDecrypt_686550_Original)
		)) throw std::runtime_error("Unable to hook CryptDecrypt_686550()");
	}

	


}

DWORD* __cdecl Hooks::CreateSaveAndInitZlib_7F0F80(DWORD* a1, unsigned int zlibCompressionLevelOrFlags)
{
	// Set zlibCompressionLevelOrFlags to not compress at all aka Z_NO_COMPRESSION
	zlibCompressionLevelOrFlags = 0;
	return Hooks::CreateSaveAndInitZlib_7F0F80_Original(a1, zlibCompressionLevelOrFlags);
}

HCRYPTPROV __fastcall Hooks::CreateAES128KeyFromSHA1Hash_686300(void* thisptr, void* /*unused*/, HCRYPTKEY* outAesKeyAndLen, const BYTE* hashHandleAlias, DWORD inputDataSize)
{
	return Hooks::CreateAES128KeyFromSHA1Hash_686300_Original(thisptr, thisptr, outAesKeyAndLen, hashHandleAlias, inputDataSize);

}

int __cdecl Hooks::GenerateHash_685A00(int SaveFile, char* HashBuffer)
{
	int result = Hooks::GenerateHash_685A00_Original(SaveFile, HashBuffer);

	std::cout << "---------GenerateHash_685A00 Called!---------" << std::endl;

	if (HashBuffer == nullptr) {
		std::cout << "HashBuffer is null" << std::endl;
		return result;
	}

	constexpr size_t HASH_LEN = 40; // known length
	std::cout << "HashBuffer (hex): ";
	for (size_t i = 0; i < HASH_LEN; ++i) {
		unsigned char b = static_cast<unsigned char>(HashBuffer[i]);
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)b;
		if (i + 1 < HASH_LEN) std::cout << ' ';
	}
	std::cout << std::dec << std::setfill(' ') << std::endl;

	// Printable ASCII representation (non-printable => '.')
	std::cout << "HashBuffer (ascii-printable): ";
	for (size_t i = 0; i < HASH_LEN; ++i) {
		unsigned char c = static_cast<unsigned char>(HashBuffer[i]);
		std::cout << (std::isprint(c) ? static_cast<char>(c) : '.');
	}
	std::cout << std::endl;

	std::cout << "---------End GenerateHash_685A00 Info---------" << std::endl;

	return result; //Always returns 1
}

BOOL __fastcall Hooks::CryptDecrypt_686550(void* thisptr, void* /*unused*/, BYTE* Data, DWORD* DataLengthPtr)
{
	std::cout << "---------CryptDecrypt_686550 Called (member wrapper)---------" << std::endl;
	std::cout << "thisptr: " << thisptr << std::endl;
	std::cout << "Data: " << static_cast<void*>(Data) << std::endl;
	std::cout << "DataLengthPtr: " << *DataLengthPtr << std::endl;
	std::cout << "---------End CryptDecrypt_686550 Info---------" << std::endl;

	// Forward to MinHook trampoline for a member function (pass thisptr twice).
	bool returnvalue = Hooks::CryptDecrypt_686550_Original(thisptr, thisptr, Data, DataLengthPtr);

	//Dump decrypted data into .bin file for analysis
	{
		static int dumpIndex = 0;
		std::ofstream outFile;
		outFile.open("plugins/Dumps/CryptDecrypt_686550_DecryptedData_" + std::to_string(dumpIndex) + ".bin", std::ios::binary);
		if (outFile.is_open()) {
			outFile.write(reinterpret_cast<const char*>(Data), *DataLengthPtr);
			outFile.close();
			std::cout << "Decrypted data dumped to CryptDecrypt_686550_DecryptedData_" << dumpIndex << ".bin" << std::endl;
			++dumpIndex;
		}
		else {
			std::cerr << "Failed to open file for dumping decrypted data." << std::endl;
		}
	}

	return returnvalue;
}


