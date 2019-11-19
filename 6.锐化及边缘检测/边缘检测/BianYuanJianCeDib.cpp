#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BianYuanJianCeDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 int j;
BianYuanJianCeDib::BianYuanJianCeDib()
{
  
}
BianYuanJianCeDib::~BianYuanJianCeDib()
{
}

/***************************************************************/
/*�������ƣ�Templat(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)                                         */
/*�������ͣ�void                                               */
/*������LPBYTE p_data:ָ��ԭDIBͼ��ָ��                        */
/*      int lineBytes:ԭͼ��4�ֽڶ�����                      */
/*      int height:ԭͼ��߶�                                  */
/*      int tempH:ģ��߶�                                     */
/*      int tempW:ģ����                                     */
/*      int tempMX:ģ�������Ԫ��X����(<tempW-1)               */
/*      int tempMY:ģ�������Ԫ��Y����(<tempH-1)               */
/*      float *fpArray:ָ��ģ�������ָ��                      */
/*      float fCoef:ģ��ϵ��                                   */
/*���ܣ���ָ��ģ��ԻҶ�ͼ����в�����                            */
/***************************************************************/
void BianYuanJianCeDib::Template(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = tempMY; y < height- (tempH - tempMY) + 1; y++) {
        for (int x = border * tempMX; x < lineBytes - border * (tempW - tempMX) + 1; x++) {
    //for (int y = tempMY; y < height - tempMY; y++) {
    //    for (int x = border * tempMX; x < lineBytes - border * tempMX; x++) {
            // ÿ��ͨ���������
            float fResult = 0.0f;
            for(int t = 0; t < tempH; t++)
                for(int s = 0; s < tempW; s++)
                    fResult += p_data[lineBytes * (y - tempMY + t) + (x - border * tempMX + s * border)] * fpArray[tempW * t + s];
                    
            //for(int t = -tempMY; t <= tempMY; t++)
            //    for(int s = -tempMX; s <= tempMX; s++)
            //        fResult += p_data[lineBytes * (y + t) + (x + s * border)] * fpArray[tempW * (tempMY + t) + (tempMX + s)];
            // ����ϵ��
            fResult *= fCoef;
            // ȡ����ֵ
            fResult = (float)fabs(fResult);
            // ��ֵ
            if(fResult > 255)
                p_temp[lineBytes * y + x] = 255;
            else
                p_temp[lineBytes * y + x] = (int)(fResult + 0.5);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

void BianYuanJianCeDib::Templat(LPBYTE p_data, int lineBytes, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{  
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    for (int y = tempMY; y < height- tempH + tempMY + 1; y++) {
        for (int x = tempMX; x < lineBytes - tempW + tempMX + 1; x++) {
            // ���
            float fResult = 0.0f;
            for(int t = 0; t < tempH; t++)
                for(int s = 0; s < tempW; s++)
                    fResult += p_data[lineBytes * (y -tempMY + t) + (x - tempMX + s)] * fpArray[tempW * t + s];
            // ����ϵ��
            fResult *= fCoef;
            // ȡ����ֵ
            fResult = (float)fabs(fResult);
            // ��ֵ
            if(fResult > 255)
                p_temp[lineBytes * y + x] = 255;
            else
                p_temp[lineBytes * y + x] = (int)(fResult + 0.5);
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
    return;
}

/***************************************************************/
/*�������ƣ�Templat24bit(BYTE *m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)                                         */
/*�������ͣ�void                                               */
/*������BYTE* m_pdata:ָ��ԭDIBͼ��ָ��                        */
/*      int DibWidth:ԭͼ����                                */
/*      int height:ԭͼ��߶�                                  */
/*      int tempH:ģ��߶�                                     */
/*      int tempW:ģ����                                     */
/*      int tempMX:ģ�������Ԫ��X����(<tempW-1)               */
/*      int tempMY:ģ�������Ԫ��Y����(<tempH-1)               */
/*      float *fpArray:ָ��ģ�������ָ��                      */
/*      float fCoef��ģ��ϵ��                                  */
/*���ܣ���ָ��ģ���24λ��ɫͼ����в�����                     */
/***************************************************************/
void BianYuanJianCeDib::Templat24bit(BYTE *m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float *fpArray, float fCoef)
{
    int i,j,k,l;  //ѭ������
    BYTE*  p_temp=new BYTE[DibWidth*height];    //��ͼ�񻺳���
    //��ʼ����ͼ��Ϊԭʼͼ��
    memcpy( p_temp,m_pdata,DibWidth*height);
    float fResult;    //����ֵ������
    for(j=tempMY;j<height-tempH+tempMY+1;j++)
        for(i=3*tempMX;i<DibWidth-3*tempW+3*tempMX+1;i++)
        {
            //��������ֵ
            fResult=0;
            for(k=0;k<tempH;k++)
                for(l=0;l<tempW;l++)
                    fResult=fResult+m_pdata[(j-tempMY+k)*DibWidth+(i-3*tempMX+l*3)]*fpArray[k*tempW+l];
            //����ϵ��
            fResult*=fCoef;
            //ȡ����ֵ
            fResult=(float)fabs(fResult);
            //�ж��Ƿ񳬹�255
            if(fResult>255)
                //������255��ֱ�Ӹ�ֵΪ255
                p_temp[j*DibWidth+i]=255;
            else
                //δ����255����ֵΪ������
                p_temp[j*DibWidth+i]=(int)(fResult+0.5);
        }

    memcpy(m_pdata, p_temp,DibWidth*height);   //���ƴ�����ͼ��
    delete  []p_temp;
}

///***************************************************************/           
/*�������ƣ�Robert()                                      
/*�������ͣ�void                                     
/*���ܣ����޲������Ӷ�ͼ����б�Ե��⡣            
/***************************************************************/ 
void BianYuanJianCeDib::Robert()
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;
    int pixel[4];   //Robert����

    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // 2*2ģ��
    for (int y = 0; y < height - border; y++) {
        for (int x = 0; x < lineBytes - border; x++) {
            // ����Robert����
            pixel[0] = p_data[lineBytes * y + x];
            pixel[1] = p_data[lineBytes * y + (x + border)];
            pixel[2] = p_data[lineBytes * (y + 1) + x];
            pixel[3] = p_data[lineBytes * (y + 1) + (x + border)];

            BYTE deltaX = abs(pixel[3] - pixel[0]);
            BYTE deltaY = abs(pixel[2] - pixel[1]);
            p_temp[lineBytes * y + x] = (deltaX + deltaY > 255) ? 255 : deltaX + deltaY;
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}
 
///***************************************************************/           
/*�������ƣ�PreWitt()                                      
/*�������ͣ�void                                     
/*���ܣ�������ά�����Ӷ�ͼ����б�Ե��⡣            
/***************************************************************/
void BianYuanJianCeDib::PreWitt()
{
    int tempH;  //ģ��߶�
    int tempW;  //ģ����
    int tempMY;   //ģ������Ԫ��Y����
    int tempMX;   //ģ������Ԫ��X����

    float tempC;  //ģ��ϵ��
    float kernel[9];   //ģ������

    int width = GetWidth();   //ԭͼ��
    int height = GetHeight(); //ԭͼ��
    int lineBytes = GetDibWidthBytes(); //ԭͼ4�ֽڶ����Ŀ�
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = this->GetData ();   //ԭͼ������ָ��
    LPBYTE p_tempX = new BYTE[size]; //x���������ͼ��ָ��
    LPBYTE p_tempY = new BYTE[size]; //y���������ͼ��ָ��

    //����ԭͼ�񵽻���ͼ��
    memcpy(p_tempX, p_data, size);
    memcpy(p_tempY, p_data, size);

    //����Prewittģ��1����
    tempW = 3;
    tempH = 3;
    tempC = 1.0;
    tempMY = 1;
    tempMX = 1;
    kernel[0] = -1.0f;
    kernel[1] = -1.0f;
    kernel[2] = -1.0f;
    kernel[3] =  0.0f;
    kernel[4] =  0.0f;
    kernel[5] =  0.0f;
    kernel[6] =  1.0f;
    kernel[7] =  1.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempX, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����Prewittģ��2����
    kernel[0] = -1.0f;
    kernel[1] =  0.0f;
    kernel[2] =  1.0f;
    kernel[3] = -1.0f;
    kernel[4] =  0.0f;
    kernel[5] =  1.0f;
    kernel[6] = -1.0f;
    kernel[7] =  0.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempY, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_data[index] = max(p_tempX[index], p_tempY[index]); // ȡ�ݶ�G = max{Gx�� Gy}
        }
    }

    delete p_tempX;
    delete p_tempY;
}

///***************************************************************/           
/*�������ƣ�Sobel()                                      
/*�������ͣ�void                                     
/*���ܣ������������Ӷ�ͼ����б�Ե��⡣            
/***************************************************************/
void BianYuanJianCeDib::Sobel()
{
    int tempH;  //ģ��߶�
    int tempW;  //ģ����
    int tempMY;   //ģ������Ԫ��Y����
    int tempMX;   //ģ������Ԫ��X����
    
    float tempC;  //ģ��ϵ��
    float kernel[9];   //ģ������
    
    int width = GetWidth();   //ԭͼ��
    int height = GetHeight(); //ԭͼ��
    int lineBytes = GetDibWidthBytes(); //ԭͼ4�ֽڶ����Ŀ�
    int size = lineBytes * height;

    LPBYTE p_data = this->GetData ();   //ԭͼ������ָ��
    LPBYTE p_tempX = new BYTE[size]; //x���������ͼ��ָ��
    LPBYTE p_tempY = new BYTE[size]; //y���������ͼ��ָ��

    //����ԭͼ�񵽻���ͼ��
    memcpy(p_tempX, p_data, size);
    memcpy(p_tempY, p_data, size);

    //����Sobelģ��1����
    tempW = 3;
    tempH = 3;
    tempC = 1.0;
    tempMY = 1;
    tempMX = 1;
    kernel[0] = -1.0f;
    kernel[1] = -2.0f;
    kernel[2] = -1.0f;
    kernel[3] =  0.0f;
    kernel[4] =  0.0f;
    kernel[5] =  0.0f;
    kernel[6] =  1.0f;
    kernel[7] =  2.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempX, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����Sobelģ��2����
    kernel[0] = -1.0f;
    kernel[1] =  0.0f;
    kernel[2] =  1.0f;
    kernel[3] = -2.0f;
    kernel[4] =  0.0f;
    kernel[5] =  2.0f;
    kernel[6] = -1.0f;
    kernel[7] =  0.0f;
    kernel[8] =  1.0f;
    this->Template(p_tempY, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_data[index] = max(p_tempX[index], p_tempY[index]); // ȡ�ݶ�G = max{Gx�� Gy}
        }
    }
    
    delete p_tempX;
    delete p_tempY;
}

///***************************************************************/           
/*�������ƣ�Laplacian()                                      
/*�������ͣ�void       
/*����������bool onlyborder ֻ�����߽磬������ϸ��
/*���ܣ���������˹���Ӷ�ͼ���Ե��⡣            
/***************************************************************/ 
void BianYuanJianCeDib::Laplacian(bool onlyborder)
{
    int tempH;  //ģ��߶�
    int tempW;  //ģ����
    int tempMY;   //ģ������Ԫ��Y����
    int tempMX;   //ģ������Ԫ��X����

    float tempC;  //ģ��ϵ��
    float kernel[9];   //ģ������

    int width = GetWidth();   //ԭͼ��
    int height = GetHeight(); //ԭͼ��
    int lineBytes = GetDibWidthBytes(); //ԭͼ4�ֽڶ����Ŀ�
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = this->GetData ();   //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memcpy(p_temp, p_data, size);

    //����Laplacianģ�����
    tempW = 3;
    tempH = 3;
    tempC = 1.0;
    tempMY = 1;
    tempMX = 1;
    kernel[0] = -1.0f;
    kernel[1] = -1.0f;
    kernel[2] = -1.0f;
    kernel[3] = -1.0f;
    kernel[4] =  8.0f; // ֻ�����߽�
    kernel[5] = -1.0f;
    kernel[6] = -1.0f;
    kernel[7] = -1.0f;
    kernel[8] = -1.0f;

    if (!onlyborder)
        kernel[4] =  9.0f; // �߽� + �ڲ���Ϣ������ԭͼ��=================================

    this->Template(p_temp, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);
    
    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*�������ƣ�Guasslaplacian()                                      
/*�������ͣ�void                                    
/*���ܣ��ø�˹������˹���Ӷ�ͼ���Ե��⡣            
/***************************************************************/ 
void BianYuanJianCeDib::Guasslaplacian()
{
    int tempH;  //ģ��߶�
    int tempW;  //ģ����
    int tempMY;   //ģ������Ԫ��Y����
    int tempMX;   //ģ������Ԫ��X����

    float tempC;  //ģ��ϵ��
    float kernel[25];   //ģ������

    int width = GetWidth();   //ԭͼ��
    int height = GetHeight(); //ԭͼ��
    int lineBytes = GetDibWidthBytes(); //ԭͼ4�ֽڶ����Ŀ�
    int size = lineBytes * height;
    int border = GetRGB() ? 1 : 3;

    LPBYTE p_data = this->GetData ();   //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memcpy(p_temp, p_data, size);

    //����Guasslaplacianģ�����
    tempW = 5;
    tempH = 5;
    tempC = 0.25;
    tempMY = 2;
    tempMX = 2;
    kernel[0]  = -2.0f;
    kernel[1]  = -4.0f;
    kernel[2]  = -4.0f;
    kernel[3]  = -4.0f;
    kernel[4]  = -2.0f;
    kernel[5]  = -4.0f;
    kernel[6]  =  0.0f;
    kernel[7]  =  8.0f;
    kernel[8]  =  0.0f;
    kernel[9]  = -4.0f;
    kernel[10] = -4.0f;
    kernel[11] =  8.0f;
    kernel[12] = 24.0f;
    kernel[13] =  8.0f;
    kernel[14] = -4.0f;
    kernel[15] = -4.0f;
    kernel[16] =  0.0f;
    kernel[17] =  8.0f;
    kernel[18] =  0.0f;
    kernel[19] = -4.0f;
    kernel[20] = -2.0f;
    kernel[21] = -4.0f;
    kernel[22] = -4.0f;
    kernel[23] = -4.0f;
    kernel[24] = -2.0f;
    this->Template(p_temp, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

///***************************************************************/           
/*�������ƣ�Krisch()                                      
/*�������ͣ�void                                     
/*���ܣ��ÿ���˹���Ӷ�ͼ���Ե��⡣            
/***************************************************************/ 
void BianYuanJianCeDib::Krisch()
{
    int tempH;  //ģ��߶�
    int tempW;  //ģ����
    int tempMY;   //ģ������Ԫ��Y����
    int tempMX;   //ģ������Ԫ��X����

    float tempC;  //ģ��ϵ��
    float kernel[9];   //ģ������

    int width = GetWidth();   //ԭͼ��
    int height = GetHeight(); //ԭͼ��
    int lineBytes = GetDibWidthBytes(); //ԭͼ4�ֽڶ����Ŀ�
    int size = lineBytes * height;

    LPBYTE p_data = this->GetData ();   //ԭͼ������ָ��
    LPBYTE p_temp1 = new BYTE[size]; //ģ��1������ͼ��ָ��
    LPBYTE p_temp2 = new BYTE[size]; //ģ��2������ͼ��ָ��

    //����ԭͼ�񵽻���ͼ��
    memcpy(p_temp1, p_data, size);
    memcpy(p_temp2, p_data, size);

    //����Kirschģ��1����
    tempW = 3;
    tempH = 3;
    tempC = 0.5;
    tempMY = 1;
    tempMX = 1;
    kernel[0] =  5.0f;
    kernel[1] =  5.0f;
    kernel[2] =  5.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp1, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����Kirschģ��2����
    kernel[0] = -3.0f;
    kernel[1] =  5.0f;
    kernel[2] =  5.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] =  5.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]); //ȡ2�������е����ֵ
        }
    }

    memcpy(p_temp2, p_data, size);

    //����Kirschģ��3����
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] =  5.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] =  5.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] =  5.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //����Kirschģ��4����
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] =  5.0f;
    kernel[6] = -3.0f;
    kernel[7] =  5.0f;
    kernel[8] =  5.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //����Kirschģ��5����
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] = -3.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] =  5.0f;
    kernel[7] =  5.0f;
    kernel[8] =  5.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //����Kirschģ��6����
    kernel[0] = -3.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] =  5.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] =  5.0f;
    kernel[7] =  5.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //����Kirschģ��7����
    kernel[0] =  5.0f;
    kernel[1] = -3.0f;
    kernel[2] = -3.0f;
    kernel[3] =  5.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] =  5.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_temp2, p_data, size);

    //����Kirschģ��7����
    kernel[0] =  5.0f;
    kernel[1] =  5.0f;
    kernel[2] = -3.0f;
    kernel[3] =  5.0f;
    kernel[4] =  0.0f;
    kernel[5] = -3.0f;
    kernel[6] = -3.0f;
    kernel[7] = -3.0f;
    kernel[8] = -3.0f;
    this->Template(p_temp2, lineBytes, height, tempH, tempW, tempMX, tempMY, kernel, tempC);

    //����������ͼ������ֵ
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            p_temp1[index] = max(p_temp1[index], p_temp2[index]);
        }
    }

    memcpy(p_data, p_temp1, size);

    delete p_temp1;
    delete p_temp2;
}
