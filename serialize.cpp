#include "portfolio_tree.h"
#include <iostream>
#include <sstream>
//generalize with <Name, <Value, Type>> instead of portfolio
void construct_test_tree(PortfolioTree& ptree)
{
	/*
{
	"PortfolioID": 1,
	"PortfolioType": "Portfolio",
	"PortfolioSize:": 10,
	"ChildPortfolio": [{
			"PortfolioID": 2,
			"PortfolioType": "Portfolio",
			"PortfolioSize:": 20,

			"ChildPortfolio": [{
					"PortfolioID": 3,
					"PortfolioType": "Fund",
					"PortfolioSize:": 30
				},
				{
					"PortfolioID": 4,
					"PortfolioType": "Fund",
					"PortfolioSize:": 40
				},
				{
					"PortfolioID": 5,
					"PortfolioType": "Index",
					"PortfolioSize:": 50
				}
			]
		},
		{
			"PortfolioID": 6,
			"PortfolioType": "Portfolio",
			"PortfolioSize:": 60
		},
		{
			"PortfolioID": 7,
			"PortfolioType": "Index",
			"PortfolioSize:": 70,
			"ChildPortfolio": [{
					"PortfolioID": 8,
					"PortfolioType": "Index",
					"PortfolioSize:": 80
				},
				{
					"PortfolioID": 9,
					"PortfolioType": "Fund",
					"PortfolioSize:": 50
				}
			]
		}
	]
}
	*/
	PortfolioNode root(1, "Portfolio", 10);
	PortfolioNode n2(2, "Portfolio", 20);
	PortfolioNode n3(3, "Fund", 30);
	PortfolioNode n4(4, "Fund", 40);
	PortfolioNode n5(5, "Index", 50);
	PortfolioNode n6(6, "Portfolio", 60);
	PortfolioNode n7(7, "Index", 70);
	PortfolioNode n8(8, "Index", 80);
	PortfolioNode n9(9, "Fund", 90);
	n2.addNode(n3);
	n2.addNode(n4);
	n2.addNode(n5);
	root.addNode(n2);
	root.addNode(n6);
	n7.addNode(n8);
	n7.addNode(n9);
	root.addNode(n7);
	ptree.root = root;
}

std::string PortfolioTree::serialize()
{
	return serializeToJSON(root);
}

std::string PortfolioTree::serializeToJSON(PortfolioNode &node)
{
	std::stringstream ss;
	ss << "{";
	ss << "\"PortfolioID\":" << node.portfolioID << ",";
	ss << "\"PortfolioType\":" << "\"" << node.portfolioType << "\",";
	ss << "\"PortfolioSize\":" << node.portfolioSize << ",";
	ss << "\"ChildPortfolio\": [";
	std::vector<PortfolioNode>::iterator it;
	for (it = node.childPortfolios.begin(); it != node.childPortfolios.end(); ++it)
	{
		if (it != node.childPortfolios.begin())
			ss << ",";
		ss << serializeToJSON(*it);
	}
	ss << "]";
	ss << "}";
	return ss.str();

}

int main(int argc, char *argv[])
{
	PortfolioTree ptree;
	construct_test_tree(ptree);
	std::cout << "root id:" << ptree.root.portfolioID << std::endl;
	std::cout << "root type:" << ptree.root.portfolioType << std::endl;
	std::cout << "root size:" << ptree.root.portfolioSize << std::endl;
	std::cout << "root child size:" << ptree.root.childPortfolios.size() << std::endl;
	std::vector<PortfolioNode>::iterator it;
	for (it = ptree.root.childPortfolios.begin(); it != ptree.root.childPortfolios.end(); ++it)
	{
		std::cout << "id:" << it->portfolioID << std::endl;
		std::cout << "type:" << it->portfolioType << std::endl;
		std::cout << "size:" << it->portfolioSize << std::endl;
		std::cout << "child size:" << it->childPortfolios.size() << std::endl;

	}
	std::string json = ptree.serialize();
	std::cout << json << std::endl;
}