#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

/** MA433 final project code to find a 1-factorization for an n degree complete graph where n is even.
 * To compile:
 * g++ -O1 k-factorization.cpp -o k-factorization
 * To run:
 * ./k-factorization <n> <inputfile> 
 * n is order of the graph. Must be even.
 * Optional input text file contains one vertex label per line.
 *
 * */

/** One match played between 2 players in a given round
 * represented as pair of ints where each int is the id of the player  */
typedef pair<int,int> match;

/* Class representing a round with multiple matches in the tournament */
class Round {
  public:
    vector<match> matches; //vector of matches in this round

    Round(int k){
        match temp {0,0};
        matches = vector<match>(k,temp);
    }

    /** Verification */
    /** return true if matches include same teams  */
    static bool equals(const match& a, const match& b){
      return (a.first == b.first) && (a.second == b.second) 
	      || (a.second == b.first) && (a.first == b.second) ;
    }
    /** return true if match appears twice WITHIN round */
    bool thereisaMatch(){
      bool flag = 0;   
      int size = matches.size();
      for(vector<match>::iterator a = matches.begin(); a!=matches.end(); a++){
        for (vector<match>::iterator b = a+1; b!=matches.end(); b++) {
	       	if (Round::equals(*a,*b)) { flag = 1; };
        }	
      }
      return flag;
    }
    /** return true if a match appears twice BETWEEN rounds*/
    bool thereisaMatch(Round& otherRound){
      bool flag = 0;   
      for(vector<match>::iterator a = matches.begin(); a!=matches.end(); a++){
        for (vector<match>::iterator b = otherRound.matches.begin(); b!=otherRound.matches.end(); b++) {
	       	if (Round::equals(*a,*b)) { flag = 1; };
        }	
      }
      return flag;
    }

    /* pretty prints the round to string*/
    string toString(){
      string daString = "";
      for(match theMatch : matches){
        daString += to_string(theMatch.first) + " and " +  to_string(theMatch.second) + ", ";
      }
      return daString;
    }
    /* pretty prints the round to string with labels*/
    string toString(const vector<string>& labels){
      string daString = "";
     for(match theMatch : matches){
       daString += labels[theMatch.first] + " and " +  labels[theMatch.second] + " , ";

     }
      return daString;
    }

};

/* Class representing a round robin style tournament with multiple rounds*/
class Tournament {
  public: 
	  
    vector<Round> rounds; //rounds in the tournament
    int n; // the number of vertices. Must be even.
  
    Tournament(int theN): n(theN){
      rounds  = vector<Round>(n-1,Round(n/2)); // n-1 rounds, each with n/2 games
    } 

    /** Application of cyclic 1-factorization from book*/
    void schedule(){
	 for (int i = 1;i<n;i++){
           rounds[i-1].matches[0] = {0,i};
           for(int j = 1;j<n/2;j++){
		   int theThing = (i+j)%(n-1); //modular arithmetic to calculate 1st team
		   int otherThing = (i+n-1-j)%(n-1); //modular arithmetic to calculate 2nd team
		   theThing = (theThing==0)?n-1:theThing; //if team = 0, replace with n-1 because 0 plays in 1st game 
		   otherThing = (otherThing==0)?n-1:otherThing;
            rounds[i-1].matches[j] = {theThing,otherThing};   
           }
        }
    }

    /** Return true if no repeat matches*/
    bool verify(){
      bool flag = 0; 
      for (vector<Round>::iterator theRound = rounds.begin();theRound!=rounds.end();theRound++){
        if (theRound->thereisaMatch()) {flag = 1; }
        for(vector<Round>::iterator otherRound = theRound+1; otherRound!=rounds.end();otherRound++){
          if (theRound->thereisaMatch(*otherRound)) {flag = 1; }
        } 
      } 
       return !flag;
    }

    /** Pretty print all matches in all rounds in tournament */
    string& toString(){
      string* pDaString  = new string("");
      for (Round theRound : rounds){
	*pDaString += theRound.toString();
        *pDaString += "\n";
      }
      return *pDaString; 
   }
   /** Pretty print all matches in all rounds with labels */
   string& toString(string inFilePath){
      string line;
      vector <string> labels;
      ifstream inFile(inFilePath);       
      int lineCount= 0;
      string* pDaString = new string("");
      if (inFile.is_open()) {
        while (getline (inFile,line) && lineCount < n )
        {
	   line.erase(line.size()-1,1);
           labels.push_back(line); 
           lineCount++;
        }
        inFile.close();
	if (lineCount < n){ cout << "Not enough labels to cover all vertices" << endl;}
     } else cout << "Unable to open file"; 

      for (Round theRound : rounds){
	*pDaString += theRound.toString(labels);
        *pDaString += "\n";
      }
     return *pDaString; 
     }
};

int main(int argc, char* argv[] ){
  if (argc <2){ //some input validation
    cout << "Not enough input arguments" << endl;
  }
  else if(stoi(argv[1])%2!=0){
    cout << "n must be even" << endl; 

  }
  else if(stoi(argv[1])<=0){
    cout << "positive non zero n please" << endl; 
  }
  else {
    int n = stoi(argv[1]);
    Tournament sln = Tournament(n);
    sln.schedule();
    if (!sln.verify()){
       cout << "Error! solution not correct" << endl;   
    }
    if (argc ==2){
      cout << sln.toString();
    }
    else{
      cout << sln.toString(argv[2]);
    }
  }

}
