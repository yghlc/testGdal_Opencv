// RSImageData.h: interface for the CRSImageData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIMAGEDATA_H__5778AFD5_E522_4130_8BF0_0537369D792D__INCLUDED_)
#define AFX_RSIMAGEDATA_H__5778AFD5_E522_4130_8BF0_0537369D792D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRSImage;

#include "CFileSystem.h"
extern CFileSystem filesys;

template <typename T>
class CRSImageData  
{
	friend class CRSImage;

private:
	//属性
	GDALDataset *poDataSet;	
	long		nWidth;			//影像宽
	long		nHeight;		//影像高
	long		nbandcount;		//影像波段数
	T**			dataperband;	//各波段影像数据
	

public:
	//////////////////////////////////////////////////////////////////////////
	//构造函数
	CRSImageData()
	{
		poDataSet		= NULL;
		nWidth			= 0;
		nHeight			= 0;
		nbandcount		= 0;
		dataperband		= NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	//析构函数
	virtual ~CRSImageData()
	{
		if (dataperband)
		{
			for (int i=0;i<nbandcount;i++)
			{
				delete []dataperband[i];
			}
			delete []dataperband;dataperband=NULL;
		}
		
	}

	//////////////////////////////////////////////////////////////////////////
	//读取整幅影像数据,所有波段，并且将数据类型转换为BYTE
	bool ReadImageDataToByte(BYTE **byteperband)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数ReadImageDataToByte: 影像参数不正确");
			return FALSE;
		}

		//分配内存
		dataperband = new T*[nbandcount];
		if(!dataperband) return FALSE;
		int i;
		for (int i=0;i<nbandcount;i++)
		{
			dataperband[i] = new T[nWidth*nHeight];
			if(!dataperband[i]) return FALSE;
			memset(dataperband[i],0,sizeof(T)*nWidth*nHeight);
		}


		//对每个波段进行处理，在此之前，已经判定过个波段间的长宽要一致，数据类型一致
		GDALRasterBand *poBand;		
		int nXSize = (int)nWidth;
		int nYSize = (int)nHeight;
		//获取影像各波段的数据
		for ( i=0;i<nbandcount;i++)
		{
			poBand = poDataSet->GetRasterBand(i+1); //针对地i+1个波段

			poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
				0, //相对于该波段影像最左边的偏移量
				0, //相对于该波段影像最上边的偏移量
				nXSize,  //影像文件中的宽 
				nYSize,  //影像文件中的长
				dataperband[i], //指向保存数据的数组指针
				nXSize, //读内存中影像的宽
				nYSize, //读内存中影像的长
				poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
				0,  //像素间x方向的偏移量
				0 ); //像素间y方向偏移量
		}
		
		//对每个波段进行线性拉伸
		for (i=0;i<nbandcount;i++)
		{
			LinearStretchT(dataperband[i],byteperband[i],nWidth*nHeight);
		}
		
		return TRUE;
		
	}


	//////////////////////////////////////////////////////////////////////////
	//读取整幅影像一个区域的数据,指定的三个波段（或少于三个波段），并且将数据类型转换为BYTE
	//添加：黄灵操
	//时间：2011-5-5
	bool ReadImageDataToByte(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, BYTE **byteperband)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数ReadImageDataToByte: 影像参数不正确");
			return FALSE;
		}

		//分配内存
		nbandcount = _nbandcount;	
		dataperband = new T*[nbandcount];	//nbandcount <= 3
		if(!dataperband) return FALSE;
		int i;
		for (i=0;i<nbandcount;i++)
		{
			dataperband[i] = new T[nXSize*nYSize];
			if(!dataperband[i]) return FALSE;
			memset(dataperband[i],0,sizeof(T)*nXSize*nYSize);
		}


		//对每个波段进行处理，在此之前，已经判定每个波段间的长宽要一致，数据类型一致
		GDALRasterBand *poBand;	
