/**
 * COP 3503 - Assignment 2: Caravan
 * MST created through Kruskal's Algorithm
 * No arraylist created for MST though as I don't need to read from the MST
 * @author Derek McCrae
 */

import java.util.*;

public class Caravan{
    public static void main (String[] Args){
	Scanner read = new Scanner(System.in);
	int numCities = 0, i = 0;
	if(read.hasNextInt())
	    numCities = read.nextInt();
	int numRoads = 0;
	if(read.hasNextInt())
	    numRoads = read.nextInt();
        Map map = new Map(numCities);

	//read in each edge and add to gragph
	int city1, city2, cost, weight_cap;
	for(i = 0; i < numRoads; i++){
	    city1 = -1;
	    city2 = -1;
	    cost = 0;
	    weight_cap = 0;
	    if(read.hasNextInt())
		city1 = read.nextInt() - 1;
	    if(read.hasNextInt())
		city2 = read.nextInt() - 1;
	    if(read.hasNextInt())
		cost = read.nextInt();
	    if(read.hasNextInt())
		weight_cap = read.nextInt();
	    //ensure two cities given are not the same
	    if(city1 == city2)
		continue;
	    map.addRoad(city1, city2, cost, weight_cap);
	}

	//read in final parameters for project
	int budget = 0, wagon_cost = 0, total_weight = 0;
	if(read.hasNextInt())
	    budget = read.nextInt();
	if(read.hasNextInt())
	    wagon_cost = read.nextInt();
	if(read.hasNextInt())
	    total_weight = read.nextInt();

	//declare variables used to determine valid construction routes
	int current_budget = 0;
        double min_cap = 0.0;
	int[] possible_wagons = new int [10];
	int count = 0;
	for(i = 0; i < 10; i++){
	    possible_wagons[i] = 0;
	}

	//run through each purchase of i amount of wagons to determine is valid MST available
	for(i = 0; i < 10; i++){
	    //calculate budget & break if no budget
	    if(budget == 0)
		break;
	    current_budget = budget;
	    current_budget = current_budget - ((i + 1) * (wagon_cost));
	    min_cap = (double) total_weight / (i + 1);

	    if(map.MST(min_cap, current_budget) == true){
		possible_wagons[i] = i + 1;
		count++;
	    }
	}

	//print to output solution(s)
	System.out.printf("%d\n", count);
	for(i = 0; i < 10; i++){
	    if(possible_wagons[i] != 0)
		System.out.printf("%d ", possible_wagons[i]);
	}
        System.out.println();
    }//end main

    static class Road{
	//declare Road variables
	int city1, city2, cost, weight_cap;

	public Road(int start_city, int end_city, int road_cost, int road_cap){
	    //assign input values to Road structure
	    city1 = start_city;
	    city2 = end_city;
	    cost = road_cost;
	    weight_cap = road_cap;
	}
    }//end Road declaration & intialization

    static class Map{
	//declare variables map for allRoads
	int cities, i = 0;
	ArrayList<Road> allRoads = new ArrayList<>();

	Map(int total_cities){
	    //passing numNodes to Map class
	    cities = total_cities;
	}

	public void addRoad(int start_city, int end_city, int road_cost, int road_cap){
	    //add edge function with our parameters
            Road edge = new Road(start_city, end_city, road_cost, road_cap);
	    allRoads.add(edge);
	}

	public boolean MST(double min_cap, int budget){
	    //declare priority queue
	    //size is equal to size of previously created allRoads arraylist & comparing each edge by cost so list sorted least to greatest
	    PriorityQueue<Road> sortedRoads = new PriorityQueue<>(allRoads.size(), Comparator.comparingInt(road -> road.cost));

	    //adding each road to priority queue
	    for(i = 0; i < allRoads.size(); i++){
		sortedRoads.add(allRoads.get(i));
	    }

	    //declare & intialize parent array
	    int[] parent = new int[cities];
	    for(i = 0; i < cities; i++){
		parent[i] = i;
	    }
            
	    //declare variables used for while loop - then loop to build MST until N-1 nodes in MST
	    int mst_edges = 0, build_cost = 0, start_city, end_city;
            i = 0;
	    while(mst_edges < cities - 1){
		Road edge = sortedRoads.poll();

		//check for cycle & avoid adding roads that cant handle caravan distrubuted capactiy
		start_city = find(parent, edge.city1);
		end_city = find(parent, edge.city2);
                double road_capacity = edge.weight_cap * 1.0;
		if(start_city != end_city && road_capacity >= min_cap){
		    //passed valid test so add road to MST
		    build_cost += edge.cost;
		    mst_edges++;
		    union(parent, start_city, end_city);
		}
                i++;
                if(i >= allRoads.size())
                    break;//ensure that if all edges tested, loop ends
	    }
	    
	    if(mst_edges == cities - 1 && budget >= build_cost){
		return true; //valid MST & parameters met
            }
	    else
		return false; //either invalid MST or build cost too high
		
	}
	
	//find function helps check for cycle
	public int find(int[] parent, int city){
	    if(parent[city] != city)
		return find(parent, parent[city]);
	    return city;
	}

	//function to union a road into parent array
	public void union(int[] parent, int start_city, int end_city){
	    parent[find(parent, end_city)] = find(parent, start_city);
	}
    }//end Map
}//end of program

