#include "Rendering.h"

//-------------init static variables-------------
const int EKSTRAVAR = 3;                            //ID, text size, box size
const int NUMBEROFVAR = 2;                          //Outer coordinates, text coordinat
const int Ekstravar_ID = 0;
const int Ekstravar_TEXTSIZE = 1;
const int Ekstravar_SIZE = 2;

int Rendering::_counter = 0;

int ** Rendering::_koordinatIF_X = new int*[20];    //when it's static other objects can sets text into them
int ** Rendering::_koordinatIF_Y = new int*[20];

char ** Rendering::_textArray = new char*[20];
bool * Rendering::_kaliStatic = 0;



//===============================================
//METHOD :  parametriseret constructor
//DESCR. :  
//===============================================
Rendering::Rendering(const int width, const int height, int *boxWidth, int *maxRow, int gyro)  //What if they doesn't calls init?
{
	//--Sets pixel dimention for Font 8-24--
	charSizeArray[0][0] = 24;
	charSizeArray[0][1] = 17;
	charSizeArray[1][0] = 20;
	charSizeArray[1][1] = 14;
	charSizeArray[2][0] = 16;
	charSizeArray[2][1] = 11;
	charSizeArray[3][0] = 12;
	charSizeArray[3][1] = 8;
	charSizeArray[4][0] = 8;
	charSizeArray[4][1] = 5;

	//----Init static 2D array _textArray----
	if (_kaliStatic == 0)
	{
		for (size_t i = 0; i < 20; i++)
		{
			_textArray[i] = new char[255];
		}

		_kaliStatic++;
	}

	//----------Sets basic colors----------
	_backgroundColor = 0xFFFFFFFF;
	_borderColor = 0xFF000000;
	_textColor = 0xFF000000;
	_innerBoxColor = 0xFFFFFFFF;

	//-----Validate and sets arguments------  
	_width = (width >= 50 ? width : 50);									//Minimumscreen size of 50 in width og higth
	_height = (height >= 50 ? height : 50);									//The reason there isn't a pointer is to take up less space
	(*boxWidth) = ((*boxWidth) >= 2 || (*boxWidth) == 0 ? (*boxWidth) : 2);	//show down const so other function can't be changed
	_gyro = (gyro >= 0 && gyro <= 4 ? gyro : 0);							//Edit the changes made by the user
	_maxRow = ((*maxRow) > 0 && (*maxRow) < 50 ? (*maxRow) : 1);
	_maxRowCounter = (_maxRow % 2);											//tells if there is a even or uneven number there is in maxRow

	//-------stes private variables--------
	_maxLength = 0;
	_textCounter = 0;
	_IDcounter = 0;
	_textArea = 0;
	_fromBorder = 5;
	_boxWidth = (*boxWidth);
	_lastTouch = 0;

}


//===============================================
//METHOD :  Deconstructor
//DESCR. :  
//===============================================
Rendering::~Rendering()
{
	delete[] _koordinatIF_X;
	delete[] _koordinatIF_Y;
	delete[] _textArray;
}


//-------------init static variables-------------

//===============================================
//METHOD :  createUI - 1
//DESCR. :  calls createUI - 2 with -1 as 
//          argument
//===============================================
void Rendering::createUI(int *ID)
{
	//----------calls createUI - 2----------
	createUI(ID, -1);
}


//===============================================
//METHOD :  createUI - 2
//DESCR. :  Calculation of the graphis and puts it  
//          into a array
//===============================================
void Rendering::createUI(int *itemsIF, int maxRow)
{
	//-------------Findes maxRow-------------
	if (maxRow == -1)  _maxRowUser = _maxRow;			//Sets Rows to a standard number
	else _maxRowUser = maxRow;							//Sets rows to a personal value

														//Sets the relation between columns and objects
	_maxRowCounterUser = (_maxRowUser % 2);				//Tells if there is a even or uneven number in maxRow
	(*itemsIF) = ((*itemsIF) >= 0 ? (*itemsIF) : 1);	//Sets the number of objects                   

	//---------Init coordinate arrays---------
	_koordinatIF_X[_counter] = new int[((*itemsIF)       * NUMBEROFVAR) + EKSTRAVAR];       //Reserves sloths to the arrays and init it
	_koordinatIF_Y[_counter] = new int[((*itemsIF)       * NUMBEROFVAR) + EKSTRAVAR];

	//-----Sets UI ID into the array-----
	_koordinatIF_X[_counter][Ekstravar_ID] = (int)itemsIF;
	_koordinatIF_Y[_counter][Ekstravar_ID] = (int)itemsIF;

	//---------------Calc UI---------------
	//--Calls calculation of object pacement-
	calcArray(itemsIF, _koordinatIF_X[_counter], _koordinatIF_Y[_counter]);

	//-----Calls calculation of text size----
	calcTextSize(itemsIF, _koordinatIF_X[_counter], _koordinatIF_Y[_counter]);

	//--Calls calculation of text placement--
	calcTextPlacement(itemsIF, _koordinatIF_X[_counter], _koordinatIF_Y[_counter]);     //Pointer is being passed on to reduce memory use


	_counter++;											//First plus count up after the text have been set up
}


