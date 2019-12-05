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
    x_sign=0;
    m_temp=0;
    x_temp=0;
    y_temp=0;
    p_temp=0;
    stop=0;
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
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
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
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
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
/*���ܣ���ͼ����б��,���ֳɲ�ͬ����ͨ����                  */
/***************************************************************/
void JisuanProcessDib::biaoji(int T)
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
        // �����ұ��
        for (int y = 1; y < height - 1; y++) {
            if (stop) // �ж���ͨ���Ƿ�̫��
                break;
            for (int x = 1; x < width - 1; x++) {
                if (signCount > 250) {
                    AfxMessageBox("��ͨ����Ŀ̫��,��������ֵ");
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
            }
        }
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

    delete p_temp;
}
//
//{
//    x_sign=0; 
//    m_temp=0;
//    x_temp=0;
//    y_temp=0;
//    stop=0;
//    memset(flag,0,255);
//    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
//    wide=this->GetWidth ();  //ȡ��ԭͼ�����������
//    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
//    if(m_pBitmapInfoHeader->biBitCount<9)    //�Ҷ�ͼ��
//    {
//        p_temp=new BYTE[wide*height];//����һ����ʱ�ڴ���
//        memset(p_temp,255,wide*height);
//        //�����ұ��
//        for(int j=1;j<height-1;j++)    // ÿ��
//        {
//            if(stop==1)//�ж���ͨ���Ƿ�̫��
//                break;
//            for(int i=1;i<wide-1;i++)    // ÿ��
//            {
//                if(x_sign>250)
//                {
//                    AfxMessageBox("��ͨ����Ŀ̫��,��������ֵ");
//                    stop=1;
//                    break;
//                }
//                if(*(p_data+(height-j-1)*wide+i)==0)//����ǰ��Ϊ�ڵ�
//                {
//                    if(*(p_data+(height-j-1+1)*wide+i+1)==0)//����
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i+1);
//                        x_temp=*(p_temp+(height-j-1+1)*wide+i+1);
//                        flag[x_temp]+=1;
//                        if(*(p_data+(height-j-1)*wide+i-1)==0&&*(p_temp+(height-j-1)*wide+i-1)!=x_temp)//��ǰ
//                        {
//                            y_temp=*(p_temp+(height-j-1)*wide+i-1);
//                            for(int m=1;m<=height-1;m++)
//                                for(int n=1;n<=wide-1;n++)
//                                {
//                                    if(*(p_temp+(height-m-1)*wide+n)==y_temp)
//                                    {    
//                                        flag[y_temp]=0;
//                                        *(p_temp+(height-m-1)*wide+n)=x_temp;
//                                        flag[x_temp]+=1;
//                                    }
//                                }
//                        }//end//��ǰ
//                        if(*(p_data+(height-j-1+1)*wide+i-1)==0&&*(p_temp+(height-j-1+1)*wide+i-1)!=x_temp)//����
//                        {
//                            y_temp=*(p_temp+(height-j-1+1)*wide+i-1);
//                            for(int m=1;m<=height-1;m++)
//                                for(int n=1;n<=wide-1;n++)
//                                {
//                                    if(*(p_temp+(height-m-1)*wide+n)==y_temp)
//                                    {    
//                                        flag[y_temp]=0;
//                                        *(p_temp+(height-m-1)*wide+n)=x_temp;
//                                        flag[x_temp]+=1;
//                                    }
//                                }
//                        }//end//����
//                    }
//                    else if(*(p_data+(height-j-1+1)*wide+i)==0)//����
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i);
//                        x_temp=*(p_temp+(height-j-1+1)*wide+i);
//                        flag[x_temp]+=1;
//                    }
//                    else if(*(p_data+(height-j-1+1)*wide+i-1)==0)//����
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i-1);
//                        x_temp=*(p_temp+(height-j-1+1)*wide+i-1);
//                        flag[x_temp]+=1;
//                    }
//                    else if(*(p_data+(height-j-1)*wide+i-1)==0)//��ǰ
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1)*wide+i-1);
//                        x_temp=*(p_temp+(height-j-1)*wide+i-1);
//                        flag[x_temp]+=1;
//                    }
//                    else//û��
//                    {                
//                        ++x_sign;
//                        m_temp=x_sign;
//                        *(p_temp+(height-j-1)*wide+i)=m_temp;
//                        flag[m_temp]=1;
//                        
//                    }
//                }//end if
//            }// ÿ��
//        }//end ÿ��
//    }
//    else    //24λ��ɫ
//    {
//        p_temp=new BYTE[wide*height];//����һ����ʱ�ڴ���
//        memset(p_temp,255,wide*height);
//        //�����ұ��
//        for(int j=1;j<height-1;j++)    // ÿ��
//        {
//            if(stop==1)//�ж���ͨ���Ƿ�̫��
//                break;
//            for(int i=1;i<wide-1;i++)    // ÿ��
//            {
//                if(x_sign>250)
//                {
//                    AfxMessageBox("��ͨ����Ŀ̫��,��������ֵ");
//                    stop=1;
//                    break;
//                }
//                if(*(p_data+(height-j-1)*wide*3+i*3)==0)//����ǰ��Ϊ�ڵ�
//                {
//                    
//                    if(*(p_data+(height-j-1+1)*wide*3+(i+1)*3)==0)//����
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+(i+1));
//                        x_temp=*(p_temp+(height-j-1+1)*wide+(i+1));
//                        flag[x_temp]+=1;
//                        if(*(p_data+(height-j-1)*wide*3+(i-1)*3)==0&&*(p_temp+(height-j-1)*wide+(i-1))!=x_temp)//��ǰ
//                        {
//                            y_temp=*(p_temp+(height-j-1)*wide+(i-1));
//                            for(int m=1;m<=height-1;m++)
//                                for(int n=1;n<=wide-1;n++)
//                                {
//                                    if(*(p_temp+(height-m-1)*wide+n)==y_temp)
//                                    {    
//                                        flag[y_temp]=0;
//                                        *(p_temp+(height-m-1)*wide+n)=x_temp;
//                                        
//                                        flag[x_temp]+=1;
//                                    }
//                                }
//                        }//end//��ǰ
//                        if(*(p_data+(height-j-1+1)*wide*3+(i-1)*3)==0&&*(p_temp+(height-j-1+1)*wide+(i-1))!=x_temp)//����
//                        {
//                            y_temp=*(p_temp+(height-j-1+1)*wide+(i-1));
//                            for(int m=1;m<=height-1;m++)
//                                for(int n=1;n<=wide-1;n++)
//                                {
//                                    if(*(p_temp+(height-m-1)*wide+n)==y_temp)
//                                    {    
//                                        flag[y_temp]=0;
//                                        *(p_temp+(height-m-1)*wide+n)=x_temp;
//                                        
//                                        flag[x_temp]+=1;
//                                    }
//                                }
//                        }//end//����
//                    }
//                    else if(*(p_data+(height-j-1+1)*wide*3+i*3)==0)//����
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+i);
//                        x_temp=*(p_temp+(height-j-1+1)*wide+i);
//                        flag[x_temp]+=1;
//                    }
//                    else if(*(p_data+(height-j-1+1)*wide*3+(i-1)*3)==0)//����
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1+1)*wide+(i-1));
//                        x_temp=*(p_temp+(height-j-1+1)*wide+(i-1));
//                        flag[x_temp]+=1;
//                    }
//                    else if(*(p_data+(height-j-1)*wide*3+(i-1)*3)==0)//��ǰ
//                    {
//                        *(p_temp+(height-j-1)*wide+i)=*(p_temp+(height-j-1)*wide+i-1);
//                        x_temp=*(p_temp+(height-j-1)*wide+(i-1));
//                        flag[x_temp]+=1;
//                    }
//                    else//û��
//                    {                
//                        ++x_sign;
//                        m_temp=x_sign;
//                        *(p_temp+(height-j-1)*wide+i)=m_temp;
//                        
//                        flag[m_temp]=1;
//                    }
//                }//end if
//            }// ÿ��
//        }//end ÿ��
//    }
//}