//		int nXSize = nWidth;
//		int nYSize = nHeight;
		//获取影像各波段的数据
		for ( i=0;i<nbandcount;i++)
		{
			if (bands[i]<=0) continue;
			poBand = poDataSet->GetRasterBand(bands[i]); //针对地bands[i]个波段

			poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
				nXOff, //相对于该波段影像最左边的偏移量
				nYOff, //相对于该波段影像最上边的偏移量
				nXSize,  //影像文件中的宽 
				nYSize,  //影像文件中的长
				dataperband[i], //指向保存数据的数组指针
				nXSize, //读内存中影像的宽
				nYSize, //读内存中影像的长
				poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
				0,  //像素间x方向的偏移量
				0 ); //像素间y方向偏移量
		}
		
		//对每个波段进行线性拉伸
		for (i=0;i<nbandcount;i++)
		{
			LinearStretchT(dataperband[i],byteperband[i],nXSize*nYSize);
		}
		
		return TRUE;

		
	}

	//////////////////////////////////////////////////////////////////////////
	//读取整幅影像一个区域的数据,指定的三个波段（或少于三个波段），并且将数据类型转换为BYTE
	//添加：黄灵操
	//时间：2013-5-30
	bool ReadImageDataToByte(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount,double* graymax,double* graymin, BYTE **byteperband)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数ReadImageDataToByte: 影像参数不正确");
			return FALSE;
		}

		//分配内存
		nbandcount = _nbandcount;	
		dataperband = new T*[nbandcount];	//nbandcount <= 3
		if(!dataperband) return FALSE;
		int i;
		for (i=0;i<nbandcount;i++)
		{
			dataperband[i] = new T[nXSize*nYSize];
			if(!dataperband[i]) return FALSE;
			memset(dataperband[i],0,sizeof(T)*nXSize*nYSize);
		}


		//对每个波段进行处理，在此之前，已经判定每个波段间的长宽要一致，数据类型一致
		GDALRasterBand *poBand;	
		//		int nXSize = nWidth;
		//		int nYSize = nHeight;
		//获取影像各波段的数据
		for ( i=0;i<nbandcount;i++)
		{
			if (bands[i]<=0) continue;
			poBand = poDataSet->GetRasterBand(bands[i]); //针对地bands[i]个波段

			poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
				nXOff, //相对于该波段影像最左边的偏移量
				nYOff, //相对于该波段影像最上边的偏移量
				nXSize,  //影像文件中的宽 
				nYSize,  //影像文件中的长
				dataperband[i], //指向保存数据的数组指针
				nXSize, //读内存中影像的宽
				nYSize, //读内存中影像的长
				poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
				0,  //像素间x方向的偏移量
				0 ); //像素间y方向偏移量
		}

		//对每个波段进行线性拉伸
		int nsize = nXSize*nYSize;
		for (i=0;i<nbandcount;i++)
		{
			BYTE *result = byteperband[i];
			T* inputtemp = dataperband[i];
			int gtemp;
			//LinearStretchT(dataperband[i],byteperband[i],nXSize*nYSize);
			int bytemax = graymax[i];
			int bytemin = graymin[i];
			double k=255.0/(bytemax - bytemin);
			//采用线性查找表
			DWORD leg = bytemax - bytemin +1; //线性查找表数组长度(从前面的代码可知，bytemax和bytemin不会相等)
			if(leg<1)return FALSE;
            LPBYTE cb= new BYTE[leg];
			memset(cb,0,sizeof(BYTE)*leg);
			for( i=0;i<leg;i++)
			{
				cb[i] = (BYTE)(i*k);
			}		
			//线性变化
			for(i=0;i<nsize;i++)
			{
				gtemp = (int)inputtemp[i];
				if(gtemp > bytemax) 
				{
					result[i] = 255;
					continue;
				}else if(gtemp < bytemin)
				{
					result[i] = 0;
					continue;
				}else
				{
					result[i] = cb[gtemp - bytemin];
				}
			}

			delete []cb;cb=NULL;
		}

		return TRUE;


	}



	//////////////////////////////////////////////////////////////////////////
	//读取整幅影像一个区域的数据,指定的段，并且将数据类型转换为float
	//添加：黄灵操
	//时间：2012-4-1
	bool ReadImageDataTofloat(int nXOff, int nYOff, int nXSize, int nYSize, int bufXSize, int bufYSize,int *bands, int _nbandcount, float **floatperband)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数ReadImageDataTofloat: 影像参数不正确");
			return FALSE;
		}

		//分配内存
		nbandcount = _nbandcount;	
		dataperband = new T*[nbandcount];	//nbandcount <= 3
		if(!dataperband) return FALSE;
		int i;
		for (i=0;i<nbandcount;i++)
		{
			dataperband[i] = new T[bufXSize*bufYSize];
			if(!dataperband[i]) return FALSE;
			memset(dataperband[i],0,sizeof(T)*bufXSize*bufYSize);
		}


		//对每个波段进行处理，在此之前，已经判定每个波段间的长宽要一致，数据类型一致
		GDALRasterBand *poBand;	
		//		int nXSize = nWidth;
		//		int nYSize = nHeight;
		//获取影像各波段的数据
		for ( i=0;i<nbandcount;i++)
		{
			if (bands[i]<=0) continue;
			poBand = poDataSet->GetRasterBand(bands[i]); //针对地bands[i]个波段

			poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
				nXOff, //相对于该波段影像最左边的偏移量
				nYOff, //相对于该波段影像最上边的偏移量
				nXSize,  //影像文件中的宽 
				nYSize,  //影像文件中的长
				dataperband[i], //指向保存数据的数组指针
				bufXSize, //读内存中影像的宽
				bufYSize, //读内存中影像的长
				poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
				0,  //像素间x方向的偏移量
				0 ); //像素间y方向偏移量
		}

		//将每个波段的数据存储为float类型
		for (i=0;i<nbandcount;i++)
		{
			//LinearStretchT(dataperband[i],byteperband[i],nXSize*nYSize);
			float *temp=floatperband[i];
			T* tempT=dataperband[i];
			for (int j=0;j<bufXSize*bufYSize;j++)
			{
				temp[j] = tempT[j];
			}
		}

		return TRUE;

	}
	
	//////////////////////////////////////////////////////////////////////////
	//进行线性拉伸，根据一个小区域内的灰度最大值和最小值进行拉伸
	//添加：黄灵操
	//时间：2011-4-17
	bool LinearStretchT(T* _data,BYTE *result,long nsize)
	{
		if (_data==NULL || result==NULL )
		{
			return FALSE;
		}
		
		//获取最大最小值
		T m_minv = _data[0];
		T m_maxv = _data[0];
		int i;
		for (i=1;i<nsize;i++)
		{
			if (_data[i]<m_minv)
			{
				m_minv = _data[i];
			}
			if (_data[i]>m_maxv)
			{
				m_maxv = _data[i];
			}
		}		
		
		//将最小值加1单位,最大值减1单位(一般背景的灰度值要么最小，要么最大，很多情况都是最小)
		double dmaxmin = m_maxv - m_minv;
		if (dmaxmin < 1)
		{
			m_minv += (float)0.01;
			m_maxv -= (float)0.01;
		}else if(dmaxmin<10)
		{
			m_minv += (float)0.1;
			m_maxv -= (float)0.1;
		}else 
		{
			m_minv += 1;
			m_maxv -= 1;
		}

		//按照修改后的最大最小值进行拉伸，这样的效果不一定好，可能灰度值分布在直方图的中间部分
		if (fabs((double)m_maxv - (double)m_minv)<0.0001)
		{
			//不拉伸，直接赋值
			for (int i=0;i<nsize;i++)
			{
				result[i] = (BYTE)_data[i];
			}
			return FALSE;
		}
		double k=255.0/(m_maxv - m_minv);
		BYTE gtemp=0;
	
		long graymean=0;
		long zhifangtu[256]={0};	//灰度直方图统计用

		for (i=0;i<nsize;i++)
		{
			if (_data[i]<m_minv)
			{
				gtemp = 0;
			}else if(_data[i]>m_maxv)
			{
				gtemp = 255;
			}else
			{
				gtemp = k*(_data[i] - m_minv);
			}
			zhifangtu[gtemp]++;//灰度直方图统计
			graymean += gtemp;
			result[i] = gtemp;
		}

		//对拉伸的结果进行直方图统计，然后按照直方图统计的结果，在进行一次线性拉伸
		double per=0.95; //保留的灰度比例
		graymean /= nsize;
		int left	= (int)graymean-1;
		int right	= (int)graymean+1;
		BYTE bytemax=0,bytemin=0;
		long pixelcout = 0;
		pixelcout += zhifangtu[graymean];
		for (i=0;i<256;i++)
		{
			if (pixelcout/((double)nsize) > per)
			{
				if (right>255) //这里要避免right出现256的影响
					bytemax = 255;
				else bytemax = right;
				if (left<0)	//这里要避免left出现-1的影响
					bytemin = 0;
				else bytemin = left;				
				break;
			}
			if (left>=0)
			{
				pixelcout += zhifangtu[left];
//				left--;		
			}
			if (right<256)
			{
				pixelcout += zhifangtu[right];
				right++;
			}
		}
		//根据新获得的最大最小值，在进行一次线性拉伸，使用查找表
		k=255.0/(bytemax - bytemin);
		//采用线性查找表
		DWORD leg = bytemax - bytemin +1; //线性查找表数组长度(从前面的代码可知，bytemax和bytemin不会相等)
		LPBYTE cb= new BYTE[leg];
		if(leg<1)return FALSE;
		memset(cb,0,sizeof(BYTE)*leg);
		for( i=0;i<leg;i++)
		{
			cb[i] = (BYTE)(i*k);
		}		
		//线性变化
		for(i=0;i<nsize;i++)
		{
			gtemp = result[i];
			if(gtemp > bytemax) 
			{
				result[i] = 255;
				continue;
			}else if(gtemp < bytemin)
			{
				result[i] = 0;
				continue;
			}else
			{
				result[i] = cb[gtemp - bytemin];
			}
		}
		
		delete []cb;cb=NULL;
/**/		
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//读取整幅影像一个区域的数据,指定的_nbandcount个波段，不进行线性拉伸
	//添加：黄灵操
	//时间：2012-3-27
	//备注：_dataperband是二维数组，在外面已经分配好内存
	bool ReadBlockData(int nXOff, int nYOff, int nXSize, int nYSize, int *bands, int _nbandcount, T** _dataperband)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数ReadBlockData: 影像参数不正确");
			return FALSE;
		}
		int i;

		nbandcount = _nbandcount;	
		//分配内存
		//
		//dataperband = new T*[nbandcount];	//nbandcount <= 3
		//if(!dataperband) return FALSE;
