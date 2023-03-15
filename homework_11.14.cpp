#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const int MIN_ITEMS = 0;
const int MAX_ITEMS = 30;

enum Status { CONTINUE , STOP };

struct Bin
{
  string name ; 
  int count;
};
  
struct Action
{
  string name;
  char key ;
  void (* action)(Bin *, int );
  Status returnVal; 
};

// function headers

void runSelectMenu( Bin * bin, int length);
Action getAction(const Action actionArray[] , string prompt);
void printActionArray(const Action []);
void doNothing(Bin * bin, int context );
void AddParts(Bin * bin, int useless);
void RemoveParts(Bin * bin, int useless);
void incrementOrDecrementBin(Bin & bin, int sign, int min, int max, string name);
int getIndexOfBin(Bin binArray[], int length);
int getIncrementValue (string name);
void printBins(Bin bins[], int length, bool );



////////////////////////////////////////////////////////////////////////////////

const Action nullAction = {"null", '\0', doNothing, CONTINUE};
const Action decrement = { "remove", 'd', RemoveParts, CONTINUE};
const Action increment = { "add", 'i', AddParts, CONTINUE};
const Action quit = {"quit", 'q', doNothing, STOP};
const Action selectAction = {"select", 's', runSelectMenu, CONTINUE}; 

//These must end with nullAction since I use that as a sigil value
const Action mainMenu [] = { selectAction, quit, nullAction}; 
const Action selectMenu [] = { increment, decrement, quit, nullAction};

int main ()
{
  Status loopStatus = CONTINUE;
  Action currentAction ;
  Bin binArray[10] = {
    {"Valve", 10},
    {"Bearing",5},
    {"Bushing",15},
    {"Coupling",21},
    {"Flange",7},
    {"Gear",5},
    {"Gear Housing", 5},
    {"Vacuum Gripper", 25},
    {"Cable", 18},
    {"Rod",12}
  };
  int length = sizeof(binArray)/sizeof(Bin); 
  
  while (loopStatus == CONTINUE)
    {
      printBins(binArray,length,false);
      currentAction = getAction(mainMenu,"What would you like to do: ");
      // do the action
      currentAction.action( binArray, length);
      loopStatus = currentAction.returnVal; 
    }
  return 0;
}

void runSelectMenu( Bin * binArray, int length)
{
  int index = getIndexOfBin(binArray,length);

  Action selectedAction = getAction(selectMenu,"What action would you like to do on the bin you selected: ");

  selectedAction.action( binArray + index,0);
}

Action getAction(const Action actionArray[] , string prompt)
{
  char key ;
  bool found = false ;
  Action returnVal ;

  while (!found)
    {
      printActionArray(actionArray);
	cout << prompt ;
	cin >> key;

    for (size_t i = 0; actionArray[i].key != '\0'; i++)
	  {
	    if (key == actionArray[i].key)
	      {
		found = true;
		returnVal = actionArray[i];
	      }
	  }
    }

  return returnVal; 
}

void printActionArray(const Action actionArray[])
{
  for (size_t i = 0; actionArray[i].key != '\0'; i++)
    {
      cout << "press: " << actionArray[i].key << " to " << actionArray[i].name << endl; 
    }
}

void doNothing(Bin * , int  )
{}

void AddParts(Bin * bin, int )
{
  incrementOrDecrementBin(*bin, 1, MIN_ITEMS,MAX_ITEMS,"add");
}

void RemoveParts(Bin * bin, int )
{
  incrementOrDecrementBin(*bin,-1,MIN_ITEMS,MAX_ITEMS,"remove");
}

void incrementOrDecrementBin(Bin & bin, int sign, int min, int max, string name)
{
  int newVal ;
  bool found = false;
  int increment = 0;

  while (!found)
    {
      increment = getIncrementValue(name);
      newVal = increment * sign + bin.count ;
      
      if ( newVal <= max && newVal >= min)
	{
	  found = true; 
	  bin.count = newVal;
	}
      else
	{
	  cout << "The number of items in the bin is out of range (min: " << min << " , max: " << max << ", sum: " << newVal << " )\n" ;
	  cout << "Please try again\n" ; 
	}
    }
}

  
int getIndexOfBin(Bin binArray[], int length)
{
  int index = 0;
  bool failed = true;
  while (failed)
    {
      printBins(binArray,length,true);

      cout << "Enter the index of the bin you would like to operate on"
	   << " (must be between 0 and " << length - 1 << "): " ;
      cin >> index;
      //not a number
      if (!cin)
	{
	  cout << "You have to enter a number please try again" << endl;
	  cin.clear();
	  cin.ignore(10000,'\n'); 
	}
      //number too big
      else if (index >= length )
	{
	  cout << "Number too big, please try again" << endl;
	}
      else if (index < 0 )
	{
	  cout << "Index is too small, please try again" << endl;
	}
      // not valid
      else
	{
	  failed = false; 
	}
    }
    
	     
  return index;
}

int getIncrementValue (string name)
{
  int returnVal ;
  bool failed = true;
  
  while (failed)
    {
      cout << "Enter a how many parts would you like to " << name << " by: ";
      cin >> returnVal;

      // invalid input
      if (!cin)
	{
	  cout << "You have to enter a number please try again" << endl;
	  cin.clear();
	  cin.ignore(10000,'\n'); 
	}
      //number too big
      else if (returnVal < 0  )
	{
	  cout << "Number cannot be negative, please try again" << endl;
	}
      else
	{
	  failed = false;
	}
    }

  return returnVal;
  
}

void printBins(Bin binArray[],int length, bool printIndex)
{
  if (!printIndex)
    cout << "Part Name" << right << setw(35) << "count\n";
  
  for (int i = 0; i < length ; i++)
    {
      Bin temp = binArray[i] ;
      if (printIndex == true)
	cout << i << ": ";
      cout << left << setw(40) << temp.name;
      if (!printIndex)
	cout << temp.count ;
      cout << endl; 
    }
}
