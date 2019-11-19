#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "HuiDuBianHuanDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
HuiDuBianHuanDib::HuiDuBianHuanDib()
{
}
HuiDuBianHuanDib::~HuiDuBianHuanDib()
{
}

///***************************************************************/           
/*�������ƣ�FanSeBianHuan()                                        
/*�������ͣ�void                                           
/*���ܣ���ͼ����з�ɫ�任                            
/***************************************************************/
void HuiDuBianHuanDib::FanSeBianHuan()//��ͼ����з�ɫ�任
{
    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��
    int wide=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    for(int i=0;i<height*wide;i++)
    {
        unsigned char  temp;
        temp=*(p_data+i);
        *(p_data+i)=255- temp;
    }    
}

///***************************************************************/           
/*�������ƣ�Fei0()                                                */
/*�������ͣ�void                                                */
/*���ܣ���ͼ����з���ȡһ���㡣                                */
/****************************************************************/
void HuiDuBianHuanDib::Fei0()//��ͼ����з���ȡһ����
{
    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��
    int wide=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    if (!GetRGB())
        RgbToGray();

    for(int i=0;i<height*wide;i++)
    {
        if(*p_data!=0)          //������ֵ��Ϊ0
            *p_data=255;         //������Ϊ255
        p_data++;
    }
}

/***************************************************************/           
/*�������ƣ�GuDing(int YuZhi)                                  */      
/*�������ͣ�void                                               */    
/*����˵����YuZhi ������ֵ                                     */       
/*���ܣ���ͼ����й̶���ֵ���㡣                               */
/***************************************************************/
void HuiDuBianHuanDib::GuDing(int YuZhi)//��ͼ����й̶���ֵ����
{
    LPBYTE p_data=this->GetData(); //ԭͼ������ָ��
    int width=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    if (!GetRGB())
        RgbToGray();

    for(int i=0;i<height*width;i++)
    {
        if(*p_data>YuZhi)          //�Ҷ�ֵ���ڸ�����ֵ����Ϊ255
            *p_data=255;
        else
            *p_data=0;             //��������Ϊ0
        p_data++;
    }
}

/***************************************************************/           
/*�������ƣ�ShuangYu(int YuZhi1,int YuZhi2,int mode)          */
/*������int YuZhi1����ֵ1��int YuZhi2����ֵ2��int mode������ʽ*/
/*�������ͣ�void                                              */
/*���ܣ���ͼ��ʹ��˫�̶���ֵ�����ж�ֵ����                     */
/***************************************************************/
void HuiDuBianHuanDib::ShuangYu(int YuZhi1,int YuZhi2,int mode)
{
    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��
    int wide=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    if (!GetRGB())
        RgbToGray();

    if(mode==0)    //0-255-0��
    {
        for(int i=0;i<height*wide;i++)
        {
            //�������صĻҶ�ֵ����������ֵ֮�䣬�⽫����Ϊ255
            if(*p_data>=YuZhi1&&*p_data<=YuZhi2)
                *p_data=255;
            else
                *p_data=0;             //��������Ϊ0
            p_data++;
        }
    }
    if(mode==1)    //255-0-255��
    {
        for(int i=0;i<height*wide;i++)
        {
            //�������صĻҶ�ֵ����������ֵ֮�䣬�⽫����Ϊ255
            if(*p_data>=YuZhi1&&*p_data<=YuZhi2)
                *p_data=0;
            else
                *p_data=255;             //������255
            p_data++;
        }
    }             
}


///***************************************************************/           
/*�������ƣ�Chuangkoubianhuan(BYTE blow,BYTE bup)                                      
/*�������ͣ�void                                          
/*����˵����blow�����½� bup�����Ͻ�                                                               
/*���ܣ���ͼ����д��ڱ任��                    
/***************************************************************/  
void HuiDuBianHuanDib::Chuangkoubianhuan(BYTE bLow, BYTE bUp)
{
    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��
    int wide=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    for(int i=0;i<height*wide;i++)
    {
        if ((*p_data) < bLow)
        {
            *p_data = 0;            // ֱ�Ӹ�ֵΪ0
        }
        else if ((*p_data) > bUp)
        {
            *p_data = 255;          // ֱ�Ӹ�ֵΪ255
        }
        else
        {
            *p_data = *p_data; //     �任��ʽ�� g = 1.0 * (d - c) / (b - a) * (f - a) + c
        }

        p_data++;
    }
}

