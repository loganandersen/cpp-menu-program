// Logan Andersen
// February 12 2023
// 2023Sp CS 3060 002
// Insructor Rory Lewis JD PhD

// Homework 11.8

// The teachers assistant told me that I don't need to split up my
// files so This is going to all be one big file.
#include <iostream>
#include <iomanip>

using namespace std; 

//Enumerations

enum Status {STOP, CONTINUE};
enum Action {EDIT, SEARCH, PRINT, INVALID, ADD_MEMBER, DELETE_MEMBER, LIST, QUIT};

//Anything that is an actual part of the customer should be before
//EDIT_INVALID since I use this in loops. Edit invalid is used to
//denote the end of the attributes
enum EditAction {NAME,ADDRESS,STATE,CITY,ZIP,PHONE_NUMBER,BALANCE,LAST_PAYMENT,EDIT_INVALID,EDIT_QUIT};

//Structs

struct Date
{
  //day
  unsigned int day; 
  //month
  unsigned int month;
  //year
  unsigned int year; 
};

struct Customer
{
  //name
  string name ;
  //address
  string address ;
  //state
  string state;
  //city
  string city;
  //zip
  unsigned int zip ;
  //phone-number
  //I decided to make this a string because people do not do any
  //arethmetic with phone numbers, and it lets me get stuff like
  //"555-555-5555".
  string phoneNumber ; 
  
  //balance
  double balance ; 
  //date of last payment
  Date lastPayment ;

  bool show; 
};

// Function prototypes
Action getMenuAction();
Status doMenuAction(Action ,Customer [],size_t  );
string actionName(Action );
void printCustomerMenu(Customer [], size_t , bool );
size_t getIndexOfCustomer(Customer [], size_t ,Action );
bool customerArrayHasPeople(Customer [],size_t );
void editCustomer(Customer & );
EditAction getEditAction();
Status doEditAction(EditAction ,Customer &);
Customer makeCustomer();
Date makeDate(); 
void printDate(Date );
void printCustomer(Customer );
unsigned int getUnsignedInt(string );
string getString(string );
void searchCustomerArray(string , Customer [], size_t );
bool isSubstring(string , string );
double getBankAccountBalance(); 
// Constants
size_t MAX_CUSTOMERS = 10;

int main()
{
  // need an array
  Customer customers[MAX_CUSTOMERS];
  Status loopStatus = CONTINUE;
  Action mainAction = QUIT;

  //make all the customers invisible.
  for (Customer& i : customers)
    {
      i.show = false; 
    }
      
  
  //loop
  while (loopStatus == CONTINUE)
    {
      //ask for menu action
      mainAction = getMenuAction();
      //do the menu action
      loopStatus = doMenuAction(mainAction, customers, MAX_CUSTOMERS);
    }
  return 0 ; 
}

//enum Status {STOP, CONTINUE};
//enum EditAction {NAME,ADDRESS,STATE,CITY,ZIP,PHONE_NUMBER};

Action getMenuAction()
{
  char action ;
  // ask for a characer
  cout << "enter 'e' to edit a customer\nenter 'p' to print a customer\n"
       << "enter 'd' to delete a customer\n"
       << "enter 'a' to add or overwrite a customer\n"
       << "enter 'l' to list all customers\n"
       << "enter 's' to search the customer base\nenter 'q' to quit"
       << endl; 
  cout << "What action would you like to do: ";
  cin >> action ; 
  // case switch to return the right action
  switch(action)
    {
    case 'e' :
      return EDIT;
    case 'p' :
      return PRINT;
    case 'q' :
      return QUIT;
    case 'a':
      return ADD_MEMBER;
    case 'd' :
      return DELETE_MEMBER;
    case 'l' :
      return LIST;
    case 's' :
      return SEARCH; 
    default :
      return INVALID; 
    }
  return INVALID;
}

