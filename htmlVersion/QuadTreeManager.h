#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "QuadTree.h"


class QuadTreeManager
{
	static QuadTreeManager* instance;	
public:
	static QuadTreeManager* get_instance()
	{
		return instance == nullptr ? (instance = new QuadTreeManager()) : instance;
	}

	QuadTree* get_value(long hash)
	{
		return hash_map.find(hash) != hash_map.end()
				? hash_map[hash]
				: nullptr;
	}
	void set_value(long hash, QuadTree* tree)
	{
		hash_map[hash] = tree;
	}

	bool has_res_value(long hash)
	{
		return hash_res_map.find(hash) != hash_res_map.end();
	}
	long get_res_value(long hash)
	{
		return hash_res_map[hash];
	}
	void set_res_value(long hash, long res_hash)
	{
		hash_res_map[hash] = res_hash;
	}
	void get_hash_list(std::vector<std::string>* res)
	{
		for (std::unordered_map<long, QuadTree*>::iterator it = hash_map.begin(); it != hash_map.end(); ++it)
		{
			res->push_back(std::to_string(it->first));
		}				
	}
private:
	QuadTreeManager(){}
	std::unordered_map<long, QuadTree*> hash_map;
	std::unordered_map<long, long> hash_res_map;	
};
QuadTreeManager* QuadTreeManager::instance = 0;