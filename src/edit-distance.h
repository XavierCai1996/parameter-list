#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include <string>
#include <vector>

class EditDistance {
private:
	EditDistance();
public:
	static unsigned int Calculate(const std::string& s1, const std::string& s2);
	static std::vector<std::string> Related(const std::string& key, const std::vector<std::string>& strs, unsigned int maxDistance = 3);
};//class EditDistance

#endif //#ifndef EDIT_DISTANCE_H
