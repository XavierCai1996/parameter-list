#include "../src/parameter-list.h"
#include <iostream>

//a simple new class for test
class Identity {
private:
	int* m_id;
public:
	Identity()
		: m_id(new int(-1)) {
		//std::cout << " Id::Constructor " << GetId() << std::endl;
	}
	
	Identity(const int id)
		: m_id(new int(id)) {
		//std::cout << " Id::ConstructorP " << GetId() << std::endl;
	}
	
	Identity(const Identity& o)
		: m_id(new int(*o.m_id)) {
		//std::cout << " Id::Copy " << GetId() << std::endl;
	}
	
	~Identity() {
		//std::cout << " Id::Deconstructor " << GetId() << std::endl;
		delete m_id;
	}
	
	int GetId() const {
		return *m_id;
	}
	
	void SetId(int id) {
		*m_id = id;
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
	//using constructor of Identity
	Parameter para1(Identity(1));

	//using object of Identity
	Identity id(2);
	Parameter para2(id);

	//print the type of value in parameter
	std::cout << "Value type is [" << para1.GetName() << "]" << std::endl;

	//two ways to copy the para2
	Parameter paraCopy1 = para2;
	Parameter paraCopy2(para2);

	//after copy, change the value in para2
	para2 = Identity(0);
	Identity idTemp = para2; //revert parameter to Identity;
	std::cout << "After change, ID = " << idTemp.GetId() << std::endl;

	//call the functions to output id information in copies
	//argument type is Parameter
	FunctionPassParameter(paraCopy1);
	//argument type is Identity (Parameter will try convert itself to Identity)
	FunctionPassIdentity(paraCopy2);

	//[Caution!] when you call the copy constrctor like below, be careful!
	//  create a parameter with value = int(1)
	Parameter intParam = Parameter::Create(1);
	//  you can call the copy constructor as below
	int i(intParam); //like this
	std::cout << "Call copy constructor int = " << int(intParam) << std::endl; //or like this
	/*  
	 *but you can not call the copy constructor of Identity like :
	 *		Identity(para1);    or    Identity identity(para);
	 *  because the complier dont know which function is expected to be called:
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

//a function coding in framework
//  to calculate area for rectangle or volume for cuboid
double CalculateCuboidVolume(const ParameterList &inList)
{
	ParameterList list;
	//Framework step 1. add parameters with default value (this step can be ignore if arguments of function have no defaults)
	list.Add("c", 1.0, "length of the third side"); //double, default to 1.0
	//Framework step 2. merge lists, and require some specific parameters (require list is not necessary)
	list.Merge(inList, ParameterList::RequireList()
	     		   .Add<double>("a", "length of the first side") //double
	     		   .Add<double>("b", "length of the second side") //double
	     		   );
	/*  a & b are required in this function
	 *    when a & b are given, calculate area as 
	 *                    a * b = a * b * c; (c = 1.0)
	 *    when a & b & c are given, calculate volume as
	 *                         a * b * c
	*/                          
	double volume = double(list.Get("a")) * double(list.Get("b")) * double(list.Get("c"));
	return volume;
}

void ParameterListExampleFlow()
{
	ParameterList list;

	//add parameter to list
	//  the first argument is [key]
	//  the second argument is [value]
	//  the third argument is [description] (default to "(no description)")
	list.Add("int", 1, "an integer value");

	//add parameters continuously
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
	//  format: key[type] : description
	std::cout << "list the parameters :\n" << listCopy1.List() << std::endl;

	//get parameter by key and pass it to function
	Parameter param = listCopy2.Get("id");
	FunctionPassIdentity(param);

	//two ways to change the value in list
	//1. dirctly
	listCopy1.Get("int") = 0;
	std::cout << "Int value in list is " << int(listCopy1.Get("int")) << std::endl;
	//2. by reference
	Parameter& paramRef = listCopy1.Get("float");
	paramRef = 0.0f;
	std::cout << "Float value in list is " << float(listCopy1.Get("float")) << std::endl;
	//the codes below can not change the value in list
	Parameter paramWithoutRef = listCopy1.Get("double");
	paramWithoutRef = 0.0;
	std::cout << "Double value in list is " << double(listCopy1.Get("double")) << std::endl;

	//example of calling a function which is using framework
	ParameterList paramCuboidVolume;
	paramCuboidVolume.Add("a", 3.0) //double
	                 .Add("b", 2.0); //double
	//calculate area for rectangle with a = 3, b = 2
	double area = CalculateCuboidVolume(paramCuboidVolume);
	std::cout << "Calculate area = " << area << std::endl;
	paramCuboidVolume.Add("c", 5.0); //double
	//then, calculate volume for cuboid with a = 3, b = 2, c = 5
	double volume = CalculateCuboidVolume(paramCuboidVolume);
	std::cout << "Calculate volume = " << volume << std::endl;
	//if you don't kown which parameters are required, run the code below! then help information will be printed with a termination
	//CalculateCuboidVolume(ParameterList::Help());
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
