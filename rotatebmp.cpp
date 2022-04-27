#include <iostream>
#include <fstream>
#pragma pack(1) //��1�ֽڣ�8λ������
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
	//argv[1]Ϊ������ļ�ָ�루char*�ͣ���argv[2]Ϊ���ɵ��ļ�ָ��
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
	int end_bu = 0;//�����β��λ
	if (width * 3 % 4) {
		end_bu = 4 - width * 3 % 4;
	}
	
	struct tagBITMAPFILEHEADER dest_fileheader= src_fileheader;
	struct tagBITMAPINFOHEADER dest_infoheader= src_infoheader;
	dest_infoheader.biWidth = height;
	dest_infoheader.biHeight = width;		
	int new_end_bu = 0;//�����µĽ�β��λ
	if (height * 3 % 4) {
		new_end_bu = 4 - height * 3 % 4;
	}
	dest_infoheader.biSizeImage = (height + new_end_bu) * width * 3;
	dest_fileheader.bfSize = dest_infoheader.biSizeImage + 54;//14+40
	
	//��ͼ�������������ڴ� (��ȥ��λ0 
	for (int i = 0; i < height; i++) {
		fin.read(old_bmp+i*width*3,width*3);			
		if(end_bu>0){//��ȥɨ���н�β�Զ�����0
			fin.read(tmp_char, end_bu);
		}
	}
	fin.close();//��ȡ��ϣ��ر��ļ�

	ofstream fout;
	fout.open(argv[2], ios::out | ios::binary);
	if (!fout) {
		cout << "New file open error!" << endl;
	}
	fout.write((char*)&dest_fileheader, 14);
	fout.write((char*)&dest_infoheader, 40);
	char bu = 0;
	for (int i = 0; i < width; i++) {//��ת��ĵ�i��(����������)j�У�����תǰ�ĵ�j��(width-i)�� 
		for (int j = 0; j < height; j++) {
				fout.write((char*)&(old_bmp[(j * width + width - i - 1) * 3]), 3);
		}
		for (int j = 0; j < new_end_bu; j++) {//�ֶ���λ 
			fout.write(&bu, 1);
		}
	}
	fout.close();//д����ϣ��ر��ļ� 
	return 0;
}
