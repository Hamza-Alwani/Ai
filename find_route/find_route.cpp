// task1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//includes
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <sstream>
#include <map>

int pops=0;
int exanded=0;
int genrated=1;


/*
* NAME: Node
* USE: STore the nodes (cities) and its succesors and its step cost as a data structure. for uninformed
*/

struct Node
{
    std::string from; //name of the current city
    std::string to;  //name of the city it can go to
    double distance; //the distance bettween the two
    Node() //defualt constructor
    {
        from = "";
        to = "";
        distance = 0;
    };
};


/*
* NAME: Node_h
* USE: STore the nodes (cities) and its succesors and its step cost and the heuritsic value  as a data structure. for informed
*/
struct Node_h 
{
    std::string from; //name of the current city
    std::string to;  //name of the city it can go to
    double distance; //the distance bettween the two
    double hval; //the heuritc value of the current city
    Node_h()//defualt constructor
    {
        from = "";
        to = "";
        distance = 0;
        hval = 0;
       
    };
};


/*
* NAME: Route
* USE: STore the route of the cities. stores the current city, its ancesstos as a vector of nodes (all previous cites and all their step cost) and total cost so far as a data structure. for uninformed
*/
struct Route
{
    std::string name; //name of the current city
    std::vector<Node> anccestor; //vecotr of all the previous nodes/cites and thier step cost sotred
    double total_cost; //total cost of route 
    Route()//defualt constructor
    {
        name = "";
        anccestor;
        total_cost = 0;
    };
};


/*
* NAME: Node_h
* USE: STore the nodes (cities) and its succesors and its step cost and the heuritsic value  as a data structure. for informed
*/
struct Route_h
{
    std::string name; //name of the current city
    std::vector<Node_h> anccestor;//vecotr of all the previous node_h/cites and thier step cost sotred
    double real_cost;//total real cost of route 
    double hue_cost; //total cost of route+huertisc cost
    Route_h()//defualt constructor
    {
        name = "";
        anccestor;
        real_cost = 0;
        hue_cost = 0;
    };
};


/*
* Input: 
    filename: name of the file
* Returns: 
*   size of the filename excul;ding the "END OF FILE"
* Use: find the number of lines in a file
*/
int getSize(char * filename)
{
    std::ifstream file(filename);
    int i = 0;
    std::string str;
    while (std::getline(file, str)) {
        i++;
    }
    return (i - 1);

}


/*
* Input:
    a: vecotr of node to be printed
    s: size of the vector to be printed
* Returns: VOID
* Use: Prints vector of nodes
*/
void print_nodes(std::vector<Node> a, int s)
{
    for (int i = 0; i < s; i++)
    {
        std::cout << a[i].from << " to " << a[i].to << ", ";
        printf("%.1lf km\n", a[i].distance);
    }
}


/*
* Input: 
    ans: ROTUE
* Returns:
*   void
* Use: Prints the Stats of the serach andthe ancessotrs of a rotue 
*/
void print_route(Route ans)
{
    std::cout << "Nodes popped: " << pops << std::endl;
    std::cout << "nodes expanded " << exanded << std::endl;
    std::cout << "nodes genrated " << genrated << std::endl;
    printf("Distance: %.1lf km\n", ans.total_cost);
    std::cout << "Route:" << std::endl;
    print_nodes(ans.anccestor, ans.anccestor.size());
}


/*
* Input:
*   inputfile: name of the inputfile
*   size: size of the input file
* Returns:
*   a vector of all the nodes in a vector
* Use:
*   sets up the file into a data strcuture. Since all data is bidirectional makes 2 nodes for each line 
    eg: Luebeck Hamburg 63
    makes:
    Luebeck -> Hamburg dist=63
    Hamburg -> Luebeck dist=63
*/
std::vector<Node> setup_uninformed(char* inputfile, int size)
{
    //Node* listofNode = new Node[size*2];
    std::vector<Node> listofNode;
    std::ifstream infile(inputfile);
    if (infile.is_open()) {
        std::string f;
        std::string t;
        double   d;
        int i = 0;
        while (infile >> f && i < size)
        {
            infile >> t;
            infile >> d;       
            Node temp;
            temp.from = f;
            temp.to = t;
            temp.distance = d;         
            //temp.distance = std::atoi(d);
            listofNode.push_back(temp);
            i++;
            temp.from = t;
            temp.to = f;
            listofNode.push_back(temp);
            i++;  
        }
    }
    else
    {
        std::cout << "unable to open file:\t"<< inputfile << std::endl;
    }
    infile.close();
    return listofNode;
}


