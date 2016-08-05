//Meant to provide both the real and imaginary solutions to polynommial equations in standard form
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
#include <deque>
#include <math.h>
using namespace std;


int findDegree(string processedinput){		//A function called by the main function to determine the degree of the polynomial
	int degree_int;
	char degree_char = '5';
	for (unsigned int d=0; d <= (processedinput.length()-1); d++)
	{
		if (processedinput[d] == '^')
			{
			degree_char = processedinput[(d+1)];		//This makes the degree equal to the first char after the first carat
			degree_int = degree_char - 48;		//Uses ASCII numbers
			//cout << "degree int = " << degree_int; //Uncomment this to debug to display the number of terms
			return degree_int;
			}
	}
	for (unsigned int cd=0; cd <= (processedinput.length()-1); cd++) //Checks for degree 1
		if (processedinput[cd] == 'x' || processedinput[cd] == 'X')
			return 1;
	return 0; } //Returns degree zero if no x's or ^'s exist in the input

deque<int> stringToInt(deque<string> stringArray){	//Used to change the type of a deque from string to int
		deque<int> intArray(stringArray.size());		//Declares new deque of string type

		for (unsigned int j=0; j <= (stringArray.size() - 1); j++)
		{
			string item = stringArray[j];
			if (item.length() == 1)			//Converts a string of length one (the degreeArray) into an int
				intArray[j] = item[0] - 48;
			else
			{
			string tempItem = "";
			for (unsigned int k=(item.length() - 1); k>0; k--)		//Converts a string into an int
			{
				if (item[k] == '0' || item[k] == '1' || item[k] == '2' || item[k] == '3' || item[k] == '4' || item[k] == '5' 
					|| item[k] == '6' || item[k] == '7' || item[k] == '8' || item[k] == '9')
					tempItem = item[k] + tempItem;
			}
			stringstream converter(tempItem);
			converter >> intArray[j];
			if (item[0] == '-')
				intArray[j] *= -1;
			}
		}		//Ends first for loop
		
	return intArray; }		//Ends stringToInt function

deque<float> findPossibleSolutions(deque<int> coefficients){			//Used to find all possible solutions using the p/q method
	deque<int> factorsLeadingCoef, factorsConstant;
    deque<float> possibleSolutions;
    int leadingCoef = coefficients[0], constant = coefficients[(coefficients.size() - 1)];	//Creates variables for the leading coefficient (q) and constant (p)

	if (leadingCoef == 0 || constant == 0)		//Returns 0 if either the leading coefficient or the constant is zero
		{
			possibleSolutions.push_front(0);
			return possibleSolutions;
		}
	if (leadingCoef < 0)		//Guarantees that both numbers will be positive
		leadingCoef *= -1;
	if (constant < 0)			//Ditto
		constant *= -1;

    for (unsigned int f=1; f <= ((float)(leadingCoef/2)); f++)		//Adds factors of leading coefficient to factorsLeadingCoef
    {
		if (leadingCoef % f == 0)
		factorsLeadingCoef.push_back(f);
	}
	for (unsigned int f2=1; f2 <= ((float)(constant/2)); f2++)		//Adds factors of constant to factorsConstant
    {
		if (constant % f2 == 0)
		factorsConstant.push_back(f2);
	}

	factorsLeadingCoef.push_front(leadingCoef);			//Adds the leading coefficient to its factor deque
    factorsConstant.push_front(constant);			//Adds the constant to its factor deque
    
    for (unsigned int d=0; d <= factorsLeadingCoef.size() - 1; d++) //Adds all p/q floats to possibleSolutions deque
    {
        int currentQ = factorsLeadingCoef[d];
        for (unsigned int e=0; e <= factorsConstant.size() - 1; e++)
        {
            int currentP = factorsConstant[e];
            possibleSolutions.push_back((float)currentP/(float)currentQ);
        }
    }
    
    deque<float> duplicateList;        //This list will be the duplicate of possibleSolutions and will be used to delete repeats
	bool duplicate = false;				// This bool stores whether or not the number is duplicated
    for (int b=0; b <= possibleSolutions.size() - 1; b++)      //This will actually duplicate possibleSolutions into duplicateList
    {
		if (b==0) duplicateList.push_front(possibleSolutions[b]); //Adds the first item in possibleSolutions no matter what

        for (int g=0; g <= duplicateList.size() - 1; g++)
        {
            if (possibleSolutions[b] == duplicateList[g])
				duplicate = true;
        }
		if (duplicate == false)		//If the number makes it through the whole loop without being 
			duplicateList.push_back(possibleSolutions[b]);
		else
			duplicate = false;
    }
    
    const int realSize = duplicateList.size();        //This is the initial size of duplicateList so that as floats are appended, an infinite loop won't start
    for (unsigned int h=0; h <= realSize - 1; h++)
    {
        duplicateList.push_back((-1.0)* duplicateList[h]); //Ensures that all values have their negative opposites included in the possibility set
    }             
	return duplicateList; }		//Ends findPossibleSolutions function

