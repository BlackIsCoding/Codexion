#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
	int i = 0;
	int id = 1;
	struct timeval now;
	long time;
	long start;

	gettimeofday(&now, NULL);

	start = (now.tv_sec * 1000) + (now.tv_usec / 1000);

	while (i < 2)
	{
		gettimeofday(&now, NULL);
		time = (now.tv_sec * 1000) + (now.tv_usec / 1000);
		printf("%ld %d compiling\n", time - start, id);

		usleep(10000);

		gettimeofday(&now, NULL);
		time = (now.tv_sec * 1000) + (now.tv_usec / 1000);
		printf("%ld %d debugging\n", time - start, id);

		usleep(10000);

		gettimeofday(&now, NULL);
		time = (now.tv_sec * 1000) + (now.tv_usec / 1000);
		printf("%ld %d refactoring\n", time - start, id);

		usleep(10000);

		i++;
		id++;
	}
}