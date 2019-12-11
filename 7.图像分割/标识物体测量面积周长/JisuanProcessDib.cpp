// JisuanProcessDib.cpp: implementation of the JisuanProcessDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSplit.h"
#include "JisuanProcessDib.h"
#include "SquareDlg.h"
#include "LINEDLG.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JisuanProcessDib::JisuanProcessDib()
{
    count = 0;
}

JisuanProcessDib::~JisuanProcessDib()
{
}

/***************************************************************/
/*�������ƣ�erzhihua(int T)                                    */
/*�������ͣ�void                                               */
/*������int T���û���������ֵ                                   */
/*���ܣ���ͼ��ʹ�ù̶���ֵ�����ж�ֵ����                         */
/***************************************************************/
void JisuanProcessDib::erzhihua(int T)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    if (GetRGB()) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE *p_temp = p_data + lineBytes * (height - y - 1) + x;
                *p_temp = (*p_temp < T) ? 0 : 255;
            }
        }
    }
    else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                BYTE *p_temp = p_data + lineBytes * (height - y - 1) + x * 3;
                if (*p_temp < T) {
                    *p_temp++ = 0;
                    *p_temp++ = 0;
                    *p_temp   = 0;
                } else {
                    *p_temp++ = 255;
                    *p_temp++ = 255;
                    *p_temp   = 255;
                }
            }
        }
    }
}

/***************************************************************/
/*�������ƣ�xiaochugulidianHEI()                               */
/*�������ͣ�void                                               */
/*���ܣ����������ڵ㡣                                         */
/***************************************************************/
void JisuanProcessDib::xiaochugulidianHEI()
{
    LPBYTE p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    int wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    int height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    if(m_pBitmapInfoHeader->biBitCount<9)    //�Ҷ�ͼ��
    {
        for(int j=1;j<height-1;j++)    // ÿ��
            for(int i=1;i<wide-1;i++)    // ÿ��
            {
                int temp=0;    
                if(*(p_data+(height-j-1)*wide+i)==0)//����Ϊ�ڵ�
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide+i+n-1);
                        }
                    if(temp>=255*6)///��Χ8���е��д��ڵ���6���׵�
                        *(p_data+(height-j-1)*wide+i)=255;
                }
            }
    }
    else    //24λ��ɫ
    {
        for(int j=1;j<height-1;j++)    // ÿ��
            for(int i=1;i<wide-1;i++)    // ÿ��
            {
                int temp=0;    
                if(*(p_data+(height-j-1)*wide*3+i*3)==0)//����Ϊ�ڵ�
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide*3+(i+n-1)*3);
                        }
                    if(temp>=255*6)///��Χ8���е��д��ڵ���6���׵�
                    {
                        *(p_data+(height-j-1)*wide*3+i*3)=255;
                        *(p_data+(height-j-1)*wide*3+i*3+1)=255;
                        *(p_data+(height-j-1)*wide*3+i*3+2)=255;
                    }
                }
            }
    }
}

/***************************************************************/
/*�������ƣ�xiaochugulidianBAI()                               */
/*�������ͣ�void                                               */
/*���ܣ����������׵㡣                                         */
/***************************************************************/
void JisuanProcessDib::xiaochugulidianBAI()
{
    LPBYTE p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    int wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    int height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    if(m_pBitmapInfoHeader->biBitCount<9)    //�Ҷ�ͼ��
    {
        for(int j=1;j<height-1;j++)    // ÿ��
            for(int i=1;i<wide-1;i++)    // ÿ��
            {
                int temp=0;            ;
                if(*(p_data+(height-j-1)*wide+i)==255)//����Ϊ�׵�
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide+i+n-1);
                        }
                    if(temp<=255*3)///��Χ8��������С�ڵ���2���׵�
                        *(p_data+(height-j-1)*wide+i)=0;
                }
            }
    }
    else    //24λ��ɫ
    {
        for(int j=1;j<height-1;j++)    // ÿ��
            for(int i=1;i<wide-1;i++)    // ÿ��
            {
                int temp=0;            ;
                if(*(p_data+(height-j-1)*wide*3+i*3)==255)//����Ϊ�׵�
                {
                    for(int m=0;m<3;m++)
                        for(int n=0;n<3;n++)
                        {
                            temp+=*(p_data+(height-j-m)*wide*3+(i+n-1)*3);
                        }
                    if(temp<=255*3)///��Χ8��������С�ڵ���2���׵�
                    {
                        *(p_data+(height-j-1)*wide*3+i*3)=0;
                        *(p_data+(height-j-1)*wide*3+i*3+1)=0;
                        *(p_data+(height-j-1)*wide*3+i*3+2)=0;
                    }
                }
            }
    }
}