//===============================================
//METHOD :  createUI - 3
//DESCR. :  Calls createUI and setText with a 
//          standart row 
//===============================================
void Rendering::createUI(int *ID, char *text[])
{
	//------------calls setText-------------
	setText(ID, text);

	//----------calls createUI - 2----------
	createUI(ID, -1);
}


//===============================================
//METHOD :  createUI - 4
//DESCR. :  calls createUI and setText with the new
//          number of rows
//===============================================
void Rendering::createUI(int *ID, char *text[], int maxRow)
{
	//------------calls setText-------------
	setText(ID, text);

	//----------calls createUI - 2----------
	createUI(ID, maxRow);
}


//===============================================
//METHOD :  calcArray
//DESCR. :  Calculate the placeing og the objects
//===============================================
void Rendering::calcArray(int const *items, int *arrayX, int *arrayY)
{
	//------------init variables-------------
	_arrayCounter = EKSTRAVAR;
	_holdStyr = 1;
	_organizer = 1;

	_rowAsynCenter = 0;                          //the asynkrone centre
	_asynCollumn = 0;

	_rowRestrict = 1;
	_columnRestrict = 1;

	_rowMove2 = 0;
	_columnMove2 = 0;

	_rowCenter = 0;								//There is no reason that these are here?
	_columnCenter = 0;                          //But to be sure that everything is okay, it's here 
												
	_columns = 0;
	_rows = 0;

	_boxSizeX = 0;
	_boxSizeY = 0;

	_columnMove = 0;
	_rowMove = 0;


	//-------sets up rows and columns-------    
	_columns = ((*items) / _maxRowUser);            //Finds the number of columns in the whole numbers
	if ((*items) < _maxRowUser)                      //Ifthere is less objects than maxRows
	{
		_columns = 1;
	}

	if ((_columns * _maxRowUser) < *items)          //The asynkront amounts
	{
		_columns++;  _asynCollumn++;
	}


	if ((*items) <= _maxRowUser)                     //Setup row
	{
		_rows = (*items);
	}
	else if (((*items) % _maxRowUser) == 0)          //Checks if maxRows runs up to the number of objects
	{
		_rows = _maxRowUser;                        //If it does the number of rows need to be the same
	}
	else
	{
		_rows = _maxRowUser;
		_rowAsynCenter = ((*items) % _maxRowUser);    //Otherwise the asynkrone rows also needs to be set
	}


	//--Sets the height and width on the objects---
	_rowCenter = _height / (_rows + 1);             //center of the rows    -   what about uneven boxes?    
	_columnCenter = _width / (_columns + 1);        //Center of columns

	_boxSizeX = (_width - (_boxWidth*(_columns + 1))) / _columns;       //the objects size in the z-axis
	_boxSizeY = (_height - (_boxWidth*(_rows + 1))) / _rows;            //the objects size on the y-axis

	arrayX[Ekstravar_SIZE] = _boxSizeX;
	arrayY[Ekstravar_SIZE] = _boxSizeY;             //Only arrays for this class can be loaded.


													//--sets synkrone X and Y coordinates---
	for (; _holdStyr < _rows + 1; _holdStyr++)      //Synkron bokse
	{
		_rowMove = _boxWidth + (((_boxSizeY / 2))*_rowRestrict) + _rowMove2 + ((_rowRestrict - 1.5) * 2);     //Calculate Y_coordinates      
		_rowMove2 = _rowMove;                       //saves a buffer which is used in the next calculation

		for (; _organizer <(_columns + 1 - _asynCollumn); _organizer++)
		{

			_columnMove = _boxWidth + ((_boxSizeX / 2)*_columnRestrict) + _columnMove2 + ((_columnRestrict - 1.5) * 2);   //Calculate Y-koordinat
			_columnMove2 = _columnMove;             //saves a buffer which is used in the next calculation
			if (_columnRestrict < 2)
			{
				_columnRestrict++;                  //Is used for finetuning
			}

			_columnCenter = _columnMove - (_boxSizeX / 2);              //Calculate corner coordinats
			_rowCenter = _rowMove - (_boxSizeY / 2);                    //Calculate corner coordinats


			arrayX[_arrayCounter] = _columnCenter;  //Sets the cornor coordinates into the array
			arrayY[_arrayCounter] = _rowCenter;

			_arrayCounter++;                        //Goes to the next sloth in the arrayet
			_arrayCounter++;
		}

		if (_rowRestrict < 2)
		{
			_rowRestrict++;                         //Is used for finetuning
		}

		_columnRestrict = 1;
		_columnMove2 = 0;
		_organizer = 1;                             //Is being set so the for-loop next time is set right
	}


	//--Sets the asynkrone X and Y coordinates--      
	if (_rowAsynCenter != 0)                         //Asynkronst + synkront
	{
		_columnMove = (_boxWidth*_columns) + (_boxSizeX*(_columns - 1)) + (_columnRestrict * 2); //Calculate the placement of the asynkrone objektex, X-coordinat

		_rowCenter = _height / (_rowAsynCenter + 1);  //Calculate the placement of the asynkrone objektex, Y-coordinat

		for (size_t i = 1; i <= _rowAsynCenter; i++)
		{
			_rowMove = (_rowCenter*i) - (_boxSizeY / 2);//Doing so the corner coordinates is found

			arrayX[_arrayCounter] = _columnMove;    //Sets into the array     
			arrayY[_arrayCounter] = _rowMove;

			_arrayCounter++;
			_arrayCounter++;
		}
	}

}




