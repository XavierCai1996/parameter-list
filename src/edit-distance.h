#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include <string>

class EditDistance {
private:
	EditDistance();
public:
	static unsigned int Calculate(const std::string& s1, const std::string& s2);
};//class EditDistance

#endif //#ifndef EDIT_DISTANCE_H
