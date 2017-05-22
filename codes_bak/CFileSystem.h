/***************************************************************
 * Name:      CFileSystem.h
 * Author:    Huang Lingcao
 * E-mail:    huanglingcao@gmail.com
 * Created:   2013-04-06
 * Copyright: Wuhan University 2013
 * License:
 **************************************************************/
 //针对文件进行一些操作，以及对文件路径的一些计算

#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include "typedefine.h"

class CFileSystem
{
    public:
        DWORD GetFileSize(string path);
        string GetFileNameWithoutExt(string fullpath);
        string GetFileNameExt(string fullpath);
        string Getsystime();
		string GetEXEPath();
        bool OutputLogMessage(string message);
        bool IsThePathEanble(string path);
        void AssignThePathOflogfile(string path);
        string GetFilePathWithoutName(string fullpath);
        string GetFileName(string fullpath);
        CFileSystem();
        virtual ~CFileSystem();

	public:
		std::string ws2s(const std::wstring& ws);
		std::wstring s2ws(const std::string& s);
    
        bool IsFileExist(string path);


    protected:
    private:
        string m_strlogpath;    //日志文件的路径
};

#endif // CFILESYSTEM_H