//enum Action {EDIT, PRINT, INVALID, ADD_MEMBER, DELETE_MEMBER, QUIT};
Status doMenuAction(Action action,Customer customerArray[],size_t length )
{
  //need a returnval
  Status returnval = CONTINUE ;
  //need an index number
  size_t index = 0;
  string searchExpression = ""; 

  // case switch with the various actions
  switch (action)
    {
      //for edit member, print, list, and delete member
    case EDIT :
    case DELETE_MEMBER :
    case PRINT :
    case LIST :
      //check if there are any members in the array the person can choose.
      if (!customerArrayHasPeople(customerArray,length))
	{
	  //if there aren't then print an error.
	  cout << "There aren't any customers in the array please add a customer with 'a'\n";
	  break;
	}
      //If there are then print the availaible customers
      printCustomerMenu(customerArray,length,action != LIST);
      //break if this is list, otherwise do the edit action on them
      if (action == LIST)
	{
	  cout << endl << endl; 
	  break ;
	}
      //fallthrough 
    case ADD_MEMBER :
      //get the index of the customer they want to do the action on  
      index = getIndexOfCustomer(customerArray, length,action );

      //Do the action that needs to be done on the customer. 
      switch (action)
	{
	case ADD_MEMBER:
	  //remove trailing newline from get index
	  cin.ignore();
	  customerArray[index] = makeCustomer();
	  break;
	case EDIT :
	  editCustomer(customerArray[index]);
	  break; 
	case DELETE_MEMBER :
	  customerArray[index].show = false;
	  break;
      //for print print all the values of the member.
	case PRINT :
	  printCustomer(customerArray[index]);
	  cout << endl << endl; 
	  break ;
	default :
	  // do nothing, I just have this so It dosen't give me useless warnings
	  break; 

	}
      break ; 
    case SEARCH :
      searchExpression = getString("Enter what the subset of the person's name to search for: ");
      searchCustomerArray(searchExpression,customerArray,length);
      
      break ;
    case QUIT:
      returnval = STOP;
      break;
    case INVALID :
      cout << "Invalid selection please try again" << endl;
      break;
    }
  return returnval;
}

string actionName(Action action)
{
  switch(action)
    {
    case EDIT :
      return "edit" ;
    case PRINT :
      return "print";
    case INVALID :
      return "invalid";
    case ADD_MEMBER :
      return "add";
    case DELETE_MEMBER :
      return "delete";
    case QUIT :
      return "quit";
    default :
      return "invalid";  
    }
}

void printCustomerMenu(Customer customerArray[], size_t length, bool withNumber)
{
  for (size_t i = 0 ; i < length ; i++)
    {
      if (customerArray[i].show)
	{
	  if (withNumber)
	    {
	      cout << i << ": " ;
	    }
	  cout << customerArray[i].name << endl; 
	}
    }
}

size_t getIndexOfCustomer(Customer customerArray[], size_t length,Action action )
{
  size_t index = 0;
  bool failed = true;
  const bool checkValid = action != ADD_MEMBER;
  while (failed)
    {
      cout << "Enter the index of the customer you would like to "
	   << actionName(action) << " (must be between 0 and " << length - 1 << "): " ;
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
      // not valid
      else if (checkValid && !customerArray[index].show)
	{
	  cout << "You can't " << actionName(action) << " this customer because it does not exist" << endl; 
	}
      else
	{
	  failed = false; 
	}
    }
    
	     
  return index;
}

bool customerArrayHasPeople(Customer customerArray[],size_t length)
{
  bool found = false ;

  for (size_t i = 0 ; i < length && !found ; i++)
    {
      found |= customerArray[i].show;      
    }
  return found;
}

//enum EditAction {NAME,ADDRESS,STATE,CITY,ZIP,PHONE_NUMBER,EDIT_INVALID,EDIT_QUIT};
// EditAction getEditAction();

void editCustomer(Customer & customer )
{
  Status loopStatus = CONTINUE;
  EditAction mainAction = EDIT_QUIT;

  //loop until quit
  while (loopStatus == CONTINUE)
    {
      //ask for edit action
      mainAction = getEditAction();
      //Remove the trailing newline
      cin.ignore() ;
      //do the edit action
      loopStatus = doEditAction(mainAction,customer);
    }
}



EditAction getEditAction()
{
  char action ;
  // ask for a characer
  
  cout << "What would part of the customer would you like to edit"
       << "type n for name, a for address, s for state, c for city\n"
       << "z for zipcode, p for Phone number, b for balance\n"
       << "d for last payment date, and q to quit\n"; 
  cin >> action ; 
  // case switch to return the right action
  switch(action)
    {
    case 'n' :
      return NAME;
    case 'a' :
      return ADDRESS;
    case 's':
      return STATE;
    case 'c' :
      return CITY;
    case 'z' :
      return ZIP;
    case 'p' :
      return PHONE_NUMBER ;
    case 'b' :
      return BALANCE;
    case 'd' :
      return LAST_PAYMENT;
    case 'q' :
      return EDIT_QUIT;
    default :
      return EDIT_INVALID; 
    }
  
}

