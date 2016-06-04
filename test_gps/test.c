#include <stdio.h>
#include <string.h>

#define BUFFSIZE 1000
#define NMEA_MAX_LENGTH 100
#define NUMBERS_OF_TRAME 2
#define NUMBERS_OF_TRAME_MAX 10

int main(int argc, char const *argv[])
{
	char trames[NUMBERS_OF_TRAME_MAX][NMEA_MAX_LENGTH];
	unsigned int oct, cpt, i;
	
	char c;
	int line = 0, column = 0, nbTrames = 0;

	// while dont have all trames
	while (nbTrames < NUMBERS_OF_TRAME) {

		if(column == 0) {
			do {
				scanf("%1s", &c);
			} while (c != '$');
		}
		else {
			scanf("%1s", &c);
		}

		trames[line][column++] = c;// or RXBUF0;
		
		// number of trames update
		if (c == 'x') {
			nbTrames++;
			line++;
			column = 0;
			printf("trame end\n");
		}
	}
	// print
	
	for (int i = 0; i < line; ++i)
	{
		printf("%d = %s\n", i, trames[i]);
	}

	memset(trames, 0, sizeof trames);

	printf("reset\n");

	for (int i = 0; i < line; ++i)
	{
		printf("%d = %s\n", i, trames[i]);
	}

	return 0;
}