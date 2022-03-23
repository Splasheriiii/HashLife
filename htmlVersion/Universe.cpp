#include "MemoizedQuadTree.h"
#include <math.h>
#include <string>
#include <vector>

#ifdef __EMSCRIPTEN__
	#include <emscripten/bind.h>
	using namespace emscripten;
#endif

class Universe
{
public:
	Universe(int level, int rootLevel, std::string content)
	{
		this->level = level;
		int size = pow(2, rootLevel);
		int index = 0;
		bool** arr = new bool*[size];
		for (size_t y = 0; y < size; y++)
		{
			arr[y] = new bool[size];
			for (size_t x = 0; x < size; x++)
			{
				arr[y][x] = (content[index] == '1');
				index++;
			}
			index++;
		}
		root = MemoizedQuadTree::get_memoized_tree(rootLevel, arr);
	}
	bool next_generation()
	{
		root = root->get_next_generation();			
		return root->get_level() <= level;
	}
	std::string to_string()
	{
		int index = 0;
		int size = pow(2, level);
		int root_level = root->get_level();
		int root_size = pow(2, root_level);
		bool** arr = root->to_array();
		int offset = 0;
		std::string content = "";
		for (size_t i = level; i > root_level; i--)
		{
			offset += pow(2, i - 2);
		}

		for (size_t y = 0; y < size; y++)
		{
			for (size_t x = 0; x < size; x++)
			{
				if (y >= offset && y <= offset + root_size - 1 &&
					x >= offset && x <= offset + root_size - 1)
				{
					content.append(arr[y - offset][x - offset] ? "1" : "0");
				}
				else
				{
					content.append("0");
				}
			}
			content.append(";");
		}
		return content;
	}
	void get_hash_list(std::vector<std::string> &res)
	{
		((MemoizedQuadTree*)root)->get_hash_list(&res);
	}

private:
	int level;
	QuadTree* root;

#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_BINDINGS(my_class_example) {   
  class_<Universe>("Universe")
    .constructor<int, int, std::string>()
    .function("next_generation", &Universe::next_generation)
	.function("to_string", &Universe::to_string)
	.function("get_hash_list", &Universe::get_hash_list);
  register_vector<std::string>("StringList"); 
}
#endif	
	

};