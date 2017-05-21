/* RSImage.h: interface for the CRSImage class.
///文件： Image.h
///输  入： 	       
///输  出： 
///返回值： 无
///功能描述： 利用GDAL对影像进行读取和保存
///全局变量：无
///成员变量：
///调用模块：
///算法说明：无
///注  释： 无
///创建人： 黄灵操
///日  期： 2010年4月16日
///修改人：
///日  期：
///备注:	在CRSImage里面，将GDALDataset保存为了全局变量，因此，
//			一个类对象CRSImage对应一个影像文件，不管是打开，还是保存，否则容易出错
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIMAGE_H__30168B24_E074_4048_8B53_C0F794203D48__INCLUDED_)
#define AFX_RSIMAGE_H__30168B24_E074_4048_8B53_C0F794203D48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageInfo.h"		//ImageInfo.h已经包含了GDAL的头文件，并链接了动态库
#include "RSImageData.h"


class CRSImage  
{
public:
	bool New(string lpstrFilePath, long nBand, long nHeight, long nWidth, GDALDataType eDataType, string lpstrFmt=("GTiff"));
	bool Open(string lpstrFileName, bool bCanUpdate=FALSE, bool bShare=TRUE);
	CRSImage();
	virtual ~CRSImage();

private:
	void Destory();
	CImageInfo *pimginfo;		//影像信息
	GDALDataset *poDataSet;	
	
	
public:
	bool WriteBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband, bool bFlushCache=false);
	bool ReadBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, void** _dataperband);
    bool ReadBlockData_overview(int nXOff, int nYOff, int nXSize, int nYSize,int bufferXsize,int bufferYsize, int *bands, int _nbandcount, void** _dataperband);
    
	bool ReadImageDataToByte(int nXOff=0, int nYOff=0, int nXSize=0, int nYsize=0, int nbands=0, int *bands=NULL, BYTE **byteperband=NULL);
	bool ReadImageDataToByte(int nXOff, int nYOff, int nXSize, int nYsize, int nbands, int *bands,double* graymax,double* graymin, BYTE **byteperband);
	bool ReadImageDataToByte(BYTE **byteperband = NULL);
	bool ReadBlockDataTofloat(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, float** _dataperband);
	bool ReadBlockDataTofloat(int nXOff, int nYOff, int nXSize, int nYSize, int bufXSize, int bufYSize, int *bands, int _nbandcount, float** _dataperband);

	double GetGeoStartPosH();
	double GetGeoStartPosV();
	double GetResolveH();
	double GetResolveV();

	bool GetGeoTransform(double **adfT);
	bool SetGeoTransform(double *adfT);
	bool SetProjection(const char *proj);
	bool GetProjection(const char **proj);

	bool IsValid(void) const{return ((poDataSet!=NULL) && pimginfo!=NULL);}
	GDALDataset* GetGdalDataSet(void){return poDataSet;};
	
	string GetImageFileName(string fullpath = (""));//从一个路径中分离出文件名,如果没有传入参数，则返回影像文件名	
	string GetImagePath(string fullpath=(""));	//将文件名从一个路径中删除，如果没有传入参数，则返回影像路劲(不包含文件名)
	string GetImageFullPath(void) const;
	bool GetImageRPCPath(char** path);

	DWORD GetBandCount(void) const;
	DWORD GetHeight(void)const ;
	DWORD GetWidth(void)const;


	GDALDataType GetRasterDataType(void);
	//获取数据类型占用的字节数
	int GetDataTypeSize(void);
	bool BuildOverView(int pyramidGrade,int *OverViewLists);
	bool GetHistMaxMinValue(double thr,double *dmax,double* dmin, int* bands=NULL, int bandcount=0);
    
    bool GetMaximum_Minimumofoneband(int bandindex,double* maxvalue,double *minvalue);
    
    bool SetNoDataValueforbands(double *nodatavalue,int* bandindex, int bandcount);
    bool SetsameNoDataValueforallbands(double nodatavalue, int bandcount);
    
    bool ConvertPrjxy_2_Pixelxy(double prj_x,double prj_y,double* pixel_x,double *pixel_y);
    bool ConvertPixelxy_2_Prjxy(double pixel_x,double pixel_y,double* prj_x,double *prj_y);
    
    bool Get_prj_location_value(float prj_x,float prj_y,int bandcount,int *bands, float* value,int resamplingAlg);
    bool Get_pixel_location_value(int pixel_x,int pixel_y,int bandcount,int *bands, float* value,int resamplingAlg);
    
};

#endif // !defined(AFX_RSIMAGE_H__30168B24_E074_4048_8B53_C0F794203D48__INCLUDED_)