///***************************************************************/           
/*�������ƣ�Zhexianbianhuan(BYTE bX1,BYTE bY1,BYTE bX2,BYTE bY2)                                           */
/*�������ͣ�void                                              */
/*����˵����bX1�۵�һ��ԭʼ�Ҷȣ�bX2�۵����ԭʼ�Ҷ�
/*         bY1�۵�һ�ı任�Ҷȣ�bY2�۵���ı任�Ҷ�                                                       
/*���ܣ���ͼ����зֶ����Ա任��                      */
/***************************************************************/ 
void HuiDuBianHuanDib::Zhexianbianhuan(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{ 
    int i;
    // �Ҷ�ӳ���
    BYTE    bMap[256];
    // ����Ҷ�ӳ���
    for (i = 0; i <= bX1; i++)    //[0 ���� X1]
    {
        // �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
        if (bX1 > 0)
        {
            // ���Ա任
            bMap[i] = (BYTE) bY1 * i / bX1;
        }
        else
        {
            // ֱ�Ӹ�ֵΪ0
            bMap[i] = 0;
        }
    }
    for (; i <= bX2; i++)        //(X1 ���� X2]
    {
        // �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
        if (bX2 != bX1)
        {
            // ���Ա任
            bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
        }
        else
        {
            // ֱ�Ӹ�ֵΪbY1
            bMap[i] = bY1;
        }
    }
    for (; i < 256; i++)        //(X2 ���� 256)
    {
        // �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
        if (bX2 != 255)
        {
            // ���Ա任
            bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
        }
        else
        {
            // ֱ�Ӹ�ֵΪ255
            bMap[i] = 255;
        }
    }

    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��
    int wide=this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int height=this->GetHeight(); // ԭͼ�߶�

    for(int i=0;i<height*wide;i++)
    {
        *p_data++ = bMap[*p_data];
    }
}

///***************************************************************/           
/*�������ƣ�Fenbujunhenghua()                                           
/*�������ͣ�void                                          
/*����˵������                                                               
/*���ܣ���ͼ����лҶȷֲ����⻯����                   
/***************************************************************/  
void HuiDuBianHuanDib::Fenbujunhenghua()
{
    if (GetRGB()) {
        float fps[256] = {}; 
        float temp[256] = {};
        int gps[256] = {}; 

        // 1.ֱ��ͼ
        ZhiFangTu(fps);

        // 2.�ۻ�ֱ��ͼ
        for (int i = 0; i < 256; i++) {
            if (i == 0)
                temp[0] = fps[0];
            else
                temp[i] = fps[i] + temp[i - 1];
        
            // 3.f(x, y)ӳ��g(x, y)
            gps[i] = (int)(255.0f * temp[i] + 0.5f);
        }

        // 4.��ӳ���ĻҶ�д��DIB  
        int width=this->GetWidth();      // ��     
        int height=this->GetHeight(); // ��
        int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

        LPBYTE p_data=this->GetData (); // ����ָ��

        // 5.�Ը����ؽ��лҶ�ͳ��
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                LPBYTE lpSrc = p_data + lineBytes * y + x;
                *lpSrc = gps[*lpSrc];
            }
        }

    }
    else {
        float fps_r[256] = {},  fps_g[256] = {}, fps_b[256] = {};
        float temp_r[256] = {}, temp_g[256] = {}, temp_b[256] = {};
        int gps_r[256] = {},  gps_g[256] = {}, gps_b[256] = {};

        // 1.ֱ��ͼ
        ZhiFangTu(fps_r, fps_g, fps_b);

        // 2.�ۻ�ֱ��ͼ
        for (int i = 0; i < 256; i++) {
            if (i == 0) {
                temp_r[0] = fps_r[0];
                temp_g[0] = fps_g[0];
                temp_b[0] = fps_b[0];
            } else {
                temp_r[i] = fps_r[i] + temp_r[i - 1];
                temp_g[i] = fps_g[i] + temp_g[i - 1];
                temp_b[i] = fps_b[i] + temp_b[i - 1];
            }

            // 3.f(x, y)ӳ��g(x, y)
            gps_r[i] = (int)(255.0f * temp_r[i] + 0.5f);
            gps_g[i] = (int)(255.0f * temp_g[i] + 0.5f);
            gps_b[i] = (int)(255.0f * temp_b[i] + 0.5f);
        }

        // 4.��ӳ���ĻҶ�д��DIB  
        int width=this->GetWidth();      // ��     
        int height=this->GetHeight(); // ��
        int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

        LPBYTE p_data=this->GetData (); // ����ָ��

        // 5.�Ը����ؽ��лҶ�ͳ��
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                LPBYTE lpSrc_b = p_data + lineBytes * y + x * 3;
                LPBYTE lpSrc_g = p_data + lineBytes * y + x * 3 + 1;
                LPBYTE lpSrc_r = p_data + lineBytes * y + x * 3 + 2;
                *lpSrc_r = gps_r[*lpSrc_r];
                *lpSrc_g = gps_g[*lpSrc_g];
                *lpSrc_b = gps_b[*lpSrc_b];
            }
        }

    }
}

