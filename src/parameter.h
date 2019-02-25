#ifndef PARAMETER_H
#define PARAMETER_H

#include "function-storage.h"
#include "simple-assert.h"

#define MEM_SAFE_PARAMETER

class Parameter {
protected:
	TypeVerify m_verify;
	FunctionStorage::Pointer m_value;
#ifdef MEM_SAFE_PARAMETER
	FunctionStorage::Impls m_impls;
#endif

	template <typename _T>
	bool TypeVerifyCheck() const
	{
		ASSERT_WITH_MSG(m_verify == TypeVerify::Create<_T>(), " type conversion failed, parameter's type is [" << m_verify.GetName() << "], not [" << TypeVerify::Create<_T>().GetName() << "]");
		return m_verify == TypeVerify::Create<_T>();
	}

public:
	~Parameter();
	Parameter(const Parameter &o);

	template <typename _T>
	Parameter(const _T &v)
		: m_verify(v)
#ifdef MEM_SAFE_PARAMETER
		, m_impls(FunctionStorage::Impls::Create<_T>())
#endif
	{
#ifndef MEM_SAFE_PARAMETER
		FunctionStorage::Store<_T>();
		m_value = FunctionStorage::Copy((const FunctionStorage::Pointer)&v, m_verify);
#else
		m_value = m_impls.GetCopyImpl()((const FunctionStorage::Pointer)&v);
#endif
	}

	const char* GetName() const;

	//for converting Parameter to the ValueType
	template <typename _T>
	operator const _T&() const
	{
		TypeVerifyCheck<_T>();
		return *(_T*)m_value;
	}
	
	template <typename _T>
	operator _T&()
	{
		TypeVerifyCheck<_T>();
		return *(_T*)m_value;
	}
	
	template <typename _T>
	operator _T() const
	{
		TypeVerifyCheck<_T>();
		return *(_T*)m_value;
	}

	//change the value in Parameter directly by using operator =
	template <typename _T>
	Parameter& operator = (const _T &v)
	{
		TypeVerifyCheck<_T>();
#ifndef MEM_SAFE_PARAMETER
		FunctionStorage::Delete(m_value, m_verify);
		m_value = FunctionStorage::Copy((const FunctionStorage::Pointer)&v, m_verify);
#else
		m_impls.GetDeleteImpl()(m_value);
		m_value = m_impls.GetCopyImpl()((const FunctionStorage::Pointer)&v);
#endif
		return *this;
	} 
	
	Parameter& operator = (const Parameter &o);

	//Get TypeVerify of parameter
	TypeVerify GetTypeVerify() const;
	//compare the type to another parameter
	bool CompareType(const Parameter &o) const;
	//compare the type to another parameter
	template <typename _T>
	bool CompareType() const
	{
		return m_verify == TypeVerify::Create<_T>();
	}
	

};//Parameter


#endif //#ifndef PARAMETER_H
