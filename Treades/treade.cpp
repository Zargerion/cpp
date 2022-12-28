#include <iostream>
#include <windows.h>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

void Loop3d(int begin, int end)
{	
	int length = 9;
	double count = 1;
	mutex mtx;
	int c;
	 
	for (int i = begin; i != end; i++)
	{
		for (int j = 1; j < length; j++)
		{
			for (int z = 1; z < length; z++)
			{
				
				cout << i << " " << j << " " << z << endl;
				mtx.lock();
				count++;
				c = i * z * j;
				mtx.unlock();
				cout << count << endl;
				
				
			}
		}
	}
	cout << c << endl;
	
}


int getCpuNum() 
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return (int)si.dwNumberOfProcessors;
}

//void threader(void (&funk)(), int range) // Funk, range first loop.
//{
//	int NumCPU = getCpuNum();
//	int TreadeRange = range/NumCPU;
//
//	std::vector <std::thread> th_vec;
//	for (int i = 0, int first = 0, int last = first + TreadeRange; i < NumCPU; ++i, first + TreadeRange, last + TreadeRange)
//	{
//		std::thread th(*funk, first, last);
//		th_vec.push_back(th);
//	}
//	for (int i = 0; i < NumCPU; ++i)
//	{
//		th_vec.at(i).join();
//	}
//}

int main()
{
	cout << getCpuNum() << endl; // Take treade num.

	std::thread th1(Loop3d, 0, 3);
	std::thread th2(Loop3d, 3, 6);
	std::thread th3(Loop3d, 6, 9);

	//std::thread th1(Loop3d, 0, 3);//1
	//std::thread th2(Loop3d, 3, 6);//2
	//std::thread th3(Loop3d, 6, 9);//3
	//std::thread th4(Loop3d, 9, 12);//4
	//std::thread th5(Loop3d, 12, 15);//5
	//std::thread th6(Loop3d, 15, 18);//6
	//std::thread th7(Loop3d, 18, 21);//7
	//std::thread th8(Loop3d, 21, 24);//8
	//std::thread th9(Loop3d, 24, 27);//9
	//std::thread th10(Loop3d, 27, 30);//10
	//std::thread th11(Loop3d, 30, 33);//11
	//std::thread th12(Loop3d, 33, 36);//12

	th1.join();
	th2.join();
	th3.join();
	//th4.join();
	//th5.join();
	//th6.join();
	//th7.join();
	//th8.join();
	//th9.join();
	//th10.join();
	//th11.join();
	//th12.join();


}