deque<float> syntheticDivision(deque<float> polynomial, deque<float> workingSolutions, int x){
	deque<float> newPoly(10);

	newPoly[0] = polynomial[0];
	for (unsigned int b=1; b <= polynomial.size() - 2; b++)
	{
		newPoly[b] = ((float)(polynomial[b-1] * workingSolutions[x]) + polynomial[b]);
	}
	newPoly.shrink_to_fit();
	return newPoly; }

//Begins main function---------------------------------------------------------------------------------------------------------------------------------------------

int main(){

	//begins the input and output process in a repeatable manner because of the while loop
	cout << "Welcome to Equation Solver Pro.\n\n";

	string ifcontinue = "yes";

	while (ifcontinue == "yes" || ifcontinue == "Yes") {
		cout << "Please enter one polynomial equation in standard form as below:" << endl << "ax^n+bx^(n-1)+c=0: ";
		string input;
		getline(cin, input);

		// cout << "." << input[(input.length()-2)] << ". ." << input[(input.length()-1)]<<"."; (Uncomment this to test if bugs occur)

		if (input.length() <= 1 )		//Checks to make sure the user entered a string longer than just one character!
			{
				cout << endl << "Sorry, but you entered one character." << endl;
				continue;
			}
		if (input[(input.length()-2)] != '=' || input[(input.length()-1)] != '0')		//Checks for the equals sign and the equals zero
			{
				cout << endl << "Sorry, but you forgot the equals sign or the zero." << endl;
				continue;
			}

		string processedinput;		//Eliminates spaces so that the input can be further processed
		for (unsigned int i=0; i < (input.length()-2); i++)
		{
			if (input[i] != ' ')
				processedinput += input[i];
		}

		//cout << processedinput << endl; (Uncomment this to display processedinput if errors occur)


		/*  The for loop below checks to make sure the input is in the format "+/-,a,x,^,n" for the first terms; then "+/-,a,x"; then "+/-,a" and then creates the array.
		The variable declarations below and findDegree test prepare the for loop with what it needs.  */
		
		string shouldbe = "+/-";		//These strings store what kind of character the current term should be and can be.
		string canAlsobe = "+/-";		//If the current kind is neither the shoulbe nor the canAlsobe, then the user made a typing error, and the program will restart.
		string restart = "no";			//This variable will cause the program to restart after exiting the for loop
		string coefOrDegree = "coef";		//This variable determines whether the current integer will be added to the coefArray or the degreeArray

		int degree = 0;			//This integer stores the degree of the polynomial. ie: fifth degree, cubic, quadratic, etc. (the 0 is just temporary)
		if (findDegree(processedinput) != 999)		//If findDegree returns 999, it is because of a user input error
			degree = findDegree(processedinput);
			//cout << "degree= " << degree << endl;} //Uncomment this to print the degree

		else { cout << endl << "Sorry, but you entered the polynomial in the wrong form." << endl;
			continue;}		//This requires the user to restart, preventing a future error

		int currentTerm = 0;		//This is the number of the current term on which the for loop is iterating
		int termsLeft = degree + 1;			//This variable is used to denote when the last two terms begin, because their forms are different. ("+/-,a,x"; then "+/-,a")
		deque<string> coefArray(degree+1);		//Prepares an array to store the coefficient of each term
		deque<string> degreeArray(degree+1);		//Prepares a second array to store the degree of each term
		if (processedinput[0] != '-')
			processedinput = "+" + processedinput;		//Makes sure that processedinput starts with a plus or a minus sign
		// cout << processedinput << endl; //Uncomment this to print processedinput

		for (unsigned int e=0; e <= (processedinput.length()-1); e++)
		{
			char currentChar = processedinput[e];		//This is the character on which the for loop is currently iterating
			
			if ((currentChar == '0' || currentChar == '1' || currentChar == '2' || currentChar == '3' || currentChar == '4' || currentChar == '5' || currentChar == '6' || 
				 currentChar == '7' || currentChar == '8' || currentChar == '9' ) && (shouldbe == "int" ||  canAlsobe == "int"))  
					//The coefficient or exponent (a or n) in the term "+/-,a,x,^,n"
			{
				if (currentTerm == degree)
				{
					shouldbe = "int";
					canAlsobe = "int";
					coefArray[currentTerm] += currentChar;
					degreeArray[currentTerm] = '0';
				}
				else
				{
					if (coefOrDegree == "coef")
					{
						coefArray[currentTerm] += currentChar;
						shouldbe = "int";
						canAlsobe = "x";
					}
					else 
					{
						degreeArray[currentTerm] += currentChar;
						shouldbe = "int";
						canAlsobe = "+/-";
					}
				}
			}

			else if ((currentChar == 'x' || currentChar == 'X' )&& (shouldbe == "x" ||  canAlsobe == "x"))		//The variable part of the term "+/-,a,x,^,n"
			{
				if (processedinput[e-1] == '+' ||  processedinput[e-1] == '-')
				{
					coefArray[currentTerm] = '1';
					shouldbe = "^";
					canAlsobe = "^";
				}
				if (currentTerm == degree - 1)
				{
				shouldbe = "+/-";
				canAlsobe = "+/-";
				degreeArray[currentTerm] = '1';
				}
				else
				{
				shouldbe = "^";
				canAlsobe = "^";
				}
				coefOrDegree = "degree";
			}

			else if (currentChar == '^' && (shouldbe == "^" ||  canAlsobe == "^"))		//The carat part of the term "+/-,a,x,^,n"
			{
				shouldbe = "int";
				canAlsobe = "int";
			}

			else if ((currentChar == '+' || currentChar == '-') && (shouldbe == "+/-" ||  canAlsobe == "+/-"))		//The sign part of the term "+/-,a,x,^,n"
			{
				if (e != 0)
				{
						currentTerm += 1;
						termsLeft -= 1;
				}

				coefOrDegree = "coef";
				coefArray[currentTerm] = processedinput[e];
				shouldbe = "int";
				canAlsobe = "x";
			}

			else 
			{
				cout << endl;
				restart = "yes";
				break;
			}
			
		}		//End of for loop which arranges the two deques

		if (restart == "yes")		//Comes from the else clause in the above for loop in case of a user-input error
		{
			cout << "Sorry, but you entered the polynomial incorrectly." <<endl << endl;
			continue;
		}

		deque<int> intCoefArray(coefArray.size());		//Prepares a deque to store the integer form of the coefficient deque
		intCoefArray = stringToInt(coefArray);
		deque<int> intDegreeArray(degreeArray.size());		 //Prepares a deque to store the integer form of the degree deque
		intDegreeArray = stringToInt(degreeArray);

		/* for (unsigned int g=0; g <= (intDegreeArray.size() - 1); g++)			//Uncomment these lines to print the contents of each deque
			cout << intDegreeArray[g] << ", ";
		cout << endl;

		for (unsigned int l=0; l <= (intCoefArray.size() - 1); l++)
			cout << intCoefArray[l] << ", ";
		cout << endl; */

		for (unsigned int d=0; d < (intDegreeArray.size() - 1); d ++)		//Checks to make sure that the degree of each term descends properly, ie. 5,4,3,2,1
		{
			if (intDegreeArray[d] != intDegreeArray[d+1] + 1)
			{
				cout << "Sorry, but you entered the polynomial incorrectly." <<endl << endl;
				continue;
			}
		}


		//Actual solution finding begins below ------------------------------------------------------------------------------------------------------------

		deque<float> possibleSolutions = findPossibleSolutions(intCoefArray);		//creates the possibleSolutions array using the p/q method
		deque<float> tempTerms;				//tempTerms holds the evaluated state of each term in the array using intCoefArray * (possibility
		deque<float> workingSolutions;		//These solutions work. They will be used later to synthetically divide.
		float testSum;
		float possibility;

		if (possibleSolutions.size() > 0)
		{
			for (int p=0; p <= possibleSolutions.size() - 1; p++)		//Tests each of the possibilities
			{
				tempTerms.clear();
				possibility = possibleSolutions[p];		//Plugs each possibility into each term using the coefArray and degreeArray deques
				
				for (int t=0; t <= intCoefArray.size() - 1; t++)
				{
					float temporary = 0;
					temporary = (float)(pow(possibility, intDegreeArray[t]));
					temporary *= (float)(intCoefArray[t]);
					tempTerms.push_back(temporary);
				}
				testSum = 0.0;
				for (int wa=0; wa <= tempTerms.size() - 1; wa++)		//Sums each of these terms in the equation
				{
					testSum = testSum + (float)(tempTerms[wa]);
				}
				if (testSum < 0.1 && testSum > -0.1)				//Tests to see if the possibility is actually a solution (sums to zero)
				{
					workingSolutions.push_back(possibility);
				}
			}
		}


        deque<float> realSolutions_int = workingSolutions;		//Declares the deques which will be used to store the real solutions in the integer type
        deque<string> imagSolutions_str;		//Declares the deques which will be used to store the real & imaginary STRING solution versions
		deque<float> polyToBeDivided;			//The acting polynomial (here) will change during each synthetic division
		deque<float> lowestDegreePoly;		//The soon to be lowest degree polynomial (degree of two, length of three)

		//if (intCoefArray.size() == 2)		//For equations like 7x+4=0
			//realSolutions_int.push_back((float)((0 - intCoefArray[1])/intCoefArray[0]));

		if ((realSolutions_int.size() <= (intCoefArray.size() - 3)) && intCoefArray.size() > 2) //Only does quadratic formula if more solutions are needed
		{
			for (unsigned int z=0; z <= (intCoefArray.size() - 1); z++) //Adds all the coefficients of the intCoefArray to polyToBeDivided & lowestDegreePolyArray
			{
				polyToBeDivided.push_back((float)(intCoefArray[z]));
				lowestDegreePoly.push_back((float)(intCoefArray[z]));
			} /* The only reason that polyToBeDivided and lowestDegreepoly are BOTH created (this is odd because they are always equal) is because
			  one has to be cleared before it can be made to equal the syntheticDivision function of itself. */

			int attempt = 0;
			while (polyToBeDivided.size() > 3 && (workingSolutions.size() >= (attempt - 1)))		//This function recursively synthetically divides the current lowest degree polynomial until its degree is two
			{
				lowestDegreePoly.clear();
				lowestDegreePoly = syntheticDivision(polyToBeDivided, workingSolutions, attempt);
				polyToBeDivided.clear();
				polyToBeDivided = lowestDegreePoly;
				attempt += 1;
			}

			float a = lowestDegreePoly[0], b = lowestDegreePoly[1], c = lowestDegreePoly[2];	//Begin quadratic formula
			float d = (b * b) - (4 * a * c);
				if(d > 0)
				{
					realSolutions_int.push_back((-b + sqrt(d))/(2 * a));
					realSolutions_int.push_back((-b - sqrt(d))/(2 * a));
				}
				else if (d == 0)
				{
					realSolutions_int.push_back(-b/(2 * a));
				}
				else
				{
					float constTerm_float = (-b/(2 * a)), iTerm_float = (sqrt(-d)/(2 * a)); //Parts of an imaginary solution
                
					stringstream intToString;					//The below turns the i term into a string and adds the "i" to it
					intToString << constTerm_float;
					string constTerm_str = intToString.str();
					if (constTerm_str == "0" || constTerm_str == "-0")
						constTerm_str = "";
  
					intToString.clear();
					intToString.str("");
                
					intToString << iTerm_float;
					string iTerm_str = intToString.str();
					string iTerm_str2 = iTerm_str;
					if (constTerm_str == "")
					{
						iTerm_str += "i";
						iTerm_str2 = "-" + iTerm_str2 + "i";
					}
					else
					{
					iTerm_str = " + " + iTerm_str + "i";
					iTerm_str2 = " - " + iTerm_str2 + "i";
					}
                
					imagSolutions_str.push_back(constTerm_str + iTerm_str);		//Adds both imaginary solutions to the imagSolutions_str list
					imagSolutions_str.push_back(constTerm_str + iTerm_str2);
				}
		} //End of quadratic for loop

		if (realSolutions_int.size() > 0 )
		{
			for (unsigned int pr=0; pr <= realSolutions_int.size() - 1; pr++)
			{
				if (realSolutions_int[pr] == -0)
					realSolutions_int.erase(realSolutions_int.begin() + pr);
			}
			
		}
		if (imagSolutions_str.size() > 0 )
		{
			for (unsigned int ir=0; ir <= imagSolutions_str.size() - 1; ir++) //Elmiminates the "1" coefficient from the iTerm if it exists
			{
				if (imagSolutions_str[ir] == "-1i")
					imagSolutions_str[ir] = "-i";
				if (imagSolutions_str[ir] == "1i")
					imagSolutions_str[ir] = "i";
			}
		}
		//Final printing below ---------------------------------------------------------------------------------------------------------------------------

		cout << endl << "The real solutions are: ";
		if (realSolutions_int.size() == 0)			//If there are no real solutions, the program will print "None"
			cout << "None";
		else
		{
			for (unsigned int r=0; r <= realSolutions_int.size() - 1; r++)
			{
				if (r == realSolutions_int.size() - 1)
				{
					if (realSolutions_int[r] == -0)
						cout << 0;
					else cout << realSolutions_int[r];
				}
				else cout << realSolutions_int[r] << ", ";
			}
		}
		cout << endl << "The imaginary solutions are: ";
		if (imagSolutions_str.size() == 0)			//If there are no imaginary solutions, the program will print "None"
			cout << "None";
		else
		{	
			for (unsigned int is=0; is <= imagSolutions_str.size() - 1; is++)
			{
				if (is == imagSolutions_str.size() - 1)
					cout << imagSolutions_str[is];
				else cout << imagSolutions_str[is] << ", ";
			}
		}

		//Startover begins here --------------------------------------------------------------------------------------------------------------------------
		do {
		cout << endl << endl << "Would you like to solve another equation?: ";
		getline(cin, ifcontinue);
		} while (ifcontinue != "yes" && ifcontinue != "Yes" && ifcontinue != "no" && ifcontinue != "No");

	cout << endl;
	}		//End of while loop and program

	return 0; }