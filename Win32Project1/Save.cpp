#include "stdafx.h"
#include "Save.h"
#include "globals.h"

int SaveToFile(const list<DrawObj*>& saveList, string& fileName)
{
	//put each object to json
	json j;
	for (auto& it : saveList)  //Draw each object in DrawObjList
	{
		json jit;
		jit["objectType"] = it->objectType;
		jit["ptBeg"] = { it->ptBeg.x, it->ptBeg.y };
		jit["ptEnd"] = { it->ptEnd.x, it->ptEnd.y };
		jit["color"] = it->color;
		jit["backgroundColor"] = it->backgroundColor;
		jit["lineWidth"] = it->lineWidth;
		if (it->objectType == 4)
		{
			TextObj* t = dynamic_cast<TextObj*>(it);
			vector<string> vs = t->text;
			jit["text"] = vs;
		}
		j.push_back(jit);
	}

	// open a file in write mode.
	ofstream outfile;
	PWSTR filePath = BasicFileSave();
	if (filePath)
	{
		string fp = wstr_to_str(filePath);
		if (!end_with(fp, ".json"))
			fp.append(".json");
		outfile.open(fp);
		outfile << j << endl;
		outfile.close();
		globals::var().lastFilePath = filePath;
	}
	else
	{
		MessageBox(NULL, L"You did not save.", L"Warning!", MB_OK);
		return 1;
	}

	string name = wstr_to_str(filePath);
	//wstring name = filePath;
	auto const pos = name.find_last_of('\\');
	auto const result = name.substr(pos + 1);
	fileName = result;

	return 0;
}

int ReadFromFile(list<DrawObj*>& readList, string& fileName)
{
	PWSTR filePath = BasicFileOpen();
	if (!filePath)
	{
		MessageBox(NULL, L"File open failed.", L"Error!", MB_OK);
		return 1;
	}

	json j;

	// open a file in read mode.
	ifstream infile;
	infile.open(filePath);
	if (!infile)
	{
		MessageBox(NULL, L"File open failed.", L"Error!", MB_OK);
		return 1;
	}
	infile >> j;
	globals::var().lastFilePath = filePath;
	string name = wstr_to_str(filePath);
	//wstring name = filePath;
	auto const pos = name.find_last_of('\\');
	auto const result = name.substr(pos + 1);
	fileName = result;
	//LPCWSTR sw = result.c_str();
	//MessageBox(NULL, sw, L"name", MB_OK);
	readList.clear();

	int i = 0;
	for (json::iterator it = j.begin(); it != j.end(); it++)
	{
		json j5 = *it;
		switch ((int)j5["objectType"])
		{
		case 1:
		{
			LineObj newLine;
			newLine.makeStart(j5["ptBeg"][0], j5["ptBeg"][1], j5["color"], j5["backgroundColor"], j5["lineWidth"]);
			newLine.makeEnd(j5["ptEnd"][0], j5["ptEnd"][1], 0, 0);
			readList.push_back(new LineObj(newLine));
			break;
		}
		case 2:
		{
			RectangularObj newRect;
			newRect.makeStart(j5["ptBeg"][0], j5["ptBeg"][1], j5["color"], j5["backgroundColor"], j5["lineWidth"]);
			newRect.makeEnd(j5["ptEnd"][0], j5["ptEnd"][1], 0, 0);
			readList.push_back(new RectangularObj(newRect));
			break;
		}
		case 3:
		{
			CircleObj newCircle;
			newCircle.makeStart(j5["ptBeg"][0], j5["ptBeg"][1], j5["color"], j5["backgroundColor"], j5["lineWidth"]);
			newCircle.makeEnd(j5["ptEnd"][0], j5["ptEnd"][1], 0, 0);
			readList.push_back(new CircleObj(newCircle));
			break;
		}
		case 4:
		{
			TextObj newText;
			newText.makeStart(j5["ptBeg"][0], j5["ptBeg"][1], j5["color"], j5["backgroundColor"], j5["lineWidth"]);
			newText.makeEnd(j5["ptEnd"][0], j5["ptEnd"][1], 0, 0);
			vector<string> text = j5["text"];
			newText.text = text;
			newText.lineWidth = newText.ptEnd.x - newText.ptBeg.x;
			readList.push_back(new TextObj(newText));
			break;
		}
		default:
			//error!
			MessageBox(NULL, L"JSON read error!", L"ERROR", MB_OK);
			break;
		}
		i++;
	}

	return 0;
}

