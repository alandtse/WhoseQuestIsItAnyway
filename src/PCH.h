#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <SimpleIni.h>
#include <xbyak/xbyak.h>

#pragma warning(push)
#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;
namespace logger = SKSE::log;
namespace string = SKSE::stl::string;

namespace stl
{
	using SKSE::stl::adjust_pointer;
	using SKSE::stl::is_in;
	using SKSE::stl::to_underlying;

	void asm_replace(std::uintptr_t a_from, std::size_t a_size, std::uintptr_t a_to);

	template <class T>
	void asm_replace(std::uintptr_t a_from)
	{
		asm_replace(a_from, T::size, reinterpret_cast<std::uintptr_t>(T::func));
	}

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[0] };
		T::func = vtbl.write_vfunc(T::size, T::thunk);
	}
}
#define DLLEXPORT __declspec(dllexport)

#include "Version.h"
