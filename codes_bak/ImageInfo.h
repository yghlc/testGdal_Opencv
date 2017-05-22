/* ImageInfo.h: interface for the CImageInfo class.
/////文件： ImageInfo.h
///输  入：
///输  出：
///返回值： 无
///功能描述： 利用GDAL对影像信息进行读取和保存
///全局变量：无
///成员变量：
///调用模块：
///算法说明：无
///注  释： 无
///创建人： 黄灵操
///日  期： 2011年4月16日
///修改人：
///日  期：
///备注:	在CImageInfo里面，将GDALDataset保存为了全局变量，因此，
//			一个类对象CImageInfo对应一个影像文件，不管是打开，还是保存，否则容易出错
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEINFO_H__1F33E17B_89DF_487A_BB72_2A791470BC9F__INCLUDED_)
#define AFX_IMAGEINFO_H__1F33E17B_89DF_487A_BB72_2A791470BC9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
//设置GDAL环境（包含GDAL头文件和静态链接库）  //黄灵操2010年5月17日 添加
#include <gdal_priv.h>                //GDAL头文件
#   pragma comment (lib, "gdal_i.lib")    //链接gdal静态链接库
//说明：2012年5月23日，将gdal的版本改为 1.9.0
//////////////////////////////////////////////////////////////////////////
#include "typedefine.h"

//#include "tchar.h"

class CRSImage;

class CImageInfo  
{
	friend class CRSImage;
public:
	long GetImageDataSize();
	GDALDataset* GetPoDataSet();
	
	GDALDataType GetRasterDataType()const{return m_datatype;}
	//获取数据类型占用的字节数
	int GetDataTypeSize()const
	{ 
		return GDALGetDataTypeSize(m_datatype)/8;
	}

	double  GetGeoStartPosH();
//	void    SetGeoStartPosH(double newValue);
	double  GetGeoStartPosV();
//	void    SetGeoStartPosV(double newValue);
//	long    GetImgStartPosH();
//	void    SetImgStartPosH(long nNewValue);
//	long    GetImgStartPosV();
//	void    SetImgStartPosV(long nNewValue);
	double  GetResolveH();
//	void    SetResolveH(double newValue);
	double  GetResolveV();
//	void    SetResolveV(double newValue);
	bool GetImageRPCPath(char** path);
    
    double GetMaximum(int bandindex,int * pbSuccess = NULL);
    double GetMinimum(int bandindex,int * pbSuccess = NULL);

	bool GetGeoTransform(double **adfT);
	bool SetGeoTransform(double *adfT);
	bool SetProjection(const char *proj);
	bool GetProjection(const char **proj);
	//从一个路径中分离出文件名,如果没有传入参数，则返回影像文件名
	string GetImageFileName(string fullpath = (""));
	//将文件名从一个路径中删除，如果没有传入参数，则返回影像路劲(不包含文件名)
	string GetImagePath(string fullpath=(""));
	bool IsValid(void) const{return  (poDataset!=NULL);}
	string GetImageFullPath(void) const{ return imgPath;}
	DWORD GetBandCount(void) const {return nbandcount;}
	DWORD GetHeight(void)const {return nHeight;} 
	DWORD GetWidth(void)const{	return nWidth;};
	bool LoadImage(string filename, bool bCanUpdate=FALSE, bool bShare=TRUE);
	bool NewImage(string lpstrFilePath, long nBand, long _nHeight, long _nWidth, GDALDataType eDataType, string lpstrFmt=("GTiff"));
	CImageInfo(void);
	virtual ~CImageInfo();

protected:
	string imgPath;			//影像的路径, 绝对路径
	DWORD nbandcount;			//影像的波段数
	DWORD nWidth;				//影像宽
	DWORD nHeight;				//影像高
	string projectiontype;		//投影类型		
	int nbandtype;				//影像像素的保存类型（如8bit，32bit等，这里认为各波段的灰度级一样,有几个bit值就是几）
	GDALDataType m_datatype;		//影像的数据类型
	GDALDataset *poDataset;		//GDALDataset类对象

private:
	void FreeData();
	string	GeoDescription;		//影像驱动描述
	string GeoMetadataItem;	//影像格式描述
	string	GeoProjectionRef;	//投影信息
	double	*adfGeoTransform;	//坐标和空间分辨率
	char*   m_strRPCpath;

};

#endif // !defined(AFX_IMAGEINFO_H__1F33E17B_89DF_487A_BB72_2A791470BC9F__INCLUDED_)
