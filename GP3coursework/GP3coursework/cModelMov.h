#ifndef _CMODELMOV_H
#define _CMODELMOV_H

#include "GameConstants.h"
#include "cModel.h"
#include "cInputMgr.h"

class cModelMov : public cModel
{
private:
	cInputMgr* m_InputMgr;
public:
	cModelMov();
	void attachInputMgr(cInputMgr* inputMgr);
	virtual void update(float elapsedTime);

	~cModelMov();
};
#endif