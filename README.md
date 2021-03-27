
[![Twitter Badge](https://img.shields.io/badge/chat-twitter-blue.svg)](https://twitter.com/ArrayLikeObj)
[![GitHub license](https://img.shields.io/github/license/ethanny2/knapsack-alogorithm-cpp)](https://github.com/ethanny2/knapsack-alogorithm-cpp)

# C++ 0-1 Knapsack Problem (Best First Search)

<p align="center">
  <img width="460" height="300" src="https://image.slidesharecdn.com/knapsackproblemusingdynamicprogramming-161012161251/95/knapsack-problem-using-dynamic-programming-2-638.jpg?cb=1476288784">
</p>

## Requirements
Requires some form of C++ compliation; preferably g++.
This uses newer features so it is compiled with the C++ 11 flag.

## Usage
+ How to compile: Run make to create the BestFirstSearch executable
+ Usage: ./BestFirstSearch <inputFile.txt> <outputFile.txt>
+ Input file is formatted in the following manner
  + The first line denotes n,c where n = #of items and c= knapsack capacity. Values are separated by a comma with no space.
  + The rest of the lines (everyone but the first) is parsed as w,p where w = the weight of the item and p = the profit of the item. Values are separated by a comma with no space.
```
// sample-input.txt
4,16    
2,40
5,30
10,50
5,10
```
## Concepts and Languages Used
### Data Structures: 
+ **Priority queue**: Holds nodes and brings the node with the highest upper bound (most promising solution) to the front of the queue via a custom
comparator. Used for holding Nodes.

+ **Vector**: A c++ dyanamic array with built in library functions. Used because it is more convenient than arrays. Used for holding Items.

### Analysis of computation time:
+ **Worst time**: All the nodes of the tree are rendered (just like the brute force method) and you get a 2^(n-1) -1 tree.
+ **Average Time**: Will usually not generate all the nodes in tree so better than brute force methods. 
+ **Bounding**:  Defined (loosely) as  figuring out which configuration/path doesn't work and not choosing to go down that path. Say the knapsack is
full when we add the first n/2 items. Since its full don't try n/2 +1 n/2+2.... items because they won't work.Cutting away a subtree like this via
bounding Saves O(2d)  nodes, where d is the depth of the subtree.
	
## Classes used and interactions:
+ **Item**: A class to abstract an item in the 0-1 Knapsack problem. Each item has its own weight,profit,density. Weight and profit are read in from
a file. Density is calculated as profit/weight. 
	+ Uses: Gloabal variable vector of Items called bestItems used to keep track of the Items in the optimal solution and the order they were added in.Initial vector of Items sorted by density via custom comparator.Member variable for the node class, so the node can keep track of the items added to the knapsack so far (up to the current node)
  
+ **Node**: A class to abstract a node in the tree for Best First Search. Each node has its own weight,profit,upperbound,level and list of items added to the
knapsack so far.	
	+ Uses: Nodes are added to priority queue and sorted via descedning upperobunds.
		





	