/*
* Input: 
* lhs: (ROUTE) left hand side of the compartion
* rhs: (ROUTE) right hand side of the compartion
* Returns:
*   bool: wheter lsh cost is less than rhs.cost
* Use:
*   use for std::sort. sorts vector for cost and if cost is equal sorts on name as tie breaker
*/
bool fringe_sorter(Route const& lhs, Route const& rhs)
{
    if (lhs.total_cost != rhs.total_cost)
        return lhs.total_cost < rhs.total_cost;
    return lhs.name < rhs.name ;
}


/*
* Input:
* list: vector of all nodes the one made in setup()  
* fringe: the entire fringe
* popped: Route that just got popped
* Returns:
*   vector<ROUTE>:a vector of the fringe wich now includes the childern.
* Use:
*  given a route that has been popped go through the list of nodes and find its successors (childern) and add them to the existing fringe.
*/
std::vector<Route> add_children(std::vector<Node> list,std::vector<Route> fringe, Route popped)
{
    for (int i = 0; i<list.size(); i++)
    {
        //genrated++;
        if (list.at(i).from == popped.name)
        {  
            genrated++;
            Route adder;
            adder.name = list.at(i).to;
            adder.total_cost = list.at(i).distance +popped.total_cost;
            adder.anccestor = popped.anccestor;

            Node ances;
            ances.from = list.at(i).from;
            ances.to = list.at(i).to;
            ances.distance = list.at(i).distance;
            adder.anccestor.push_back(ances);
            fringe.push_back(adder);
        }
    }
    return fringe;

}


/*
* Input: 
*  * list: vector of all nodes the one made in setup()
    start: name of the starting node
    end: name of the goal
* Returns:
* ROUTE: the optiaml route it found
* Use:
*   Runs Unifor serch and returns the optiamal solution
*/
Route ucs(std::vector<Node> list, std::string start, std::string end)
{
    std::vector<Route> fringe;
    std::vector<std::string> closed;
    Route s;
    s.name = start; 
    s.total_cost = 0;
    fringe.push_back(s); //add starting point to the firnge 
    int i = 0;
    while (fringe.size()>0 && i < list.size()+1) //while fringe isnt empty or while not every ndoe is touched atleast once 
    {
        pops++; //pop the top
        if (fringe.at(0).name == end) //if top is the destianion return it 
            return fringe.at(0);
        if (! std::count(closed.begin(), closed.end(), fringe.at(0).name)) //if top isnt in the closed list
        {
             exanded++; 
            fringe = add_children(list, fringe, fringe.at(0)); //expand it
            closed.push_back(fringe.at(0).name); //add top to closed
        }
       
        fringe.erase(fringe.begin()); //remove the top (memory wise it is now popped)
        std::sort(fringe.begin(), fringe.end(), &fringe_sorter); //sort  the list
        i++;
    }
    s.total_cost = -1; //if its not found set it to -1 so the function caller knows 
    return s; //return the optimal route
}


/*
* Input:
    inputfile: name of the input file
    start: name of the starting node
    end: name of the goal 
* Returns: VOID
* Use: Runs the uninformed algo
*/
int  uninformed(char* inputfile, char* start, char* end)
{

    std::vector<Node> listofNode = setup_uninformed(inputfile, getSize(inputfile)*2); //creates a list of nodes
   
    if (listofNode.size() == 0)  //if list is size 0 something sent wrong abort
        return 1;
    Route ans = ucs(listofNode, start, end);

    if (ans.total_cost == -1)
    {
        std::cout << "Nodes popped: " << pops << std::endl;
        std::cout << "nodes expanded " << exanded << std::endl;
        std::cout << "nodes genrated " << genrated << std::endl;
        //std::cout << "Distance " << ans.total_cost << std::endl;
        printf("Distance: Infinity\n");
        std::cout << "Route:" << std::endl;
        std::cout << "None\n";
    }
    else
    {
        print_route(ans);
    }
    return 0;
}




/*
* Input:
    a: vecotr of node_h to be printed
    s: size of the vector to be printed
* Returns: VOID
* Use: Prints vector of node_h
*/
void print_h_nodes(std::vector<Node_h> a, int s)
{
    for (int i = 0; i < s; i++)
    {
        std::cout << a[i].from << " to " << a[i].to << ", ";
        printf("%.1lf km\n", a[i].distance);
    }

}


