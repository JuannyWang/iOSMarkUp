#import "ImgInpaintFMM.h"
//#define INFINITY 100000  ////∂®“Âº´¥Û÷µ
//#include <cmath>

//#include <android/log.h>
//#define LOG_TAG "Beauty Process"
//#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
//#ifdef DEBUG
//#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
//#else 
//#define LOGD(...)
//#endif


static int size=30;
List_BoundPoints FBoundPoint;
multimap<float, List_BoundPoints> Flist_buffer;
multimap<float, List_BoundPoints> CopFlist_buffer;
multimap<float, List_BoundPoints> Olist_buffer;
multimap<float, List_BoundPoints>::iterator Flist_index;

ImgInpaintFMM::ImgInpaintFMM(RGB_Data ** ImageData, int Height, int Width){
	ImgData = ImageData;
	m_iCols = Width;
	m_iRows = Height;
	InitialPoint();
	ComputeDistance();
	InpaintProcess(CopFlist_buffer, CopDataMatrixIn);
    NSLog(@"4 in ImgInpaintFMM m_iCols,m_iRows:%d,%d",m_iCols,m_iRows);
	//LOGE("4 in ImgInpaintFMM m_iCols,m_iRows:%d,%d \n" + m_iCols,m_iRows);
}

//释放内存空间
ImgInpaintFMM::~ImgInpaintFMM(){
	for(int i = 0; i < m_iRows; i++){
		if (DataMatrixIn[i] != NULL) delete[] DataMatrixIn[i];
		DataMatrixIn[i] = NULL;

		if (CopDataMatrixIn[i] != NULL) delete[] CopDataMatrixIn[i];
		CopDataMatrixIn[i] = NULL;

		if (DataMatrixOut[i] != NULL) delete[] DataMatrixOut[i];
		DataMatrixOut[i] = NULL;

		if (CopDataMatrixOut[i] != NULL) delete[] CopDataMatrixOut[i];
		CopDataMatrixOut[i] = NULL;
	}

	if (DataMatrixIn != NULL){
		delete DataMatrixIn;
		DataMatrixIn = NULL;
	}

	if (CopDataMatrixIn != NULL){
		delete CopDataMatrixIn;
		CopDataMatrixIn = NULL;
	}

	if (DataMatrixOut != NULL){
		delete DataMatrixOut;
		DataMatrixOut = NULL;
	}

	if (CopDataMatrixOut != NULL){
		delete CopDataMatrixOut;
		CopDataMatrixOut = NULL;
	}

	Flist_buffer.clear();
	CopFlist_buffer.clear();
	Olist_buffer.clear();
}

