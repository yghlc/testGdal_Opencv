// RSImage.cpp: implementation of the CRSImage class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "rsimagelc.h"
#include "RSImage.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "CFileSystem.h"
extern CFileSystem filesys;

CRSImage::CRSImage()
{

	pimginfo = NULL;
	poDataSet = NULL;

}

CRSImage::~CRSImage()
{
	Destory();
}

/*************************************************************************************
 * 函数名称: void Destory()
 * 
 * 函数功能: 清除内存
*************************************************************************************/
void CRSImage::Destory()
{
	if (pimginfo)
	{
//		delete []pimginfo;pimginfo=NULL;
        delete pimginfo;pimginfo=NULL;
	}

}

/*************************************************************************************
 * 函数名称: bool Open(string lpstrFileName, bool bCanUpdate, bool bShare)
 * 
 * 函数功能: 打开图像文件
 *
 * 参数说明: 
 * string lpstrFileName: 输入参数，含义: 图像文件完整路径名
 * bool bCanUpdate:       输入参数，含义: 是否以可写的方式打开
 * bool bShare:           输入参数，含义: 是否以共享的方式打开
 *
 * 返回值: 打开成功返回TRUE,否则返回FALSE
*************************************************************************************/
bool CRSImage::Open(string lpstrFileName, bool bCanUpdate, bool bShare)
{
	Destory();

	char version[256];
	const  char* ver = GDALVersionInfo(version);	//获取gdal版本信息  hlc 2012-5-23
    string strVer = ver;
    filesys.OutputLogMessage("GDAL Version is: "+ strVer);

	pimginfo = new CImageInfo;
	if (!pimginfo->LoadImage(lpstrFileName,bCanUpdate,bShare))
	{
		filesys.OutputLogMessage("打开影像失败!");
		Destory();
		return FALSE;
	}
	poDataSet = pimginfo->GetPoDataSet();

	return TRUE;
	

}

/*************************************************************************************
 * 函数名称: double GetGeoStartPosH()
 * 
 * 函数功能: 获取水平方向的起始坐标
*************************************************************************************/
double CRSImage::GetGeoStartPosH()
{
	if (!pimginfo)
	{
		return 0;
	}
	return pimginfo->GetGeoStartPosH();
}

/*************************************************************************************
 * 函数名称: double GetGeoStartPosV()
 * 
 * 函数功能: 获取垂直方向的起始坐标
*************************************************************************************/
double CRSImage::GetGeoStartPosV()
{
	if (!pimginfo)
	{
		return 0;
	}
	return pimginfo->GetGeoStartPosV();
}
/*************************************************************************************
 * 函数名称: double GetResolveH()
 * 
 * 函数功能: 获取水平方向的分辨率
*************************************************************************************/
double CRSImage::GetResolveH()
{
	if (!pimginfo)
	{
		return 0;
	}
	return pimginfo->GetResolveH();
}

/*************************************************************************************
 * 函数名称: double GetResolveV()
 * 
 * 函数功能: 获取垂直方向的分辨率
*************************************************************************************/
double CRSImage::GetResolveV()
{
	if (!pimginfo)
	{
		return 0;
	}
	return pimginfo->GetResolveV();
}

/*************************************************************************************
 * 函数名称: bool GetGeoTransform()
 * 
 * 函数功能: 获取反射变换系数
*************************************************************************************/
bool CRSImage::GetGeoTransform(double **adfT)
{
	if(!pimginfo) return FALSE;
	return pimginfo->GetGeoTransform(adfT);
}
/*************************************************************************************
 * 函数名称: bool SetGeoTransform()
 * 
 * 函数功能: 设置反射变换系数
*************************************************************************************/
bool CRSImage::SetGeoTransform(double *adfT)
{
	if(!pimginfo) return FALSE;
	pimginfo->SetGeoTransform(adfT);
	return TRUE;
}
/*************************************************************************************
 * 函数名称: bool GetProjection()
 * 
 * 函数功能: 获取投影信息
*************************************************************************************/
bool CRSImage::GetProjection(const char **proj)
{
	if(!pimginfo) return FALSE;
	pimginfo->GetProjection(proj);
	return TRUE;
}

