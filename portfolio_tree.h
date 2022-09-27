#pragma once
#include <string>
#include <vector>

class PortfolioNode
{
public:
	PortfolioNode()
	{
		portfolioID = 0;
		portfolioSize = 0;
	}
	PortfolioNode(int id, const std::string& ptype, int psize) : portfolioID(id), portfolioType(ptype), portfolioSize(psize)
	{
	};
	void addNode(PortfolioNode& node)
	{
		childPortfolios.push_back(node);
	};

	int portfolioID;
	std::string portfolioType;
	int portfolioSize;
	std::vector<PortfolioNode> childPortfolios;

};

class PortfolioTree
{
public:
	PortfolioTree() {};
	PortfolioTree(PortfolioNode r): root(r)
	{};

	PortfolioNode root;
	std::string serialize();
	std::string serializeToJSON(PortfolioNode& node);
};