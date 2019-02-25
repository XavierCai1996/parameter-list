#include "function-storage.h"
#include "simple-assert.h"

FunctionStorage::Impls::Impls(FunctionStorage::CopyFunction cImpl, FunctionStorage::DeleteFunction dImpl)
	: copyImpl(cImpl), deleteImpl(dImpl)
{ }

const FunctionStorage::CopyFunction FunctionStorage::Impls::GetCopyImpl() const
{
	return copyImpl;
}

const FunctionStorage::DeleteFunction FunctionStorage::Impls::GetDeleteImpl() const
{
	return deleteImpl;
}


		

FunctionStorage::FunctionStorage()
{ }

const FunctionStorage::Impls& FunctionStorage::GetImpls(const TypeVerify &tv) const
{
	std::map<TypeVerify, Impls>::const_iterator find = m_storage.find(tv);
	ASSERT_WITH_MSG(find != m_storage.end(), " can not find type [" << tv.GetName() << "] in storage");
	return find->second;
}



FunctionStorage* FunctionStorage::GetInstance()
{
	static FunctionStorage instance;
	return &instance;
}

FunctionStorage::Pointer FunctionStorage::Copy(const FunctionStorage::Pointer o, const TypeVerify &tv)
{
	const FunctionStorage::Impls& impls = GetInstance()->GetImpls(tv);
	return impls.GetCopyImpl()(o);
}

void FunctionStorage::Delete(FunctionStorage::Pointer &o, const TypeVerify &tv)
{
	const FunctionStorage::Impls& impls = GetInstance()->GetImpls(tv);
	return impls.GetDeleteImpl()(o);
}
