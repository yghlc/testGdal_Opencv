/* RSImage.h: interface for the CRSImage class.
///�ļ��� Image.h
///��  �룺 	       
///��  ���� 
///����ֵ�� ��
///���������� ����GDAL��Ӱ����ж�ȡ�ͱ���
///ȫ�ֱ�������
///��Ա������
///����ģ�飺
///�㷨˵������
///ע  �ͣ� ��
///�����ˣ� �����
///��  �ڣ� 2010��4��16��
///�޸��ˣ�
///��  �ڣ�
///��ע:	��CRSImage���棬��GDALDataset����Ϊ��ȫ�ֱ�������ˣ�
//			һ�������CRSImage��Ӧһ��Ӱ���ļ��������Ǵ򿪣����Ǳ��棬�������׳���
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIMAGE_H__30168B24_E074_4048_8B53_C0F794203D48__INCLUDED_)
#define AFX_RSIMAGE_H__30168B24_E074_4048_8B53_C0F794203D48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageInfo.h"		//ImageInfo.h�Ѿ�������GDAL��ͷ�ļ����������˶�̬��
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
	CImageInfo *pimginfo;		//Ӱ����Ϣ
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
	
	string GetImageFileName(string fullpath = (""));//��һ��·���з�����ļ���,���û�д���������򷵻�Ӱ���ļ���	
	string GetImagePath(string fullpath=(""));	//���ļ�����һ��·����ɾ�������û�д���������򷵻�Ӱ��·��(�������ļ���)
	string GetImageFullPath(void) const;
	bool GetImageRPCPath(char** path);

	DWORD GetBandCount(void) const;
	DWORD GetHeight(void)const ;
	DWORD GetWidth(void)const;


	GDALDataType GetRasterDataType(void);
	//��ȡ��������ռ�õ��ֽ���
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
