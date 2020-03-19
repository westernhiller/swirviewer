#include "CrashRpt.h"
#include "crashhandler.h"
#include <QApplication>
#include <QDir>
#include <QMainWindow>
#include <QMessageBox>

QString DxCrashHandler::AddAppPath(const QString relativepath)
{
	QDir root(QCoreApplication::applicationDirPath());
	return QDir::cleanPath(root.absoluteFilePath(relativepath));
}
// Define the callback function that will be called on crash
int CALLBACK CrashCallback(CR_CRASH_CALLBACK_INFO* pInfo)
{
	// The application has crashed!
	if (pInfo->nStage == CR_CB_STAGE_PREPARE)
	{
		// Close log file here when necessary
	}
	else //CR_CB_STAGE_FINISH
	{
		QMessageBox::critical(QApplication::activeWindow(), "FTT3D", "There is some critical error in the application. So it will be closed. \nPlease send error reports and related information to the administrators. \nThat will help us to improve the product.");
	}
	// Return CR_CB_DODEFAULT to generate error report
	return CR_CB_NOTIFY_NEXT_STAGE; //CR_CB_DODEFAULT;
}

DxCrashHandler::DxCrashHandler()
{
	// Install crash reporting
	CR_INSTALL_INFO info;
	memset(&info, 0, sizeof(CR_INSTALL_INFO));
	info.cb = sizeof(CR_INSTALL_INFO);
	info.pszAppName = L"swirviewer"; // Define application name.
//	info.pszAppVersion = L"2.0.2.3"; // Define application version.

	//Install all available exception handlers.
	info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
	info.dwFlags |= CR_INST_DONT_SEND_REPORT;
	info.dwFlags |= CR_INST_STORE_ZIP_ARCHIVES;
	info.dwFlags |= CR_INST_AUTO_THREAD_HANDLERS;

	//Set the error reprot path
	std::string str = AddAppPath("ErrorReport").toStdString();
	std::wstring wstr(str.length(), L' '); //初始化宽字节wstr
	std::copy(str.begin(), str.end(), wstr.begin()); //将str复制到wstr
	info.pszErrorReportSaveDir = wstr.c_str();

	//     Article effective minidumps recommand the MiniDumpWithPrivateReadWriteMemory. But it cause the dump file up to 50 MB.
	//     So we use the MiniDumpWithIndirectlyReferencedMemory as alternative, which lost some information but ensure the
	//     a reasonable dump size.
		// Not all flag will be used due to some callback not provided in crashrpt licrary. If more dump needed, we must modify the 
		// crashrpt code about crashdump.
	info.uMiniDumpType = (MINIDUMP_TYPE)( // MiniDumpWithPrivateReadWriteMemory | 
		MiniDumpWithDataSegs |
		MiniDumpWithHandleData |
		// The following parameters are supported by 6.2 and later
		MiniDumpWithIndirectlyReferencedMemory |
		MiniDumpWithFullMemoryInfo |
		MiniDumpWithThreadInfo |
		MiniDumpWithUnloadedModules
		);
	int nResult = crInstall(&info);
	if (nResult != 0)
	{
		QMessageBox::warning(NULL, "swirviewer", "Prepare error report handler failed!");
	}
	//      Set crash callback function
	crSetCrashCallback(CrashCallback, NULL);
	// Add the last application log 
	crAddFile2(
		(LPCTSTR)(AddAppPath("swirviewer.log")).toLocal8Bit().data(),
        L"swirviewer.log",
		L"Log file",
		CR_AF_MAKE_FILE_COPY | CR_AF_MISSING_FILE_OK);
	// Take screenshot of the app window at the moment of crash
	crAddScreenshot2(CR_AS_MAIN_WINDOW | CR_AS_USE_JPEG_FORMAT, 95);
}

DxCrashHandler::~DxCrashHandler()
{
	//Uninstall crash reporting
}
