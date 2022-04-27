#include <iostream>
#include <fstream>
#pragma pack(1) //以1字节（8位）对齐
using namespace std;
struct tagBITMAPFILEHEADER {
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
};
struct tagBITMAPINFOHEADER {
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
};

int main(int argc, char* argv[]) {
	//argv[1]为读入的文件指针（char*型），argv[2]为生成的文件指针
	struct tagBITMAPFILEHEADER src_fileheader;
	struct tagBITMAPINFOHEADER src_infoheader;
	
	ifstream fin;
	fin.open(argv[1], ios::in | ios::binary);
	if (!fin) {
		cout << "Source file open error!" << endl;
	}
	fin.read((char*)&src_fileheader, 14);
	fin.read((char*)&src_infoheader, 40);
	unsigned int width = src_infoheader.biWidth;
	unsigned int height = src_infoheader.biHeight;
	char* old_bmp = new char[height * width * 3+1];
	char* tmp_char=new char[4];
	int end_bu = 0;//计算结尾补位
	if (width * 3 % 4) {
		end_bu = 4 - width * 3 % 4;
	}
	
	struct tagBITMAPFILEHEADER dest_fileheader= src_fileheader;
	struct tagBITMAPINFOHEADER dest_infoheader= src_infoheader;
	dest_infoheader.biWidth = height;
	dest_infoheader.biHeight = width;		
	int new_end_bu = 0;//计算新的结尾补位
	if (height * 3 % 4) {
		new_end_bu = 4 - height * 3 % 4;
	}
	dest_infoheader.biSizeImage = (height + new_end_bu) * width * 3;
	dest_fileheader.bfSize = dest_infoheader.biSizeImage + 54;//14+40
	
	//将图像数据区读入内存 (除去补位0 
	for (int i = 0; i < height; i++) {
		fin.read(old_bmp+i*width*3,width*3);			
		if(end_bu>0){//除去扫描行结尾自动补的0
			fin.read(tmp_char, end_bu);
		}
	}
	fin.close();//读取完毕，关闭文件

	ofstream fout;
	fout.open(argv[2], ios::out | ios::binary);
	if (!fout) {
		cout << "New file open error!" << endl;
	}
	fout.write((char*)&dest_fileheader, 14);
	fout.write((char*)&dest_infoheader, 40);
	char bu = 0;
	for (int i = 0; i < width; i++) {//旋转后的第i行(从下往上数)j列，是旋转前的第j行(width-i)列 
		for (int j = 0; j < height; j++) {
				fout.write((char*)&(old_bmp[(j * width + width - i - 1) * 3]), 3);
		}
		for (int j = 0; j < new_end_bu; j++) {//手动补位 
			fout.write(&bu, 1);
		}
	}
	fout.close();//写入完毕，关闭文件 
	return 0;
}
