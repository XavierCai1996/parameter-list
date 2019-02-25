#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include "parameter.h"
#include <map>
#include <string>
#include "simple-assert.h"
#include <vector>

class ParameterList {
private:
	template <typename _T>
	class ItemTemplate {
	public:
		virtual ~ItemTemplate() { }
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
	ParameterList& operator = (const ParameterList& o);
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
	const Parameter& Get(const std::string key) const;

	//Remove parameter by key
	ParameterList& Remove(const std::string key);

	//Inquire if the key is in the list
	bool Have(const std::string key) const;

	//Get parameter information as string
	std::string List() const;
	
	//Clear all parameters in list
	void Clear();


	//code block below is to perform Merge
	//Merge is usually used in passing ParameterList as fucntion arguments
private:
	typedef ItemTemplate<TypeVerify> RequireItem;
	bool m_isRequireHelp; //indicate if is requiring help when using Merge, true means print the parameters that are required

public:
	//for function Merge
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

	//for funciotn Merge
	class MergeMode {
	private:
		char m_mode;
		bool IsOne(unsigned int index) const;
		static char GetOne(unsigned int index);
		bool IsReplace() const;
		bool IsGreedy() const;
		friend class ParameterList;
		
	public:
		MergeMode(char mode);
		static char replace, retain; //a mode set
		static char greedy, conservative; // a mode set
	};//class MergeMode
	
	/*
	 *  Merge another list to this list
	 *    Rule of mode[greedy/conservative](default to greedy) :
	 *      if a specific key which appeared in another list but not this list
	 *      add/ignore it, in addition, if the key is included in the require list, add it;
	 *    Rule of mode[replace/retain](default to replace) :
	 *  	if a specific key which appeared both in two lists
	 *      [replace:]replace the value for the same key with type checking, and the description will not be changed
	 *      [retain:]ignore it if it not included in the require list, otherwise, replace it;
	*/
	ParameterList& Merge(const ParameterList& o, const RequireList& requires, const MergeMode& mode = MergeMode::replace|MergeMode::greedy);
	ParameterList& Merge(const ParameterList& o, const MergeMode& mode = MergeMode::replace|MergeMode::greedy);

	//pass this list as argument to functions which is coding in framework, then the parameters that are required will be printed
	static const ParameterList& Help();
	
};//class ParameterList

#endif //#ifndef PARAMETER_LIST_H