/***************************************************************/
/*�������ƣ�LianTong(int T)                                         */
/*�������ͣ�void                                               */
/*����������int T ��ֵ����ֵ                                    */
/*���ܣ�����ͨ������,���ÿ����ͨ���������                       */
/***************************************************************/
void JisuanProcessDib::LianTong(int T)
{
    biaoji(T);  //���ñ�Ǻ���
    if(stop!=1)//�ж���ͨ���Ƿ�̫��
    {
        int fg[255]={0};//����һ������
        memset(fg,0,255);//��ʼ����ֵ��Ϊ0
        int y_sign=0;
        int m_Area=0;//����һ�����
        for(int i=0;i<255;i++)
        {
            if(flag[i]!=0)
            {
                if(fg[y_sign]==0)
                {
                    fg[y_sign]=flag[i];
                    ++y_sign;
                }            
            }
            m_Area+=flag[i];
        }
        SquareDlg  dlg;//����Ի���
        dlg.m_number=y_sign;//�����ͨ�������
        dlg.m_squareALL=m_Area;//�����ͨ������ܻ�
        CString ss[20];
        //�ڶԻ��������ÿ����ͨ����������������ظ�����
        for(int i=0;i<y_sign;i++)
        {
            ss[i].Format("��ͨ����%3d  �������:%10.0d\r\n",i+1,fg[i]);
            dlg.m_ShuChu+=ss[i];
        }
        dlg.DoModal();
        for(int i=0;i<255;i++) //��������pp_area��Ϊ0
        {
            pppp[i].pp_area=0;
        }
        for(int t=1;t<=x_sign;t++)
        {    pppp[t].pp_number=t;
        for(int j=1;j<height-1;j++)
            for(int i=1;i<wide-1;i++)
            {
                if(*(p_temp+(height-j-1)*wide+i)==t)
                {
                    pppp[t].pp_x=i;
                    pppp[t].pp_y=j;
                    pppp[t].pp_area=flag[t];
                    break;
                }                
            }
        }
    }//end if(stop!=1)
}