Status doEditAction(EditAction action,Customer &customer)
{
  Status returnval = CONTINUE;
  switch (action)
    {
    case NAME :
      customer.name = getString("Enter the customer's name: ");
      break;
    case ADDRESS :
      customer.address = getString("Enter the customer's address: ") ;
      break;
    case STATE :
      customer.state = getString("Enter the customer's state: ");
      break;
    case CITY :
      customer.city = getString("Enter the customer's city: ") ;
      break;
    case ZIP :
      customer.zip = getUnsignedInt("Enter the customer's zipcode: "); 
      cin.ignore();
      break;
    case PHONE_NUMBER :
      customer.phoneNumber = getString("Enter the customer's phone number: ");
      break ; 
    case BALANCE :
      customer.balance = getBankAccountBalance();

      break ; 
    case LAST_PAYMENT :
      customer.lastPayment = makeDate();
      cin.ignore();
      break;
    case EDIT_INVALID :
      cout << "Invalid selection please try again: ";
      break;
    case EDIT_QUIT :
      returnval = STOP;
      break;
    }

  return returnval;
}

// doEditAction can add a new part to any customer variable so I
// don't have to repeat code
Customer makeCustomer()
{
  Customer temp ;
  for (int action = 0 ; action < EDIT_INVALID ; action++)
    {
      doEditAction(static_cast<EditAction>(action),temp); 
    }

  temp.show = true; 
  return temp;
}

Date makeDate()
{
  Date temp;
 
  temp.month = getUnsignedInt("Enter the month the customer made the last payment: "); 
  temp.day = getUnsignedInt("Enter the day the customer made the last payment: ");
  temp.year =  getUnsignedInt("Enter the year the customer made the last payment: ");

  return temp;
  
}

void printCustomer(Customer customer)
{

  cout << "The name of the customer is: " << customer.name << endl;
  cout << "The address of the customer is: " << customer.address << endl;
  cout << "The state of the customer is: " << customer.state << endl;
  cout << "The city of the customer is: " << customer.city << endl;
  cout << "The zip  of the customer is: " << customer.zip  << endl;
  cout << "The phone number  of the customer is: " << customer.phoneNumber  << endl;
  cout << fixed << showpoint << setprecision(2) << "The balance of the customer is: " << customer.balance  << endl;
  cout << resetiosflags( cout.flags()); 
  cout << "The last payment of the customer is: " ;
  printDate(customer.lastPayment) ;

  
}

void printDate(Date date)
{
  cout << setw(2) << setfill('0') ; 
  cout << date.month << "/" << setw(2) << date.day << "/" << setw(4) << date.year;
  cout << resetiosflags( cout.flags()); 
}

double getBankAccountBalance()
{
  double temp = 0;
  switch (2)
    {
      do
	{
	  case 1 :
	    // non numeric input
	    if (!cin)
	      {
		cout << "That is not a number please try again" << endl;
		cin.clear();
		cin.ignore(10000,'\n');
	      }
	    else
	      {
		cout << "Bank account ballance cannot be negative please try again" << endl;
	      }
	    //fallthrough
	  case 2 :
	    cout << "Enter the customer's bank account balance: ";
	    cin >> temp;
	} while (!cin || temp < 0 ) ;
    }
  return temp; 
}

unsigned int getUnsignedInt(string prompt)
{
  unsigned int temp;

  switch (2)
    {
      do
	{
	  case 1:
	    cout << "That is not a number, please try again" << endl ;
	    cin.clear();
	    cin.ignore(10000,'\n');
	    //fallthrough 
	  case 2: 
	    cout << prompt ;
	    cin >> temp ;
	} while (!cin) ; 
    }
  return temp; 
}

string getString(string prompt)
{
  string temp = ""; 
  switch (2)
    {
      do
	{
	  case 1:
	    cout << "The string must be non empty please try again" << endl ;
	    //fallthrough 
	  case 2: 
	    cout << prompt ;
	    getline(cin, temp); ;
	} while (temp == "") ; 
    }
  return temp; 
}

void searchCustomerArray(string searchExpression, Customer customerArray[], size_t length)
{
  bool found = false;
  for (size_t i = 0 ; i < length ; i++)
    {
      if (customerArray[i].show && isSubstring(searchExpression,customerArray[i].name))
	{
	  printCustomer(customerArray[i]);
	  cout << endl << endl ; 
	  found = true; 
	}
    }
  if (!found)
    {
      cout << "No customer matches\n"; 
    }
}

bool isSubstring(string search, string larger)
{
  bool found = false; 
  for (size_t i = 0 ; larger[i] != '\0' && !found   ; i++)
    {
      // if first element of search is equal to current character of
      // larger. 
      if (search[0] == larger[i])
	{
	  size_t j = 0;
	  //empty for loop, all I need to do is see if I run out of characters in search
	  for (j = 1 ; search[j] != '\0' && larger[i+j] != '\0' && search[j] == larger[i+j] ; j++);
	  // if the thing has been found
	  if (search[j] == '\0')
	    {
	      found = true; 
	    }
	}
    }

  return found;
}
  