/**
 **初始化缺损区域信息（正向及逆向）
 **/
 void ImgInpaintFMM::InitialPoint(){
	int i;
	//像素点信息数据存放空间申请
	DataMatrixIn =  new PrioInfor*[m_iRows];
	CopDataMatrixIn =  new PrioInfor*[m_iRows];
	DataMatrixOut =  new PrioInfor*[m_iRows];
	CopDataMatrixOut =  new PrioInfor*[m_iRows];

	for(i = 0; i < m_iRows; i++){
		DataMatrixIn[i] =  new PrioInfor[m_iCols];
		CopDataMatrixIn[i] =  new PrioInfor[m_iCols];
		DataMatrixOut[i] =  new PrioInfor[m_iCols];
		CopDataMatrixOut[i] =  new PrioInfor[m_iCols];
	}

	for(int i = 0; i < m_iRows; i++){
		for(int j = 0; j < m_iCols; j++){
			DataMatrixIn[i][j].gx = 0;
			DataMatrixIn[i][j].gy = 0;

			CopDataMatrixIn[i][j].gx = 0;
			CopDataMatrixIn[i][j].gy = 0;

			DataMatrixOut[i][j].gx = 0;
			DataMatrixOut[i][j].gy = 0;

			CopDataMatrixOut[i][j].gx = 0;
			CopDataMatrixOut[i][j].gy = 0;

			if ((ImgData[i][j].b + ImgData[i][j].g + ImgData[i][j].r)/3 != 255){
				if (i>size && i < (m_iRows-size) && j>size && j<(m_iCols-size)){
					if ((ImgData[i-1][j].b + ImgData[i-1][j].g + ImgData[i-1][j].r)/3 == 255 ||
						(ImgData[i+1][j].b + ImgData[i+1][j].g + ImgData[i+1][j].r)/3 == 255 ||
						(ImgData[i][j-1].b + ImgData[i][j-1].g + ImgData[i][j-1].r)/3 == 255 ||
						(ImgData[i][j+1].b + ImgData[i][j+1].g + ImgData[i][j+1].r)/3 == 255){
                        DataMatrixIn[i][j].bflag = 0;       //去边界值
                        DataMatrixIn[i][j].TVvalue = 1;     //T值取1
                        CopDataMatrixIn[i][j].bflag = 0;    //备份：边界点
                        CopDataMatrixIn[i][j].TVvalue = 1;  //T值取1
                        //插入待修复边界点链表
							FBoundPoint.size_x = i;
							FBoundPoint.size_y = j;
							Flist_buffer.insert(pair<float, List_BoundPoints>(DataMatrixIn[i][j].TVvalue, FBoundPoint));
							CopFlist_buffer.insert(pair<float, List_BoundPoints>(CopDataMatrixIn[i][j].TVvalue, FBoundPoint));
					}else{
							DataMatrixIn[i][j].bflag = 1;      
							DataMatrixIn[i][j].TVvalue = 0;    
							CopDataMatrixIn[i][j].bflag = 1;  
							CopDataMatrixIn[i][j].TVvalue = 0;
					}
				}else{
					//图像边缘上点标志为2，T取一个极值
					DataMatrixIn[i][j].bflag = 2;      
					DataMatrixIn[i][j].TVvalue = INFINITY;    
					CopDataMatrixIn[i][j].bflag = 2;  
					CopDataMatrixIn[i][j].TVvalue = INFINITY;
				}
			}else{
				if (i > size && i < (m_iRows - size) && j > size && j < (m_iCols - size)){
					//跳过图像边界，代修补点标志为-1，T取值为极值
					DataMatrixIn[i][j].bflag = -1;      
					DataMatrixIn[i][j].TVvalue = INFINITY;    
					CopDataMatrixIn[i][j].bflag = -1;  
					CopDataMatrixIn[i][j].TVvalue = INFINITY;
				}else{
					//图像边缘上点标志为2，T取一个极值
					DataMatrixIn[i][j].bflag = 2;      
					DataMatrixIn[i][j].TVvalue = INFINITY;    
					CopDataMatrixIn[i][j].bflag = 2;  
					CopDataMatrixIn[i][j].TVvalue = INFINITY;
				}
			}

			//保存信息缺损区域相对于信息完好区域的边界点，遇上边的处理正好相反
			if ((ImgData[i][j].b + ImgData[i][j].r + ImgData[i][j].r)/3 == 255){
				if (i > size && i < (m_iRows - size) && j > size && j < (m_iCols - size)){
					if ((ImgData[i-1][j].b + ImgData[i-1][j].r + ImgData[i-1][j].r)/3 != 255 ||
						(ImgData[i+1][j].b + ImgData[i+1][j].r + ImgData[i+1][j].r)/3 != 255 ||
						(ImgData[i][j-1].b + ImgData[i][j-1].r + ImgData[i][j-1].r)/3 != 255 ||
						(ImgData[i][j+1].b + ImgData[i][j+1].r + ImgData[i][j+1].r)/3 != 255){
						DataMatrixOut[i][j].bflag = 0;      //逆向的边界点的值
						DataMatrixOut[i][j].TVvalue = 1;    //T取值1
						CopDataMatrixOut[i][j].bflag = 0;		//(备份)逆向的边界点的值
						CopDataMatrixOut[i][j].TVvalue = 1;    //T取值1
						//插入信息完区域边界点链表
						FBoundPoint.size_x = i;
						FBoundPoint.size_y = j;
						Olist_buffer.insert(pair<float, List_BoundPoints>(DataMatrixOut[i][j].TVvalue, FBoundPoint));
					}else{
						DataMatrixOut[i][j].bflag = 1;      //逆向的边界点的值
						DataMatrixOut[i][j].TVvalue = 0;    //T取值0
						CopDataMatrixOut[i][j].bflag = 1;	//(备份)逆向的边界点的值
						CopDataMatrixOut[i][j].TVvalue = 0;  //T取值0
					}
				}else{
					//图像边缘上点标识为2，T取一个极值
					DataMatrixOut[i][j].bflag = 2;				//逆向的边界点的值
					DataMatrixOut[i][j].TVvalue = INFINITY;     //T取值INFINITY
					CopDataMatrixOut[i][j].bflag = 2;			//(备份)逆向的边界点的值
					CopDataMatrixOut[i][j].TVvalue = INFINITY;   //T取值INFINITY
                }
			}else{
				//œÒÀÿ÷µµ»”⁄255µƒµ„ø¥◊ˆ ««¯”Úƒ⁄µƒµ„
				if (i > size && i < (m_iRows-size) && j> size && j < (m_iCols-size)){
					//Ã¯π˝ÕºœÒ±ﬂΩÁ£ª“—÷™µ„±Í÷æŒª-1£¨T÷µ»°Œ™º´÷µ
					DataMatrixOut[i][j].bflag = -1;				//ƒÊœÚµƒ±ﬂΩÁµ„µƒ÷µ
					DataMatrixOut[i][j].TVvalue = INFINITY;     //T»°÷µINFINITY
					CopDataMatrixOut[i][j].bflag = -1;			//(±∏∑›)ƒÊœÚµƒ±ﬂΩÁµ„µƒ÷µ
					CopDataMatrixOut[i][j].TVvalue = INFINITY;   //T»°÷µINFINITY
				}else{
                    //像素值等于255的点看做是区域内的点
                    if (i > size && i < (m_iRows-size) && j> size && j < (m_iCols-size)){
                        //跳过图像边界；已知点标志位-1，T值取为极值
                        DataMatrixOut[i][j].bflag = -1;				//逆向的边界点的值
                        DataMatrixOut[i][j].TVvalue = INFINITY;     //T取值INFINITY
                        CopDataMatrixOut[i][j].bflag = -1;			//(备份)逆向的边界点的值
                        CopDataMatrixOut[i][j].TVvalue = INFINITY;   //T取值INFINITY
                    }else{
                        //图像边缘上点标志位2，T值取为极值
                        DataMatrixOut[i][j].bflag = 2;				//逆向的边界点的值
                        DataMatrixOut[i][j].TVvalue = INFINITY;     //T取值INFINITY
                        CopDataMatrixOut[i][j].bflag = 2;			//(备份)逆向的边界点的值
                        CopDataMatrixOut[i][j].TVvalue = INFINITY;   //T取值INFINITY
                    }
				}
			}
		}
	}
}