/***************************************************************/
/*�������ƣ�ClearSMALL(int m_value)                            */
/*�������ͣ�void                                               */
/*������int m_value���û���������ֵ                            */
/*���ܣ��������С�ڸ�������ֵ��С����                       */
/***************************************************************/
void JisuanProcessDib::ClearSMALL(int m_value)
{
    biaoji(m_value);  //���ñ�Ǻ���
    if(m_pBitmapInfoHeader->biBitCount<9)    //�Ҷ�ͼ��
    {
        if(stop!=1)//�ж���ͨ���Ƿ�̫��
        {
            for(int i=1;i<=x_sign;i++)
            {
                if(flag[i]<m_value)//�ж���ͨ������������ظ������Ƿ�����
                {
                    for(int m=1;m<height-1;m++)
                        for(int n=1;n<wide-1;n++)
                        {
                            if(*(p_temp+(height-m-1)*wide+n)==i)
                                *(p_data+(height-m-1)*wide+n)=255;
                        }        
                }
            }
        }
    }
    else    //24λ��ɫ
    {
        if(stop!=1)//�ж���ͨ���Ƿ�̫��
        {
            for(int i=1;i<=x_sign;i++)
            {
                if(flag[i]<m_value)//�ж���ͨ������������ظ������Ƿ�����
                {
                    for(int m=1;m<height-1;m++)
                        for(int n=1;n<wide-1;n++)
                        {
                            if(*(p_temp+(height-m-1)*wide+n)==i)
                            {
                                *(p_data+(height-m-1)*wide*3+n*3)=255;
                                *(p_data+(height-m-1)*wide*3+n*3+1)=255;
                                *(p_data+(height-m-1)*wide*3+n*3+2)=255;
                            }
                        }        
                }
            }
        }
    }
}

