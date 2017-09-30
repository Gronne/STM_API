#include "mbed.h"


//===============================================
//METHOD :  Validaiton
//DESCR. :  validate if 'guess' is equal to code.
//===============================================
class Validation
{
public:
	//----Construction----
	Validation();
	~Validation();

	//--------code--------
	int setValue(int number);                       //0 - not full, 1 - Right password, 2 - Worng password
	void clearValues(void);
	void changePassword(int *kode);


private:
	//---------Privat functions---------
	bool codeVali(void);

	//-------------Variables-------------
	int _code[7];
	int _guess[7];
	int _length;
	int _numberCounter;

};