/*Input:
ans: ROTUE_H
* Returns :void
  Use : Prints the Stats of the serach andthe ancessotrs of a rotue
*/  
void print_route_h(Route_h ans)
{
    std::cout << "Nodes popped: " << pops << std::endl;
    std::cout << "Nodes expanded " << exanded << std::endl;
    std::cout << "Nodes genrated " << genrated << std::endl;
    printf("Distance: %.1lf km\n", ans.real_cost);
    std::cout << "Route:" << std::endl;
    print_h_nodes(ans.anccestor, ans.anccestor.size());
}


/*
* Input: 
* hfile: name of the heuristic file
* size: size of hfile
* 
* Returns:
* map<string,double>: map of all cites mapped to thier h_value
* Use: connects the city to its hvalue 
*/
std::map<std::string, double> setup_heuristic(char* hfile, int size)
{
    std::map<std::string, double> h_dis;
    std::ifstream infile(hfile);

    if (infile.is_open()) {
        std::string f;
        int   d;
        int i = 0;
        while (infile >> f && i < size)
        {
            infile >> d;
            h_dis.insert(std::pair<std::string, int>(f, d));
            i++;
        }
    }
    infile.close();
    return h_dis;
}

/*
* Input:
* inputfile: name of the input file
* size: size of inputfiel
* h_dis: map of cites and thier h-values
* Returns:
*   a vector of all the node_h in a vector
* Use:
*   sets up the file into a data strcuture. Since all data is bidirectional makes 2 nodes for each line 
    eg: Luebeck Hamburg 63 
    makes:
    Luebeck -> Hamburg dist=63 hval= 300
    Hamburg -> Luebeck dist=63 hval= 200
*/
std::vector<Node_h> setup_informed(char* inputfile, int size, std::map<std::string,double> h_dis)
{
    //Node* listofNode = new Node[size*2];
    std::vector<Node_h> listofNode;
    std::ifstream infile(inputfile);
    if (infile.is_open()) {
        std::string f;
        std::string t;
        int   d;
        int i = 0;
        while (infile >> f && i < size)
        {
            infile >> t;
            infile >> d;
            Node_h temp;
            temp.from = f;
            temp.to = t;
            temp.distance = d;
   

            std::map<std::string, double>::iterator it;
            it = h_dis.find(f);
            if (it != h_dis.end())
                temp.hval = h_dis.find(temp.from)->second;

            listofNode.push_back(temp);
            i++;
            temp.from = t;
            temp.to = f;
            temp.distance = d;

            it = h_dis.find(f);
            if (it != h_dis.end())
                temp.hval = h_dis.find(temp.from)->second;

            i++;
            listofNode.push_back(temp);
            
        }
    }
    else
    {
        std::cout << "unable to open file:\t" << inputfile << std::endl;
    }
    infile.close();
    return listofNode;
}


/*
* Input:
* lhs: (ROUTE_h) left hand side of the compartion
* rhs: (ROUTE_H) right hand side of the compartion
* Returns:
*   bool: wheter lsh.hue-cost is less than rhs.hue-cost
* Use:
*   use for std::sort. sorts vector for cost and if cost is equal sorts on name as tie breaker
*/
bool fringe_sorter_h(Route_h const& lhs, Route_h const& rhs)
{
    if (lhs.hue_cost != rhs.hue_cost)
        return lhs.hue_cost < rhs.hue_cost;
    return lhs.real_cost < rhs.real_cost;
}


/*
* Input:
* list: vector of all node_h the one made in setup_h()
* fringe: the entire fringe
* popped: Route that just got popped
* Returns:
*   vector<ROUTE>:a vector of the fringe which now includes the childern.
* Use:
*  given a route that has been popped go through the list of nodes and find its successors (childern) and add them to the existing fringe.
*/
std::vector<Route_h> add_h_children(std::vector<Node_h> list, std::vector<Route_h> fringe, Route_h popped, std::map<std::string, double> hue)
{
    for (int i = 0; i < list.size(); i++)
    {
        //genrated++;
        if (list.at(i).from == popped.name)
        {
            genrated++;
            Route_h adder;
            adder.name = list.at(i).to;
            adder.real_cost= list.at(i).distance + popped.real_cost;
            adder.hue_cost = hue.find(list.at(i).to)->second + adder.real_cost;
            adder.anccestor = popped.anccestor;


            Node_h ances;
            ances.from = list.at(i).from;
            ances.to = list.at(i).to;
            ances.distance = list.at(i).distance;
            ances.hval = hue.find(ances.to)->second;
            

            adder.anccestor.push_back(ances);
            fringe.push_back(adder);
        }
    }
    return fringe;

}


