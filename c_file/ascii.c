#include <stdio.h>
int main(){
printf("ASCII TABLE FOR ALPHABET\n");
for (int i = 0; i < 127; i++){
if (i < 32){
printf("\ndec : %d hex : %x - char : NON-READABLE", i, i);
}
else{
printf("\ndec : %d hex : %x - char : %c", i, i, i);
}
}
return 0;
}
