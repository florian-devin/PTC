
#include "PTC_strOperateurs.h"

//*****************************************************************************	 
//*****************************************************************************	 
// strcmp -- Compare deux chaine de char*
int my_strcmp(const char *s1, const char *s2) {
	while (*s1 && *s2) {
		if (*s1 == *s2) {
			s1++;
			s2++;
		}
		else {
			return 0;
        }
	}
	if (!(*s1) && (*s2)) {
		return 0;
    }
	if (!(*s2) && (*s1)){
		return 0;
    }
	return 1;
}

//*****************************************************************************	 
//*****************************************************************************	 
// strcat -- concatene deux chaine de char*
char *my_strcat(char *s1, const char *s2){
	int len;
	int idx;

	len = ft_strlen(s1);
	idx = 0;
	while (s2[idx])
	{
		s1[len + idx] = s2[idx];
		idx++;
	}
	s1[idx + len] = '\0';
	return (s1);
}

//*****************************************************************************	 
//*****************************************************************************	 
// strlen -- retourne la taille d'une chaine de caractere
size_t my_strlen(char const *str){
	int a;

	a = 0;
	while (*str++)
		a++;
	return (a);
}
//remet str a rien
void RAZ_str(char *str){ 
	while(*str != '\0') {
		*str = '\0';
		str++;
    }
}