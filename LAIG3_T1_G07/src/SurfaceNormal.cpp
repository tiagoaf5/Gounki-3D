#include "SurfaceNormal.h"

using namespace std;

vector<float> CalculateSurfaceNormal(vector<vector<float>> polygon)
{
	vector<float> normal;
	normal.push_back(0.0);
	normal.push_back(0.0);
	normal.push_back(0.0);

	for(int i = 0; i < polygon.size(); i++)
	{
		vector<float> current = polygon[i];
		vector<float> next = polygon[(i+1) % polygon.size()];

		normal[0] += ((current[1]-next[1]) * (current[2]+next[2]));
		normal[1] += ((current[2]-next[2]) * (current[0]+next[0]));
		normal[2] += ((current[0]-next[0]) * (current[1]+next[1]));
	}

	return normal;
}