int SaveToLastFilePath(const list<DrawObj*>& saveList)
{
	//put each object to json
	json j;
	for (auto& it : saveList)  //Draw each object in DrawObjList
	{
		json jit;
		jit["objectType"] = it->objectType;
		jit["ptBeg"] = { it->ptBeg.x, it->ptBeg.y };
		jit["ptEnd"] = { it->ptEnd.x, it->ptEnd.y };
		jit["color"] = it->color;
		jit["backgroundColor"] = it->backgroundColor;
		jit["lineWidth"] = it->lineWidth;
		if (it->objectType == 4)
		{
			TextObj* t = dynamic_cast<TextObj*>(it);
			vector<string> ls = t->text;
			jit["text"] = ls;
		}
		j.push_back(jit);
	}

	// open a file in write mode.
	ofstream outfile;
	//PWSTR filePath = BasicFileSave();
	if (globals::var().lastFilePath.size() > 0)
	{
		string fp = wstr_to_str(globals::var().lastFilePath);
		outfile.open(fp);
		if (outfile.is_open()) 
		{
			outfile << j << endl;
			outfile.close();
		}
		else
		{
			MessageBox(NULL, L"SaveToLastFilePath Failed", L"ERROR!", MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, L"SaveToLastFilePath Failed", L"ERROR!", MB_OK);
		return 1;
	}
	return 0;
}

PWSTR BasicFileOpen()
{
	PWSTR pszFilePath = NULL;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						//CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return pszFilePath;
}

PWSTR BasicFileSave()
{
	PWSTR pszFilePath = NULL;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *pFileSave;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		// Set the options on the dialog.
		DWORD dwFlags;

		// Before setting, always get the options first in order 
		// not to override existing options.
		hr = pFileSave->GetOptions(&dwFlags);

		if (SUCCEEDED(hr))
		{
			// In this case, get shell items only for file system items.
			hr = pFileSave->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

			// Set the file types to display only. 
			// Notice that this is a 1-based array.
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"JSON Files (*.json)", L"*.json" },
				//{ L"All Files (*.*)", L"*.*" }
			};
			hr = pFileSave->SetFileTypes(ARRAYSIZE(rgSpec), rgSpec);

			// Set the default extension to be ".doc" file.
			//hr = pFileSave->SetDefaultExtension(L"txt");

			// Show the Open dialog box.
			hr = pFileSave->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						//CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
	return pszFilePath;
}

std::string wstr_to_str(const std::wstring &wstr)
{
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}

bool end_with(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::wstring str_to_wstr(const std::string& s)
{
	wstring stemp = wstring(s.begin(), s.end());
	return stemp;
}

int DisplayConfirmClearMessageBox(const string fileName)
{
	wstring ws = str_to_wstr(fileName);
	ws += L" has been modified.\nDo you want to save it?";
	int msgboxID = MessageBox(
		NULL,
		ws.c_str(),
		L"Confirm to Clear",
		MB_ICONEXCLAMATION | MB_YESNO
	);

	if (msgboxID == IDYES)
	{
		// TODO: add code
	}

	return msgboxID;
}

int DisplayConfirmNewFileMessageBox(const string fileName)
{
	wstring ws = str_to_wstr(fileName);
	ws += L" has been modified.\nDo you want to save it?";
	int msgboxID = MessageBox(
		NULL,
		ws.c_str(),
		L"Confirm Save",
		MB_ICONEXCLAMATION | MB_YESNO
	);

	if (msgboxID == IDYES)
	{
		// TODO: add code
	}

	return msgboxID;
}