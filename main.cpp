#include<iostream>
#include<string>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<math.h>
#include<vector>
#include<algorithm>
#include <queue>
using namespace std;


/* The k is just the number of nodes in the optimal solution */

class Item;
/*Global variables to indicate the number of items and the capacity of the bag. Global so function parameters are not cluttered. */
int numItems = 0;
int capacity = 0;
/*Counter for number of nodes and leaves traversed in algorithm */ 
int nodesTraversed = 0;
int leavesTraversed= 0;
vector<Item> bestItems;



/**
	Class to abstract and hold both the weight and the profit of each item in a class.
	Fields are public for convenience. Density = profit/weight we will round down if the value is not an integer. 
*/
class Item {
public:
	int weight,profit,density;
	Item(int,int,int);
};

/**
	Constructor to set the values of each Item class instance 
*/
Item::Item(int x, int y,int z){
	weight = x;
	profit = y;
	density = z;
}

/** 
	Custom function used for std::sort to sort the items from most descending order of profit/weight (Density). Normally implementation of
	std::sort returns a list in ascending order so our custom function instead chooses True when the first argument is larger meaning it gets sorted in
	descending order.
	@param x The first item to compare if it is bigger it is placed before the second item.
	@param y  The second item to compare
	@return Returns true if the first item is placed before the second item in the vector, false for vice versa.
*/
bool itemComparison(Item x ,Item y){
	return x.density > y.density;
}


/**
	Class to abstract the node in the bound and branch tree. Has 3 fields
	-The upperbound (for that level)
	-The curWeight (of the current solution path)
	-The  curProfit (of the current solution path)
	-The current level the node is at in the tree
*/
class Node{
public:
	int curProfit;
	int curWeight;
	int upperbound;
	int level;
		/* The items added to the bag up to the current node*/
	vector<Item> items;
	Node(int, int, int,int);
	Node();
};

/* For sorting nodes by highest bound in priority queue*/
struct CompareBound {
	bool operator()(Node const & n1, Node const & n2) {
        // return "true" if "n1" is ordered before "n2", for example:
		return n1.upperbound < n2.upperbound;
	}
};


/* Prototypes  */
int bound(Node u , vector<Item> itemArr);


/**
	Constructor fot the Node class 
*/
Node::Node(int x, int y, int z,int level){
	curProfit = x;
	curWeight = y;
	upperbound = z;
	level = level;
}

/**
	Empty constructor for the dummy node/root
*/
Node::Node(){
	curProfit = 0;
	curWeight = 0;
	upperbound = 0;
	level= level;
}



/**
	Implementation of the knapsack 0-1 branch and bound. Gets the weight and number of items from global variables
	@param itemArr Passes in the sorted and already constructed vector of Item instances
	@return returns the maximum profit from the 0-1 knapsack problem performed on the contraints specified in the input file
*/

