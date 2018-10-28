#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include "parameter.h"
#include <map>
#include <string>
#include <ostream>
#include "simple-assert.h"

#include <vector>
//#include <stdarg.h>

class ParameterList {
private:
	template <typename _T>
	class ItemTemplate {
	public:
		_T param;
		std::string key;
		std::string description;
		
		ItemTemplate<_T>(const _T &_param, const std::string _key, const std::string _description)
			: param(_param), key(_key), description(_description)
		{ }

		virtual std::string Print();
	};//class ItemTemplate
	
	typedef ItemTemplate<Parameter> Item;
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
	ParameterList& Add(const std::string key, const _T& v, const std::string description = "(no description)")
	{
		bool result = m_list.insert(std::make_pair(key, new Item(Parameter(v), key, description))).second;
		if(!result)
		{
			ListMap::iterator find = m_list.find(key);
			if(find != m_list.end())
			{
				ASSERT_WITH_MSG(false, key << " is already exsit : " << find->second->Print());
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
	//const version
	Parameter GetConst(const std::string key) const;

	//Remove parameter by key
	ParameterList& Remove(const std::string key);

	//Inquire if the key is in the list
	bool Have(const std::string key) const;

	//Get parameter information as string
	std::string List() const;


	//code block below is to perform Merge
	//Merge is usually used in passing ParameterList as fucntion arguments
private:
	typedef ItemTemplate<TypeVerify> RequireItem;
	bool m_isRequireHelp; //indicate if is requiring help when using Merge, true means print the parameters that are required

public:
	class RequireList {
	private:
		std::vector<RequireItem> m_requires;
	public:
		template <typename _T>
		RequireList& Add(const std::string key, const std::string description = "(no description)")
		{
			m_requires.push_back(RequireItem(TypeVerify::Create<_T>(), key, description));
			return *this;
		}

		RequireItem Get(unsigned int i) const;
		std::size_t Size() const;
	};//class RequireList
	
	//Merge another list to this list
	//  rule 1. add new key to this list
	//  rule 2. replace the value for same key with type checking, and the description will not be changed
	ParameterList& Merge(const ParameterList& o, const RequireList& requires = RequireList());

	//pass this list as argument to functions which is coding in framework, then the parameters that are required will be printed
	const static ParameterList& Help();
	
};//class ParameterList

#endif //#ifndef PARAMETER_LIST_H
