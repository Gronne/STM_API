#include "Communication.h"

Communication::Communication(int type)
{
	//for(size_t i = 0; i < 10; i++)
	//{
	//    _rxInfo[i] = 0;
	//}

	_toogle = 0;

}


void Communication::sendInformation(int information)
{
	//---------- Get information ------------------
	_toogle = !_toogle;

	if (_toogle) _rxInfo = 1;
	else if (!_toogle) _rxInfo = 2;


	//---------- send information -----------------



}


void Communication::receiveInformation()
{
	//---------- receive information --------------



	//---------- send information -----------------

}


void Communication::sendToValidation()
{


}


int Communication::getInformation()
{
	return _rxInfo;

}