/**
 **计算正/逆向距离场T
 **/
void ImgInpaintFMM::DiffuseProcc(multimap<float, List_BoundPoints> &list_buffer, PrioInfor **DataMatrix, float throld){
	//计算距离场T
	int min_i, min_j;
	while(list_buffer.size()){
		Flist_index = list_buffer.begin(); //按照T值由小到大的顺序推进边界
		min_i = Flist_index->second.size_x;
		min_j = Flist_index->second.size_y;
		DataMatrix[min_i][min_j].bflag = 1; //边界点变为已知点并从链表中檫除
		list_buffer.erase(Flist_index);
		if (DataMatrix[min_i][min_j].TVvalue >= throld){
			//T值不能大于制定的阀值
			continue;
		}
        
		//更新边界点，找出当前点四领域内的未知点作为新的边界点，并及时更新T值
		int nn = 0;
		if (DataMatrix[min_i -1][min_j].bflag == -1){
			//内部点
			int i = min_i -1, j=min_j;
			nn++;
			float vi_lj = DataMatrix[i-1][j].TVvalue;
			float vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue;
			float vij_l = DataMatrix[i][j-1].TVvalue;
			//点类别
			int fi_lj = DataMatrix[i-1][j].bflag;
			int fijxl = DataMatrix[i][j+1].bflag;
			int fixlj = DataMatrix[i+1][j].bflag;
			int fij_l = DataMatrix[i][j-1].bflag;
			//通过当前未知点四领域的信息更新其T值
			float sol = INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				//保存满足条件的边界点
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				list_buffer.insert(pair<float,List_BoundPoints>(sol, FBoundPoint));
			}
		}
		if ( DataMatrix[min_i+1][min_j].bflag == -1){
			int i = min_i + 1, j = min_j;
			nn++;
			//T值
			float vi_lj = DataMatrix[i-1][j].TVvalue;
			float vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue;
			float vij_l = DataMatrix[i][j-1].TVvalue;
			//点类别
			int fi_lj = DataMatrix[i-1][j].bflag;
			int fijxl = DataMatrix[i][j+1].bflag;
			int fixlj = DataMatrix[i+1][j].bflag;
			int fij_l = DataMatrix[i][j-1].bflag;
			float sol = INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				list_buffer.insert(pair<float,List_BoundPoints>(sol, FBoundPoint));
			}
		}
		if ( DataMatrix[min_i][min_j-1].bflag == -1){
			int i = min_i, j = min_j-1;
			nn++;
			//T值
			float vi_lj = DataMatrix[i-1][j].TVvalue;
			float vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue;
			float vij_l = DataMatrix[i][j-1].TVvalue;
			//点类别
			int fi_lj = DataMatrix[i-1][j].bflag;
			int fijxl = DataMatrix[i][j+1].bflag;
			int fixlj = DataMatrix[i+1][j].bflag;
			int fij_l = DataMatrix[i][j-1].bflag;
			float sol = INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				list_buffer.insert(pair<float,List_BoundPoints>(sol, FBoundPoint));
			}
		}
		if ( DataMatrix[min_i][min_j+1].bflag == -1){
			int i = min_i, j = min_j+1;
			nn++;
			//T值
			float vi_lj = DataMatrix[i-1][j].TVvalue;
			float vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue;
			float vij_l = DataMatrix[i][j-1].TVvalue;
			//点类别
			int fi_lj = DataMatrix[i-1][j].bflag;
			int fijxl = DataMatrix[i][j+1].bflag;
			int fixlj = DataMatrix[i+1][j].bflag;
			int fij_l = DataMatrix[i][j-1].bflag;
			float sol = INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				list_buffer.insert(pair<float,List_BoundPoints>(sol, FBoundPoint));
			}
		}

		//四领域内五缺损点（缺损点已处理）
		if (!nn){
			DataMatrix[min_i][min_j].bflag = 2;
			continue;
		}
	}
}