/***************************************************************/
/*�������ƣ�biaoji(int T)                                       */
/*�������ͣ�int                                                */
/*����������int T ��ֵ����ֵ                                    */
/*         LPBYTE pdata ���ָ��                               */
/*���ܣ���ֵ������ͼ����,���ֳɲ�ͬ����ͨ����                  */
/***************************************************************/
int JisuanProcessDib::biaoji(int T, LPBYTE pdata)
{
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size]; // ���ÿ�����أ����ڻ�����ͨ����
    memset(p_temp, 255, size);
    memset(flag, 0, sizeof(flag));

    int signID = 0;
    int signCount = 0;
    bool stop = false;

    if (GetRGB()) {
        this->erzhihua(T); ///ͼ���ֵ��

        // �����ұ��
        for (int y = 1; y < height - 1; y++) { // ÿ��
            if (stop) // �ж���ͨ���Ƿ�̫��
                break;
            for (int x = 1; x < width - 1; x++) { // ÿ��
                if (signCount > 250) {
                    /*AfxMessageBox("��ͨ����Ŀ̫��,��������ֵ");*/
                    stop = true;
                    break;
                }

                int x0 = x;
                int y0 = height - y - 1;
                int indexCurrent = lineBytes * y0 + x0;
                if (p_data[indexCurrent] == 0) {// ����ǰ��Ϊ�ڵ�
                    // ��������
                    int xRightUp = x + 1;
                    int yRightUp = height - (y - 1) - 1;
                    int xUp = x;
                    int yUp = height - (y - 1) - 1;
                    int xLeftUp = x - 1;
                    int yLeftUp = height - (y - 1) - 1;
                    int xLeft = x - 1;
                    int yLeft = height - y - 1;
                    int indexRightUp = lineBytes * yRightUp + xRightUp;
                    int indexUp = lineBytes * yUp + xUp;
                    int indexLeftUp = lineBytes * yLeftUp + xLeftUp;
                    int indexLeft = lineBytes * yLeft + xLeft;

                    // ���
                    if (p_data[indexRightUp] == 0) { //����
                        signID = p_temp[indexRightUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;

                        // 1.�����ǰ�����ϱ�ǲ�ͬ����������ͨ�ģ��޸ı��Ϊ����
                        if (p_data[indexLeft] == 0 && p_temp[indexLeft] != signID) { //��ǰ
                            int oldSign = p_temp[indexLeft];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//��ǰ

                        // 2.������������ϱ�ǲ�ͬ����������ͨ�ģ��޸ı��Ϊ����
                        if (p_data[indexLeftUp] == 0 && p_temp[indexLeftUp] != signID) { //����
                            int oldSign = p_temp[indexLeftUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//����

                        // 3.������������ϱ�ǲ�ͬ����������ͨ�ģ��޸ı��Ϊ����
                        if (p_data[indexUp] == 0 && p_temp[indexUp] != signID) { //����
                            int oldSign = p_temp[indexUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//����
                    }//end//����
                    else if (p_data[indexUp] == 0) { //����
                        signID = p_temp[indexUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeftUp] == 0) { //����
                        signID = p_temp[indexLeftUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeft] == 0) { //��ǰ
                        signID = p_temp[indexLeft];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else { // û��
                        signCount++;
                        signID = signCount;
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                } //end if
            } //end ÿ��
        } //end ÿ��
    }
    else {
        this->RgbToGray();
        //// ������ת��Ϊ24λ�Ҷȣ���ֻͳ�� B ͨ�� ////
        this->erzhihua(T); ///ͼ���ֵ��

        // �����ұ��
        for (int y = 1; y < height - 1; y++) { // ÿ��
            if (stop) // �ж���ͨ���Ƿ�̫��
                break;
            for (int x = 1; x < width - 1; x++) { // ÿ��
                if (signCount > 250) {
                    AfxMessageBox("��ͨ����Ŀ̫��,��������ֵ");
                    stop = true;
                    break;
                }

                int x0 = x;
                int y0 = height - y - 1;
                int indexCurrent = lineBytes * y0 + x0 * 3;
                if (p_data[indexCurrent] == 0) {// ����ǰ��Ϊ�ڵ�
                    // ��������
                    int xRightUp = x + 1;
                    int yRightUp = height - (y - 1) - 1;
                    int xUp = x;
                    int yUp = height - (y - 1) - 1;
                    int xLeftUp = x - 1;
                    int yLeftUp = height - (y - 1) - 1;
                    int xLeft = x - 1;
                    int yLeft = height - y - 1;
                    int indexRightUp = lineBytes * yRightUp + xRightUp * 3;
                    int indexUp = lineBytes * yUp + xUp * 3;
                    int indexLeftUp = lineBytes * yLeftUp + xLeftUp * 3;
                    int indexLeft = lineBytes * yLeft + xLeft * 3;

                    // ���
                    if (p_data[indexRightUp] == 0) { //����
                        signID = p_temp[indexRightUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;

                        // 1.�����ǰ�����ϱ�ǲ�ͬ����������ͨ�ģ��޸ı��Ϊ����
                        if (p_data[indexLeft] == 0 && p_temp[indexLeft] != signID) { //��ǰ
                            int oldSign = p_temp[indexLeft];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n * 3;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//��ǰ

                        // 2.������������ϱ�ǲ�ͬ����������ͨ�ģ��޸ı��Ϊ����
                        if (p_data[indexLeftUp] == 0 && p_temp[indexLeftUp] != signID) { //����
                            int oldSign = p_temp[indexLeftUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n * 3;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//����

                        // 3.������������ϱ�ǲ�ͬ����������ͨ�ģ��޸ı��Ϊ����
                        if (p_data[indexUp] == 0 && p_temp[indexUp] != signID) { //����
                            int oldSign = p_temp[indexUp];
                            for(int m = 1; m <= height - 1; m++) {
                                for(int n = 1; n <= width - 1; n++) {
                                    int pixelIndex = lineBytes * (height - m - 1) + n * 3;
                                    if (p_temp[pixelIndex] == oldSign) {
                                        flag[oldSign]--;
                                        p_temp[pixelIndex] = signID;
                                        flag[signID]++;
                                    }
                                }
                            }
                        }//end//����
                    }//end//����
                    else if (p_data[indexUp] == 0) { //����
                        signID = p_temp[indexUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeftUp] == 0) { //����
                        signID = p_temp[indexLeftUp];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else if (p_data[indexLeft] == 0) { //��ǰ
                        signID = p_temp[indexLeft];
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                    else { // û��
                        signCount++;
                        signID = signCount;
                        p_temp[indexCurrent] = signID;
                        flag[signID]++;
                    }
                } //end if
            } //end ÿ��
        } //end ÿ��
    }

    count = 0;
    if (!stop) {
        // ����ͳ��
        for (int i = 1; i <= signCount; i++) {
            if (flag[i] != 0)
                ++count;
        }

        // ����λ��
        for(int i = 0;i < 255; i++)
            pppp[i].pp_area = 0;

        int dibFactor = GetRGB() ? 1 : 3;
        //int dibFactor = (m_pBitmapInfoHeader->biBitCount == 24) ? 3 : 1;
        for (int t = 1; t <= signCount; t++) {
            pppp[t].pp_number = t;
            for (int y = 1; y < height - 1; y++) {
                for (int x = 1; x < width - 1; x++) {
                    if (p_temp[lineBytes * y + x * dibFactor] == t) { // ����ÿ�������λ��
                        pppp[t].pp_x = x;
                        pppp[t].pp_y = height - y - 1;
                        pppp[t].pp_area = flag[t];
                        break;
                    }
                }

                if (pppp[t].pp_area != 0)
                    break;
            }
        }

        if (pdata != nullptr) 
            memcpy(pdata, p_temp, size);
    }

    delete p_temp;
    return signCount;
}

/***************************************************************/
/*�������ƣ�ClearSMALL(int T1, int T2)                          */
/*�������ͣ�int                                                 */
/*������int T1����ֵ����ֵ                                       */
/*     int T2�����������ֵ                                      */
/*���ܣ��������С�ڸ�������ֵ��С����                           */
/***************************************************************/
int JisuanProcessDib::ClearSMALL(int T1, int T2)
{ 
    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    int size = lineBytes * height;
    LPBYTE p_data = GetData();
    LPBYTE p_temp = new BYTE[size];

    int count = biaoji(T1, p_temp); // ���
    if (count > 0) {
        if(m_pBitmapInfoHeader->biBitCount < 9) {   //�Ҷ�ͼ��
            for (int i = 1; i < count; i++) {
                if(flag[i] > 0 && flag[i] < T2) { // �ж���ͨ������������ظ������Ƿ�����
                    for(int y = 1; y < height - 1; y++) {
                        for(int x = 1; x < width - 1; x++) {
                            if (p_temp[lineBytes * (height - y - 1) + x] == i) {// ���
                                p_data[lineBytes * (height - y - 1) + x] = 255;
                                pppp[i].pp_area = 0;
                            }
                        } 
                    }
                }
            }
        }
        else { //24λ��ɫ
            for (int i = 1; i < count; i++) {
                if(flag[i] < T2) { // �ж���ͨ������������ظ������Ƿ�����
                    for(int y = 1; y < height - 1; y++) {
                        for(int x = 1; x < width - 1; x++) {
                            if (p_temp[lineBytes * (height - y - 1) + x * 3] == i) {// ���
                                p_data[lineBytes * (height - y - 1) + x * 3] = 255;
                                p_data[lineBytes * (height - y - 1) + x * 3 + 1] = 255;
                                p_data[lineBytes * (height - y - 1) + x * 3 + 2] = 255;
                                pppp[i].pp_area = 0;
                            }
                        } 
                    }
                }
            }
        }
    }

    delete p_temp;
    return count;
}

/***************************************************************/
/*�������ƣ�Borderline(int T)                                  */
/*�������ͣ�int                                                */
/*�������ͣ�int                                                */
/*���ܣ���ÿ����ͨ�����б߽���٣���ȡ�߽磬����ܳ���             */
/***************************************************************/
int JisuanProcessDib::Borderline(int T)
{
    int count = biaoji(T); // ���
    if (count > 0) {
        if (m_pBitmapInfoHeader->biBitCount == 24)
            Lunkuotiqu2(T);
        else
            Lunkuotiqu(T); // ���ñ߽����
        
        count = biaoji(T); // ���±��

        // �����ܳ�
        for(int i = 0;i < 255; i++) {
            pppp[i].pp_line = pppp[i].pp_area;
            pppp[i].pp_area = 0;
        }
    }

    return count;
}

//��24λ��ɫͼ��ת��Ϊ24λ�Ҷ�ͼ
void JisuanProcessDib::RgbToGray()
{
    if (GetRGB())
        return;

    int width = GetWidth();
    int height = GetHeight();
    int lineBytes = GetDibWidthBytes();
    LPBYTE p_data = GetData();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x += 3) {
            BYTE* pbyBlue = p_data++;   //�õ���ɫֵ
            BYTE* pbyGreen = p_data++;  //�õ���ɫֵ
            BYTE* pbyRed = p_data++;    //�õ���ɫֵ
            BYTE r = *pbyRed;
            BYTE g = *pbyGreen;
            BYTE b = *pbyBlue;
            int gray = (30 * r + 59 * g + 11 * b) /100;
            *pbyBlue = gray;   
            *pbyGreen = gray; 
            *pbyRed = gray;
        }
    }
}

//����δ�����ԭͼ��
void JisuanProcessDib::Baoliu(LPBYTE temp)
{
    // ָ��DIB����ָ��
    LPBYTE p_data;
    // ָ�򻺴�ͼ���ָ��
    LPBYTE    lpDst;
    // �ҵ�DIBͼ��������ʼλ��
    p_data= GetData();
    // DIB�Ŀ��
    LONG wide = GetDibWidthBytes();
    // DIB�ĸ߶�
    LONG height = GetHeight();    
    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
    lpDst = (LPBYTE)temp;
    memset(lpDst, (BYTE)255, wide * height);
    memcpy(lpDst,p_data,wide*height);
}

///***************************************************************/           
/*�������ƣ�Lunkuotiqu(int T)                                      
/*�������ͣ�void  
/*����������int T ��ֵ����ֵ
/*���ܣ���ͼ�����������ȡ���Զ�ֵͼ�������Ϳ��ڲ��㷨����            
/***************************************************************/ 
void JisuanProcessDib::Lunkuotiqu(int T)
{
    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    width = GetRGB() ? width : lineBytes;

    // ��ֵ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // �жϺڵ���Χ8�����Ƿ�ȫΪ�ڣ����ȫ�����ð�
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (p_data[lineBytes * y + x] == 0) {
                int n1 = p_data[lineBytes * (y - 1) + (x - 1)];
                int n2 = p_data[lineBytes * (y - 1) + (x)];
                int n3 = p_data[lineBytes * (y - 1) + (x + 1)];
                int n4 = p_data[lineBytes * (y) + (x - 1)];
                int n5 = p_data[lineBytes * (y) + (x + 1)];
                int n6 = p_data[lineBytes * (y + 1) + (x - 1)];
                int n7 = p_data[lineBytes * (y + 1) + (x)];
                int n8 = p_data[lineBytes * (y + 1) + (x + 1)];
                
                if (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 == 0)
                    p_temp[lineBytes * y + x] = 255; // �Ϳ��ڲ���
                else
                    p_temp[lineBytes * y + x] = 0;
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}

void JisuanProcessDib::Lunkuotiqu2(int T)
{
    this->RgbToGray();
    //this->Lunkuotiqu(T);

    int width = this->GetWidth(); // ԭͼ���
    int height = this->GetHeight(); // ԭͼ�߶�
    int lineBytes = this->GetDibWidthBytes(); // ԭͼ 4 �ֽڶ����Ŀ��
    int size = lineBytes * height;
    LPBYTE p_data = this->GetData (); //ԭͼ������ָ��
    LPBYTE p_temp = new BYTE[size];
    memset(p_temp, 255, size);

    // ��ֵ��
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < lineBytes; x++) {
            int index = lineBytes * y + x;
            if (p_data[index] > T)
                p_data[index] = 255;
            else
                p_data[index] = 0;
        }
    }

    // �жϺڵ���Χ8�����Ƿ�ȫΪ�ڣ����ȫ�����ð�
    for (int y = 1; y < height - 1; y++) {
        for (int x = 3; x < width - 3; x++) {
            if (p_data[lineBytes * y + x * 3] == 0) {
                int n1 = p_data[lineBytes * (y - 1) + (x - 3) * 3];
                int n2 = p_data[lineBytes * (y - 1) + (x) * 3];
                int n3 = p_data[lineBytes * (y - 1) + (x + 3) * 3];
                int n4 = p_data[lineBytes * (y) + (x - 3) * 3];
                int n5 = p_data[lineBytes * (y) + (x + 3) * 3];
                int n6 = p_data[lineBytes * (y + 1) + (x - 3) * 3];
                int n7 = p_data[lineBytes * (y + 1) + (x) * 3];
                int n8 = p_data[lineBytes * (y + 1) + (x + 3) * 3];

                if (n1 + n2 + n3 + n4 + n5 + n6 + n7 + n8 == 0) {
                    p_temp[lineBytes * y + x * 3] = 255; // �Ϳ��ڲ���
                    p_temp[lineBytes * y + x * 3 + 1] = 255;
                    p_temp[lineBytes * y + x * 3 + 2] = 255;
                }
                else {
                    p_temp[lineBytes * y + x * 3] = 0;
                    p_temp[lineBytes * y + x * 3 + 1] = 0;
                    p_temp[lineBytes * y + x * 3 + 2] = 0;
                }
            }
        }
    }

    memcpy(p_data, p_temp, size);
    delete p_temp;
}