//===============================================
//METHOD :  calcTextSize
//DESCR. :  Calculate text size for every UI
//===============================================
void Rendering::calcTextSize(int const *items, int *arrayX, int *arrayY)                    //First calculate the size and then the placement
{
	//-------------Init variables------------      
	char *textArray[20];                            //Is calculationg for every interface one of the time
	int baseLength = 0;
	_maxLength = 0;                                 //Length string

													//-----Gets relevent text arrays-----
	if (!getText(items, textArray))					//Gets the array with text by using a pointer
	{
		//error                                      //If something is wrong there will be an error
	}


	//--------Findes the length string----------
	for (size_t i; i < *items; i++)                  //Findes the longest line
	{
		baseLength = 0;                             //BaseLength is the length of the string
		for (size_t x = 0; (int)textArray[i][x] != 0; x++)                                   //Findes the length of string[i]
		{
			baseLength++;
		}

		(baseLength > _maxLength) ? _maxLength = baseLength : _maxLength = _maxLength;       //After the array have been run though it's the longest array: _maxLength
	}

	//---------Beregner boks areal-----------
	int outerArea = _koordinatIF_X[_counter][Ekstravar_SIZE] * _koordinatIF_Y[_counter][Ekstravar_SIZE];    //Findes the outer area
	_boxArea = outerArea - ((_boxWidth * 3) * _koordinatIF_X[_counter][Ekstravar_SIZE]);    //Finds the inner area


	//---Calculate areal for the length of the string-----
	_textSizeNumber = 0;                            //There will be a new calculation of the area

	_textArea = charSizeArray[_textSizeNumber][0] * charSizeArray[_textSizeNumber][1];      //Text area for 1 char is found     

	//----------Find tekst størrelse---------
	for (_textSizeNumber++; _textArea > _boxArea; _textSizeNumber++)                         //Runs though until the _textArea is bigger than _boxArea
	{
		_textArea = charSizeArray[_textSizeNumber][0] * charSizeArray[_textSizeNumber][1];  //What if it can't be there?
	}

	//--------Sets text size--------             
	arrayX[Ekstravar_TEXTSIZE] = charSizeArray[_textSizeNumber][1];                         //How? What will there be written?
	arrayY[Ekstravar_TEXTSIZE] = charSizeArray[_textSizeNumber][0];
}


