#include "portfolio_tree.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>

std::string PortfolioTree::serialize()
{
    return serializeToJSON(root);
}

std::string PortfolioTree::serializeToJSON(PortfolioNode &node)
{
    std::stringstream ss;
    ss << "{";
    ss << "\"PortfolioID\":" << node.portfolioID << ",";
    ss << "\"PortfolioType\":"
       << "\"" << node.portfolioType << "\",";
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

int main()
{
    //file preprocess, remove spaces and new lines
    //each tree on own line, store into string[] array
    //for loop through array instead of while loop through file
    std::ifstream currFile("jsontreeformat.txt");
    std::string thisLine;
    PortfolioTree ptree;

    std::stack<int> openStack;
    std::queue<std::string> dataValues;
    std::vector<std::vector<int>> adjList;

    std::vector<PortfolioNode> pNodes;
    std::vector<bool> hasParent;

    int cNode = -1;
    while (getline(currFile, thisLine))
    {
        for (int i = 0; i < thisLine.length(); i++)
        {
            if (thisLine[i] == '{')
            {
                cNode++;
                adjList.push_back(std::vector<int>());
                hasParent.push_back(false);
            }
            else if (thisLine[i] == '[')
            {
                openStack.push(cNode);
            }
            else if (thisLine[i] == ']')
            {
                int parent = openStack.top();
                openStack.pop();

                if (cNode - parent > 0)
                {
                    for (int j = 0; j < (cNode - parent); j++)
                    {
                        if (!hasParent[cNode - j])
                        {
                            adjList[parent].push_back(cNode - j);
                            hasParent[cNode - j] = true;
                        }
                    }
                }
            }

            else if (thisLine[i] == ':' && thisLine[i + 2] != '[')
            {
                std::string cword;
                while (i + 1 < thisLine.length())
                {
                    if (thisLine.at(i + 1) == ',' || thisLine.at(i + 1) == ']')
                    {
                        break;
                    }

                    cword.push_back(thisLine[i + 1]);
                    i++;
                }
                dataValues.push(cword);
            }
        }
    }
    // Build Tree Here
    for (int i = 0; i <= cNode; i++)
    {
        std::string temp = dataValues.front();
        temp = temp.substr(1, std::string::npos);
        int id = stoi(temp);
        dataValues.pop();
        std::string type = dataValues.front();
        type.erase(std::remove(type.begin(), type.end(), '"'), type.end());
        dataValues.pop();
        temp = dataValues.front();
        temp = temp.substr(3, std::string::npos);
        int size = stoi(temp);
        dataValues.pop();
        pNodes.push_back(PortfolioNode(id, type, size));
    }
    for (int i = adjList.size() - 1; i >=0 ; i--)
    {
        for (int j = adjList[i].size() - 1; j >= 0; j--)
        {
            pNodes[i].addNode(pNodes[adjList[i][j]]);
        }
    }
    ptree.root = pNodes[0];

    std::string json = ptree.serialize();
    std::cout << json << std::endl;
}
