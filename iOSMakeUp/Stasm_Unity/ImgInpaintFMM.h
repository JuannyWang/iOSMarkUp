#import <Foundation/Foundation.h>
#define input
#define print
using namespace std;

//像素点信息结构体
struct PrioInfor{
	int bflag;
	float TVvalue;
	float gx;
	float gy;
};

//待修补区域边界点链表
struct List_BoundPoints{
	int size_x;
	int size_y;
};

struct RGB_Data{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class ImgInpaintFMM{
public:
	ImgInpaintFMM(RGB_Data ** ImageData, int Height, int Width);
	virtual ~ImgInpaintFMM();
	void InitialPoint(); //初始化缺损区域信息（正向及逆向）
	void InpaintPoint(int x, int y); //修复方法
	//执行修复并及时更新T值
	void InpaintProcess(multimap<float, List_BoundPoints> &list_buffer, PrioInfor **DataMatrix);
	//计算正/逆向距离场T
	void DiffuseProcc(multimap<float, List_BoundPoints> &list_buffer, PrioInfor **DataMatrix, float throld);
	void solve(int, int, float, float, float&); //T值计算方法
	//得到初始时刻距离场T
	void ComputeDistance();
	//求最小值
	float min(float x, float y){return x<y?x:y;}
    
	float SQR(float x) {return x * x;}
	
private:
	int m_iCols;                  //图像的列
	int m_iRows;                  //图像的行
	PrioInfor **DataMatrixIn;     //保存信息完好区域相对于缺损区域的边界点
	PrioInfor **CopDataMatrixIn;  //缺损区域边界点备份
	PrioInfor **DataMatrixOut;    //保存缺损区域相对于信息完好区域的边界点
	PrioInfor **CopDataMatrixOut; //完好区域边界点备份
	RGB_Data  **ImgData;   //彩色图像
};
