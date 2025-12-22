#pragma once
#include "../Hook.h"
#include <wincrypt.h>

class SaveHooks : public Hook
{
	public:

		void Setup() override;

}; 

namespace Hooks {

	//_DWORD *__cdecl CreateSaveAndInitZlib_7F0F80(_DWORD *a1, unsigned int a2)
	using CreateSaveAndInitZlib_7F0F80Fn = DWORD * (__cdecl*)(DWORD* a1, unsigned int zlibCompressionLevelOrFlags);
	inline CreateSaveAndInitZlib_7F0F80Fn CreateSaveAndInitZlib_7F0F80_Original = nullptr;
	DWORD* __cdecl CreateSaveAndInitZlib_7F0F80(DWORD* a1, unsigned int zlibCompressionLevelOrFlags);

	// Note: The original function uses __thiscall (member function, 'this' in ECX).
	// MinHook hooks it using __fastcall, so the first parameter is 'this', the second is a dummy (usually nullptr).
	//HCRYPTPROV __thiscall CreateAES128KeyFromSHA1Hash_686300(HCRYPTKEY* outAesKeyAndLen,const BYTE* hashHandleAlias,DWORD inputDataSize)
	using CreateAES128KeyFromSHA1Hash_686300Fn = HCRYPTPROV(__fastcall*)(void* thisptr, void* /*unused*/, HCRYPTKEY* outAesKeyAndLen, const BYTE* hashHandleAlias, DWORD inputDataSize);
	inline CreateAES128KeyFromSHA1Hash_686300Fn CreateAES128KeyFromSHA1Hash_686300_Original = nullptr;
	HCRYPTPROV __fastcall CreateAES128KeyFromSHA1Hash_686300(void* thisptr, void* /*unused*/, HCRYPTKEY* outAesKeyAndLen, const BYTE* hashHandleAlias, DWORD inputDataSize);

	//int __cdecl GenerateHash_685A00(int SaveFile, char *HashBuffer)
	using GenerateHash_685A00Fn = int(__cdecl*)(int SaveFile, char* HashBuffer);
	inline GenerateHash_685A00Fn GenerateHash_685A00_Original = nullptr;
	int __cdecl GenerateHash_685A00(int SaveFile, char* HashBuffer);

	// Member wrapper: ECX=this, then stack params: BYTE* Data, DWORD* DataLengthPtr
	using CryptDecrypt_686550Fn = BOOL(__fastcall*)(void* thisptr, void* /*unused*/, BYTE* Data, DWORD* DataLengthPtr);
	inline CryptDecrypt_686550Fn CryptDecrypt_686550_Original = nullptr;
	BOOL __fastcall CryptDecrypt_686550(void* thisptr, void* /*unused*/, BYTE* Data, DWORD* DataLengthPtr);

};