/***************************************************************/
/*�������ƣ�Borderline()                                       */
/*�������ͣ�void                                               */
/*���ܣ���ÿ����ͨ�����б߽���٣���ȡ�߽磬����ܳ���         */
/***************************************************************/
void JisuanProcessDib::Borderline()
{
    biaoji(100);  //���ñ�Ǻ���
    LPBYTE    lpSrc;  // ָ��Դͼ���ָ��
    LPBYTE    lpDst;    // ָ�򻺴�ͼ���ָ��
    LPBYTE    temp;  // ָ�򻺴�DIBͼ���ָ��
    int pixel;    //����ֵ
    bool bFindStartPoint;    //�Ƿ��ҵ���ʼ�㼰�ص���ʼ��
    bool bFindPoint;    //�Ƿ�ɨ�赽һ���߽��
    Point StartPoint,CurrentPoint;    //��ʼ�߽���뵱ǰ�߽��
    //�˸��������ʼɨ�跽��
    int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
    int BeginDirect;
    if(m_pBitmapInfoHeader->biBitCount<9)    //�Ҷ�ͼ��
    {
        temp = new BYTE[wide*height];     // ��ʱ�����ڴ棬�Ա�����ͼ��
        lpDst = temp;    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
        memset(lpDst, (BYTE)255, wide * height);
        if(stop!=1)//�ж���ͨ���Ƿ�̫��
        {
            //���ҵ������Ϸ��ı߽��
            for(int t=1;t<=x_sign;t++)
            {
                if(flag[t]!=0)
                {    
                    bFindStartPoint = false;
                    for (int j = 0;j < height && !bFindStartPoint;j++)
                    {
                        for(int i = 0;i < wide && !bFindStartPoint;i++)
                        {
                            // ָ��Դͼ������j�У���i�����ص�ָ��            
                            lpSrc =  (LPBYTE)(p_temp + wide * j + i);            
                            //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
                            pixel =  *lpSrc;            
                            if(pixel ==t)
                            {
                                bFindStartPoint = true;
                                StartPoint.Height = j;
                                StartPoint.Width = i;
                                // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
                                lpDst = (LPBYTE)(temp + wide * j + i);    
                                *lpDst =  t;
                            }        
                        }
                    }
                    //������ʼ���������·�������ʼɨ�������Ϸ���
                    BeginDirect = 0;
                    //���ٱ߽�
                    bFindStartPoint = false;
                    //�ӳ�ʼ�㿪ʼɨ��
                    CurrentPoint.Height = StartPoint.Height;
                    CurrentPoint.Width = StartPoint.Width;
                    while(!bFindStartPoint)
                    {
                        bFindPoint = false;
                        while(!bFindPoint)
                        {
                            //��ɨ�跽��鿴һ������
                            lpSrc = (LPBYTE)(p_temp + wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
                                + (CurrentPoint.Width + Direction[BeginDirect][0]));
                            pixel =  *lpSrc;
                            if(pixel == t)
                            {
                                bFindPoint = true;
                                CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
                                CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
                                if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
                                {
                                    bFindStartPoint = true;
                                }
                                lpDst =  (LPBYTE)(temp + wide * CurrentPoint.Height + CurrentPoint.Width);
                                *lpDst = t;
                                //ɨ��ķ�����ʱ����ת����
                                BeginDirect--;
                                if(BeginDirect == -1)
                                    BeginDirect = 7;
                                BeginDirect--;
                                if(BeginDirect == -1)
                                    BeginDirect = 7;
                            }
                            else
                            {
                                //ɨ�跽��˳ʱ����תһ��
                                BeginDirect++;
                                if(BeginDirect == 8)
                                    BeginDirect = 0;
                            }
                        }//////end{while(!bFindPoint)}
                    }//////end{while(!bFindStartPoint)}
                }//////end{if(flag[t]!=0)}
            }//////end{for(int t=1;t<=x_sign;t++)}
            memcpy(p_data, temp, wide * height);// ���Ƶ�ԭ������
            delete[] temp;     // �ͷ��ڴ�
            /////////////////////////////////////////////////
            int x_line=0;
            int fm[255]={0};//����һ������
            memset(fm,0,255);//��ʼ����ֵ��Ϊ0
            //ͳ��ÿ���߽�����ظ���
            for(int j=0;j<height;j++)
            {
                for(int i=0;i<wide;i++)
                {
                    if(*(p_data+(height-j-1)*wide+i)!=0)
                    {
                        x_line=*(p_data+(height-j-1)*wide+i);
                        ++fm[x_line];
                    }
                }
            }    
            int fn[255]={0};//����һ������
            memset(fn,0,255);//��ʼ����ֵ��Ϊ0
            int y_line=0;
            int m_line=0;//����һ�������
            for(int i=0;i<x_line;i++)
            {
                if(fm[i]!=0)
                {
                    if(fn[y_line]==0)
                    {
                        fn[y_line]=fm[i];
                        ++y_line;
                    }            
                }
                m_line+=fm[i];
            }
            LINEDLG  dlg;//����Ի���
            dlg.m_shumu=y_line;//�����ͨ�������
            dlg.m_zongshu=m_line;//�����ͨ������ܻ�
            CString ss[20];
            //�ڶԻ��������ÿ����ͨ�����ܳ����߽����ظ�����
            for(int i=0;i<y_line;i++)
            {
                ss[i].Format("��ͨ����%3d, �����ܳ�:%10.0d",i+1,fn[i]);
                dlg.m_line+=ss[i];
            }
            dlg.DoModal();
            /////////////////////////////////////////////////////////////////
            for(int i=0;i<255;i++) //��������pp_lin��Ϊ0
            {
                pppp[i].pp_line=0;
            }
            for(int t=1;t<=x_line;t++)
            {    
                for(int j=1;j<height-1;j++)
                    for(int i=1;i<wide-1;i++)
                    {
                        if(*(p_temp+(height-j-1)*wide+i)==t)
                        {
                            pppp[t].pp_x=i;
                            pppp[t].pp_y=j;
                            pppp[t].pp_line=fm[t];
                            break;
                        }                
                    }
            }
        }//end if(stop!=1)
    }
    else    //24λ��ɫ
    {
        p_data=this->GetData();
        temp = new BYTE[wide*height*3];     // ��ʱ�����ڴ棬�Ա�����ͼ��
        lpDst = temp;    // ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
        memset(lpDst, (BYTE)255, wide * height*3);
        if(stop!=1)//�ж���ͨ���Ƿ�̫��
        {
            //���ҵ������Ϸ��ı߽��
            for(int t=1;t<=x_sign;t++)
            {
                if(flag[t]!=0)
                {    
                    bFindStartPoint = false;
                    for (int j = 0;j < height && !bFindStartPoint;j++)
                    {
                        for(int i = 0;i < wide && !bFindStartPoint;i++)
                        {
                            // ָ��Դͼ������j�У���i�����ص�ָ��            
                            lpSrc =  (LPBYTE)(p_temp + wide * j + i);            
                            //ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
                            pixel =  *lpSrc;            
                            if(pixel ==t)
                            {
                                bFindStartPoint = true;
                                StartPoint.Height = j;
                                StartPoint.Width = i;
                                // ָ��Ŀ��ͼ������j�У���i�����ص�ָ��            
                                lpDst = (LPBYTE)(temp + wide * j*3 + i*3);    
                                *lpDst =  t;
                                *(lpDst+1) =  t;
                                *(lpDst+2) =  t;
                            }        
                        }
                    }
                    //������ʼ���������·�������ʼɨ�������Ϸ���
                    BeginDirect = 0;
                    //���ٱ߽�
                    bFindStartPoint = false;
                    //�ӳ�ʼ�㿪ʼɨ��
                    CurrentPoint.Height = StartPoint.Height;
                    CurrentPoint.Width = StartPoint.Width;
                    while(!bFindStartPoint)
                    {
                        bFindPoint = false;
                        while(!bFindPoint)
                        {
                            //��ɨ�跽��鿴һ������
                            lpSrc = (LPBYTE)(p_temp + wide * ( CurrentPoint.Height + Direction[BeginDirect][1])
                                + (CurrentPoint.Width + Direction[BeginDirect][0]));
                            pixel =  *lpSrc;
                            if(pixel == t)
                            {
                                bFindPoint = true;
                                CurrentPoint.Height = CurrentPoint.Height + Direction[BeginDirect][1];
                                CurrentPoint.Width = CurrentPoint.Width + Direction[BeginDirect][0];
                                if(CurrentPoint.Height == StartPoint.Height && CurrentPoint.Width == StartPoint.Width)
                                {
                                    bFindStartPoint = true;
                                }
                                lpDst =  (LPBYTE)(temp + 3*wide * CurrentPoint.Height + CurrentPoint.Width*3);
                                *lpDst = t;
                                *(lpDst+1) = t;
                                *(lpDst+2) = t;
                                //ɨ��ķ�����ʱ����ת����
                                BeginDirect--;
                                if(BeginDirect == -1)
                                    BeginDirect = 7;
                                BeginDirect--;
                                if(BeginDirect == -1)
                                    BeginDirect = 7;
                            }
                            else
                            {
                                //ɨ�跽��˳ʱ����תһ��
                                BeginDirect++;
                                if(BeginDirect == 8)
                                    BeginDirect = 0;
                            }
                            
                        }//////end{while(!bFindPoint)}
                    }//////end{while(!bFindStartPoint)}
                }//////end{if(flag[t]!=0)}
            }//////end{for(int t=1;t<=x_sign;t++)}
            memcpy(p_data, temp, wide * height*3);// ���Ƶ�ԭ������
            delete[] temp;     // �ͷ��ڴ�
            /////////////////////////////////////////////////
            int x_line=0;
            int fm[255]={0};//����һ������
            memset(fm,0,255);//��ʼ����ֵ��Ϊ0
            //ͳ��ÿ���߽�����ظ���
            for(int j=0;j<height;j++)
                for(int i=0;i<wide;i++)
                {
                    if(*(p_data+(height-j-1)*wide*3+i*3)!=0)
                    {
                        x_line=*(p_data+(height-j-1)*wide*3+i*3);
                        ++fm[x_line];
                    }
                }
            int fn[255]={0};//����һ������
            memset(fn,0,255);//��ʼ����ֵ��Ϊ0
            int y_line=0;
            int m_line=0;//����һ�������
            for(int i=0;i<x_line;i++)
            {
                if(fm[i]!=0)
                {
                    if(fn[y_line]==0)
                    {
                        fn[y_line]=fm[i];
                        ++y_line;
                    }            
                }
                m_line+=fm[i];
            }
            LINEDLG  dlg;//����Ի���
            dlg.m_shumu=y_line;//�����ͨ�������
            dlg.m_zongshu=m_line;//�����ͨ������ܻ�
            CString ss[20];
            //�ڶԻ��������ÿ����ͨ�����ܳ����߽����ظ�����
            for(int i=0;i<y_line;i++)
            {
                ss[i].Format("��ͨ����%3d  �����ܳ�:%10.0d",i+1,fn[i]);
                dlg.m_line+=ss[i];
            }
            dlg.DoModal();
            /////////////////////////////////////////////////////////////////
            for(int i=0;i<255;i++) //��������pp_lin��Ϊ0
            {
                pppp[i].pp_line=0;
            }
            for(int t=1;t<=x_line;t++)
            {    
                for(int j=1;j<height-1;j++)
                    for(int i=1;i<wide-1;i++)
                    {
                        if(*(p_temp+(height-j-1)*wide+i)==t)
                        {
                            pppp[t].pp_x=i;
                            pppp[t].pp_y=j;
                            pppp[t].pp_line=fm[t];
                            break;
                        }                
                    }
            }
        }//end if(stop!=1)
    }
}

//��24λ��ɫͼ��ת��Ϊ24λ�Ҷ�ͼ
void JisuanProcessDib::RgbToGray()
{
    BYTE *p_data;     //ԭͼ������ָ��
    int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
    p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();   //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
    DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
    for(int j=0;j<height;j++)    // ÿ��
        for(int i=0;i<DibWidth;i+=3)    // ÿ��
        {
            BYTE* pbyBlue = p_data++;   //�õ���ɫֵ
            BYTE* pbyGreen = p_data++;  //�õ���ɫֵ
            BYTE* pbyRed = p_data++;    //�� ����ɫֵ
            BYTE r = *pbyRed;
            BYTE g = *pbyGreen;
            BYTE b = *pbyBlue;
            int gray=0;
            gray=(30*r+59*g+11*b)/100;        
            *pbyBlue = gray;     
            *pbyGreen = gray;    
            *pbyRed = gray;            
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