/*************************************************************************************
 * 函数名称: bool SetProjection()
 * 
 * 函数功能: 设置投影信息
*************************************************************************************/
bool CRSImage::SetProjection(const char *proj)
{
	if(!pimginfo) return FALSE;
	pimginfo->SetProjection(proj);
	return TRUE;
}


/*************************************************************************************
 * 函数名称: string GetImageFileName()
 * 
 * 函数功能: 从一个路径中分离出文件名,如果没有传入参数，则返回影像文件名
*************************************************************************************/
string CRSImage::GetImageFileName(string fullpath)
{
	if(!pimginfo) return ("");
	return pimginfo->GetImageFileName(fullpath);
}

/*************************************************************************************
 * 函数名称: string GetImagePath()
 * 
 * 函数功能: 将文件名从一个路径中删除，如果没有传入参数，则返回影像路径(不包含文件名)
*************************************************************************************/
string CRSImage::GetImagePath(string fullpath)
{
	if(!pimginfo) return ("");
	return pimginfo->GetImagePath(fullpath);
	
}

/*************************************************************************************
 * 函数名称: string GetImageFullPath()
 * 
 * 函数功能: 获取影像的全局路径
*************************************************************************************/
string CRSImage::GetImageFullPath() const
{
	if(!pimginfo) return ("");
	return pimginfo->GetImageFullPath();
	
}

/*************************************************************************************
 * 函数名称: string GetImageRPCPath()
 * 
 * 函数功能: 获得影像的RPC路径，如果有的话
*************************************************************************************/
bool CRSImage::GetImageRPCPath(char** path)
{
	if(!pimginfo) return FALSE;
	return pimginfo->GetImageRPCPath(path);
}

/*************************************************************************************
 * 函数名称: DWORD GetBandCount()
 * 
 * 函数功能: 获取影像的波段数
*************************************************************************************/
DWORD CRSImage::GetBandCount()const
{
	if(!pimginfo)  return 0;
	return pimginfo->GetBandCount();
}

/*************************************************************************************
 * 函数名称: DWORD GetHeight()
 * 
 * 函数功能: 获取影像高
*************************************************************************************/
DWORD CRSImage::GetHeight()const
{
	if(!pimginfo)  return 0;
	return pimginfo->GetHeight();
}

/*************************************************************************************
 * 函数名称: DWORD GetWidth()
 * 
 * 函数功能: 获取影像宽
*************************************************************************************/
DWORD CRSImage::GetWidth()const
{
	if(!pimginfo)  return 0;
	return pimginfo->GetWidth();
}