//===============================================
//METHOD :  calcTextPlacement
//DESCR. :  Calculate start coordinates for text
//===============================================
void Rendering::calcTextPlacement(int const *items, int *arrayX, int *arrayY)
{
	//-------------Init variables------------
	char *textArray[20];
	int baseLength = 0;
	int lines = 0;
	int length2X;
	int length2Y;

	//-----Gets relevant text arrays-----
	if (!getText(items, textArray))
	{
		//error   
	}



	for (size_t i = 0; i < *items; i++)             //Through this for-loop it will find the starting coordinates.
	{
		//----------Findes string length---------
		baseLength = 0;
		for (size_t x = 0; (int)textArray[i][x] != 0; x++)
		{
			baseLength++;
		}

		//---Lines the string will be destributes over---
		float length = (charSizeArray[_textSizeNumber][1] * baseLength);                    //Length of text
		length = length / ((_koordinatIF_X[_counter][Ekstravar_SIZE]) - (2 * _boxWidth));   //Number of lines (float)

		(length > (float)(int)length) ? lines = (int)length + 1 : lines = (int)length;      //Finds the number of lines

		//-----------------Lines----------------
		if (lines)
		{
			//-----Calculate the length of the first line-----//If there only is one line of text
			if ((baseLength * arrayX[Ekstravar_TEXTSIZE]) <= (arrayX[Ekstravar_SIZE] - (2 * _boxWidth) - (_fromBorder * 2)))
			{
				length2X = arrayX[Ekstravar_TEXTSIZE] * baseLength;                         //Calculates the indentation on the x-axis                      
			}
			//-----Calculate the length of the second line------//If there is more than one line of text
			else
			{
				length2X = arrayX[Ekstravar_SIZE] - (2 * _boxWidth) - (_fromBorder * 2);    //Calculates the indentation on the x-axis 
			}

			//-------Beregn X og Y koordinater-------
			length2Y = charSizeArray[_textSizeNumber][0] * lines;                           //Calculates the indentation on the y-axis on the number of lines

			_textKoordinatX = arrayX[EKSTRAVAR + i * 2] + _boxWidth + (((arrayX[Ekstravar_SIZE] - (2 * _boxWidth)) / 2) - (length2X / 2));
			_textKoordinatY = arrayY[EKSTRAVAR + i * 2] + _boxWidth + (((arrayY[Ekstravar_SIZE] - (2 * _boxWidth)) / 2) - (length2Y / 2));

		}
		else                      //If there is more than one line.
		{
			//Error or no text
		}

		//--Insert coordinates in the coordinat array--
		arrayX[EKSTRAVAR + 1 + i * 2] = _textKoordinatX;
		arrayY[EKSTRAVAR + 1 + i * 2] = _textKoordinatY;
	}
}


//===============================================
//METHOD :  getKoordinatIF_X
//DESCR. :  Returns the wanted coordinate array
//===============================================
const int * Rendering::getKoordinatIF_X(int *ID) const
{
	//-------------Init variables-------------
	int count = 0;

	//---------------Finds array---------------
	for (count = 0; (int)ID != (int)_koordinatIF_X[count][Ekstravar_ID]; count++);

	//-------------returns Array-------------
	return _koordinatIF_X[count];
}


//===============================================
//METHOD :  getKoordinatIF_Y
//DESCR. :  Returns the wanted coordinate array
//===============================================
const int * Rendering::getKoordinatIF_Y(int *ID) const
{
	//-------------Init variables-------------
	int count = 0;

	//---------------Finds array---------------
	for (count = 0; (int)ID != (int)_koordinatIF_X[count][Ekstravar_ID]; count++);

	//-------------Returns Array-------------
	return _koordinatIF_Y[count];     //Instead of counter it needs to find the right right one to return. 
}


//===============================================
//METHOD :  getText
//DESCR. :  Returns the wanted array with string
//===============================================
bool Rendering::getText(int const *ID, char *returnArray[])
{
	//----------Runs through the array----------
	for (size_t i = 0; true; i++)
	{

		//----------Checks if ID matcher---------
		if ((int)_textArray[i] == (int)((char *)ID))
		{
			for (size_t x = 0; x < *ID; x++)
			{
				returnArray[x] = _textArray[i + x + 1]; //Will return the array which is send in.
			}                                       //It's the rendering class, which will use it.
			break;
		}

		if (i > _textCounter)
		{
			return 0;
		}
	}

	return 1;                   //0 - fail to get text ? 1 - All okay
}


//===============================================
//METHOD :  getText
//DESCR. :  Returns the wanted array with a string
//===============================================
void Rendering::setText(int const *ID, char *text[])
{
	//Sets ID for the UI onto sloth IDCounter
	_textArray[_textCounter++] = (char *)ID;

	for (size_t i = 0; i < *ID; i++)
	{
		//-----Sets text in on return array-----
		_textArray[_textCounter++] = text[_textCounter - _IDcounter - 1];
	}

	//------Set new value for the IDCounter----
	_IDcounter += *ID + 1;

}