///***************************************************************/           
/*�������ƣ�Pipeibianhuan(BYTE bNum,int *npNu,float*fpPu)                                            */
/*�������ͣ�void                                              */
/*����˵����bNum�涨ֱ��ͼ�Ҷȼ� npNu ֱ��ͼӳ���ϵ
/*         fpPu�Ҷȷֲ�����                                                             
/*���ܣ���ͼ����лҶ�ƥ��任��                       */
/***************************************************************/  
void HuiDuBianHuanDib::Pipeibianhuan(BYTE bNum, int *npNu, float *fpPu)
{
    // ѭ������
    LONG i;
    LONG j;
    LONG wide;
    LONG height;
    // �Ҷȷֲ��ܶ�
    int midu[256],midu2[256],midu3[256];
    // �Ҷȷֲ�����
    float gailu[256],gailu2[256],gailu3[256];
    // �м���ʱ����
    float temp[256],temp2[256],temp3[256];
    // ָ��DIB����ָ��
    LPBYTE p_data;
    // �ҵ�DIBͼ��������ʼλ��
    p_data = this->GetData();
    wide=this->GetWidth ();    
    // DIB�ĸ߶�
    height = GetHeight();     
    if (m_pBitmapInfoHeader->biBitCount<9)        //�Ҷ�ͼ��
    {   
        // �ԻҶ��ܶȷֲ�����ͳ��    
        Midufenbu(midu);
        // �ԻҶȷֲ����ʽ���ͳ��
        ZhiFangTu(gailu);
        
        // ����ԭʼ�ۼ�ֱ��ͼ
        for (i = 0; i < 256; i++)
        {
            if (i == 0)
            {
                temp[0] = gailu[0];             
            }
            else
            {
                temp[i] = temp[i-1] + gailu[i];             
            }
            gailu[i] = temp[i];
        }
        // ����涨�ۼ�ֱ��ͼ
        for (i = 0; i < bNum; i++)
        {
            if (i == 0)
            {
                temp[0] = fpPu[0];
            }
            else
            {
                temp[i] = temp[i-1] + fpPu[i];
            }
            fpPu[i] = temp[i];
        }
        // ȷ��ӳ���Ӧ��ϵ
        for (i = 0; i < 256; i++)
        {
            // ��ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
            int m_r = 0;
            // ��С��ֵ
            float min_value_r = 1.0f;
            // �Թ涨ֱ��ͼ���ҶȽ���ö��
            for (j = 0; j < bNum; j++)
            {
                // ��ǰ��ֵ
                float now_value = 0.0f;
                //  �����ֵ
                if (gailu[i] - fpPu[j] >= 0.0f)
                    now_value = gailu[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu[i];
                // Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
                if (now_value < min_value_r)
                {
                    // ��ӽ��ĻҶȼ�
                    m_r = j;
                    // �ݴ���С��ֵ
                    min_value_r = now_value;
                }
            }
            // �����Ҷ�ӳ���
            midu[i] = npNu[m_r];
        }
        // �Ը����ؽ��д���
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                // �����ؽ��е�ӳ������ֱ��ͼ�涨��ӳ�䴦��
                unsigned char temp = *((unsigned char *)p_data + wide * j + i);
                *((unsigned char *)p_data + wide * j + i) = midu[temp];
            }
        }
    }
    else        //24λ��ɫ
    {
        // �ԻҶ��ܶȷֲ�����ͳ��    
        Midufenbu(midu,midu2,midu3);
        // �ԻҶȷֲ����ʽ���ͳ��
        ZhiFangTu(gailu,gailu2,gailu3);
        
        // ����ԭʼ�ۼ�ֱ��ͼ
        for (i = 0; i < 256; i++)
        {
            if (i == 0)
            {
                temp[0] = gailu[0];    
                temp2[0] = gailu2[0];
                temp3[0] = gailu3[0];
            }
            else
            {
                temp[i] = temp[i-1] + gailu[i];
                temp2[i] = temp2[i-1] + gailu2[i];    
                temp3[i] = temp3[i-1] + gailu3[i];    
            }
            gailu[i] = temp[i];
            gailu2[i] = temp2[i];
            gailu3[i] = temp3[i];
            
        }
        // ����涨�ۼ�ֱ��ͼ
        for (i = 0; i < bNum; i++)
        {
            if (i == 0)
            {
                temp[0] = fpPu[0];            
            }
            else
            {
                temp[i] = temp[i-1] + fpPu[i];
            }
            fpPu[i] = temp[i];
        }
        // ȷ��ӳ���Ӧ��ϵ
        for (i = 0; i < 256; i++)
        {
            // ��ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
            int m_r = 0;
            int m_g = 0;
            int m_b = 0;                 
            // ��С��ֵ
            float min_value_r = 1.0f;
            float min_value_g = 1.0f;
            float min_value_b = 1.0f;
            // �Թ涨ֱ��ͼ���ҶȽ���ö��
            for (j = 0; j < bNum; j++)
            {
                // ��ǰ��ֵ
                float now_value = 0.0f;
                //  �����ֵ
                if (gailu[i] - fpPu[j] >= 0.0f)
                    now_value = gailu[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu[i];
                // Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
                if (now_value < min_value_r)
                {
                    // ��ӽ��ĻҶȼ�
                    m_r = j;
                    // �ݴ���С��ֵ
                    min_value_r = now_value;
                }
                //  �����ֵ
                if (gailu2[i] - fpPu[j] >= 0.0f)
                    now_value = gailu2[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu2[i];
                // Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
                if (now_value < min_value_g)
                {
                    // ��ӽ��ĻҶȼ�
                    m_g = j;
                    // �ݴ���С��ֵ
                    min_value_g = now_value;
                }
                //  �����ֵ
                if (gailu3[i] - fpPu[j] >= 0.0f)
                    now_value = gailu3[i] - fpPu[j];
                else
                    now_value = fpPu[j] - gailu3[i];
                // Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
                if (now_value < min_value_b)
                {
                    // ��ӽ��ĻҶȼ�
                    m_b = j;
                    // �ݴ���С��ֵ
                    min_value_b = now_value;
                }
            }
            // �����Ҷ�ӳ���
            midu[i] = npNu[m_r];
            midu2[i] = npNu[m_g];
            midu3[i] = npNu[m_b];
        }
        // �Ը����ؽ��д���
        for (j = 0; j < height; j ++)
        {
            for (i = 0; i < wide; i ++)
            {
                // �����ؽ��е�ӳ������ֱ��ͼ�涨��ӳ�䴦��
                unsigned char temp = *((unsigned char *)p_data + wide * j*3 + i*3);
                *((unsigned char *)p_data + wide * j*3 + i*3) = midu[temp];
                unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
                *((unsigned char *)p_data + wide * j*3 + i*3+1) = midu[temp2];
                unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
                *((unsigned char *)p_data + wide * j*3 + i*3+2) = midu[temp3];
            }
        }
    }
}

///***************************************************************/           
/*�������ƣ�Midufenbu()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ������ܶȷֲ��任��            
/***************************************************************/  
void HuiDuBianHuanDib::Midufenbu(int *nNs_R)
{
    // ѭ������
    LONG i;
    LONG j;
    
    LONG wide;
    LONG height;
    //������ʼ��
    memset(nNs_R,0,sizeof(int) * 256);
    // ָ��DIB����ָ��
    LPBYTE p_data;
    // �ҵ�DIBͼ��������ʼλ��
    p_data = this->GetData();        
    wide=this->GetWidth ();
    // DIB�ĸ߶�
    height = GetHeight();
    // �Ը����ؽ��лҶ�ת��
    for (j = 0; j < height; j ++)
    {
        for (i = 0; i <wide; i ++)
        {
            // �Ը����ؽ��лҶ�ͳ��
            unsigned char temp = *((unsigned char *)p_data + wide * j + i);
            nNs_R[temp]++;        
        }
    } 
}

void HuiDuBianHuanDib::Midufenbu(int *nNs_R, int *nNs_G, int *nNs_B)
{
    // ѭ������
    LONG i;
    LONG j;
    LONG wide;
    LONG height;
    //������ʼ��
    memset(nNs_R,0,sizeof(int) * 256);
    memset(nNs_G,0,sizeof(int) * 256);
    memset(nNs_B,0,sizeof(int) * 256);
    // ָ��DIB����ָ��
    LPBYTE p_data;
    // �ҵ�DIBͼ��������ʼλ��
    p_data = this->GetData();        
    wide=this->GetWidth ();
    // DIB�ĸ߶�
    height = GetHeight();
    // �Ը����ؽ��лҶ�ת��
    for (j= 0;j < height;j ++)
    {
        for (i = 0; i < wide; i ++)
        {
            // �Ը����ؽ��лҶ�ͳ��
            unsigned char temp = *((unsigned char *)p_data + wide * j*3 + i*3);
            unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
            unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
            nNs_R[temp]++;        
            nNs_G[temp2]++;
            nNs_B[temp3]++;
        }
    } 
}

///***************************************************************/           
/*�������ƣ�Shuipingtouying()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ˮƽͶӰ��            
/***************************************************************/  
void HuiDuBianHuanDib::Shuipingtouying()
{
    //��ͼ����ж�ֵ��
    GuDing(128);

    int width=this->GetWidth(); // ԭͼ���
    int height=this->GetHeight(); // ԭͼ�߶�
    int lineBytes = GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��

    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��

    LONG lBlackNum;
    LPBYTE temp = new BYTE[lineBytes * height];
    memset(temp, 255, lineBytes * height);

    for(int y = 0; y < height; y++) {
        lBlackNum = 0;

        // ͳ��ÿ�к����ظ���
        for (int x = 0; x < lineBytes; x++) {
            // �ų��� 4 �ֽڶ�������Ķ�������
            bool beyond = GetRGB() ? x >= width : x >= width * 3;
            if (beyond) 
                continue;

            if(p_data[lineBytes * y + x] == 0)
                lBlackNum++;
        }

        // ����ÿ��
        for (int x = 0; x < lBlackNum; x++) {
            temp[lineBytes * y + x] = 0;
        }
    }

    memcpy(p_data, temp, lineBytes * height);
    delete []temp;    
}

///***************************************************************/           
/*�������ƣ�Chuizhitouying()                                      
/*�������ͣ�void                                      
/*���ܣ���ͼ����д�ֱͶӰ��            
/***************************************************************/  
void HuiDuBianHuanDib::Chuizhitouying()
{
    //��ͼ����ж�ֵ��
    GuDing(128);

    int width=this->GetWidth(); // ԭͼ���
    int height=this->GetHeight(); // ԭͼ�߶�
    int lineBytes = GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��

    LPBYTE p_data=this->GetData (); //ԭͼ������ָ��

    LONG lBlackNum;
    LPBYTE temp = new BYTE[lineBytes * height];
    memset(temp, 255, lineBytes * height);

    for(int x = 0; x < lineBytes; x++) {
        lBlackNum = 0;

        // ͳ��ÿ�к����ظ���
        for (int y = 0; y < height; y++) {
            // �ų��� 4 �ֽڶ�������Ķ�������
            bool beyond = GetRGB() ? x >= width : x >= width * 3;
            if (beyond) 
                continue;

            if(p_data[lineBytes * y + x] == 0)
                lBlackNum++;
        }

        // ����ÿ��
        for (int y = 0; y < lBlackNum; y++) {
            temp[lineBytes * y + x] = 0;
        }
    }

    memcpy(p_data, temp, lineBytes * height);
    delete []temp;

    //LPBYTE  p_data, p_datatemp;     //ԭͼ������ָ��
    //int wide,height;                //ԭͼ������
    //int i,j;                        //ѭ������
    //long int lBlackNum;                //ͼ����ÿ���ںڵ�ĸ���
    //p_datatemp=this->GetData();
    //p_data=p_datatemp;
    //if(m_pBitmapInfoHeader->biBitCount<9)    
    //    wide=this->GetWidth ();
    //else
    //    wide=this->GetDibWidthBytes();
    //height=this->GetHeight ();
    ////��ͼ����ж�ֵ��
    //for(j=0;j<height;j++)
    //{
    //    for(i=0;i<wide;i++)
    //    { 
    //        if (*p_data>128)
    //            *p_data=255;
    //        else 
    //            *p_data=0;
    //        p_data++;
    //    }
    //}
    //p_data=p_datatemp;
    //LPBYTE  temp=new BYTE[wide*height];        //��ͼ�񻺳���
    //memset( temp,255,wide*height);            //�趨��ͼ���ֵΪ255
    //for(i=0;i<wide;i++)
    //{
    //    lBlackNum=0;
    //    for(j=0;j<height;j++)
    //    {
    //        
    //        if(p_data[wide*j+i]==0)
    //            lBlackNum++;
    //    }
    //    for(j=0;j<lBlackNum;j++)
    //        temp[wide*j+i]=0;
    //}
    //memcpy(p_data, temp,wide*height);
    //delete  temp;        
}

///**************************************************************/           
/*�������ƣ�ZhiFangTu(float *tongji)                               
/*�������ͣ�void                                         
/*����˵����tongji �Ҷȷֲ��ܶ�ͳ��                                                        
/*���ܣ���ͼ����лҶ�ֱ��ͼͳ�ơ�                     
/***************************************************************/
void HuiDuBianHuanDib::ZhiFangTu(float *tongji)
{
    int huidu[256] = {};  // �Ҷȼ���   
    int width=this->GetWidth();      // ��     
    int height=this->GetHeight(); // ��
    int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

    LPBYTE p_data=this->GetData(); // ����ָ��

    // �Ը����ؽ��лҶ�ͳ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE fxy = *(p_data + lineBytes * y + x);
            huidu[fxy]++; // �Ҷ�ͳ�Ƽ���
        }
    }

    // ����Ҷȷֲ��ܶ�
    for(int i = 0; i < 256; i++) {
        tongji[i] = huidu[i] / (height * width * 1.0f);
    }
}

