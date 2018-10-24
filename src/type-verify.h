#ifndef TYPE_VERIFY_H
#define TYPE_VERIFY_H

#include <typeinfo>

#if defined(__linux__)
	#include <cxxabi.h>
#else
	#define abi::__cxa_demangle(type, p2, p3, p4) (type)
#endif //#if defined(__linux__)

#if __cplusplus <= 199711L //C++11 is not supported
	#include <string>
	#define type_id_type  std::string
	#define type_id_get name
#else
	#define type_id_type  unsigned int
	#define type_id_get hash_code //feature in C++11
#endif //#if __cplusplus <= 199711L

/*
 * the information about a particular type
*/
class TypeVerify {
private:
	typedef type_id_type Verify;

	Verify m_verify;
	const char* m_name;
	
	template <typename _T>
	static Verify GetVerify()
	{
		return Verify(typeid(_T).type_id_get());
	}

	template <typename _T>
	static const char* GetName()
	{
		return abi::__cxa_demangle(typeid(_T).name(), NULL, NULL, NULL);
	}

	TypeVerify(Verify verify, const char* name);
	
public:
	template <class _T>
	static TypeVerify Create()
	{
		return TypeVerify(GetVerify<_T>(), GetName<_T>());
	}

	const char* GetName() const;

	bool operator == (const TypeVerify& o) const;
	bool operator != (const TypeVerify& o) const;
	bool operator < (const TypeVerify& o) const;
	bool operator <= (const TypeVerify& o) const;
	bool operator > (const TypeVerify& o) const;
	bool operator >= (const TypeVerify& o) const;
	
};//TypeVerify

#endif //#ifndef TYPE_VERIFY_H
