#include "type-verify.h"

TypeVerify::TypeVerify(TypeVerify::Verify verify, const char* name)
	 : m_verify(verify), m_name(name)
{ }

const char* TypeVerify::GetName() const
{
	return m_name;
}

bool TypeVerify::operator == (const TypeVerify& o) const
{
	return m_verify == o.m_verify;
}

bool TypeVerify::operator != (const TypeVerify& o) const
{
	return m_verify != o.m_verify;
}

bool TypeVerify::operator < (const TypeVerify& o) const
{
	return m_verify < o.m_verify;
}

bool TypeVerify::operator <= (const TypeVerify& o) const
{
	return m_verify <= o.m_verify;
}

bool TypeVerify::operator > (const TypeVerify& o) const
{
	return m_verify > o.m_verify;
}

bool TypeVerify::operator >= (const TypeVerify& o) const
{
	return m_verify >= o.m_verify;
}