/**
 **T值计算方法
 **/
void ImgInpaintFMM::solve(int fi_lj, int fij_l, float vi_lj, float vij_l, float& sol){
	float ss,d,r;
	//更新 T 值
	if (fi_lj == 1 || fi_lj == 2)
    {
        if(fij_l == 1 || fij_l == 2)
        {
			d = 2 - SQR(vi_lj-vij_l);
			if (d >= 0)
            {
				r = sqrt(d);
				ss = ((vi_lj + vij_l))/2;
				if (ss >= vi_lj && ss >= vij_l)
                {
					sol = min(sol,ss);
				}
                else
                {
					ss += r;
					if (ss >= vi_lj && ss >= vij_l) sol = min(sol, ss);
				}
			}
            else
            {
				return;
			}
		}
        else
        {
            if (fij_l == 1 || fij_l == 2)
                sol = min(sol, 1 + vij_l);
		}
    }
}

/**
 **得到初始时刻距离场T
 **/
void ImgInpaintFMM::ComputeDistance(){
	int i,j;
	float thld = pow(10.0,6);
	//正向推进--通过信息完好区域计算信息缺损区域T值
	DiffuseProcc(Flist_buffer, DataMatrixIn,thld);
	//逆向推进--通过信息完好区域计算信息完好区域T值
	DiffuseProcc(Olist_buffer, DataMatrixOut,10);
	for(i = size + 1; i < m_iRows - size; i++){
		for(int j = size + 1; j < m_iCols - size; j++){
			//用逆向推进后所得T值更新初始信息完好区域
			//相对于信息缺损区域的边界点的T值
			if (CopDataMatrixIn[i][j].bflag == 1)
				DataMatrixIn[i][j].TVvalue = -DataMatrixOut[i][j].TVvalue;
			if (CopDataMatrixOut[i][j].bflag == -1)
				DataMatrixIn[i][j].TVvalue = 0;
		}
	}
	for(i = size + 1; i < m_iRows - size; i++){
		for(j = size + 1; j < m_iCols - size; j++){
			DataMatrixIn[i][j].gx = DataMatrixIn[i+1][j].TVvalue - DataMatrixIn[i][j].TVvalue;
			DataMatrixIn[i][j].gy = DataMatrixIn[i][j+1].TVvalue - DataMatrixIn[i][j].TVvalue;
			float r = sqrt(DataMatrixIn[i][j].gx*DataMatrixIn[i][j].gx + DataMatrixIn[i][j].gy*DataMatrixIn[i][j].gy);
			if (r > 0.00001){
				DataMatrixIn[i][j].gx /=r;
				DataMatrixIn[i][j].gy /=r;
			}
		}
	}
}


