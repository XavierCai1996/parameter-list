#include "edit-distance.h"
#include <vector>

EditDistance::EditDistance()
{ }

unsigned int EditDistance::Calculate(const std::string& s1, const std::string& s2)
{
	int len1 = s1.length();
	int len2 = s2.length();
	
	std::vector< std::vector<int> > distance;
	distance.resize(len1 + 1);
	for(int i = 0; i <= len1; i++)
	{
		distance[i].resize(len2 + 1);
	}

	static int aA = 'a' - 'A';
	bool captrans = false;
	for(int i = 0; i <= len1; i++)
	{
		distance[i][0] = i;
	}
	for(int j = 0; j <= len2; j++)
	{
		distance[0][j] = j;
	}
	for(int i = 1; i <= len1; i++)
	{
		for(int j = 1; j <= len2; j++)
		{
			int cost = 1;
			if(s1[i-1] == s2[j-1])
			{
				cost = 0;
			}
			else if ((s1[i-1] <= 'z' && s1[i-1] >= 'a' && s2[j-1] <= 'Z' && s2[j-1] >= 'A' && s1[i-1] - s2[j-1] == aA)
					|| (s1[i-1] <= 'Z' && s1[i-1] >= 'A' && s2[j-1] <= 'z' && s2[j-1] >= 'a' && s2[j-1] - s1[i-1] == aA))
			{
				cost = 0;
				captrans = true;
			}

			int deletion = distance[i-1][j] + 1;
			int insertion = distance[i][j-1] + 1;
			int substitution = distance[i-1][j-1] + cost;
			int min = deletion > insertion ? insertion : deletion;
			min = substitution > min ? min : substitution;
			distance[i][j] = min;
		}
	}
	return distance[len1][len2] + (captrans ? 1 : 0);
}
