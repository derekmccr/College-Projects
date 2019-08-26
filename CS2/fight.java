/**
 * author: Derek McCrae
 * COP 3503 - Assignment 5: Epic Fight
 * Dynamic Programming
 */

import java.util.*;

public class fight{
    static class Move{
	int personalDamage, enemyDamage;

	public Move(int staminaCost, int damage){
	    personalDamage = staminaCost;
	    enemyDamage = damage;
	}
    }

    static class Outcome{
	//declare variables
	int xovierStamina0, xovierStamina1, ruffusStamina0, ruffusStamina1;

	public Outcome(int xStamina0, int xStamina1, int rStamina0, int rStamina1){
	    xovierStamina0 = xStamina0;
	    xovierStamina1 = rStamina1;
	    ruffusStamina0 = rStamina0;
	    ruffusStamina1 = xStamina1;
	}
    }
	    
    static class DP{
	//declare arrays
	ArrayList<Move> Ruffus = new ArrayList<>();
	ArrayList<Move> Xovier = new ArrayList<>();
	ArrayList<Outcome> combos = new ArrayList<>();
	int outcomes;

	public void addXovierMove(int selfDamage, int enemyDamage){
	    //add Xovier move to array list
	    Move move = new Move(selfDamage, enemyDamage);
	    Xovier.add(move);
	}
	
	public void addRuffusMove(int selfDamage, int enemyDamage){
	    //add Ruffus move to array list
            Move move = new Move(selfDamage, enemyDamage);
	    Ruffus.add(move);
	}

	public void addAllCombos(int xovierMoves, int ruffusMoves){
	    //declare variables
	    int xStamina0, rStamina1, rStamina0, xStamina1;
	    Outcome possibleCombo;
	    Move xovierCurrent, ruffusCurrent;
	    
	    //add combo to array list of all possible combos available
	    for(int x = 0; x < xovierMoves; x++){
		for(int r = 0; r < ruffusMoves; r++){
		    xovierCurrent = Xovier.get(x);
		    ruffusCurrent = Ruffus.get(r);
		    possibleCombo = new Outcome(xovierCurrent.personalDamage, ruffusCurrent.enemyDamage, ruffusCurrent.personalDamage, xovierCurrent.enemyDamage);
		    combos.add(possibleCombo);
		}
	    }
	}

	public void addCombo(int xStamina0, int rStamina1, int rStamina0, int xStamina1){
	    //add combo to current fight combo array list
            Outcome combo = new Outcome(xStamina0, rStamina1, rStamina0, xStamina1);
	    combos.add(combo);
	}
	
	public int solution(ArrayList<Integer> previousList, int arrLen, int numMovesAllowed, int fightTime, int xovierStamina, int ruffusStamina, int size){
	    //declare variables
	    int i, xStaminaHold, rStaminaHold, move = 0, x, r, j;
	    Outcome combo;
	    xStaminaHold = xovierStamina;
	    rStaminaHold = ruffusStamina;

	    //base case
	    //if arrayList holds max amount of moves allowed based on fightTime, stop adding and determine if list valid
	    if(size == numMovesAllowed){
		for(i = 0; i <= fightTime; i++){
		    x = xStaminaHold;
		    r = rStaminaHold;
		    combo = combos.get(previousList.get(move));
		    //if even time, only payment of move for each player
		    if(i % 2 == 0){
			xStaminaHold = xStaminaHold - combo.xovierStamina0;
			rStaminaHold = rStaminaHold - combo.ruffusStamina0;
			if(xStaminaHold > xovierStamina)
			    xStaminaHold = xovierStamina;
			if(rStaminaHold > ruffusStamina)
			    rStaminaHold = ruffusStamina;
		    }
		    //if odd time, take cost of move by each player
		    else{
			xStaminaHold = xStaminaHold - combo.ruffusStamina1;
			rStaminaHold = rStaminaHold - combo.xovierStamina1;
			if(xStaminaHold > xovierStamina)
			    xStaminaHold = xovierStamina;
			if(rStaminaHold > ruffusStamina)
			    rStaminaHold = ruffusStamina;
			move++;
		    }

		    //check stamina
		    if(i == fightTime){
			//if end of fight and Ruffus only player knocked out, valid list of combos
			if(xStaminaHold > 0 && rStaminaHold <= 0){
			    outcomes++;
			    return outcomes;
			}
			//if Ruffus still awake or Xovier knocked out, invalid list of combos
			else{
			    return outcomes;
			}
		    }
		    else{
			//if not end of fight yet, one player is out then not valid list of combos
			if(xStaminaHold <= 0 || rStaminaHold <= 0){
			    return outcomes;
			}
		    }
		}
		return outcomes;
	    }
	    
	    for(i = 0; i < arrLen; i++){
		//add combo to current possible combo list
		ArrayList<Integer> newList = new ArrayList<>(previousList);
		newList.add(i);
		outcomes = solution(newList, arrLen, numMovesAllowed, fightTime, xStaminaHold, rStaminaHold, size + 1);
	    }
	    return outcomes;
	}
    }
    
    public static void main(String[] args){
	//declare scanner & variables
	Scanner read = new Scanner(System.in);
	int fightTime, xovierMoves, xovierStamina, ruffusMoves, ruffusStamina, i, possibleOutcomes, x, r;
	int staminaCost, moveDamage, numMoves, possibleMoves;
	ArrayList<Integer> previousList = new ArrayList<>();
	DP fight = new DP();

	//read in match info
	fightTime = read.nextInt();

	//read in all info for Xovier
	xovierMoves = read.nextInt();
	xovierStamina = read.nextInt();
	for(i = 0; i < xovierMoves; i++){
	    //loop through each move listed for Xorvier
	    //2 ints for each line -> a = staminaCost, b = damageToEnemy
	    staminaCost = read.nextInt();
	    moveDamage = read.nextInt();
	    fight.addXovierMove(staminaCost, moveDamage);
	}

	//read in all info for Ruffus
	ruffusMoves = read.nextInt();
	ruffusStamina = read.nextInt();
	for(i = 0; i < ruffusMoves; i++){
	    //loop through each move listed for Ruffus
	    //2 ints for each line -> a = staminaCost, b = damageToEnemy
	    staminaCost = read.nextInt();
	    moveDamage = read.nextInt();
	    fight.addRuffusMove(staminaCost, moveDamage);
	}
	
	//calculate each outcome for moves made by each person
        possibleMoves = ruffusMoves * xovierMoves;
	fight.addAllCombos(xovierMoves, ruffusMoves);
	
	//calculate number of moves that happen between both people
        numMoves = 0;
	if(fightTime == 0 || fightTime == 1)
	    numMoves = 1;
	else if(fightTime == 2)
	    numMoves = 2;
	else if(fightTime > 2 && fightTime % 2 == 0)
	    numMoves = (fightTime / 2) + 1;
	else if(fightTime > 2 && fightTime % 2 != 0)
	    numMoves = (fightTime+1) / 2;

	//call solution function that returns a version of answer. Print it out
	possibleOutcomes = fight.solution(previousList, possibleMoves, numMoves, fightTime, xovierStamina, ruffusStamina, 0) % 10007;
	System.out.printf("%d\n", possibleOutcomes);
    }
}