/*************************************************************************************
 * 函数名称: bool ReadImageDataToByte()
 * 
 * 函数功能: 读取整幅影像数据,所有波段，并且将数据类型转换为BYTE
*************************************************************************************/
bool CRSImage::ReadImageDataToByte(BYTE **byteperband)
{
	if (!byteperband || !pimginfo) return FALSE;

	if (pimginfo->GetImageDataSize()>100*1024*1024)	//如果影像超过了100M，则不允许直接将整幅影像读入内存
	{
		filesys.OutputLogMessage("影像文件较大，不宜直接将所有影像数据读入内存!");
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(byteperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(byteperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(byteperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(byteperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(byteperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(byteperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}
	
	return TRUE;

}

/*************************************************************************************
 * 函数名称: bool ReadImageDataToByte()
 * 
 * 函数功能: 读取整幅影像一个区域的数据,指定的三个波段（或少于三个波段），并且将数据类型转换为BYTE
 * 参数说明:	nXOff	区域与影像左边界的偏移值
				nYOff	区域与影像上边界的偏移值
				nXSize	区域宽
				nYSize	区域高
				bands	保存波段序列的数组（包含三个数字，分别表示需要读取哪三个波段或者少于三个波段,值为0表示无效 ）
				nbands	波段数(小于或等于3)
				byteperband 保存，读取并且经过转换的数据
*************************************************************************************/
bool CRSImage::ReadImageDataToByte(int nXOff, int nYOff, int nXSize, int nYsize, int nbands, int *bands, BYTE **byteperband)
{

	if (nXOff<0 || nYOff<0 || nXSize<1 || nYsize<1 || nbands<1 || bands==NULL || byteperband==NULL ||!pimginfo )
	{
		return FALSE;
	}
	//如果三个波段的值都为0（无效），就返回
	if (bands[0]==0 &&bands[1]==0 &&bands[2]==0)
	{
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;
		}
	case GDT_Int32:
		{
			CRSImageData<long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands, byteperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}
	
	return TRUE;

}

/*************************************************************************************
 * 函数名称: bool ReadImageDataToByte()
 * 
 * 函数功能: 读取整幅影像一个区域的数据,指定的三个波段（或少于三个波段），并且将数据类型转换为BYTE(线性拉伸指定最大最小值)
 * 参数说明:	nXOff	区域与影像左边界的偏移值
				nYOff	区域与影像上边界的偏移值
				nXSize	区域宽
				nYSize	区域高
				bands	保存波段序列的数组（包含三个数字，分别表示需要读取哪三个波段或者少于三个波段,值为0表示无效 ）
				nbands	波段数(小于或等于3)
				byteperband 保存，读取并且经过转换的数据
*************************************************************************************/
bool CRSImage::ReadImageDataToByte(int nXOff, int nYOff, int nXSize, int nYsize, int nbands, int *bands,
	double* graymax,double* graymin, BYTE **byteperband)
{

	if (nXOff<0 || nYOff<0 || nXSize<1 || nYsize<1 || nbands<1 || bands==NULL || byteperband==NULL ||!pimginfo ||  graymax==NULL ||graymin==NULL)
	{
		return FALSE;
	}
	//如果三个波段的值都为0（无效），就返回
	if (bands[0]==0 &&bands[1]==0 &&bands[2]==0)
	{
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;
		}
	case GDT_Int32:
		{
			CRSImageData<long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataToByte(nXOff, nYOff, nXSize, nYsize, bands, nbands,graymax,graymin, byteperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}
	
	return TRUE;

}


/*************************************************************************************
 * 函数名称: GDALDataType GetRasterDataType()
 * 
 * 函数功能: 栅格数据的数据类型
*************************************************************************************/
GDALDataType CRSImage::GetRasterDataType(void)
{
	if(!pimginfo)  return GDT_Unknown;
	return pimginfo->GetRasterDataType();
}

/*************************************************************************************
 * 函数名称: int GetDataTypeSize()
 * 
 * 函数功能: 栅格数据的数据类型占用的字节数
*************************************************************************************/
int CRSImage::GetDataTypeSize(void)
{
	if(!pimginfo)  return 0;
	return pimginfo->GetDataTypeSize();
}


/*************************************************************************************
 * 函数名称: bool New(string lpstrFilePath, string lpstrFmt, long nBand, long nHeight, long nWidth, RSDataType eDataType)
 * 
 * 函数功能: 创建一个新的RSImage对象
 *
 * 参数说明: 
 * string lpstrFilePath:  输入参数，含义: 文件路径名
 * string lpstrFmt:       输入参数，含义: 图像格式短名
 * long nBand:             输入参数，含义: 图像波段数
 * long nHeight:           输入参数，含义: 图像高度
 * long nWidth:            输入参数，含义: 图像宽度
 * RSDataType eDataType:   输入参数，含义: 图像数据类型
 *
 * 返回值:创建成功返回TRUE,否则返回FALSE
*************************************************************************************/
bool CRSImage::New(string lpstrFilePath, long nBand, long nHeight, long nWidth, GDALDataType eDataType, string lpstrFmt)
{
	Destory();
	pimginfo = new CImageInfo;
	if (!pimginfo->NewImage(lpstrFilePath,nBand,nHeight,nWidth,eDataType,lpstrFmt))
	{
		filesys.OutputLogMessage("创建影像失败!");
		Destory();
		return FALSE;
	}
	poDataSet = pimginfo->GetPoDataSet();	

	return TRUE;
}

/*************************************************************************************
 * 函数名称: ReadBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband)
 * 
 * 函数功能: 读取影像块数据
 *
 * 参数说明: 
 *
 * 返回值:读取成功返回TRUE,否则返回FALSE
*************************************************************************************/
bool CRSImage::ReadBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband)
{
	if(nXSize<1 || nYSize<1 || bands==NULL || _nbandcount<1 || _dataperband==NULL)
	{
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(BYTE**)_dataperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(unsigned short**)_dataperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(short**)_dataperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(unsigned long**)_dataperband);
			break;
		}
	case GDT_Int32:
		{
			CRSImageData<long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(long**)_dataperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(float**)_dataperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,(double**)_dataperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}


	return TRUE;
}

/*************************************************************************************
 * 函数名称: ReadBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband)
 *
 * 函数功能: 读取影像块数据
 *
 * 参数说明:
 *
 * 返回值:读取成功返回TRUE,否则返回FALSE
 *************************************************************************************/
bool CRSImage::ReadBlockData_overview(int nXOff, int nYOff, int nXSize, int nYSize,int bufferXsize,int bufferYsize, int *bands, int _nbandcount, void** _dataperband)
{
    if(nXSize<1 || nYSize<1 || bands==NULL || _nbandcount<1 || _dataperband==NULL)
    {
        return FALSE;
    }
    
    switch(pimginfo->GetRasterDataType())
    {
        case GDT_Byte:
        {
            CRSImageData<BYTE> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(BYTE**)_dataperband);
            break;
        }
        case GDT_UInt16:
        {
            CRSImageData<unsigned short> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(unsigned short**)_dataperband);
            break;
        }
        case GDT_Int16:
        {
            CRSImageData<short> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(short**)_dataperband);
            break;
        }
        case GDT_UInt32:
        {
            CRSImageData<unsigned long> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(unsigned long**)_dataperband);
            break;
        }
        case GDT_Int32:
        {
            CRSImageData<long> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(long**)_dataperband);
            break;
        }
        case GDT_Float32:
        {
            CRSImageData<float> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(float**)_dataperband);
            break;
        }
        case GDT_Float64 :
        {
            CRSImageData<double> data;
            data.poDataSet	= poDataSet;
            data.nbandcount = GetBandCount();
            data.nWidth		= GetWidth();
            data.nHeight	= GetHeight();
            data.ReadBlockData_overview(nXOff,nYOff,nXSize,nYSize,bufferXsize,bufferYsize,bands,_nbandcount,(double**)_dataperband);
            break;		
        }	
        case GDT_CInt32:
            return FALSE;
            break;
        case GDT_CFloat32 :
            return FALSE;
            break;
        case GDT_CFloat64:
            return FALSE;
            break;
        default:
            return FALSE;
            break;
    }
    
    
    return TRUE;
}

