
#ifndef __SKT_SVG_DEFINE_H__
#define __SKT_SVG_DEFINE_H__


//����SVG���ڿ��ܵĸ��ֲ���
enum SVG_TOOLS
{
	TOOLS_NONE,			//�޲���  
	TOOLS_SELECT,		//����ѡ��
	TOOLS_ZOOMIN,		//�����Ŵ�
	TOOLS_ZOOMOUT,	//������С
	TOOLS_PAN,			//����ƽ��
	TOOLS_RECT,			//��������
	TOOLS_TEXT,			//�����ı�
	TOOLS_ELLIPSE,	//������Բ
	TOOLS_LINE,			//����ֱ��
	TOOLS_POLYLINE,	//��������
	TOOLS_PATH,			//����·��
	TOOLS_PIC				//����ͼƬ
};

//����ؼ��Ŀ�֪ͨ�¼�
#define UM_SVG_OBJECT_LCLICKED		WM_USER+1024
#define UM_SVG_OBJECT_RCLICKED		WM_USER+1025
#define UM_SVG_OBJECT_DBLCLICKED	WM_USER+1026


#endif