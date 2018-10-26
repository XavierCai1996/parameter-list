#ifndef PARAMETER_H
#define PARAMETER_H

#include "function-storage.h"
#include "simple-assert.h"

class Parameter {
private:
	TypeVerify m_verify;
	FunctionStorage::Pointer m_value;

	Parameter(const TypeVerify &verify, const FunctionStorage::Pointer value);

	template <typename _T>
	bool TypeVerifyCheck() const
	{
		ASSERT_WITH_MSG(m_verify == TypeVerify::Create<_T>(), " type conversion failed, parameter's type is [" << m_verify.GetName() << "]");
		return m_verify == TypeVerify::Create<_T>();
	}

public:
	~Parameter();
	Parameter(const Parameter &o);

	//you dont need to call this function
	/*
	template <typename _T>
	_T GetValue() const
	{
		TypeVerifyCheck<_T>();
		return *(_T*)m_value;
	}
	*/

	const char* GetName() const;

	//Create a parameter
	template <typename _T>
	static Parameter Create(const _T &v)
	{
		FunctionStorage::Store<_T>();
		TypeVerify tv = TypeVerify::Create<_T>();
		return Parameter(tv, FunctionStorage::Copy((const FunctionStorage::Pointer)&v, tv));
	}

	//for converting Parameter to the ValueType
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
		FunctionStorage::Delete(m_value, m_verify);
		m_value = FunctionStorage::Copy((const FunctionStorage::Pointer)&v, m_verify);
		return *this;
	} 
	
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