/**
 **彩色图像修复
 **/
void ImgInpaintFMM::InpaintPoint(int i, int j){
	float gx_r=0,gy_r=0,gx_g=0,gy_g=0,gx_b=0,gy_b=0,im_r=0,im_g=0,im_b=0;
	int ii,jj;
	float cnt = 0, cntx = 0, cnty = 0, r;
	float dst0 = DataMatrixIn[i][j].TVvalue;
	int N = 10;
	
	int count = 0;
	for(ii = -N; ii <= N; ii++){
		for(jj = -N; jj <= N; jj++){
			//跳过已知的点
			if (CopDataMatrixIn[i+ii][j+jj].bflag != 1) continue;
			//跳过当前点
			if (ii == 0 && jj ==0) continue;
			float dirx = -ii;
			float diry = -jj;
			//得到未知点到当前边界点的距离
			float dd = sqrt(dirx*dirx + diry*diry);
			if (dd > N) continue;
			//单位化x、y方向因子
			float ndirx = dirx/dd, ndiry = diry/dd;
			float dst = DataMatrixIn[i+ii][j+jj].TVvalue;
			//公式--方向因子
			r = ndirx * DataMatrixIn[i][j].gx + ndiry*DataMatrixIn[i][j].gy;
			r = fabs(r);
			r /= dd*dd;
			r /= (1 + (dst - dst0) * (dst - dst0));
			//离因子
			//计算加权函数乘上像素值的和
			im_r += r*ImgData[i+ii][j+jj].r;
			im_g += r*ImgData[i+ii][j+jj].g;
			im_b += r*ImgData[i+ii][j+jj].b;
			//求和加权函数
			cnt+=r;
            
			count++;
			//计算梯度与加权函数及x方向的乘积
			if (CopDataMatrixIn[i+ii+1][j+jj].bflag != -1 &&
				CopDataMatrixIn[i+ii-1][j+jj].bflag != -1){
                gx_r += dirx*r*(ImgData[i+ii+1][j+jj].r - ImgData[i+ii-1][j+jj].r);
                gx_g += dirx*r*(ImgData[i+ii+1][j+jj].g - ImgData[i+ii-1][j+jj].g);
                gx_b += dirx*r*(ImgData[i+ii+1][j+jj].b - ImgData[i+ii-1][j+jj].b);
                cntx += r;
			}
            
			//计算梯度与加权函数及y方向的乘积
			if (CopDataMatrixIn[i+ii][j+jj+1].bflag != -1 &&
				CopDataMatrixIn[i+ii][j+jj-1].bflag != -1){
                gy_r += diry*r*(ImgData[i+ii][j+jj+1].r - ImgData[i+ii][j+jj-1].r);
                gy_g += diry*r*(ImgData[i+ii][j+jj+1].g - ImgData[i+ii][j+jj-1].g);
                gy_b += diry*r*(ImgData[i+ii][j+jj+1].b - ImgData[i+ii][j+jj-1].b);
                cnty += r;
			}
		}
	}
	
	//printf("======================count: %d \n", count);
    
	float c_r=0,c_g=0,c_b=0;
	//特殊情况处理
	if (cnt == 0 || cntx == 0 || cnty == 0){
		im_r = im_g = im_b = 0;
		cnt = 0;
		for(ii = i - 2; ii <= i + 2; ii++)
			for(jj = j - 2; jj <= j + 2; jj++)
				if(CopDataMatrixIn[ii][jj].bflag != -1){
					im_r += ImgData[ii][jj].r;
					im_g += ImgData[ii][jj].g;
					im_b += ImgData[ii][jj].b;
					cnt++;
				}
	}else{
		//单位化
		gx_r /= cntx; gy_r /= cnty;
		gx_g /= cntx; gy_g /= cnty;
		gx_b /= cntx; gy_b /= cnty;
		r = sqrt(gx_r*gx_r + gy_r*gy_r);
		if (r > 0.00001){
			gx_r /= r;
			gy_r /= r;
		}
        
		r = sqrt(gx_g*gx_g + gy_g*gy_g);
		if (r > 0.00001){
			gx_g /= r;
			gy_g /= r;
		}
        
		r = sqrt(gx_b*gx_b + gy_b*gy_b);
		if (r > 0.00001){
			gx_b /= r;
			gy_b /= r;
		}
        
		c_r = gx_r + gy_r;
		c_g = gx_g + gy_g;
		c_b = gx_b + gx_b;
	}
	ImgData[i][j].r = im_r / cnt + c_r;
	ImgData[i][j].g = im_g / cnt + c_g;
	ImgData[i][j].b = im_b / cnt + c_b;
}