/*		
		//hlc  2012-6-7注释  读取数据的内存，改为在外面分配，否则，容易内存泄露
		int i;
		for (i=0;i<nbandcount;i++)
		{
			_dataperband[i] = new T[nXSize*nYSize];
			if(!_dataperband[i]) return FALSE;
			memset(_dataperband[i],0,sizeof(T)*nXSize*nYSize);
		}
*/

		//对每个波段进行处理，在此之前，已经判定每个波段间的长宽要一致，数据类型一致
		GDALRasterBand *poBand;	
		//		int nXSize = nWidth;
		//		int nYSize = nHeight;
		//获取影像各波段的数据
		for ( i=0;i<nbandcount;i++)
		{
			if (bands[i]<=0) continue;
			poBand = poDataSet->GetRasterBand(bands[i]); //针对地bands[i]个波段

			poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
				nXOff, //相对于该波段影像最左边的偏移量
				nYOff, //相对于该波段影像最上边的偏移量
				nXSize,  //影像文件中的宽 
				nYSize,  //影像文件中的长
				_dataperband[i], //指向保存数据的数组指针
				nXSize, //读内存中影像的宽
				nYSize, //读内存中影像的长
				poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
				0,  //像素间x方向的偏移量
				0 ); //像素间y方向偏移量
		}

		return TRUE;
	}
    
    //////////////////////////////////////////////////////////////////////////
    //读取整幅影像一个区域的数据,指定的_nbandcount个波段，不进行线性拉伸
    //添加：黄灵操
    //时间：2016-3-31
    //备注：_dataperband是二维数组，在外面已经分配好内存,可以读取金字塔上层的数据
    bool ReadBlockData_overview(int nXOff, int nYOff, int nXSize, int nYSize,int nbufferXsize,int nbufferYsize, int *bands, int _nbandcount, T** _dataperband)
    {
        if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1)
        {
            filesys.OutputLogMessage("位于CRSImageData中的函数ReadBlockData: 影像参数不正确");
            return FALSE;
        }
        int i;
        
        nbandcount = _nbandcount;
        //分配内存
        //
        //dataperband = new T*[nbandcount];	//nbandcount <= 3
        //if(!dataperband) return FALSE;
        /*
         //hlc  2012-6-7注释  读取数据的内存，改为在外面分配，否则，容易内存泄露
         int i;
         for (i=0;i<nbandcount;i++)
         {
         _dataperband[i] = new T[nXSize*nYSize];
         if(!_dataperband[i]) return FALSE;
         memset(_dataperband[i],0,sizeof(T)*nXSize*nYSize);
         }
         */
        
        //对每个波段进行处理，在此之前，已经判定每个波段间的长宽要一致，数据类型一致
        GDALRasterBand *poBand;
        //		int nXSize = nWidth;
        //		int nYSize = nHeight;
        //获取影像各波段的数据
        for ( i=0;i<nbandcount;i++)
        {
            if (bands[i]<=0) continue;
            poBand = poDataSet->GetRasterBand(bands[i]); //针对地bands[i]个波段
            
            poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
                             nXOff, //相对于该波段影像最左边的偏移量
                             nYOff, //相对于该波段影像最上边的偏移量
                             nXSize,  //影像文件中的宽 
                             nYSize,  //影像文件中的长
                             _dataperband[i], //指向保存数据的数组指针
                             nbufferXsize, //读内存中影像的宽
                             nbufferYsize, //读内存中影像的长
                             poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
                             0,  //像素间x方向的偏移量
                             0 ); //像素间y方向偏移量
        }
        
        return TRUE;
    }
	
	//////////////////////////////////////////////////////////////////////////
	//写入整幅影像一个区域的数据,针对所有波段进行保存
	//添加：黄灵操
	//时间：2012-3-27
	//备注：_dataperband是二维数组，在外面传入的影像块数据
	bool WriteBlockData(int nXOff, int nYOff, int nXSize, int nYSize,int *bands, int _nbandcount,GDALDataType datatype, T** _dataperband)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数WriteBlockData: 影像参数不正确");
			return FALSE;
		}
	
		GDALRasterBand *poBand;
		//保存各波段的数据
		for (int i=0;i<_nbandcount;i++ )
		{
			poBand = poDataSet->GetRasterBand(bands[i]);
			poBand->RasterIO( 
				GF_Write, 
				nXOff,
				nYOff,
				nXSize, 
				nYSize,		//保存的影像上下左右
				_dataperband[i], 
				nXSize, 
				nYSize,		//内存中影像的宽,长
				datatype,
				0,
				0);   
		}
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//计算一个影像指定波段根据灰度直方图比例的灰度最大最小值
	//添加：黄灵操
	//时间：2011-5-5
	bool GetHistMaxMinValue(double thr,double *dmax,double* dmin, int* bands, int _bandcount)
	{
		if (poDataSet==NULL || nWidth<1 || nHeight<1 || nbandcount<1) 
		{
			filesys.OutputLogMessage("位于CRSImageData中的函数ReadImageDataToByte: 影像参数不正确");
			return FALSE;
		}

		//分配内存
		nbandcount = _bandcount;		
		int i;

		//对每个波段进行处理，在此之前，已经判定每个波段间的长宽要一致，数据类型一致
		GDALRasterBand *poBand;	
		for ( i=0;i<nbandcount;i++)
		{
			if (bands[i]<=0) continue;
			poBand = poDataSet->GetRasterBand(bands[i]); //针对地bands[i]个波段
			long width = poBand->GetXSize();
			long height = poBand->GetYSize();
			double size = width*height;

			//T* dataperline = new T[width];	
			//memset(dataperline,0,sizeof(T)*width);

			//if(!dataperline) 
			//	return FALSE;

			int pbSuccess=0;
			int graymax = (int)poBand->GetMaximum(&pbSuccess);
			int graymin =(int)poBand->GetMinimum(&pbSuccess);
			//int a=2;
			//double maxmin[2];
			if(!pbSuccess)
			{

				//return FALSE;
				double maxmin[2];
				poBand->ComputeRasterMinMax(TRUE,maxmin);		//TRUE表示进行精确统计，比较慢
				graymin = maxmin[0];
				graymax = maxmin[1];
			}			
			
			int graynums = int(graymax-graymin+1);
			unsigned long long* pDensity = new unsigned long long[graynums];   // modify for gdal2.0 by hlc 2015.12.28
			memset(pDensity,0,sizeof(int)*graynums);

			//统计灰度直方图
			//for (int line = 0;line<height;line++)
			//{
			//	poBand->RasterIO( GF_Read, 	//读入内存（还可以从内存写入文件）
			//		0, //相对于该波段影像最左边的偏移量
			//		line, //相对于该波段影像最上边的偏移量
			//		width,  //影像文件中的宽 
			//		1,  //影像文件中的长
			//		dataperline, //指向保存数据的数组指针
			//		width, //读内存中影像的宽
			//		1, //读内存中影像的长
			//		poBand->GetRasterDataType(), //灰度级的类型(读入后保存的类型)
			//		0,  //像素间x方向的偏移量
			//		0 ); //像素间y方向偏移量

			//	

			//	for (int sample = 0;sample<width;sample++)
			//	{
			//		int nvalue = (int)dataperline[sample];
			//		nvalue -= graymin;
			//		pDensity[nvalue] += (double)1.0/size;
			//	}
			//	//delete []dataperline;
			//}
			int graymintemp = graymin;
			if(CE_None!=poBand->GetHistogram(graymin,graymax,graynums,pDensity,TRUE,FALSE,NULL,NULL))
			{
				return FALSE;
			}
			double sum = 0;
			double th = thr;
			for (int i = 0;i<graynums;i++)
			{
				sum += pDensity[i]/size;
				if(sum>th)
				{
					graymin = i+graymintemp;
					break;
				}
			}
			sum = 0;
			for (int i = graynums-1;i>0;i--)
			{
				sum += pDensity[i]/size;
				if(sum>th)
				{
					graymax = i+graymintemp;
					break;
				}
			}
			
			delete []pDensity;


			*dmax = graymax;
			dmax++;
			*dmin = graymin;
			dmin++;
		}

		return TRUE;
	}

};

#endif // !defined(AFX_RSIMAGEDATA_H__5778AFD5_E522_4130_8BF0_0537369D792D__INCLUDED_)