/*************************************************************************************
 * 函数名称: bool ReadBlockDataTofloat()
 * 
 * 函数功能: 读取影像一个区域的数据,指定的几个波段，并且将数据类型转换为float
 * 参数说明:	nXOff	区域与影像左边界的偏移值
				nYOff	区域与影像上边界的偏移值
				nXSize	区域宽
				nYSize	区域高
				bands	保存波段序列的数组（包含几个数字，分别表示需要读取哪三个波段或者少于三个波段,值为0表示无效 ）
				nbands	波段数
				_dataperband 保存，读取并且经过转换的数据
*************************************************************************************/
bool CRSImage::ReadBlockDataTofloat(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, float** _dataperband)
{
	if(nXSize<1 || nYSize<1 || bands==NULL || _nbandcount<1 || _dataperband==NULL)
	{
		return FALSE;
	}

	if (bands[0]==0/* &&bands[1]==0 &&bands[2]==0*/)
	{
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, nXSize, nYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, nXSize, nYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, nXSize, nYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, nXSize, nYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, nXSize, nYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, nXSize, nYSize, bands, _nbandcount, _dataperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;

}

/*************************************************************************************
 * 函数名称: bool ReadBlockDataTofloat()
 * 
 * 函数功能: 读取影像一个区域的数据,指定的几个波段，并且将数据类型转换为float，可以读取金字塔影像
 * 参数说明:	nXOff	区域与影像左边界的偏移值
				nYOff	区域与影像上边界的偏移值
				nXSize	区域宽
				nYSize	区域高
				bufXSize 读取到内存中的宽
				bufYSize 读取到内存中的高
				bands	保存波段序列的数组（包含几个数字，分别表示需要读取哪三个波段或者少于三个波段,值为0表示无效 ）
				nbands	波段数
				_dataperband 保存，读取并且经过转换的数据
				*************************************************************************************/
bool CRSImage::ReadBlockDataTofloat(int nXOff, int nYOff, int nXSize, int nYSize, int bufXSize, int bufYSize, int *bands, int _nbandcount, float** _dataperband)
{
	if(nXSize<1 || nYSize<1 ||bufXSize<1 || bufYSize<1 || bands==NULL || _nbandcount<1 || _dataperband==NULL)
	{
		return FALSE;
	}

	if (bands[0]==0/* &&bands[1]==0 &&bands[2]==0*/)
	{
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Int32:
		{
			CRSImageData<long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.ReadImageDataTofloat(nXOff, nYOff, nXSize, nYSize, bufXSize, bufYSize, bands, _nbandcount, _dataperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;

}

/*************************************************************************************
 * 函数名称: WriteBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband)
 * 
 * 函数功能: 写入影像块数据
 *
 * 参数说明: 
 *
 * 返回值:写入成功返回TRUE,否则返回FALSE
*************************************************************************************/
bool CRSImage::WriteBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband, bool bFlushCache)
{

	if(nXSize<1 || nYSize<1 || bands==NULL || _nbandcount<1 || _dataperband==NULL)
	{
		return FALSE;
	}

	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			//测试
			/*	BYTE **bit = (BYTE**)_dataperband;
			int a;
			for (int i=0;i<4;i++)
			{
			for (int j=0;j<10;j++)
			{
			a=bit[i][j];
			}
			}*/

			data.WriteBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,GDT_Byte, (BYTE**)_dataperband);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.WriteBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount, GDT_UInt16,(unsigned short**)_dataperband);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.WriteBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount, GDT_Int16,(short**)_dataperband);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.WriteBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount, GDT_UInt32,(unsigned long**)_dataperband);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.WriteBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount,GDT_Float32,(float**)_dataperband);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.WriteBlockData(nXOff,nYOff,nXSize,nYSize,bands,_nbandcount, GDT_Float64,(double**)_dataperband);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}
    
    if (bFlushCache) {
        poDataSet->FlushCache();    //将数据强制保存的硬盘上，
                                    //否则数据量大的时候，数据留在缓存，会导致内存不足  hlc 2012.6.5
    }

		
    

	return TRUE;
}

