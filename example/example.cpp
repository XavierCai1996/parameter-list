#include "../src/parameter-list.h"
#include <iostream>

class Identity {
private:
	int m_id;
public:
	Identity()
		: m_id(-1)
	{
		//std::cout << " Id::Constructor 0" << std::endl;
	}
	
	Identity(const int id)
		: m_id(id)
	{
		//std::cout << " Id::Constructor 1" << std::endl;
	}
	
	Identity(const Identity& o)
		: m_id(o.m_id)
	{
		//std::cout << " Id::Copy" << std::endl;
	}
	
	~Identity()
	{
		//std::cout << " Id::Deconstructor" << std::endl;
	}
	
	int GetId() const
	{
		return m_id;
	}
	
	void SetId(int id)
	{
		m_id = id;
	}
};

void FunctionPassIdentity(const Identity &id)
{
	std::cout << "@" << __FUNCTION__ << " ID = " << id.GetId() << std::endl;
}



void FunctionPassParameter(const Parameter &param)
{
	Identity id = param;
	std::cout << "@" << __FUNCTION__ << " ID = " << id.GetId() << std::endl;
}


/*************** The example of using Parameter [start] ***************/
void ParameterExampleFlow()
{
	//using construct function of Identity
	Parameter para1 = Parameter::Create(Identity(1));

	//using object of Identity
	Identity id(2);
	Parameter para2 = Parameter::Create(id);

	//print the type of value in para
	std::cout << "Value type is [" << para1.GetName() << "]" << std::endl;

	//two ways to copy the para2
	Parameter paraCopy1 = para2;
	Parameter paraCopy2(para2);

	//after copy, change the value of para2
	para2 = Identity(0);
	Identity idTemp = para2; //revert parameter to Identity;
	std::cout << "After change, ID = " << idTemp.GetId() << std::endl;

	//call the functions to output id information in copies
	//parameter's type is Parameter
	FunctionPassParameter(paraCopy1);
	//parameter's type is Identity (Parameter will try convert itself to Identity)
	FunctionPassIdentity(paraCopy2);

	//[Caution!] when you use the function below, be careful!
	//  create a parameter with value = int(1)
	Parameter intParam = Parameter::Create(1);
	//  you can call the copy constructor as below
	int i(intParam); //like this
	std::cout << "Call copy constructor int = " << int(intParam) << std::endl; //or like this
	/*  
	 *but you can not call the copy constructor of Identity like :
	 *		Identity(para1);    or    Identity identity(para);
	 *  because the complier dont know which function should be called:
	 *    is copy constructor         :  Identity(const Identity& o);   by convert Parameter to Identity
	 *    or the constructor with int :  Identity(const int id);        by convert Parameter to int
	 *  and then the complier will give errors that overloaded ‘Identity(Parameter&)’ is ambiguous
	*/
	//std::cout << "Call copy constructor ID = " << Identity(para1).GetId() << std::endl; //error
	/*
	 *  so if a constructor with ONE parameter has been defined in the class
	 *  copy constructor can not be called correctly
	 *  insteadly, you should copy the object by operator = 
	*/ 
	Identity correctCopy = para1;
	std::cout << "Call copy constructor ID = " << correctCopy.GetId() << std::endl; //correct
}
/*************** The example of using Parameter [ end ] ***************/


/*************** The example of using ParameterList [start] ***************/
void ParameterListExampleFlow()
{
	ParameterList list;

	//add parameter to list
	//  the first parameter is key
	//  the second parameter is value
	//  the third parameter is describe
	list.Add("int", 1, "an integer value");

	//add parameter continuously
	list.Add("float", 2.0f, "a float value")
	    .Add("double", 3.0, "a double value")
	    .Add("id", Identity(1), "identify");

	//two ways to copy the list
	ParameterList listCopy1 = list;
	ParameterList listCopy2(list);

	//remove paramters from list
	list.Remove("int")
	    .Remove("double")
	    .Remove("float");
	
	//list the parameters in copy
	//  format: key[type] : describe
	std::cout << "list the parameters :\n" << listCopy1.List() << std::endl;

	//get parameter by key and pass it to function
	Parameter param = listCopy2.Get("id");
	FunctionPassParameter(param);

	//two ways to change the value in list
	//1.dirctly
	listCopy1.Get("int") = 0;
	std::cout << "Int value in list is " << int(listCopy1.Get("int")) << std::endl;
	//2.reference
	Parameter& paramRef = listCopy1.Get("float");
	paramRef = 0.0f;
	std::cout << "Float value in list is " << float(listCopy1.Get("float")) << std::endl;
	//the code below can not change the value in list, just change value in parameter
	Parameter paramWithoutRef = listCopy1.Get("double");
	paramWithoutRef = 0.0;
	std::cout << "Double value in list is " << double(listCopy1.Get("double")) << std::endl;
}
/*************** The example of using ParameterList [ end ] ***************/


int main(int argn, char* argv[])
{
	std::cout << "\n------------------------\n";
	std::cout << "[Example]::Parameter";
	std::cout << "\n------------------------\n";
	ParameterExampleFlow();
	std::cout << "\n------------------------\n";
	std::cout << "[Example]::ParameterList";
	std::cout << "\n------------------------\n";
	ParameterListExampleFlow();
	std::cout << "\n------------------------\n";
	std::cout << "[Example]::End";
	std::cout << "\n------------------------\n";
	return 0;
}