//===============================================
//METHOD :  setTextSize
//DESCR. :  sets text sixe
//===============================================
void Rendering::setTextSize(int size)
{
	switch (size)
	{
	case 24: lcd.SetFont(&Font24);              //Sets text sixe for the &FontX
		break;
	case 20: lcd.SetFont(&Font20);
		break;
	case 16: lcd.SetFont(&Font16);
		break;
	case 12: lcd.SetFont(&Font12);
		break;
	case 8: lcd.SetFont(&Font8);
		break;

	default: lcd.SetFont(&Font24);
		break;
	}
}


//===============================================
//METHOD :  setBackgroundColor
//DESCR. :  Sets bagground color
//===============================================
void Rendering::setBackgroundColor(const int color)
{
	if (color <= 0xFFFFFFFF && 0xFFFFFFFF >= 0xFF000000) _backgroundColor = color;   //Sets colors if it's valid
}


//===============================================
//METHOD :  setTextColor
//DESCR. :  Setx text color
//===============================================          
void Rendering::setTextColor(const int color)
{
	if (color <= 0xFFFFFFFF && 0xFFFFFFFF >= 0xFF000000) _textColor = color;         //Sets color if it's valid
}


//===============================================
//METHOD :  setBorderColor
//DESCR. :  Sets border color
//===============================================                        
void Rendering::setBorderColor(const int color)
{
	if (color <= 0xFFFFFFFF && 0xFFFFFFFF >= 0xFF000000) _borderColor = color;       //Sets color if it's valid
}


//===============================================
//METHOD :  setInnerBoxColor
//DESCR. :  Sets inner boxfarven
//===============================================
void Rendering::setInnerBoxColor(const int color)
{
	if (color <= 0xFFFFFFFF && color >= 0xFF000000) _innerBoxColor = color;          //Sets color if it's valid
}


//===============================================
//METHOD :  showUI
//DESCR. :  Writes the UI out on the screen
//===============================================
bool Rendering::showUI(int *ID)
{
	//----gets the coordinat for interfacet---
	int boxWidthReserve;
	int textCount;

	char *textArray[20];
	const int *koordinatX = getKoordinatIF_X(ID);
	const int *koordinatY = getKoordinatIF_Y(ID);

	//------Gets the text for the interface-------
	if (!getText(ID, textArray))
	{
		return 0;                                   //error in getting the text
	}
	_IDnow = ID;

	//-----------Calls setTextSize-----------
	setTextSize(koordinatX[Ekstravar_TEXTSIZE]);

	//----------Sets background color---------
	lcd.Clear(_backgroundColor);


	textCount = 0;
	for (size_t i = EKSTRAVAR; i <= (*ID * NUMBEROFVAR + 2) || i == 1; i += NUMBEROFVAR)
	{
		if (*ID == 0) boxWidthReserve = 0;          //If there is 0 objects.                               

		//-----------Sets border color----------
		lcd.SetTextColor(0x00F700D2);
		//-----------Drwsa outer squars----------
		lcd.FillRect(koordinatX[i], koordinatY[i], koordinatX[Ekstravar_SIZE], koordinatY[Ekstravar_SIZE]);

		//---------Sets inner box color--------
		lcd.SetTextColor(_innerBoxColor);
		//-------Tegner inner boks firkant--------
		lcd.FillRect((koordinatX[i] + _boxWidth), (koordinatY[i] + _boxWidth), (koordinatX[Ekstravar_SIZE] - (_boxWidth * 2)), (koordinatY[Ekstravar_SIZE] - (_boxWidth * 2)));

		//-----------Sets text farcolorve-----------
		lcd.SetTextColor(_textColor);
		//------------Calls printText------------
		printText(textArray[textCount], koordinatX, koordinatY, koordinatX[i + 1], koordinatY[i + 1], i);


		textCount++;
		if (*ID == 0) boxWidthReserve = _boxWidth;
	}

	return 1;
}


