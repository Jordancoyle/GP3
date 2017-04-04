#ifndef _cFpCamera_H
#define _cFpCamera_H

#include "GameConstants.h"
#include "cInputMgr.h"
#include "cModel.h"

class cFpCamera
{
private:
	cInputMgr* m_InputMgr;
public:
	cFpCamera();
	void attachInputMgr(cInputMgr* inputMgr);
	virtual void update(float elapsedTime);

	~cFpCamera();
};

#endif