#include "parameter.h"

Parameter::Parameter(const TypeVerify &verify, const FunctionStorage::Pointer value)
	: m_value(value), m_verify(verify)
{ }

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