/*************************************************************************************
* 函数名称: GDALTermProgress( double dfComplete, const char *pszMessage, void * pProgressArg )  
 * 
 * 函数功能: 显示处理的进度条
 *
 * 参数说明: 
 *
 * 返回值:
*************************************************************************************/
int CPL_STDCALL GDALTermProgress( double dfComplete, const char *pszMessage,   
	void * pProgressArg )  

{  
	static int nLastTick = -1;  
	int nThisTick = (int) (dfComplete * 40.0);  

	(void) pProgressArg;  

	nThisTick = MIN(40,MAX(0,nThisTick));  

	// Have we started a new progress run?     
	if( nThisTick < nLastTick && nLastTick >= 39 )  
		nLastTick = -1;  

	if( nThisTick <= nLastTick )  
		return TRUE;  

	while( nThisTick > nLastTick )  
	{  
		nLastTick++;  
		if( nLastTick % 4 == 0 )  
			fprintf( stdout, "%d", (nLastTick / 4) * 10 );  
		else  
			fprintf( stdout, "." );  
	}  

	if( nThisTick == 40 )  
		fprintf( stdout, " - done.\n" );  
	else  
		fflush( stdout );  

	return TRUE;  
}  

/*************************************************************************************
 * 函数名称: BuildOverView(int pyramidGrade,int *OverViewLists)
 * 
 * 函数功能: 建立影像金字塔
 *
 * 参数说明: 
 *
 * 返回值:写入成功返回TRUE,否则返回FALSE
*************************************************************************************/
bool CRSImage::BuildOverView(int pyramidGrade,int *OverViewLists)
{
	if (!pimginfo || pyramidGrade<1 || OverViewLists==NULL)
	{
		return FALSE;
	}
	
	//判断是否存在金字塔
	GDALRasterBand *poBand;	
	int nhitcount=0;
	for (int i=0;i<pimginfo->GetBandCount();i++)
	{
		poBand = poDataSet->GetRasterBand(i+1);
		int overcount = poBand->GetOverviewCount();
		if(overcount >=  pyramidGrade/*poBand->HasArbitraryOverviews() &&*/ )	//判断是否存在金字塔影像,如果金字塔层数超过需要的层数，就满足条件
		{
			nhitcount++;
		}
	}
	if(nhitcount==pimginfo->GetBandCount())	//每个波段都存在金字塔
	{
		return TRUE;
	}	
	
	//建立金字塔影像
	if(poDataSet->BuildOverviews("GAUSS",pyramidGrade,OverViewLists,0,NULL,GDALTermProgress,NULL) != CE_None )
	{
		filesys.OutputLogMessage(("建立金字塔影像出错！"));
		return FALSE;
	}

	return  TRUE;	
}

