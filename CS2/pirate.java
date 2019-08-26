/**
 * author: Derek McCrae
 * COP 3503 - Assignment 3: Greedy Pirates
 * Greedy algorithm
 */

import java.util.*;

public class pirate{
    public static void main(String[] args){
	//declare scanner & variables
	Scanner read = new Scanner(System.in);
	int x1, x2, y1, y2, numPirates, temp = 0;

	//read in variables
	x1 = read.nextInt();
	y1 = read.nextInt();
	x2 = read.nextInt();
	y2 = read.nextInt();
	if(y2 != y1){
	    System.out.printf("0\n");
	    return;
	}
	numPirates = read.nextInt();
	if(x1 > x2){
	    temp = x2;
	    x2 = x1;
	    x1 = temp;
	}

	//read in each chair position then find interval of which a pirate can see from their chair
	Greedy everyone = new Greedy(numPirates);
	int x, y;
	double slope1, slope2, startLoot, endLoot;
	for(int i = 0; i <  numPirates; i++){
	    //Pirate i + 1 location
	    x = read.nextInt();
	    y = read.nextInt();

	    //compute slope
	    slope1 = (y - y1)*1.0/(x - x1);
	    slope2 = (y - y2)*1.0/(x - x2);
	    

	    //compute wall intercepts
	    startLoot = (-y1/slope1) + x1;
	    endLoot = (-y2/slope2) + x2;

	    //add pirates interval to arraylist
	    everyone.addPirate(startLoot, endLoot, i+1);
	}

	//call starts solution process
	everyone.solutions();
	
    }//end static main

    static class Interval implements Comparable<Interval>{
	double start, end;
	int pirate;
	public Interval(double begin, double finish, int numPirate){
	    start = begin;
	    end = finish;
	    pirate = numPirate;
	}

        public int compareTo(Interval o){
	    if(o.end > end)
		return 1;
	    else if(o.end < end)
		return -1;
	    else
		return 0;
	}
    }

    static class IntervalComparator implements Comparator<Interval>{
	public int compare(Interval one, Interval two){
	    if(one.end > two.end)
		return -1;
	    else if(one.end < two.end)
		return 1;
	    else
		return 0;
	}
    }

    static class Greedy{
	//declare variables
	int totalPirates;
	ArrayList<Interval> allPirates = new ArrayList<>(totalPirates);

	Greedy(int numPirates){
	    totalPirates = numPirates;
	}

	public void addPirate(double startLoot, double endLoot, int numPirate){
	    //add each pirate to array list
	    Interval pirate = new Interval(startLoot, endLoot, numPirate);
	    allPirates.add(pirate);
	}

	public void solutions(){
	    //declare variables
	    Collections.sort(allPirates);
	    PriorityQueue<Interval> possiblePirates = new PriorityQueue<>(totalPirates, new IntervalComparator());
	    Interval currentPirate;
	    Interval nextPirate;
	    int i, ans = 0, count, remove = 0;

	    //loop for each reveal until allPirates arraylist is empty or has 1 pirate left
	    while(allPirates.size() > 1){
		//declare variables
		remove = 0;
	        currentPirate = allPirates.remove(0);
		count = allPirates.size();
		
		for(i = 0; i < count; i++){
		    nextPirate = allPirates.get(i - remove);
		    //list is order greatest to least so only need to check if
		    if(currentPirate.start >= nextPirate.end){
			possiblePirates.add(nextPirate);
			allPirates.remove(i - remove);
			remove++;
		    };
		}
		//greedy choice - take first pirate in pq and add to current solution
		if(!possiblePirates.isEmpty()){
		    currentPirate = possiblePirates.poll();
		}

		//run through priorityqueue until empty to find max number of pirates in a certain reveal
		count = possiblePirates.size();
		for(i = 0; i < count; i++){
		    nextPirate = possiblePirates.poll();
		    if(currentPirate.start >= nextPirate.end){
			currentPirate = nextPirate;
		    }
		    else{
			allPirates.add(nextPirate);
		    }
		}

		//end of current reveal so add 1 to solution
		ans++;
		possiblePirates.clear();
	    }
	    //case if 1 pirate left in arraylist
	    if(allPirates.size() == 1){
		ans++;
	    }

	    //print miniumum reveals
	    System.out.printf("%d\n", ans);
	}
    }
}//end program

    
