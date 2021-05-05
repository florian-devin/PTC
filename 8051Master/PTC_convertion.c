//------------------------------------------------------
// Date: <DATE>
// Heure: <TIME>
// Nom/Prénom: Devin florian
// Email: <florian.devin@cpe.fr>
// Nom: PTC_convertion.c
// Destination: 8150F020 Carte Master
// Description: Fonctions de convertion
//------------------------------------------------------

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

char	*my_itoa(const int n, char *dst) {
	long int	div;
	long int	nb;
	int			i;
	if (n == 0){
		dst[0] = '0';
		dst[1] = '\0';
		return (dst);
	}
	div = 1;
	i = 0;
	nb = (n < 0) ? -n : n;

	while (div <= nb)
		div *= 10;
	dst[(div % 10) + 1 + ((n < 0) ? 1 : 0)] = '\0';
	if (n < 0)
		dst[i++] = '-';
	if (n == 0)
		dst[i] = '0';
	while ((div = div / 10) != 0)
		dst[i++] = (((nb / div) % 10) + 48);
    dst[i++] = '\0';
	return (dst);
}