/**
 **执行修复并及时更新T值
 **/
void ImgInpaintFMM::InpaintProcess(multimap<float, List_BoundPoints> &list_buffer, PrioInfor **DataMatrix){
	//根据已经初始化处理后的T值进行修复图像，并同时更新T值
	while(list_buffer.size()){
		int min_i, min_j;
		//优先处理边界点链表中T值最小的边界点的四邻域
		Flist_index = list_buffer.begin();
		min_i = Flist_index ->second.size_x;
		min_j = Flist_index ->second.size_y;
		//LOGE("y,x--%d,%d \n",min_i, min_j);
		//更新边界点标记为已知点并从边界链表中删除
		DataMatrix[min_i][min_j].bflag = 1;
		list_buffer.erase(Flist_index);
		//查找当前点四邻域内的未知点执行修复并更新相应的T值
		//进而根据T值大小插入边界点链表中
		if (DataMatrix[min_i-1][min_j].bflag == -1){ //上
			int i = min_i - 1, j = min_j;
			float vi_lj = DataMatrix[i-1][j].TVvalue,
            vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue,
            vij_l = DataMatrix[i][j-1].TVvalue;
			int fi_lj = DataMatrix[i-1][j].bflag,
            fijxl = DataMatrix[i][j+1].bflag;
			int	fixlj = DataMatrix[i+1][j].bflag,
            fij_l = DataMatrix[i][j-1].bflag;
			//更新相应T值并根据T值大小插入边界点链表中
			float sol =INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				//根据T值大小插入边界点链表中
				list_buffer.insert(pair<float, List_BoundPoints>(sol,FBoundPoint));
			}
			//对未知点执行修复
			InpaintPoint(i, j);
		}
        
		if (DataMatrix[min_i+1][min_j].bflag == -1){ //下
			int i = min_i + 1, j = min_j;
			float vi_lj = DataMatrix[i-1][j].TVvalue,
            vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue,
            vij_l = DataMatrix[i][j-1].TVvalue;
			int fi_lj = DataMatrix[i-1][j].bflag,
            fijxl = DataMatrix[i][j+1].bflag;
			int	fixlj = DataMatrix[i+1][j].bflag,
            fij_l = DataMatrix[i][j-1].bflag;
			//更新相应T值并根据T值大小插入边界点链表中
			float sol =INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				//根据T值大小插入边界点链表中
				list_buffer.insert(pair<float, List_BoundPoints>(sol,FBoundPoint));
			}
			//对未知点执行修复
			InpaintPoint(i, j);
		}
        
		if (DataMatrix[min_i][min_j-1].bflag == -1){ //左
			int i = min_i, j = min_j-1;
			float vi_lj = DataMatrix[i-1][j].TVvalue,
            vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue,
            vij_l = DataMatrix[i][j-1].TVvalue;
			int fi_lj = DataMatrix[i-1][j].bflag,
            fijxl = DataMatrix[i][j+1].bflag;
			int	fixlj = DataMatrix[i+1][j].bflag,
            fij_l = DataMatrix[i][j-1].bflag;
			//更新相应T值并根据T值大小插入边界点链表中
			float sol =INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				//根据T值大小插入边界点链表中
				list_buffer.insert(pair<float, List_BoundPoints>(sol,FBoundPoint));
			}
			//对未知点执行修复
			InpaintPoint(i, j);
		}
        
		if (DataMatrix[min_i][min_j+1].bflag == -1){ //右
			int i = min_i, j = min_j+1;
			float vi_lj = DataMatrix[i-1][j].TVvalue,
            vijxl = DataMatrix[i][j+1].TVvalue;
			float vixlj = DataMatrix[i+1][j].TVvalue,
            vij_l = DataMatrix[i][j-1].TVvalue;
			int fi_lj = DataMatrix[i-1][j].bflag,
            fijxl = DataMatrix[i][j+1].bflag;
			int	fixlj = DataMatrix[i+1][j].bflag,
            fij_l = DataMatrix[i][j-1].bflag;
			//更新相应T值并根据T值大小插入边界点链表中
			float sol =INFINITY;
			solve(fi_lj, fij_l, vi_lj, vij_l, sol);
			solve(fixlj, fij_l, vixlj, vij_l, sol);
			solve(fi_lj, fijxl, vi_lj, vijxl, sol);
			solve(fixlj, fijxl, vixlj, vijxl, sol);
			if (sol < INFINITY/2){
				DataMatrix[i][j].bflag = 0;
				DataMatrix[i][j].TVvalue = sol;
				FBoundPoint.size_x = i;
				FBoundPoint.size_y = j;
				//根据T值大小插入边界点链表中
				list_buffer.insert(pair<float, List_BoundPoints>(sol,FBoundPoint));
			}
			//对未知点执行修复
			InpaintPoint(i, j);
		}
	}
}
