#include "PTC_convertion.h"
#include "PTC_strOperateurs.h"

//Bibliotheque de fonction de convetion
// my_atoi -- convertie un str en un entier
int my_atoi(const char *str){
	int i;
	int negatif;
	int number;

	i = 0;
	number = 0;
	if (my_strcmp(str, "32 767") == 1){
		return (32767);
    }
	if (my_strcmp(str, "-32 767") == 1){
		return (-32767);
    }
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r'){
		i++;
    }
	negatif = (str[i] == '-') ? 1 : 0;
	if (str[i] == '-' || str[i] == '+'){
		i++;
    }
	while (str[i] >= '0' && str[i] <= '9'){
		number *= 10;
		number += (int)str[i] - '0';
		i++;
	}
	if (negatif == 1){
		number *= -1;
    }
	return (number);
}