///*************************************************************/           
/*�������ƣ�ZhiFangTu(float *tongji_r, float *tongji_g, float *tongji_b)                               
/*�������ͣ�void                                         
/*����˵����tongji_r R�Ҷȷֲ��ܶ�ͳ��
           tongji_g G�Ҷȷֲ��ܶ�ͳ��
           tongji_b B�Ҷȷֲ��ܶ�ͳ��
/*���ܣ���24λ��ɫͼ����лҶȸ���ͳ�ơ�                     
/***************************************************************/
void HuiDuBianHuanDib::ZhiFangTu(float *tongji_r, float *tongji_g, float *tongji_b)
{ 
    ASSERT(GetRGB() == NULL);

    int width=this->GetWidth();      // ��     
    int height=this->GetHeight(); // ��
    int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

    LPBYTE p_data=this->GetData(); // ����ָ��

    // �Ҷ�ͳ��
    int huidu_r[256], huidu_g[256], huidu_b[256];
    memset(huidu_r, 0, sizeof(huidu_r));
    memset(huidu_g, 0, sizeof(huidu_g));
    memset(huidu_b, 0, sizeof(huidu_b));

    // ��r��g��b��ͨ���ֱ���лҶ�ͳ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            BYTE temp_b = p_data[lineBytes*y + x*3] ;
            BYTE temp_g = p_data[lineBytes*y + x*3 + 1] ;
            BYTE temp_r = p_data[lineBytes*y + x*3 + 2] ;

            huidu_r[temp_r]++;
            huidu_g[temp_g]++;
            huidu_b[temp_b]++;
        }
    }

    // ����Ҷȷֲ��ܶ�
    for(int i = 0; i < 256; i++) {
        if (tongji_r)
            tongji_r[i] = huidu_r[i] / (height * width * 1.0f);

        if (tongji_g)
            tongji_g[i] = huidu_g[i] / (height * width * 1.0f);
        
        if (tongji_b)
            tongji_b[i] = huidu_b[i] / (height * width * 1.0f);
    }
}

