#ifndef _DXCRASHHANDLER_H_
#define _DXCRASHHANDLER_H_

#include <QString>
class DxCrashHandler
{
public:
	DxCrashHandler();
	~DxCrashHandler();
private:
	QString AddAppPath(const QString relativepath);
};

#endif // _DXCRASHHANDLER_H_