int knapsack(vector<Item> itemArr){
	/*Initialize a queue to construct the tree with a dummy [node v] (weight 0, profit 0 , bound 0) root node.
	Add the node to the queue and continue to loop through the items while there are promising nodes in the queue
	u is the child node
	v is the root/dummy node
 	*/

	//One for the root
	nodesTraversed++;
	int counter=0;
	int numItemsBestSol = 0;
	//queue<Node> Q;
	priority_queue<Node, vector<Node>, CompareBound> Q;
	Node u,v;
	v.level = -1;
	v.curProfit = 0;
	v.curWeight = 0;
	/*The solution current has no items so don't push anything to items */
	//v.items 
	v.upperbound= bound(v,itemArr);
	/*The max profit you keep track of is intially 0. */
	int maxProfit = 0;
	Q.push(v);
	/*Iterate through items/nodes that have promising feasible solutions */
	while(!Q.empty()){
	//	cout<<"**************"<<endl;
		/*Pop the node in the queue THIS IS ALSO A LEAF!  */
		v = Q.top();
		Q.pop();
		if(v.upperbound >maxProfit){
		//counter++;
		//cout<<"At queue ITERATION # "<<counter<<" With "<<nodesTraversed<<" nodes traversed"<<endl;
			if(v.level==-1){
				u.level=0;
			}
		/*A problem of size n will produce at most a n-level tree. If we start counting the dummy/root node as 0. 
		If the calculated level of the next child node is a leaf use "continue" to skip the iteration. (Still popped from queue)
		And increment the leaf visited variable */
			if(v.level==numItems-1){
				continue;
			}

		/* Next child's level is +1 the current level*/
			u.level = v.level +1;
	//	cout<<"CURRENT NODE LEVEL: "<<v.level<<" , weight: "<<v.curWeight<<" , profit: "<<v.curProfit<<" , upperbound: "<<v.upperbound<<endl;

		/**
			Steps for traversing the left side of the tree. Meaning you add the current parent node's weight and profit to the
			current child nodes weight and profit and sotre it in the child node (index into array to the item corresponding to the level)
		*/
			u.curWeight = v.curWeight+(itemArr[u.level]).weight;
			u.curProfit = v.curProfit + itemArr[u.level].profit;
			nodesTraversed++;
		/*Include parent's items and current level item (in that order) */
			u.items = v.items;
			u.items.push_back(itemArr[u.level]);


		//cout<<"u.level is "<<u.level<<endl;
		//cout<<"CUR Profit SHOLD BE 40 FOR THE FIRST NODE  itemArr[1]-----> "<<itemArr[u.level].profit<<endl;
		//cout<<"u.curProfit should be 40 for the first node "<<u.curProfit<<endl;
		/* If the weight of the calculated child node  (with current level item inclusion) is feasible (does not exceed capcity)
		and is better than the current maxProfit make the nodes profit the new max profit*/
		//cout<<"Max profit before current level item consideration: "<<maxProfit<<endl;
			if((u.curWeight<=capacity) && (u.curProfit > maxProfit)){
				maxProfit = u.curProfit;
			/* If node creates better max profit it will be part of the optimal solution. */
				bestItems = u.items;
			}
		//cout<<"Max profit after current level item consideration: "<<maxProfit<<endl;
		/* Calculate the bound (using the greedy selection of the fractional knapsack). If the upperbound of the current node exceeds the 
		current maxProfit it is promising and add it to the queue to continue exploring it else don't add it.*/ 
			int temp= bound(u,itemArr);
			u.upperbound = temp;
			if(u.upperbound>maxProfit){
				Q.push(u);
			}else{
			/*This is a leaf */
				leavesTraversed++;
			}
		//cout<<"Left Child node has upperbound "<<u.upperbound<<endl;
		//cout<<"Upperbound LEFT CHILD IS "<<u.upperbound<<endl;

		/**
			Steps for traversing the right side of the tree. Meaning you do not take the item at the current level into consideration. 
			Carry over the weight and profit from the parent node. Still calculate the bound and add it to the queue if it meets the 
			conditions delineated above.
		*/
			u.curWeight = v.curWeight;
			u.curProfit = v.curProfit;
			u.upperbound = bound(u,itemArr);
			nodesTraversed++;
		/* For right side node take only parent items list, not the item at the current level*/
			u.items = v.items;
		//cout<<"Right child node has upperbound "<<u.upperbound<<endl;
			if(u.upperbound>maxProfit){
				Q.push(u);
			}else{
			/*This is a leaf */
				leavesTraversed++;
			}
		//cout<<"Upperbound is RIGHT CHILD IS "<<u.upperbound<<endl;

		//cout<<"At end of iteration number "<<counter<<" With maxprofit "<<maxProfit<<endl;
		//cout<<"****************"<<endl;
		}else{
			leavesTraversed++;
		}/* End conditional statement to check if current node is still promising*/
	}/*End queue while loop */

	/* Checking item solution*/
			for (auto i: bestItems){
  		//cout <<"Weight: "<<i.weight << " , Profit: "<<i.profit<<endl ;
			}
	//cout<<"Size of optimal solution AKA number of items "<<bestItems.size()<<endl;
			return maxProfit;
		}


