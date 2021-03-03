
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

//remet str a rien
void RAZ_str(char *str){ 
	while(*str != '\0') {
		*str = '\0';
		str++;
    }
}