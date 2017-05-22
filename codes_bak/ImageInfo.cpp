// ImageInfo.cpp: implementation of the CImageInfo class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "RSImagelc.h"
#include "ImageInfo.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "CFileSystem.h"
extern  CFileSystem filesys;

//#include <io.h>

CImageInfo::CImageInfo()
{
	nbandcount=0;   //影像的波段数
	nWidth=0;  //影像宽
	nHeight=0; //影像高
	poDataset=NULL;
	imgPath = ("");
	nbandtype = 0;

//	GeoDescription=NULL;		//影像驱动描述
//	GeoMetadataItem=NULL;	//影像格式描述
	GeoProjectionRef="";	//投影信息
	adfGeoTransform = new double[6];	//坐标和空间分辨率

	m_strRPCpath = NULL;


}
CImageInfo::~CImageInfo()
{
	FreeData();
}

/************************************************************************/
/* 利用GDAL来读取影像
                                                                     */
/************************************************************************/
bool CImageInfo::LoadImage(string filename, bool bCanUpdate, bool bShare)
{
	imgPath = filename;
	
	
	GDALAllRegister(); //注册所有驱动
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");		//使得支持中文路径  

	if (bCanUpdate==FALSE)
	{
		poDataset = (GDALDataset*)GDALOpen(filename.c_str(),GA_ReadOnly);	//只读打开
	}
	else
	{
		poDataset = (GDALDataset*)GDALOpen(filename.c_str(),GA_Update );	//可更新
	}
	
	if (poDataset==NULL)
	{
		filesys.OutputLogMessage("Open Image File Failed!  No.1");
		return FALSE;
	}

	nbandcount=poDataset->GetRasterCount();//获取影像波段数
	nWidth = poDataset->GetRasterXSize();//获取影像宽
	nHeight = poDataset->GetRasterYSize();//获取影像高

	//判断各波段间的长宽是否一致
	//判断各波段间的数据类型是否一致
	GDALDataType datatypetemp = GDT_Unknown;

	GDALRasterBand *poBand; //对每个波段进行处理
	int nXSize=0; //各波段影像的宽
	int nYsize=0; //各波段影像的长
	for (int i=0;i<nbandcount;i++)
	{
		poBand = poDataset->GetRasterBand(i+1); //针对地i+1个波段
		//获取该波段影像长宽
		nXSize = poBand->GetXSize(); 
		nYsize =poBand->GetYSize();
		if(nXSize!=nWidth || nYsize!=nHeight)
		{
			filesys.OutputLogMessage("某一波段的数据大小与整个影像的大小不符! 返回!!! Error No.3");
			FreeData();
			return FALSE;
		}

		//获取波段的保存类型
		if(i==0)
			m_datatype = poBand->GetRasterDataType();
		else
		{
			datatypetemp =  poBand->GetRasterDataType();
			if (m_datatype != datatypetemp)
			{
				filesys.OutputLogMessage("不同波段间的数据类型不一致_error!");
				FreeData();
				return FALSE;
			}
		}
	}	

	if (m_datatype==0)
	{
		filesys.OutputLogMessage("未知数据类型 Error No.2!");
		FreeData();
		return FALSE;
	}

	nbandtype = GDALGetDataTypeSize(m_datatype);


	//获取影像格式	
	GeoDescription = poDataset->GetDriver()->GetDescription();	//这里数据类型要用const char*，用CString得不到结果，有点怪
	GeoMetadataItem = poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME );

	//获取地理信息
	GeoProjectionRef = poDataset->GetProjectionRef();//const char*		//投影
	if( GeoProjectionRef.empty() )
        GeoProjectionRef = "";
	if(poDataset->GetGeoTransform( adfGeoTransform ) != CE_None )		//分辨率，地理坐标等
    {
		delete []adfGeoTransform;	//即使没有获取成功，得到的是默认值，这里就不删除内存  hlc 2012-6-2
		adfGeoTransform = NULL;

	}
	

	return TRUE;

}



/************************************************************************/
/* 从一个完全路径中分离出文件名
                                                                     */
/************************************************************************/
string CImageInfo::GetImageFileName(string fullpath)
{
	//if (fullpath=="")
	//{
	//	fullpath = imgPath;
	//}
	//int i;
	//for (i=fullpath.GetLength()-1; i>=0;i--)
	//{
	//	if (fullpath[i]=='\\')
	//	{
	//		break;
	//	}
	//}
	//return fullpath.Mid(i+1);

	return filesys.GetFileName(imgPath);

	return "";

}

