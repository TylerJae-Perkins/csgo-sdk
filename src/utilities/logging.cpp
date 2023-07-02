#include "logging.h"

//@TODO: make it so this returns to original colour after function is finished calling!
void Logger::LogToConsole(std::string_view strString, ELogType log, bool bCreateNewLine) {
	HANDLE std_output_handle = GetStdHandle(((DWORD)-11));

	std::string strText = "";
	switch (log) {
	case LOG_MESSAGE:
		SetConsoleTextAttribute(std_output_handle, 2);
		strText = "[MESSAGE] : ";
		break;
	case LOG_WARNING:
		SetConsoleTextAttribute(std_output_handle, 14);
		strText = "[WARNING] : ";
		break;
	case LOG_ERROR:
		SetConsoleTextAttribute(std_output_handle, 12);
		strText = "[ERROR]   : ";
		break;
	}

	strText += strString.data();

	if (bCreateNewLine)
		strText += "\n";

	printf(strText.data());
}