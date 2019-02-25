#include "parameter.h"

Parameter::~Parameter()
{
#ifdef MEM_SAFE_PARAMETER
	m_impls.GetDeleteImpl()(m_value);
#else
	FunctionStorage::Delete(m_value, m_verify);
#endif
}

Parameter::Parameter(const Parameter &o)
	: m_verify(o.m_verify), m_value(
#ifndef MEM_SAFE_PARAMETER
	FunctionStorage::Copy(o.m_value, o.m_verify))
#else
	o.m_impls.GetCopyImpl()(o.m_value)),
	m_impls(o.m_impls)
#endif
{ }

const char* Parameter::GetName() const
{
	return m_verify.GetName();
}

Parameter& Parameter::operator = (const Parameter &o)
{
#ifndef MEM_SAFE_PARAMETER
	FunctionStorage::Delete(m_value, m_verify);
	m_value = FunctionStorage::Copy(o.m_value, o.m_verify);
#else
	m_impls.GetDeleteImpl()(m_value);
	m_value = o.m_impls.GetCopyImpl()(o.m_value);
	m_impls = o.m_impls;
#endif
	m_verify = o.m_verify;
	return *this;
}

TypeVerify Parameter::GetTypeVerify() const
{
	return m_verify;
}

bool Parameter::CompareType(const Parameter &o) const
{
	return m_verify == o.m_verify;
}