//从一个完全路径中取出文件的路径
string CImageInfo::GetImagePath(string fullpath)
{
	//if (fullpath=="")
	//{
	//	fullpath = imgPath;
	//}
	//int i=0,j=0; 
 //   while( i<fullpath.GetLength() ) 
 //   { 
 //       if( fullpath[i]=='\\' ) 
 //           j = i; 
 //       i++; 
 //   } 
 //   return fullpath.Left( j+1 ); 

	//return filesys.
	return filesys.GetFilePathWithoutName(imgPath);//

	return "";

}

//设置影像文件的投影信息(地理信息)
//添加：黄灵操 2011.3.12
bool CImageInfo::SetProjection(const char *proj)
{
	if (proj==NULL) return FALSE;
	if (poDataset->SetProjection(proj)!=CE_None)
		return FALSE;
	
	return TRUE;
}

//设置影像文件的坐标信息，分辨率
//添加：黄灵操 2011.3.12
bool CImageInfo::SetGeoTransform(double *adfT)
{
	if (adfT==NULL) return FALSE;
	if (poDataset->SetGeoTransform(adfT)!=CE_None)
		return FALSE;
	return TRUE;
}



//获取影像文件的坐标信息，分辨率
//添加：黄灵操 2011.3.20
bool CImageInfo::GetGeoTransform(double **adfT)
{
	if (adfGeoTransform==NULL)
	{
		//////////////////////////////////////////////////////////////////////////
		//黄灵操  2012-9-24
		//改成这样，是为了方便对没有坐标的影像匹配。
		//但对于影像分割之后进行栅格矢量化的时候，如果没有坐标，这样子设置，是有问题。	-黄灵操 2014-1-1
		//adfGeoTransform = new double[6];
		//adfGeoTransform[0] = 0;
		//adfGeoTransform[1] = 1;
		//adfGeoTransform[2] = 0;
		//adfGeoTransform[3] = 0;
		//adfGeoTransform[4] = 0;
		//adfGeoTransform[5] = 1;
		//*adfT = adfGeoTransform;
		//return TRUE;		
		//////////////////////////////////////////////////////////////////////////


		return FALSE;
	}
	*adfT = adfGeoTransform;
	return TRUE;	
}

//获取影像rpc文件路径
//添加：黄灵操 2013.12.15
bool CImageInfo::GetImageRPCPath(char** path)
{
	if (imgPath=="")
	{
		return FALSE;
	}

	if (m_strRPCpath==NULL)
	{
		string temp = GetImagePath(imgPath);
		string rpcpath = filesys.GetFileNameWithoutExt(imgPath);
		rpcpath += "_rpc.txt";
		rpcpath = temp+rpcpath;
		if((access(rpcpath.c_str(), 0 )) != -1)
		{
			//RPC参数存在就用rpc参数
		}else
		{
			rpcpath = "no";
		}
		m_strRPCpath = new char[rpcpath.length()+1]; //这里字符串长度要 +1，strcpy拷贝时内存不一致，释放内存时会出现 heap corruption detected的错误
		strcpy(m_strRPCpath, rpcpath.c_str());
		//m_strRPCpath = rpcpath.c_str();

		*path = m_strRPCpath;		
	}else
	{
		*path = m_strRPCpath;
	}
	
	return TRUE;	
}

//获取影像文件的投影信息等
//添加：黄灵操 2011.3.20
bool CImageInfo::GetProjection(const char **proj)
{
	if (GeoProjectionRef.empty())
	{
		return FALSE;
	}
	*proj = GeoProjectionRef.c_str();
	return TRUE;
}

void CImageInfo::FreeData()
{
	if(poDataset)
	{
		GDALClose(poDataset); //关闭图像文件
	}

//	if(GeoDescription) delete []GeoDescription;//GeoDescription =NULL;
//	if (GeoMetadataItem) delete []GeoMetadataItem; GeoMetadataItem =NULL;
//	if (GeoProjectionRef) delete []GeoProjectionRef;GeoProjectionRef = NULL;
	if (adfGeoTransform) delete []adfGeoTransform;adfGeoTransform=NULL;	

	if (m_strRPCpath)
	{
		delete []m_strRPCpath;m_strRPCpath=NULL;
	}

}


