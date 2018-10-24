#ifndef FUNCTION_STORAGE_H
#define FUNCTION_STORAGE_H

#include "type-verify.h"
#include <map>

/*
 * store the Copy Function and Delete Function for each type
 *   every object's pointer can be convert to the type Pointer(aka void*)
 *   but you don't know how to copy or free them
 *   in this time, you can use the functions stored in this FunctionStorage
 *   functions are stored by the TypeVerify
 *   so, you need to provide a specific type or TypeVerify to determine which function will be used
*/
class FunctionStorage {
public:
	typedef void* Pointer;//Common pointer type
	
private:
	//for duplicating
	template<class _T>
	static Pointer CopyImpl(const Pointer o)
	{
		return (Pointer)(new _T(*(_T*)o));
	}

	//for freeing
	template<class _T>
	static void DeleteImpl(Pointer o)
	{
		delete (_T*)(o);
	}

	//typedef to simplify the codes
	typedef Pointer (*CopyFunction)(const Pointer);
	typedef void (*DeleteFunction)(Pointer);

	//each type will have multiple functions, so we need a union
	class Impls {
	private:
		friend class FunctionStorage;
		Impls(CopyFunction cImpl, DeleteFunction dImpl);

		CopyFunction copyImpl; //copy handle
		DeleteFunction deleteImpl; //delete handle
		
		template <typename _T>
		static Impls Create()
		{
			return Impls(&CopyImpl<_T>, &DeleteImpl<_T>);
		}

		const CopyFunction GetCopyImpl() const; //interface to get the copy handle
		const DeleteFunction GetDeleteImpl() const; //interface to get the delete handle
	};

	std::map<TypeVerify, Impls> m_storage; //the storage
	const Impls& GetImpls(const TypeVerify &tv) const; //find functions for a specific type

	//singleton pattern, we just need ONE storage
	FunctionStorage();
	static FunctionStorage* GetInstance();
	
public:
	//store functions for type _T
	template <typename _T>
	static void Store()
	{
		TypeVerify tv = TypeVerify::Create<_T>();
		if(GetInstance()->m_storage.find(tv) == GetInstance()->m_storage.end())
		{
			GetInstance()->m_storage.insert(std::make_pair(tv, Impls::Create<_T>()));
		}
	}
	
	//call copy function by TypeVerify
	static Pointer Copy(const Pointer o, const TypeVerify &tv);
	//call delete function by TypeVerify
	static void Delete(Pointer o, const TypeVerify &tv);

	//call copy function by specify type _T
	template <typename _T>
	static Pointer Copy(const Pointer o)
	{
		return Copy(o, TypeVerify::Create<_T>());
	}

	//call delete function by specify type _T
	template <typename _T>
	static void Delete(Pointer o)
	{
		return Delete(o, TypeVerify::Create<_T>());
	}


};//class FunctionStorage

#endif //#ifndef FUNCTION_STORAGE_H