// ��ʾͨ��ֱ��ͼ��ͼ��
void HuiDuBianHuanDib::ShowChannelZhiFangTu(float *tongji_r, float *tongji_g, float *tongji_b)
{
    ZhiFangTu(tongji_r, tongji_g, tongji_b);

    int width=this->GetWidth();      // ��     
    int height=this->GetHeight(); // ��
    int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��
    LPBYTE p_data=this->GetData(); // ����ָ��

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LPBYTE b = p_data + lineBytes * y + x * 3;
            LPBYTE g = p_data + lineBytes * y + x * 3 + 1;
            LPBYTE r = p_data + lineBytes * y + x * 3 + 2;

            if (!tongji_r)
                *r = 0;
            if (!tongji_g)
                *g = 0;
            if (!tongji_b)
                *b = 0;
        }
    }
}

void HuiDuBianHuanDib::RedZhiFangTu(float *tongji)
{
    ShowChannelZhiFangTu(tongji, NULL, NULL);
}

void HuiDuBianHuanDib::GreenZhiFangTu(float *tongji)
{
    ShowChannelZhiFangTu(NULL, tongji, NULL);
}

void HuiDuBianHuanDib::BlueZhiFangTu(float *tongji)
{
    ShowChannelZhiFangTu(NULL, NULL, tongji);
}

