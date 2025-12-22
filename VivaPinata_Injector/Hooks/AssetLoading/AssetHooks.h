#pragma once
#include "../Hook.h"

#define __PAIR64M__(h, l) ((static_cast<unsigned __int64>(h) << 32) | static_cast<unsigned __int64>(l))
#define HIDWORDM(_qw)   ((unsigned int)(((_qw) >> 32) & 0xFFFFFFFF))

class AssetHooks : public Hook
{
public:

	void Setup() override;

};

struct assetEntry {
	char BundlePath[260];
};

namespace Hooks {

	inline unsigned int HashManual(std::string ChunkName)
	{
		unsigned __int8 v2; // al
		unsigned int v3; // esi
		int v4; // ecx
		unsigned __int8* i; // edi
		unsigned __int64 v6; // kr00_8
		unsigned int v7; // eax

		if (ChunkName.length() < 5) return 0;

		v2 = ChunkName[4];
		v3 = 0;
		v4 = 0;
		for (i = (unsigned __int8*)(ChunkName.c_str() + 5); v2; ++i)
		{
			v6 = 16 * __PAIR64M__(v4, v3) + (v2 & 0xDF);
			v4 = HIDWORDM(v6);
			v3 = v6;
			v7 = HIDWORDM(v6) & 0xF0000000;
			if ((HIDWORDM(v6) & 0xF0000000) != 0)
			{
				v3 = (__PAIR64M__(v7, 0) >> 24) ^ v6;
				v4 = (v7 | HIBYTE(v7)) ^ HIDWORDM(v6);
			}
			v2 = *i;
		}
		return v3;
	};

	//int __cdecl FindAssetVDATptr_8BD640(char *FileName, int **a2, _DWORD *a3, int a4, char *a5, int a6)
	using FindAssetVDATptrFn_008BD640 = int(__cdecl*)(char* FileName, int** a2, DWORD* a3, int a4, char* a5, int a6) noexcept;
	inline FindAssetVDATptrFn_008BD640 FindAssetVDATptr_008BD640_Original = nullptr;
	int __cdecl FindAssetVDATptr_008BD640(char* FileName, int** a2, DWORD* a3, int a4, char* a5, int a6) noexcept;

	//int __cdecl FindAssetWithTag_008BC290(char *Source)
	using FindAssetWithTagFn_008BC290 = int(__cdecl*)(char* Source) noexcept;
	inline FindAssetWithTagFn_008BC290 FindAssetWithTag_008BC290_Original = nullptr;
	int __cdecl FindAssetWithTag_008BC290(char* Source) noexcept;

	//int __cdecl sub_8B8750(int a1, _DWORD *a2, int a3)
	using sub_8B8750Fn = int(__cdecl*)(int a1, DWORD* a2, int a3) noexcept;
	inline sub_8B8750Fn sub_8B8750_Original = nullptr;
	int __cdecl sub_8B8750(int a1, DWORD* a2, int a3) noexcept;

	//unsigned int __cdecl FindPackageTableEntry(int tableInfo,int first,int last,int(__cdecl* CompareFn)(unsigned int, int),int pkgNumber)
	using FindPackageTableEntryFn = unsigned int(__cdecl*)(int tableInfo, int first, int last, int(__cdecl* CompareFn)(unsigned int, int), int pkgNumber) noexcept;
	inline FindPackageTableEntryFn FindPackageTableEntry_Original = nullptr;
	unsigned int __cdecl FindPackageTableEntry_0086C760(int tableInfo, int first, int last, int(__cdecl* CompareFn)(unsigned int, int), int pkgNumber) noexcept;

	//int __cdecl BeginPackageLoad_8B8170(int PKG_Number, int a2, int a3, int a4)
	using BeginPackageLoadFn_008B8170 = int(__cdecl*)(int PKG_Number, int a2, int a3, int a4);
	inline BeginPackageLoadFn_008B8170 BeginPackageLoad_008B8170_Original = nullptr;
	int __cdecl BeginPackageLoad_008B8170(int PKG_Number, int a2, int a3, int a4);

	//int __cdecl Hash_8BC9E0(int String)
	using HashFn_008BC9E0 = int(__cdecl*)(int String) noexcept;
	inline HashFn_008BC9E0 Hash_8BC9E0_Original = nullptr;
	int __cdecl Hash_8BC9E0(int String) noexcept;

	//DWORD* __cdecl PackageLoad_8B6D00(int bundleMgr, DWORD* ctx, int entry)
	using PackageLoadFn_008B6D00 = DWORD * (__cdecl*)(int bundleMgr, DWORD* ctx, int entry) noexcept;
	inline PackageLoadFn_008B6D00 PackageLoad_008B6D00_Original = nullptr;
	DWORD* __cdecl PackageLoad_8B6D00(int bundleMgr, DWORD* ctx, int entry) noexcept;

	//int __cdecl sub_93AE80(_DWORD *a1, _DWORD *a2) (Checks the requirements so we can make sure its reading the correct ones that are custom)
	using sub_93AE80Fn = int(__cdecl*)(DWORD* a1, DWORD* a2) noexcept;
	inline sub_93AE80Fn sub_93AE80_Original = nullptr;
	int __cdecl sub_93AE80(DWORD* a1, DWORD* a2) noexcept;

};

