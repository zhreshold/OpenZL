#include "core.hpp"


using namespace zl;


int main()
{
	Mat_<uint> test;

	printf("%d\n", test.empty());

	test.create(10, 10, 3);
	printf("%d, %d, %d, %d\n", test.rows(), test.cols(), test.channels(), test.step());
	printf("%d\n", test.empty());
	test.dump();

	Mati test2(3, 3, 1);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			test2(i, j) = i * 3 + j;
		}
	}
	test2.dump();

	int key = zl::waitkey(100);
	printf("%d\n", key);
	zl::hold_screen();
	return 0;
}