#include "mbed.h"
#include "TS_DISCO_F746NG.h"
#include "LCD_DISCO_F746NG.h"      


//===============================================
//METHOD :  Rendering
//DESCR. :  Render det grafiske på skærmen og
//          registrere touch.
//===============================================
class Rendering
{
public:
	//----Construction----
	Rendering(const int, const int, int*, int*, int);
	~Rendering();


	//-------Render-------
	void createUI(int *ID);                             // - 1                    
	void createUI(int *ID, int maxRow);					//Overloading for bigger design freedom and easier setup
	void createUI(int *ID, char *text[]);               // - 2
	void createUI(int *ID, char *text[], int maxRow);   // - 3


														//--------Text--------     
	void setText(int const *ID, char *text[]);			//You can't make zero boxes       //will this be a bool function?


														//---------UI---------
	bool showUI(int *ID);
	int getTouch(bool);									//Returnes touches from the last UI there have been called
	void editUIText(int *ID, char text[], int line);
	void addUIText(int *ID, char text[], int line);

	void setBackgroundColor(const int);					//Later there can be a overloading. This way there can also be inserted chars
	void setTextColor(const int);						//Later there kan be an rainbow function =)
	void setBorderColor(const int);
	void setInnerBoxColor(const int);


private:
	//---------Private functions---------
	void calcArray(int const *items, int *arrayX, int *arrayY);
	void calcTextSize(int const *items, int *arrayX, int *arrayY);
	void calcTextPlacement(int const *items, int *arrayX, int *arrayY);

	void setTextSize(int size);
	void printText(char text[], const int *arrayX, const int *arrayY, int, int, int);

	const int * getKoordinatIF_X(int *ID) const;
	const int * getKoordinatIF_Y(int *ID) const;

	bool getText(int const *ID, char *returnArray[]);   //Public function? so it can be used in the UI class if needed?


														//----------init variables-----------
	int _width;
	int _height;
	int _boxWidth;
	int _boxSize;
	int _maxRow;
	int _maxRowUser;
	int _maxRowCounter;
	int _maxRowCounterUser;
	int _gyro;


	//---------Coordinate arrays----------
	static int** _koordinatIF_X;
	static int** _koordinatIF_Y;

	static int _counter;
	int _boxArea;
	int _textArea;
	int _textSizeNumber;
	int _textKoordinatY;
	int _textKoordinatX;


	//---------------Touch---------------
	int _lastTouch;


	//---------Function variables---------
	int _arrayCounter;
	int _holdStyr;
	int _organizer;

	int _rowCenter;
	int _columnCenter;

	int _rowAsynCenter;
	int _asynCollumn;

	int _columns;
	int _rows;

	int _boxSizeX;
	int _boxSizeY;

	int _columnMove;
	int _rowMove;

	int _rowRestrict;
	int _columnRestrict;

	int _rowMove2;
	int _columnMove2;

	int _maxLength;
	int charSizeArray[5][2];

	int *_IDnow;


	//-------------text-class-------------
	int _textCounter;
	int _IDcounter;
	int _backgroundColor;
	int _borderColor;
	int _textColor;
	int _innerBoxColor;
	int _fromBorder;


	//----------Static variables----------
	static char **_textArray;
	static bool *_kaliStatic;


	//--------------Objects---------------
	LCD_DISCO_F746NG    lcd;
	TS_DISCO_F746NG     ts;
	TS_StateTypeDef     TS_State;

};

