#include "parameter.h"

Parameter::~Parameter()
{
	FunctionStorage::Delete(m_value, m_verify);
}

Parameter::Parameter(const Parameter &o)
	: m_value(FunctionStorage::Copy(o.m_value, o.m_verify)), m_verify(o.m_verify)
{ }

const char* Parameter::GetName() const
{
	return m_verify.GetName();
}

Parameter& Parameter::operator = (const Parameter &o)
{
	FunctionStorage::Delete(m_value, m_verify);
	m_value = FunctionStorage::Copy(o.m_value, o.m_verify);
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