double  CImageInfo::GetResolveH()
{
	if (!adfGeoTransform)
	{
		return 0;
	}
	return adfGeoTransform[1];
}


double  CImageInfo::GetResolveV()
{
	if (!adfGeoTransform)
	{
		return 0;
	}
	return adfGeoTransform[5];
}

double  CImageInfo::GetGeoStartPosH()
{
	if (!adfGeoTransform)
	{
		return 0;
	}
	return adfGeoTransform[0];
}

double  CImageInfo::GetGeoStartPosV()
{
	if (!adfGeoTransform)
	{
		return 0;
	}
	return adfGeoTransform[3];
}

//获取数据集的指针
//添加：黄灵操
//时间：2011-4-17
GDALDataset* CImageInfo::GetPoDataSet()
{

	if (poDataset==NULL)
	{
		return NULL;
	}
	return poDataset;
}

//功能：获取影像文件的数据部分的大小(以字节为单位)
//添加：黄灵操
//时间：2011-4-17
long CImageInfo::GetImageDataSize()
{
	if (!poDataset)
	{
		return 0 ;
	}
	if (nHeight<1 || nWidth<1 || nbandtype<1)
	{
		return 0;
	}
//	DWORD typesize = (DWORD)nbandtype;
	double temp= 0;			//这里数据类型要用double，用long(DWORD)会超出范围，得到结果不正确
	temp = (double)nHeight*nWidth*nbandcount*nbandtype;
	return temp/8;		//(以字节为单位)

}

//////////////////////////////////////////////////////////////////////////
//功能：获取影像中背景像素的灰度值
//添加：黄灵操
//时间：2011-4-17
//DEL double CImageInfo::GetNoDataValue()
//DEL {
//DEL 	return 0;
//DEL 
//DEL }


bool CImageInfo::NewImage(string lpstrFilePath, long nBand, long _nHeight, long _nWidth, GDALDataType eDataType, string lpstrFmt)
{
	imgPath = lpstrFilePath;
	GDALAllRegister(); //注册所有驱动

	nbandcount=nBand;	//影像波段数
	nWidth = _nWidth;	//影像宽
	nHeight = _nHeight;	//影像高

	//数据类型
	m_datatype = eDataType;	
	nbandtype = GDALGetDataTypeSize(m_datatype);
	
	const char *pszFormat = lpstrFmt.c_str(); //数据格式
	//LPSTR  pszFormat = "GTiff";
	GDALDriver *poDriver;
	char **papszMetadata;

	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	if( poDriver == NULL)
		return FALSE;
	papszMetadata = poDriver->GetMetadata();

	//测试该格式是否支持Create方法
	if( !CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
	{
		/*CString format;
		format.Format("Driver %s not supports Create() method.\n",pszFormat);*/
		char str[FORMAT_CHAR_LENGTH];
		sprintf(str,"Driver %s not supports Create() method.\n",pszFormat);
		filesys.OutputLogMessage(str);
		return FALSE;
	}

	GDALDataset *poDstDS;       
	char **papszOptions = NULL;

	poDstDS = poDriver->Create(lpstrFilePath.c_str(), //创建的影像文件的路径
		(int)nWidth,  //影像宽
		(int)nHeight,  //影像长
		(int)nBand,    // 影像的波段数
		eDataType, //保存的数据类型
		papszOptions );//格式控制层参数列表(不懂什么意思，先不管)

	poDataset = poDstDS;		//保存问全局变量
	
	return TRUE;
}

double CImageInfo::GetMaximum(int bandindex,int * pbSuccess)
{
    if (!poDataset)
    {
        *pbSuccess = 0;
        return 0 ;
    }
    GDALRasterBand *poBand;
    double max_value=0;
    poBand = poDataset->GetRasterBand(bandindex);
    if (poBand)
    {
        max_value = poBand->GetMaximum(pbSuccess);
    }
    return max_value;
}
double CImageInfo::GetMinimum(int bandindex,int * pbSuccess)
{
    if (!poDataset)
    {
        *pbSuccess = 0;
        return 0 ;
    }
    GDALRasterBand *poBand;
    double min_value=0;
    poBand = poDataset->GetRasterBand(bandindex);
    if (poBand)
    {
        min_value = poBand->GetMinimum(pbSuccess);
    }
    return min_value;
    
}