//===============================================
//METHOD :  getTouch
//DESCR. :  Returns the objext that the user is pressing
//===============================================
int Rendering::getTouch(bool state)                 //0 - the whole time, 1 - only one time.
{
	//Henter koordinat for sidst viste interface
	const int *koordinatX = getKoordinatIF_X(_IDnow);
	const int *koordinatY = getKoordinatIF_Y(_IDnow);

	//------Gets informations from the touch------
	ts.GetState(&TS_State);                         //Returns from the UI it's on at the time.

	//-------------TouchDetected()------------
	if (TS_State.touchDetected)
	{
		for (int i = 0; i < *_IDnow; i++)
		{
			if ((TS_State.touchX[0] > koordinatX[EKSTRAVAR + i * 2]) && (TS_State.touchY[0] > koordinatY[EKSTRAVAR + i * 2]))    //Checks if touch is under upper-left cornor coordinates
			{
				if ((TS_State.touchX[0] < (koordinatX[EKSTRAVAR + i * 2] + koordinatX[Ekstravar_SIZE])) && (TS_State.touchY[0] < (koordinatY[EKSTRAVAR + i * 2] + koordinatY[Ekstravar_SIZE])))  //Checks if touch if over lower-right conor coordinates
				{
					//---------Same touch as last time?---------
					if (_lastTouch == i + 1 && state == 1)
					{
						return 0;                   //Returns 0, If it's the same touch as last time
					}

					_lastTouch = i + 1;               //safes last touch
					return i + 1;                     //Returns touchen
				}
			}
		}
	}

	_lastTouch = 0;

	return 0;                                       //If the user isn't pressing a object.
}


//===============================================
//METHOD :  editUIText
//DESCR. :  Edit a text in a text array
//===============================================
void Rendering::editUIText(int *ID, char text[], int line)
{
	//-------Finds The wanted text String------
	for (size_t i = 0; i <= _IDcounter; i++)
	{
		if ((int)_textArray[i] == (int)((char *)ID))
		{
			//-----------Sets the new text into the arrya----------
			_textArray[line + i] = text;
			break;
		}
	}

}


//===============================================
//METHOD :  addUIText
//DESCR. :  Adds a text to the existing text in 
//          the text array
//===============================================
void Rendering::addUIText(int *ID, char text[], int line)
{
	//------------finds text array------------
	for (size_t i = 0; i <= _IDcounter; i++)
	{
		if ((int)_textArray[i] == (int)((char *)ID))            //Otherwise use baseLength alogrithmn.
		{
			int baseLength = 0;
			int baseLength2 = 0;

			//-----Find Length of the string from array----
			for (size_t x = 0; (int)_textArray[i + line][x] != 0; x++)
			{
				baseLength++;
			}

			//---Find length of string from the argument---
			for (size_t x = 0; (int)text[x] != 0; x++)
			{
				baseLength2++;
			}


			//------allocate memory for the new Array------
			char newChar[baseLength + baseLength2 + 1];
			int co = 0;

			//-Sets the original array into newArray
			for (size_t x = 0; x < baseLength; x++)
			{
				newChar[co] = _textArray[line + i][x];
				co++;
			}

			//----sets the new array into newArray----
			for (size_t x = 0; x < baseLength2; x++)
			{
				newChar[co] = text[x];
			}

			//-----------calls editUIText()----------
			editUIText(ID, newChar, line);

			delete[] newChar;

			break;
		}
	}

}


//===============================================
//METHOD :  printText
//DESCR. :  Prints all chars i a array out onto
//          the screen
//===============================================
void Rendering::printText(char text[], const int *arrayX, const int *arrayY, int Xkoordinat, int Ykoordinat, int i)      //It's needed that there only can be send the individual strings.
{
	//-------------Init variables-------------
	int lengthOfPrint = 0;
	int line = 0;
	int mark = Xkoordinat - arrayX[i] + (_boxWidth * 2);

	//---------Draws chars out on the screen--------
	for (size_t x = 0; (int)text[x] != 0; x++)       //it will edit the lines.
	{
		//-----------in the end of the line----------
		if ((lengthOfPrint + arrayX[Ekstravar_TEXTSIZE]) > (arrayX[Ekstravar_SIZE] - (_boxWidth * 2) - (_fromBorder * 2)))
		{
			//------------More than one line-----------     
			line += arrayY[Ekstravar_TEXTSIZE];     //Moves a row down
			lengthOfPrint = 0;                      //Starts from the start of the x-cordinat again
		}

		//--------Draws chars out on the screen-------
		lcd.DisplayChar(Xkoordinat + lengthOfPrint, Ykoordinat + line, text[x]);

		lengthOfPrint += arrayX[Ekstravar_TEXTSIZE];//Moves x-coordinat a char forwards.
	}
}