/**
	Calculated by using the greedy selection of the fractional knapsack problem. Using the current node's level iterate through
	the remaining list of items (anything on the level below the current node) while ...
	1)There are still items to iterate over
	2) The total item weights do not exceed the capacity
	Keep track of the total profit accured by the loop, this is the upperbound. Keep track of the weight so you know when to quit the loop (i.e when
	the solution becomes unfeasible).

	After the loop you have MOSTLY preformed the fractional greedy selection. The loop accounted for any whole numbers weights that could be taken
	after the loop has ended and there are still items on the list...
	1) Take the most possible (a fraction) from the current (last) item and add it to the upperbound.

	To take the remaining fractional amount do (capcity-calculatedWeightSoFar)

	@param u An instance of the node you want to perform the bound function on.
	@param itemArr The sorted (descending order) list of items
	@return The upperbound for the current node. 

*/
		int bound(Node u,vector<Item> itemArr){
	/*Check if single nodes weight is too much */
			if(u.curWeight >= capacity){
				return 0;
			}
	/* Inlcude the current profit as a base to add onto for the upperbound ONLY IF YOU ARE ON THE LEFT SIDE OF THE TREE E.G Took the item*/
			int upperbound = u.curProfit;
	//cout<<"Initial upper bound value in bound() function "<<u.upperbound<<endl;
	//cout<<"Initial weight value in bound() function "<<u.curWeight<<endl;
	/* Run the selection on every item below the current level*/
			int remainingItemIndex = u.level+1;
	/* Keep track of weight*/
			int totalWeight = u.curWeight;
//	cout<<"totalWEIGHT BEFORE LOOP IS "<<totalWeight<<endl;

	/*ERROR ACCESSING ARRAY OUT OF BOUNDS */
			while( (remainingItemIndex<numItems) && ( (totalWeight + itemArr[remainingItemIndex].weight) <= capacity)){
	//	cout<<"remainingItemIndex: "<<remainingItemIndex<<endl;
	//	cout<<"itemArr["<<remainingItemIndex<<"].weight is "<<itemArr[remainingItemIndex].weight<<endl;
				totalWeight += itemArr[remainingItemIndex].weight;
				upperbound += itemArr[remainingItemIndex].profit;
	//	cout<<"TOTAL WEIGHT IN LOOP AFTER ADDING ITH WEIGHT "<<totalWeight<<endl;
	//	cout<<"totalWeight + itemArr["<<remainingItemIndex<<"].weight is "<<totalWeight+itemArr[remainingItemIndex].weight<<endl;
				remainingItemIndex++;
			}

	/*The "fractional" part of the knapsack greedy selection to calculate the upperbound. 
	If there are still items to access add the fraction to the upperbound*/

//	cout<<"Upperbound in function before the fractional knapsack problem is "<<upperbound<<endl;
			if(remainingItemIndex<numItems){
		/*How much weight left to fill  */
		//int temp1 = (capacity - totalWeight);
		/* Density of current item you stopped at*/
		//int temp2 = itemArr[remainingItemIndex].density;
		//upperbound+= temp1*temp2;
				upperbound += (capacity - totalWeight) * (itemArr[remainingItemIndex].profit /itemArr[remainingItemIndex].weight);
			}
//	cout<<"Caculated totalWeight "<<totalWeight<<endl;
//	cout<<"***************"<<endl;
	//cout<<"UPPERBOUND IN bound() function"<<upperbound<<endl;
//	cout<<"***************"<<endl;
			return upperbound;
		}



		int main(int argc, char *argv[]){
	/*Parsing command line file paths and error checking */
			if(argc!=3){
				cout<<"Incorrect number of command line args\n";
				cout<<"Format: BestFirstSearch <inputFile.txt> <outputFile.txt>\n";
				exit(1);
			}
	/*Hold list of items to sort them */
			static vector<Item> itemList;
			ifstream inputFile;
			ofstream outputFile;
			inputFile.open(argv[1]);
	//cout<<"argv[1] "<<argv[1]<<endl;
			outputFile.open(argv[2]);
			if(!inputFile){
				cout<<"Unable to open input file with name "<<argv[1]<<endl;
				exit(1);
			}
			if(!outputFile){
				cout<<"Unable to open output file with name "<<argv[2]<<endl;
				exit(1);
			}

	/*Read list from input file. Which is formated as 2 numbers
	with the first 2 numbers being n,c where n= number of items and c= capcaity.
	The rest are the weight,profit for each of the items*/
			string line;
			bool firstLine = true;
			int x,y;
			char c;
			while((inputFile>>x>>c>>y) &&(c==',')){
		/* Parse the input file for the first line setting the variables for capacity and number of items*/
				if(firstLine){
					numItems = x;
					capacity = y;
					firstLine = false;
				}else{
			/* The rest of these are item descriptions add them create an instance of the class and add them to the vector to sort them*/
					Item temp(x,y,y/x);
					itemList.push_back(temp);

				}
		//cout<<"First number x:["<<x<<"] ";
		//cout<<"Second number y:["<<y<<"] "<<endl;

			}
			inputFile.close();
	/*Sort the list of items */
			sort(itemList.begin(),itemList.end(),itemComparison);
	/*Loop test through vector using c++ 11 range */
			
	//cout<<"Capcity is : "<<capacity<<endl;
	//cout<<"Number of items is: "<<numItems<<endl;

	/*Run the knapsack function */
			int answer = knapsack(itemList);
	//cout<<"Max profit returned is: " <<answer<<'\n';
	//cout<<"Number of nodes visited is "<<nodesTraversed<<'\n';
	//cout<<"Number of leaves visitied is "<<leavesTraversed<<'\n';

	/* Write the answer*/
			
	/* Line 1: size of problem,optimal solution (maxprofit ret val),number of items in optimal solution*/
			outputFile<<numItems<<","<<answer<<","<<bestItems.size()<<'\n';
	/*Line 2: Nubmer of nodesVisited,number of leaf nodes visited */
			outputFile<<nodesTraversed<<","<<leavesTraversed<<'\n';
	/*Line 3: Weight,profit [of each item in the solution in the order they were found] */
			for( auto i:bestItems){
				outputFile<<i.weight<<","<<i.profit<<'\n';
			}
			outputFile.close();

}/*End of main */




