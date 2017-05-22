/***************************************************************
 * Name:      CFileSystem.cpp
 * Author:    Huang Lingcao
 * E-mail:    huanglingcao@gmail.com
 * Created:   2013-04-06
 * Copyright: Wuhan University 2013
 * License:
 **************************************************************/

//#include "stdafx.h"
#include "CFileSystem.h"
#include <time.h>
#include <string>

CFileSystem::CFileSystem()
{
    //ctor
    m_strlogpath = "system.log";
}

CFileSystem::~CFileSystem()
{
    //dtor
}

/************************************************************************/
/* 从一个完全路径中分离出文件名
                                                                     */
/************************************************************************/
string CFileSystem::GetFileName(string fullpath)
{
	if (fullpath=="")
	{
	    return "";
	}
	string str =fullpath;

	#ifdef __LINUX__
	int pos = (int)str.find_last_of('/');
	#else
	int pos = (int)str.find_last_of('\\');
	#endif // __LINUX__

    return str.substr(pos+1, str.length());
}

/************************************************************************/
/* 从一个完全路径中分离出文件名,不包含扩展名
                                                                     */
/************************************************************************/
string CFileSystem::GetFileNameWithoutExt(string fullpath)
{
    string filename = GetFileName(fullpath);
    int pos=(int)filename.find_last_of('.');
    if(pos>=0 && pos<filename.length())
        return filename.substr(0,pos);
    else
        return filename;
}

/* 从一个完全路径中分离出文件名的扩展名
                                                                     */
/************************************************************************/
string CFileSystem::GetFileNameExt(string fullpath)
{
    int pos=(int)fullpath.find_last_of('.');
    if(pos>=0 && pos<fullpath.length())
        return fullpath.substr(pos+1,fullpath.length());
    else
        return "";
}

/************************************************************************/
/* 从一个完全路径中分离出路径，不包含文件名
                                                                     */
/************************************************************************/
string CFileSystem::GetFilePathWithoutName(string fullpath)
{
	if (fullpath=="")
	{
	    return "";
	}
	string str =fullpath;

	#ifdef __LINUX__
	int pos = (int)str.find_last_of('/');
	#else
	int pos = str.find_last_of('\\');
	#endif // __LINUX__

    return str.substr(0,pos+1);
}



//指定日志文件的路径
//add by Huang Lingcao
//2013-04-06-15.20
void CFileSystem::AssignThePathOflogfile(string path)
{
    m_strlogpath = path;
}

//判断文件路径是否有效
//add by Huang Lingcao
//2013-04-06-15.20
bool CFileSystem::IsThePathEanble(string path)
{

    return TRUE;
}
//输出日志信息
//add by Huang Lingcao
//2013-04-06-15.20
bool CFileSystem::OutputLogMessage(string message)
{
    FILE* fp=fopen(m_strlogpath.c_str(),"a");
    if(NULL==fp)
    {
        cout<<"Open log file"<<m_strlogpath<<"Faild"<<endl;
        return FALSE;
    }
    string time = Getsystime();
    cout<<time<<": "<<message<<endl;        //输出到屏幕
    fprintf(fp,"%s: %s\n",time.c_str(),message.c_str());     //输出到文件
    fclose(fp);

    return TRUE;
}

//Get system time
//add by Huang Lingcao
//2013-04-06-15.20

string CFileSystem::Getsystime()
{
    time_t t = time(0);
    char tmp[64];
    //strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天 %z",localtime(&t) );
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X ",localtime(&t) );
    return tmp;
}

//获取可执行程序的绝对路径
string CFileSystem::GetEXEPath()
{
#ifdef __LINUX__
	
	//linux下面获取路径
    return "";
#else
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260  
	memset(exeFullPath,0,MAX_PATH);  
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	return exeFullPath;

#endif
	
}

//#define __LINUX__
DWORD CFileSystem::GetFileSize(string path)
{
    #ifdef __LINUX__
    struct stat buf;
    if(stat(path.c_str(), &buf)<0)
        {
            return 0;
    }
    return buf.st_size;
	#else
		//获取文件大小
	WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	DWORD fileSize;
	hFind = FindFirstFile(path.c_str() ,&fileInfo);
	if(hFind != INVALID_HANDLE_VALUE)
	fileSize = fileInfo.nFileSizeLow;
	FindClose(hFind);
	return fileSize;
#endif // __LINUX__


}

//C++ STRING 和WSTRING 之间的互相转换函数（转）
//#include <string>
//wstring 转换为 string
std::string CFileSystem::ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}
// string转换为 wstring
std::wstring CFileSystem::s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

bool CFileSystem::IsFileExist(string path)
{
    if (access(path.c_str(), 0) == 0 )
    {
        return true;
    }
    else
        return false;
}





