#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include "parameter.h"
#include <map>
#include <string>
#include <ostream>
#include "simple-assert.h"

class ParameterList {
private:
	class Item {
	private:
		Parameter param;
		std::string key;
		std::string describe;
		
		Item(const Parameter &_param, const std::string _key, const std::string _describe);

		friend class ParameterList;
		friend std::ostream& operator << (std::ostream &os, const ParameterList::Item &item);
	};//class Item
	friend std::ostream& operator << (std::ostream &os, const ParameterList::Item &item); //this function need to access the private class Item

	typedef std::map<std::string, Item*> ListMap; //Item* for reducing overhead when using std::map and std::pair
	ListMap m_list; //the list

	//Get related parameter information as string when the key can not be found
	std::string ListRelated(const std::string key, unsigned int maxDistance = 3) const;

public:
	ParameterList();
	ParameterList(const ParameterList& o); //Copy Item* in this function
	~ParameterList(); //delete Item* in this function

	//Add parameter
	template <typename _T>
	ParameterList& Add(const std::string key, const _T& v, const std::string describe = "(no describe)")
	{
		bool result = m_list.insert(std::make_pair(key, new Item(Parameter::Create(v), key, describe))).second;
		if(!result)
		{
			ListMap::iterator find = m_list.find(key);
			if(find != m_list.end())
			{
				ASSERT_WITH_MSG(false, key << " is already exsit : " << *(find->second));
			}
			ASSERT_WITH_MSG(false, "unkown dump when Add (maybe memory is not enough)");
		}
		return *this;
	}

	//Set parameter by key
	template <typename _T>
	ParameterList& Set(const std::string key, const _T& v)
	{
		ListMap::iterator find = m_list.find(key);
		bool result = find != m_list.end();
		ASSERT_WITH_MSG(result, "can not find key : " << key << "\n" << ListRelated(key));
		find->second->param = v;
		return *this;
	}

	//Get parameter by key
	Parameter& Get(const std::string key);

	//Remove parameter by key
	ParameterList& Remove(const std::string key);

	//Get parameter information as string
	std::string List() const;

};//class ParameterList

std::ostream& operator << (std::ostream &os, const ParameterList::Item &item);

#endif //#ifndef PARAMETER_LIST_H
