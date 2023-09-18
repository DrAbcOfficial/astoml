#include <extdll.h>

#include "angelscript.h"
#include "asext_api.h"
#include "angelscriptlib.h"

#include <meta_api.h>

angelhook_t g_AngelHook;


template <typename T>
void RegisteRefObject(CASDocumentation* pASDoc, const char* szName) {
	asSFuncPtr reg;
	reg = asMETHOD(T, AddRef);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "AddRef", szName, asBEHAVE_ADDREF, "void AddRef()", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, Release);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Release", szName, asBEHAVE_RELEASE, "void Release()", &reg, asCALL_THISCALL);
}
template <typename T>
void RegisteGCObject(CASDocumentation* pASDoc, const char* szName) {
	RegisteRefObject<T>(pASDoc, szName);
	asSFuncPtr reg;
	reg = asMETHOD(T, SetGCFlag);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Set GC Flag", szName, asBEHAVE_SETGCFLAG, "void SetGCFlag()", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, GetGCFlag);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Get GC Flag", szName, asBEHAVE_GETGCFLAG, "bool GetGCFlag() const", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, GetRefCount);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Get reference count", szName, asBEHAVE_GETREFCOUNT, "int GetRefCount() const", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, EnumReferences);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Enumerate references held by this class", szName, asBEHAVE_ENUMREFS, "void EnumReferences(int& in)", &reg, asCALL_THISCALL);
	reg = asMETHOD(T, ReleaseReferences);
	ASEXT_RegisterObjectBehaviourEx(pASDoc, "Release all references held by this class", szName, asBEHAVE_RELEASEREFS, "void ReleaseReferences(int& in)", &reg, asCALL_THISCALL);
}
/// <summary>
/// Regiter
/// </summary>
#define REGISTE_OBJMETHODEX(r, d, e, c, m, cc, mm, call) r=asMETHOD(cc,mm);ASEXT_RegisterObjectMethodEx(d,e,c,m,&r,call)
#define REGISTE_OBJMETHODPREX(r, d, e, c, m, cc, mm, pp, rr, call) r=asMETHODPR(cc,mm, pp, rr);ASEXT_RegisterObjectMethodEx(d,e,c,m,&r,call)
void RegisterAngelScriptMethods(){

	ASEXT_RegisterDocInitCallback([](CASDocumentation* pASDoc) {
		ASEXT_RegisterFuncDef(pASDoc, "Cron Callback", "void fnCronCallback(string&in name, string&in time, any@ param)");
		ASEXT_RegisterFuncDef(pASDoc, "Cron Callback", "void fnCronNoParamCallback(string&in name, string&in time)");
		asSFuncPtr reg;
		ASEXT_RegisterObjectType(pASDoc, "Global Cron Item", "CCron", 0, asOBJ_REF | asOBJ_NOCOUNT);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Add Scheduler", "CCron", "void Add(string&in name, string&in time, fnCronCallback@ callback, any@ param)", CASCron, Add, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Add Scheduler", "CCron", "void Add(string&in name, string&in time, fnCronNoParamCallback@ callback)", CASCron, AddWithoutParam, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Remove Scheduler", "CCron", "void Remove(string&in name)", CASCron, Remove, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Remove all Scheduler", "CCron", "void RemoveAll()", CASCron, RemoveAll, asCALL_THISCALL);
		REGISTE_OBJMETHODEX(reg, pASDoc, "Count Scheduler", "CCron", "uint Count()", CASCron, Count, asCALL_THISCALL);	
		ASEXT_RegisterGlobalProperty(pASDoc, "Global cron item", "CCron g_Cron", &g_pASCron);
	});
}
#undef REGISTE_OBJMETHODEX
#undef REGISTE_OBJMETHODPREX

#define CREATE_AS_HOOK(item, des, tag, name, arg) g_AngelHook.item=ASEXT_RegisterHook(des,StopMode_CALL_ALL,2,ASFlag_MapScript|ASFlag_Plugin,tag,name,arg)
void RegisterAngelScriptHooks(){
	
}
#undef CREATE_AS_HOOK