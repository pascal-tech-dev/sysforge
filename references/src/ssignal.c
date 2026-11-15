#include<signal.h>
#include<stdio.h>

void intr(int a) {
	printf("signal %d\n", a);
}

int main() {
	int inp;
	signal(SIGINT, intr);

	scanf("%d\n", &inp);

	return 0;
}