/*************************************************************************************
 * 函数名称:  GetHistMaxMinValue(double thr,double *dmax,double* dmin)
 * 
 * 函数功能: 根据直方图的阈值，计算直方图上选取的灰度最大最小值
 *
 * 参数说明: 
 *
 * 返回值:写入成功返回TRUE,否则返回FALSE
 * 添加： 黄灵操  2013-5-30
*************************************************************************************/
bool CRSImage:: GetHistMaxMinValue(double thr,double *dmax,double* dmin, int* bands, int bandcount)
{
	if (!pimginfo)
	{
		return FALSE;
	}
    bool bfree_bands = false;
	if(bands==NULL)
	{
		bandcount = (int)GetBandCount();
		bands = new int[bandcount];
		for (int i=0;i<bandcount;i++)
		{
			bands[i]  = i+1;
		}
        bfree_bands  = true;
	}


	switch(pimginfo->GetRasterDataType())
	{
	case GDT_Byte:
		{
			CRSImageData<BYTE> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;
		}
	case GDT_UInt16:
		{
			CRSImageData<unsigned short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;
		}
	case GDT_Int16:
		{
			CRSImageData<short> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;
		}
	case GDT_UInt32:
		{
			CRSImageData<unsigned long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;
		}
	case GDT_Int32:
		{
			CRSImageData<long> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;
		}
	case GDT_Float32:
		{
			CRSImageData<float> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;
		}
	case GDT_Float64 :
		{
			CRSImageData<double> data;
			data.poDataSet	= poDataSet;
			data.nbandcount = GetBandCount();
			data.nWidth		= GetWidth();
			data.nHeight	= GetHeight();
			data.GetHistMaxMinValue(thr,dmax,dmin,bands,bandcount);
			break;		
		}	
	case GDT_CInt32:
		return FALSE;
		break;
	case GDT_CFloat32 :
		return FALSE;
		break;
	case GDT_CFloat64:
		return FALSE;
		break;
	default:
		return FALSE;
		break;
	}
    
    if(bfree_bands==true)
        delete []bands;



	return TRUE;
}