/*
* Input:
*  * list: vector of all nodes the one made in setup_h()
    start: name of the starting node
    end: name of the goal
    hue: map of nodes/cites (name and h-value) 
* Returns:
* ROUTE: the optiaml route it found using a*
* Use:
*   Runs inforemed serch (a*) and returns the optiamal solution
*/
Route_h Astar(std::vector<Node_h> list, std::string start, std::string end, std::map<std::string, double> hue)
{
    std::vector<Route_h> fringe;
    std::vector<std::string> closed;
    Route_h s;
    s.name = start;
    s.real_cost = 0;
    s.hue_cost = 0;
    fringe.push_back(s); //adds starting point to the fringe.
 
    int i = 0;

    while (fringe.size() > 0 && i < list.size() + 1) // while fringe isnt empty or while not every ndoe is touched atleast once
    {
        pops++; //pop the top
        if (fringe.at(0).name == end) //if top is the destianion return it 
            return fringe.at(0);
        if (!std::count(closed.begin(), closed.end(), fringe.at(0).name)) //if top isnt in the closed list
        {
            exanded++;
            fringe = add_h_children(list, fringe, fringe.at(0),hue); //expand top
            closed.push_back(fringe.at(0).name); //add top to closed 
        }

        fringe.erase(fringe.begin()); //remove the top (memory wise it is now popped)
        std::sort(fringe.begin(), fringe.end(), &fringe_sorter_h); //sort the fringe 
        i++;
    }
    s.real_cost = -1; //if its not found set it to -1 so the function caller knows 
    return s; //return the optimal route
}


/*
* Input:
    inputfile: name of the input file
    start: name of the starting node
    end: name of the goal
    hfile: name of the hfile
* Returns: 
int return 0 if all good
else return 1 if something went wrong
* Use: Runs the informed algo
*/
int  informed(char* inputfile, char* start, char* end, char* hfile)
{
    
    std::map<std::string, double> h_dis; //map of cites to h-vals
    h_dis = setup_heuristic(hfile, getSize(hfile)); //set up the map

    if (h_dis.size() == 0) //if map is empty it eihter didnt open file
    {
        std::cout << "unable to open H-file ran uninformed " << std::endl; //run uninfomred;
        uninformed(inputfile, start, end); //uninformed
        return 1; //return 1
    }
       

    std::vector<Node_h> listofNode = setup_informed(inputfile, getSize(inputfile)*2 ,h_dis); //set up vecotr on nodes

    if (listofNode.size() == 0) 
        return 1 ; 
   // print_h_nodes(listofNode, listofNode.size());

    Route_h ans = Astar(listofNode, start, end, h_dis);
    if (ans.real_cost == -1)
    {
        std::cout << "Nodes popped: " << pops << std::endl;
        std::cout << "nodes expanded " << exanded << std::endl;
        std::cout << "nodes genrated " << genrated << std::endl;
        //std::cout << "Distance " << ans.total_cost << std::endl;
        printf("Distance: Infinity\n");
        std::cout << "Route:" << std::endl;
        std::cout << "None\n";
    }
    else
    {
        print_route_h(ans);
        
    }
    return 0;
  
    
}


/*
* Input: 
    argc: integer number of arguments passed
    argv: array of arrguments passed
* Returns:: int: 0 on success
* Use: MAIN method
*/
int main(int argc, char* argv[])
{
    
    /*
    * Argv should be task1.exe inputfile start end hfile (informed)
    * or  task1.exe inputfile start end  (uninformed)
    */
    int cheker = 0;
    if (argc < 4) 
    {
        std::cout << "not enough input files passed";
        return -1;
    }
    else if(argc == 4) 
    {
        cheker= uninformed(argv[1],argv[2],argv[3]);
    }
    else if (argc == 5)
    {
        cheker= informed(argv[1], argv[2], argv[3],argv[4]);
    }
    else
    {
        std::cout << "too many input files passed try \"find_route input_filename origin_city destination_city heuristic_filename \" ";
        return -1;
    }
    return 0;
    

    
    
}

