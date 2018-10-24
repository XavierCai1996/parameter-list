#include "parameter-list.h"
#include "edit-distance.h"
#include "simple-assert.h"

#include <sstream>

ParameterList::Item::Item(const Parameter &_param, const std::string _key, const std::string _describe)
	: param(_param), key(_key), describe(_describe)
{ }

std::ostream& operator << (std::ostream &os, const ParameterList::Item &item)
{
	os << item.key << "[" << item.param.GetName() << "] : " << item.describe;
	return os;
}



ParameterList::ParameterList()
{ }

ParameterList::ParameterList(const ParameterList& o)
{
	for(ListMap::const_iterator ite = o.m_list.begin(); ite != o.m_list.end(); ite++)
	{
		m_list.insert(std::make_pair(ite->first, new Item(*(ite->second))));
	}
}

ParameterList::~ParameterList()
{
	for(ListMap::iterator ite = m_list.begin(); ite != m_list.end(); ite++)
	{
		delete ite->second;
	}
}

ParameterList& ParameterList::Remove(const std::string key)
{
	ListMap::iterator find = m_list.find(key);
	bool result = find != m_list.end();
	ASSERT_WITH_MSG(result, "can not find key : " << key << "\n" << ListRelated(key));
	delete find->second;
	m_list.erase(find);
	return *this;
}

Parameter& ParameterList::Get(const std::string key)
{
	ListMap::iterator find = m_list.find(key);
	bool result = find != m_list.end();
	ASSERT_WITH_MSG(result, "can not find key : " << key << "\n" << ListRelated(key));
	return find->second->param;
}

std::string ParameterList::ListRelated(const std::string key, unsigned int maxDistance) const
{
	std::ostringstream oss;
	int len = key.length();
	bool first = true;
	for(ListMap::const_iterator ite = m_list.begin(); ite != m_list.end(); ite++)
	{
		if(abs(ite->first.length() - len) <= maxDistance)
		{
			unsigned int editDistance = EditDistance::Calculate(key, ite->first);
			if(editDistance <= maxDistance)
			{
				if(first)
				{
					first = false;
					oss << "related parameters (with max edit distance = " << maxDistance << ") : \n";
				}
				oss << " " << *(ite->second) << "\n";
			}
		}
	}
	return oss.str();
}

std::string ParameterList::List() const
{
	std::ostringstream oss;
	unsigned int index = 1;
	if(m_list.size() <= 0)
	{
		oss << "no parameters in list";
	}
	else
	{
		for(ListMap::const_iterator ite = m_list.begin(); ite != m_list.end(); ite++, index++)
		{
			oss << "list(" << index << ") " << *(ite->second) << "\n";
		}
	}
	return oss.str();
}