/*************************************************************************************
 * 函数名称:  GetHistMaxMinValue(double thr,double *dmax,double* dmin)
 *
 * 函数功能: 根据直方图的阈值，计算直方图上选取的灰度最大最小值
 *
 * 参数说明:
 *
 * 返回值:写入成功返回TRUE,否则返回FALSE
 * 添加： 黄灵操  2013-5-30
 *************************************************************************************/
bool CRSImage::GetMaximum_Minimumofoneband(int bandindex,double* maxvalue,double *minvalue)
{
    if (pimginfo==nullptr)
    {
        return false;
    }
    int psuccess = 0;
    double dmax = 0,dmin = 0;
    dmax = pimginfo->GetMaximum(bandindex,&psuccess);
    if(psuccess == 0)
        return false;
    
    dmin = pimginfo->GetMinimum(bandindex,&psuccess);
    if(psuccess==0)
        return false;
    
    *maxvalue = dmax;
    *minvalue = dmin;
    
    return true;
}


/*************************************************************************************
 * 函数名称:  SetNoDataValueforbands(double *nodatavalue,int* bandindex, int bandcount)
 *
 * 函数功能: 设置多个波段的 NoData
 *
 * 参数说明:
 *
 * 返回值:写入成功返回true,否则返回false
 * 添加： 黄灵操  2016-1-18
 *************************************************************************************/
bool CRSImage::SetNoDataValueforbands(double *nodatavalue,int* bandindex, int bandcount)
{
    if (!pimginfo)
    {
        return false;
    }
    if(bandindex==nullptr || nullptr==nodatavalue)
    {
        return false;
    }
    GDALRasterBand *poBand;
    for (int i=0; i<bandcount; i++)
    {
        poBand = poDataSet->GetRasterBand(bandindex[i]);
        
        if(poBand->SetNoDataValue(nodatavalue[i])!=CE_None)
            return false;
    }
    return true;
}

bool CRSImage::SetsameNoDataValueforallbands(double nodatavalue, int bandcount)
{
    if(bandcount<0)
        return false;
    double* tempdouble = new double[bandcount];
    int* bandindex = new int[bandcount];
    for (int i=0; i<bandcount; i++)
    {
        tempdouble[i] = nodatavalue;
        bandindex[i] = i+1;
    }
    bool result = SetNoDataValueforbands(tempdouble, bandindex,  bandcount);
    delete []tempdouble;
    delete []bandindex;
    return result;

}

/*************************************************************************************
 * 函数名称:  Get_prj_location_value(float prj_x,float float_y,int *bands, float* value,int resamplingAlg)
 *
 * 函数功能: 获取指定位置的指定波段的像素值
 *
 * 参数说明:
 *
 * 返回值:写入成功返回true,否则返回false
 * 添加： 黄灵操  2016-4-12
 *************************************************************************************/
