#include "Validation.h"



//===============================================
//METHOD :  Default constructor
//DESCR. :  
//===============================================
Validation::Validation()
{
	_length = 7;                                    //Sets a standart length

	for (size_t i = 0; i < _length; i++)            //sets a standart code
	{
		_code[i] = i + 1;
		_guess[i] = 0;
	}

	_numberCounter = 0;                             //Guess startes on place 0
}


//===============================================
//METHOD :  setValue
//DESCR. :  gets a number that will be matched with 
//           a code and returns the status.
//===============================================
int Validation::setValue(int number)
{
	//-------sets a number into an array-------
	_guess[_numberCounter] = number;                //Sets number into the 'guess' array

	_numberCounter++;                               //Counts the sloths and length one up for guess

	//-----------Matches lenght----------
	if (_numberCounter == _length)                  //Length defined from choicen code
	{
		_numberCounter = 0;

		//--------calls codeValue()---------
		if (codeVali() == 1)
		{
			return 1;                               //right kode
		}
		return 2;                                   //wrong kode
	}
	return 0;                                       //Guess array not full
}


//===============================================
//METHOD :  changePassword
//DESCR. :  Change the password
//===============================================
void Validation::changePassword(int *code)
{
	int length = 0;

	//---------findes the length of the new code--------
	for (size_t i = 0; code[i] > 0 && code[i] < 1000; i++)
	{
		length++;                                   //until the array is done
	}

	//allocade password and number array with new length
	int *_code = new int[length];
	int *_guess = new int[length];                  //the length is equal to the length of the new code
	_length = length;

	//---sets the  newcode into the code array----
	for (size_t i = 0; i < length; i++)
	{
		_code[i] = code[i];                         //copy array
	}

}


//===============================================
//METHOD :  codeVali
//DESCR. :  Validates if guess and code matches
//===============================================
bool Validation::codeVali(void)
{
	int rightNumbers = 0;

	//-------------checks code--------------
	for (size_t i = 0; i < _length; i++)
	{
		if (_guess[i] == _code[i])                  //If the numbers matches on the same array sloth
		{
			rightNumbers++;
		}
	}

	if (rightNumbers == _length)                    //Number of matches is equal to the length of the _code
	{
		return 1;
	}

	return 0;
}


//===============================================
//METHOD :  clearValues
//DESCR. :  Clear guess arrayet, so the code needs
//          to be typed from the beginning
//===============================================
void Validation::clearValues(void)
{
	//--------Clear guess array--------
	_numberCounter = 0;                             //Guess starts on sloth 0
}



//===============================================
//METHOD :  Deconstructor
//DESCR. :  
//===============================================
Validation::~Validation()
{
	delete[] _code;                                 //Delete memory
	delete[] _guess;

}