// ��ɫͼת�Ҷ�ͼ
void HuiDuBianHuanDib::RgbToGray()
{
    ASSERT(GetRGB() == NULL);

    int width=this->GetWidth();      // ��     
    int height=this->GetHeight(); // ��
    int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

    LPBYTE p_data=this->GetData(); // ����ָ��

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LPBYTE pb = p_data + lineBytes*y + x*3;
            LPBYTE pg = p_data + lineBytes*y + x*3 + 1;
            LPBYTE pr = p_data + lineBytes*y + x*3 + 2;

            // rgb��Ȩƽ��
            BYTE gray = 0.11 * (*pr) + 0.59 * (*pg) + 0.3 * (*pb); // g = 0.11 * r + 0.59 * g + 0.3b;
            *pb = gray;
            *pg = gray;
            *pr = gray;
        }
    }
}

void HuiDuBianHuanDib::RgbToGrayFast()
{
    ASSERT(GetRGB() == NULL);

    int width=this->GetWidth();      // ��     
    int height=this->GetHeight(); // ��
    int lineBytes=this->GetDibWidthBytes(); // 4 �ֽڶ����Ŀ��

    LPBYTE p_data=this->GetData(); // ����ָ��

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            LPBYTE pb = p_data + lineBytes*y + x*3;
            LPBYTE pg = p_data + lineBytes*y + x*3 + 1;
            LPBYTE pr = p_data + lineBytes*y + x*3 + 2;

            int gray = max(*pb, *pg, *pr);   // rgb���ֵ������Ҫʵ������
            *pb = gray;
            *pg = gray;
            *pr = gray;
        }
    }
}