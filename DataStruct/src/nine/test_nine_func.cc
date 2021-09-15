#include "nine.h" 

void printLine(int line) {
	for(int i = 1; i <= line; i++) {
		printf("%d x %d = %d, ", i, line, i * line);
	}
	printf("\n");
}
void
test_base() { 
	for(int i = 1; i < 10; i++) {
		printLine(i);
	}
}

void sort(int* nums, int size) {
	int min = nums[0], temp = 0;
	for(int i = 0; i < size; i++) {
		min = nums[i];
		for(int j = i + 1; j < size; j++) {
			if(nums[j] < min) {
				temp = min;
				min = nums[j];
				nums[j] = temp;
			}
		}
		if(nums[i] > min) {
			nums[i] = min;
		}
	}

	for(int i = 0; i < size; i++) {
		printf("%d, ", nums[i]);
	}
	std::cout << std::endl;
}
`
int
main(int argc, char** argv) {
	std::cout << "Hello, nine!" << std::endl;
	test_base();
	int nums[5] = { 5, 4, 3, 2, 1};
	sort(nums, 5);
	return 0; 
}


