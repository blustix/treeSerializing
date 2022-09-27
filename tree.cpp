#include "portfolio_tree.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>
// Basic Structure: iterate the JSON string, building the tree in c++
/*Implementation Details:
Must track open and closing { } [ ] and " "
While iterating characters, track every instance of opening in stack/heap and
pop when closing to the same character How to implement this stack; The newest
closing tag will close the most recent open tag

Build AdjList while traversing and finish with data post traversal
comma , to split between data members
*/
// time to test
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
    std::ifstream currFile("JSONtree.txt");
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
            // char cChar = thisLine[i];
            //{ and [ open and closing tags, when { is encountered,
            //  move to the next node and increase node index
            if (thisLine[i] == '{')
            {
                cNode++;
                adjList.push_back(std::vector<int>());
                hasParent.push_back(false);
            }
            // [ ]  open and closing tags, stored in stack with node number in pair
            else if (thisLine[i] == '[')
            {
                openStack.push(cNode);
            }
            // when closing, ] means that we loop from opening index to closing index
            //***Edit to only include direct children*** i think its correct
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
                            // std::cout << parent << " " << (cNode - j) << "\n";
                        }
                    }
                }
            }
            // comma - colon : store data from colon to comma and store in next
            // available member

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
                // std::cout << cword << "\n";
                // std::cout << i << "\n";
            }
        }
    }
    // Build Tree Here
    for (int i = 0; i <= cNode; i++)
    {
        int id = stoi(dataValues.front());
        dataValues.pop();
        std::string type = dataValues.front();
        type.erase(std::remove(type.begin(), type.end(), '"'), type.end());
        dataValues.pop();
        int size = stoi(dataValues.front());
        dataValues.pop();
        //std::cout << id << type << size << "\n";
        pNodes.push_back(PortfolioNode(id, type, size));
    }
    // only add nodes to closest ancestor
    // nodes only pushed back when not yet added to a parent
    //tree ancestry is built from wrong order
    //children need to be added from bottom up
    for (int i = adjList.size() - 1; i >=0 ; i--)
    {
        for (int j = adjList[i].size() - 1; j >= 0; j--)
        {
            //std::cout << i << adjList[i][j] << "\n";
            pNodes[i].addNode(pNodes[adjList[i][j]]);
            //std::cout << i+1 << " " << adjList[i][j]+1 << "\n";
        }
    }
    //std::cout << pNodes[0].portfolioID; 
    ptree.root = pNodes[0];
    //std::cout << ptree.root.portfolioID;

    // add the reprint function and verify/finalize
    std::string json = ptree.serialize();
    std::cout << json << std::endl;
}