#pragma once
#include "../Hook.h"

struct TestStruct {
	int a;
	int b;
	int c;
	int d;
	int e;
	int f;
	int g;
	int h;
	int i;
	int j;
	int k;
	int l;
	int m;
};

class XLiveHooks : public Hook
{
	public:

		void Setup() override;

}; 

namespace Hooks {

	///int __stdcall XShowGamerCardUI_5252(int a1, int a2, int a3) // Hooked due to crash when pressing Y on the send crate menu
	using XShowGamerCardUI_5252Fn = int(__stdcall*)(int a1, int a2, int a3);
	inline XShowGamerCardUI_5252Fn XShowGamerCardUI_5252_Original = nullptr;
	int __stdcall XShowGamerCardUI_5252(int a1, int a2, int a3);

	///int XUserCreateStatsEnumeratorByRank_5284()
	using XUserCreateStatsEnumeratorByRank_5284Fn = int(*)();
	inline XUserCreateStatsEnumeratorByRank_5284Fn XUserCreateStatsEnumeratorByRank_5284_Original = nullptr;
	int XUserCreateStatsEnumeratorByRank_5284();

	///int XFriendsCreateEnumerator_5312()
	using XFriendsCreateEnumerator_5312Fn = int(*)();
	inline XFriendsCreateEnumerator_5312Fn XFriendsCreateEnumerator_5312_Original = nullptr;
	int XFriendsCreateEnumerator_5312();

	///int XUserCreateAchievementEnumerator_5280()
	using XUserCreateAchievementEnumerator_5280Fn = int(*)();
	inline XUserCreateAchievementEnumerator_5280Fn XUserCreateAchievementEnumerator_5280_Original = nullptr;
	int XUserCreateAchievementEnumerator_5280();

	///int __stdcall XLiveOnResetDevice_5007(int a1)
	using XLiveOnResetDevice_5007Fn = int(__stdcall*)(int a1);
	inline XLiveOnResetDevice_5007Fn XLiveOnResetDevice_5007_Original = nullptr;
	int __stdcall XLiveOnResetDevice_5007(int a1);

	///int XLiveRender_5002()
	using XLiveRender_5002Fn = int(*)();
	inline XLiveRender_5002Fn XLiveRender_5002_Original = nullptr;
	int XLiveRender_5002();

	///int XLiveInput_5001()
	using XLiveInput_5001Fn = int(*)();
	inline XLiveInput_5001Fn XLiveInput_5001_Original = nullptr;
	int XLiveInput_5001();

	///void __cdecl sub_8499E0(_DWORD *a1, int a2)
	using sub_8499E0Fn = void(__cdecl*)(uint32_t* a1, int a2);
	inline sub_8499E0Fn sub_8499E0_Original = nullptr;
	void __cdecl sub_8499E0(uint32_t* a1, int a2);

	// Note: The original function uses __thiscall (member function, 'this' in ECX).
	// MinHook hooks it using __fastcall, so the first parameter is 'this', the second is a dummy (usually nullptr).
	///int __thiscall sub_6B3490(void *this, int a2)
	using sub_6B3490Fn = int(__fastcall*)(void* _this, void* unused, int a2);
	inline sub_6B3490Fn sub_6B3490_Original = nullptr;
	int __fastcall sub_6B3490(void* _this, void* unused, int a2);
};
