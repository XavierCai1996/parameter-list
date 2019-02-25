#include "type-verify.h"
#include <string.h>
#include <stdlib.h>

TypeVerify::TypeVerify(TypeVerify::Verify verify, char* name)
	 : m_verify(verify), m_name(name)
{ }

const char* TypeVerify::GetName() const
{
	return m_name;
}

TypeVerify::TypeVerify(const TypeVerify& o)
	: m_name(NULL)
{
	*this = o;
}

TypeVerify& TypeVerify::operator = (const TypeVerify& o)
{
	if(m_name != NULL)
	{
		free(m_name);
		m_name = NULL;
	}
	m_verify = o.m_verify;
	if(o.m_name != NULL)
	{
		unsigned int size = strlen(o.m_name);
		m_name = (char*)malloc(size + 1);
		strncpy(m_name, o.m_name, size);
		m_name[size] = '\0';
	}
	return *this;
}

TypeVerify::~TypeVerify()
{
	free(m_name);
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
