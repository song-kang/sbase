/**
 *
 * �� �� �� : EscPrint.cpp
 * �������� : 2012-10-17 13:12
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ESC(EPSON Standard Code for Printer)�����ӡ����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 **/

#include "SEscPrint.h"

#define ESC 0x1B 
#define FF 0x0C  
// static unsigned char LQPrintData[][10]={  
// 	{ESC,'@'},{2},/*��ӡ����ʼ��*/  
// 	{ESC,'*'},{2},/*ѡ��ͼ��ģʽ m n1 n2*/ 
// 	{ESC,'3'},{2},/*�趨n/180Ӣ���м��*/ 
// 	{ESC,'2'},{2},/*�趨1/6Ӣ���м��*/ 
// 		};  

SEscPrint::SEscPrint()
{

}

SEscPrint::~SEscPrint()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �򿪴�ӡ��
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 13:23
// ����˵��:  @sAttrΪ��ӡ������
// �� �� ֵ:  true�򿪳ɹ���false��ʧ��
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::Open(SString sAttr)
{
	BYTE buf[]={ESC,'@'};//��ӡ����ʼ��
	if(Write(buf,sizeof(buf)) != sizeof(buf))
	{
		LOGERROR("��ӡ����ʼ��ָ���ʧ��!");
		Close();
		return false;
	}
	SwitchTextMode();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ѡ���ı���ӡģʽ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 15:34
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::SwitchTextMode()
{
	BYTE buf[]={ESC,'3',0x20};//���0x20
	if(Write(buf,sizeof(buf)) != sizeof(buf))//�о�
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ѡ��ͼ�δ�ӡģʽ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 15:35
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::SwitchGraphicMode()
{
	BYTE buf[]={ESC,'3',0x18};//���0x18
	if(Write(buf,sizeof(buf)) != sizeof(buf))//�о�
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ�ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 15:41
// ����˵��:  @strΪ�ı����ݣ�\nΪ���У�����ʱ����Ҫ\r��
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintText(SString str)
{
	if(Write((BYTE*)str.data(),str.length()) != str.length())//�о�
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡָ����BMP�ļ������ļ�����Ϊ1λ��ɫ�ĺڰ�ͼƬ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 18:13
// ����˵��:  @sBmpFileΪ�ļ���ȫ·��
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintBmpFile(SString sBmpFile)
{
	SFile file(sBmpFile);
	if(!file.open(IO_ReadOnly))
	{
		LOGWARN("��BMP�ļ�(%s)ʧ��!",sBmpFile.data());
		return false;
	}
	stuBitmapFileHeader fileHeader;
	stuBitmapInfo info;
	int filesize = file.size();
	file.seekBegin();
	int ret,w,h,datasize;
	ret = file.readBlock((BYTE*)&fileHeader,sizeof(fileHeader));
	SKT_SWAP_WORD(fileHeader.bfType);
	SKT_SWAP_WORD(fileHeader.bfSize);
	SKT_SWAP_WORD(fileHeader.bfOffBits);
	if(ret != (int)sizeof(fileHeader) || fileHeader.bfType != 0x4d42 || (int)fileHeader.bfSize != filesize )//BM
	{
		LOGWARN("BMP�ļ�(%s)��ʽ��Ч!",sBmpFile.data());
		file.close();
		return false;
	}
	ret = file.readBlock((BYTE*)&info,sizeof(info));
	if(ret != sizeof(info))
	{
		LOGWARN("BMP�ļ�(%s)��ʽ��Ч!",sBmpFile.data());
		file.close();
		return false;
	}
	SKT_SWAP_WORD(info.bmiHeader.biBitCount);
	SKT_SWAP_DWORD(info.bmiHeader.biWidth);
	SKT_SWAP_DWORD(info.bmiHeader.biHeight);
	SKT_SWAP_DWORD(info.bmiHeader.biSize);
	if(info.bmiHeader.biBitCount != 1)
	{
		LOGWARN("BMP�ļ�(%s)���ǵ�ɫͼƬ,���ܴ�ӡ!",sBmpFile.data());
		file.close();
		return false;
	}
	w = info.bmiHeader.biWidth;
	h = info.bmiHeader.biHeight;
	datasize = w*h/8;
	if((w*h)%8 != 0)
		datasize ++;
	if(datasize == 0)
	{
		LOGWARN("BMP�ļ�(%s)��СΪ0!",sBmpFile.data());
		file.close();
		return false;
	}
	BYTE *pBuffer = new BYTE[datasize];
	file.seekTo(fileHeader.bfOffBits);//������������ʼλ��
	ret = file.readBlock(pBuffer,datasize);
	if(ret != datasize)
	{
		delete[] pBuffer;
		LOGWARN("BMP�ļ�(%s)��ʽ��Ч!",sBmpFile.data());
		file.close();
		return false;
	}
	if(!PrintMonochromeBmpMem(w,h,pBuffer,datasize))
	{
		delete[] pBuffer;
		LOGWARN("BMP�ļ�(%s)��ӡʧ��!",sBmpFile.data());
		file.close();
		return false;
	}
	
	delete[] pBuffer;
	file.close();
	LOGDEBUG("BMP�ļ�(%s)��ӡ���!",sBmpFile.data());
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡָ����С���������ĵ�ɫͼƬ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 18:32
// ����˵��:  @wΪͼƬ���
//            @hΪͼƬ�߶�
//            @pBufferΪͼƬ���ݻ�����ָ��
//            @iBytesΪͼƬ���ݻ������ֽ���
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintMonochromeBmpMem(int w,int h,BYTE *pBuffer,int iBytes)
{
	/*
	�����ڲ�ɫ��֮�����ͼ�������ֽ����С�ͼ���ÿһɨ�����ɱ�ʾͼ�����ص��������ֽ���ɣ�
	ÿһ�е��ֽ���ȡ����ͼ�����ɫ��Ŀ�������ر�ʾ��ͼ���ȡ�ɨ�������ɵ����ϴ洢�ģ�
	�����˵�������еĵ�һ���ֽڱ�ʾλͼ���½ǵ����أ������һ���ֽڱ�ʾλͼ���Ͻǵ����ء�
	*/
	//һ�δ�ӡһ�У�ÿ�е�ÿһ�о���һ��24λ(24���ӡ��)��3�ֽ���ɣ�ÿ�м�w*3���ֽ�
	//h�����ǳ���8����4��������
// 	if(h%8 != 0 || (h/8) % 4 != 0)
// 		h = (((h/8)/4)+1)*4*8;

	int rowsize = 6+w*3;
	BYTE *pRowData = new BYTE[rowsize];
	pRowData[0] = 0x1b;
	pRowData[1] = 0x2a;
	pRowData[2] = 0x21;
	pRowData[3] = (BYTE)(w&0xff);
	pRowData[4] = (BYTE)(w>>8);
	pRowData[rowsize-1] = 0x0A;//���з�
	int row,rows = h/24;
	int col;
	int p,ps;
	int w_d8=w/8;
	if(h%24 != 0)
		rows++;
	BYTE *pPixel;
	SwitchGraphicMode();
	PrintLF();
	for(row=0;row<rows;row++)//24����ɵ���
	{
		memset(pRowData+5,0,rowsize-6);//����л���
		ps = 24;//һ����24���������
		if(row == rows-1)
		{
			//���һ�п��Բ���24������
			ps = (h-row*24);
		}
		BYTE *pRow = &pBuffer[(w*h-w*row*24-w)/8];//24���е����һ�������ж�Ӧ����ʼ��ַ,��Ӧ��ǰ�е�һ��������
		BYTE *pMatrix=new BYTE[24*w];
		memset(pMatrix,0,w*24);
		for(p=0;p<ps;p++)//һ�δ�ӡ�������24���������
		{
			for(col=0;col<w;col++)//ÿһ��
			{
				pPixel = pRowData+5+col*3+(p>>3);//��ʼλ��
			
				//LOGDEBUG("p=%d,col=%d,pixel=%d, v=%d",p,col,(pPixel-pRowData)-5,((pRow[col>>3] >> (7-col&7)) & 1));
				if((int)(((pRow[col>>3] >> (7-(col&7))) & 1) << (7-(col&7))) == 0)
				{
					pMatrix[p*w+col] = 1;
					*pPixel |= 1 << (7-(p&7));//(((pRow[col>>3] >> (7-col&7)) & 1) << (7-col&7));
				}
			}
			pRow -= w_d8;//����һ��
		}
		//printf("\n");
		/*
		for(p=0;p<24;p++)
		{
			printf("%02d:    ",p);
			for(col=0;col<w;col++)
			{
				if(pMatrix[p*w+col] != 0)
					printf("��");
				else
					printf("��");
			}
			printf("\n");
		}
		*/
		//printf("\n");
			
	//	LOGDEBUG("WRITE(len:%d):%s",rowsize,SString::HexToStr(pRowData,rowsize).data());

//		PrintText("��ӡ��");
		Write(pRowData,rowsize);
		delete[] pMatrix;
	}

	delete[] pRowData;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡһ��ͼ����
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-18 20:44
// ����˵��:  @wΪ����
//            @pBufferΪͼ�λ�����ָ�룬ÿ��24��(��)��ɣ���3���ֽڱ�ʾһ����
//            @iBytesΪ�������ֽ���������Ϊw��3��
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintGraphicLine(int w,BYTE *pBuffer,int iBytes)
{
	BYTE buf[]={0x1b,0x2a,0x21,0,0};
	buf[3] = (BYTE)(w&0xff);
	buf[4] = (BYTE)(w>>8);
	if(iBytes != w*3)
	{
		LOGWARN("��������С��������ƥ��!");
		return false;
	}
	if(Write(buf,sizeof(buf)) != sizeof(buf))
	{
		LOGWARN("д��ӡ����ʧ��!");
		return false;
	}
	Write(pBuffer,iBytes);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ���з�
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 16:37
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintLF()
{
	Write((BYTE*)"\n",1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ�س���
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 16:37
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::PrintCR()
{
	Write((BYTE*)"\r",1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ����ֽ����ҳ��,����ӡ�������е�����ȫ����ӡ���������ر�׼ģʽ��
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 16:43
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::MoveToNextPage()
{
	BYTE buf[]={0x0c};
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ����ֽ����ҳ��,����ӡ�������е�����ȫ����ӡ���������ر�׼ģʽ��
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 16:43
// ����˵��:  @pointsΪ��ֱ������һ����ֱ���Ϊ0.33mm
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::MovePaperPoints(BYTE points)
{
	BYTE buf[]={ESC,'J',0};
	buf[2] = (BYTE)points;
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	return true;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӡ����ֽlines ��,��ӡ��ӡ�������е����ݲ���ֽlines�ַ���
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-17 16:43
// ����˵��:  @linesΪ��ֽ������
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::MovePaperLines(BYTE lines)
{
	BYTE buf[]={ESC,'d',0};
	buf[2] = (BYTE)lines;
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����Ƿ�ȱֽ
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-24 9:34
// ����˵��:  void
// �� �� ֵ:  true��ʾ��ȱֽ��false��ʾȱֽ
//////////////////////////////////////////////////////////////////////////
bool SEscPrint::CheckPaper()
{
	BYTE buf[]={16,4,1};
	if(Write(buf,sizeof(buf)) != sizeof(buf))
		return false;
	Read(buf,1);
	return true;

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2012-10-24 10:14
// ����˵��:  @iTimes��ʾ��������
//            @iSlp��ʾ���η�����ļ��ʱ�䣬��λ����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SEscPrint::Beep(int iTimes/*=1*/,int iSlp/*=0*/)
{
	BYTE buf[]={7};
	for(int i=0;i<iTimes;i++)
	{
		if(Write(buf,sizeof(buf)) != sizeof(buf))
			return;
		if(i<iTimes-1)
			SApi::UsSleep(iSlp*1000);
	}
}