bool CRSImage::Get_prj_location_value(float prj_x,float prj_y,int bandcount,int *bands, float* value,int resamplingAlg)
{
    double pixel_x=0,pixel_y=0;
    if (ConvertPrjxy_2_Pixelxy(prj_x, prj_y, &pixel_x, &pixel_y)==false)
        return false;
    return Get_pixel_location_value(round(pixel_x),round(pixel_y),bandcount,bands, value,resamplingAlg);
    
}
bool CRSImage::Get_pixel_location_value(int pixel_x,int pixel_y,int bandcount,int *bands, float* value,int resamplingAlg)
{
//    if (pimginfo == nullptr)
//    {
//        filesys.OutputLogMessage("please open the image file first");
//        return false;
//    }
//    int win_x = 3;
//    int win_y = 3;
    int width = (int)pimginfo->GetWidth();
    int height = (int)pimginfo->GetHeight();
    float **dataperband = new float*[bandcount];
    for(int i=0;i<bandcount;i++)
    {
        dataperband[i] =  new float[1];
    }
//    int nXOff,nYOff,nXright,nYbottom,nXSize,nYsize;
    if(pixel_x>=0 && pixel_y>=0 && pixel_x<width && pixel_y<height)
    {
        if(resamplingAlg==1)
        {
            
            ReadBlockDataTofloat(pixel_x, pixel_y, 1, 1, bands, bandcount, dataperband);
            for(int i=0;i<bandcount;i++)
            {
                value[i] = dataperband[i][0];
            }
            
        }else
        {
//            nXOff = pixel_x - win_x/2;
//            if(nXOff<0)
//                nXOff = 0;
//            nYOff = pixel_y - win_y/2;
//            if(nYOff<0)
//                nYOff = 0;
//            nXright =pixel_x + win_x/2;
//            if(nXright > )
//                nYbottom = pixel_y + win_y/2;
//            ReadBlockDataTofloat()
        }

        
    }else
    {
        for(int i=0;i<bandcount;i++)
        {
            delete []dataperband[i];
        }
        delete [] dataperband;
        filesys.OutputLogMessage("pixel location exceeds the range");
        cout<<"pixel_x:"<<pixel_x<<"  pixel_y:"<<pixel_y<<"  width:"<<width<<" height:"<<height<<endl;
        return false;
    }
    for(int i=0;i<bandcount;i++)
    {
        delete []dataperband[i];
    }
    delete [] dataperband;
    
    return true;
}

/*************************************************************************************
 * 函数名称:  ConvertPrjxy_2_Pixelxy(double prj_x,double prj_y,double* pixel_x,double *pixel_y)
 *
 * 函数功能:
 *
 * 参数说明:
 *
 * 返回值:写入成功返回true,否则返回false
 * 添加： 黄灵操  2016-4-12
 *************************************************************************************/
bool CRSImage::ConvertPrjxy_2_Pixelxy(double prj_x,double prj_y,double* pixel_x,double *pixel_y)
{
    double *adfGeoTransform = nullptr;
    pimginfo->GetGeoTransform(&adfGeoTransform);
    if(adfGeoTransform==nullptr)
    {
        filesys.OutputLogMessage("please open image file first");
        return false;
    }
    *pixel_x = (prj_x - adfGeoTransform[0])/adfGeoTransform[1];
    *pixel_y = (prj_y - adfGeoTransform[3])/adfGeoTransform[5];
    
    return true;
}

bool CRSImage::ConvertPixelxy_2_Prjxy(double pixel_x,double pixel_y,double* prj_x,double *prj_y)
{
    double *adfGeoTransform = nullptr;
    pimginfo->GetGeoTransform(&adfGeoTransform);
    if(adfGeoTransform==nullptr)
    {
        filesys.OutputLogMessage("eror : getting adfGeoTransform is failed");
        return false;
    }
    
    *prj_x = adfGeoTransform[0] + pixel_x*adfGeoTransform[1];//adfGeoTransform[0] + pixel_x*adfGeoTransform[1] + pixel_y*adfGeoTransform[2];
    *prj_y = adfGeoTransform[3] + pixel_y*adfGeoTransform[5];//adfGeoTransform[3] + pixel_x*adfGeoTransform[4] + pixel_y*adfGeoTransform[5];
    
    return true;
}

