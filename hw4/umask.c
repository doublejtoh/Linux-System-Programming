#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(){
	umask(0); //sets the calling process's file mode creation mask (umask) to (mask & 0777)
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // USR가 read, write 되고, Group이 read , write되고, others가 read, write할 수 있다. bitwise or operation. (옵션 키는거라고 생각하면됨) 

		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // GROUP Read, GROUP Write, OTHERS Read, OTHERS Write option에 대해서는 후에 chmod, creat할 때 변경할 수 없다. 무조건 끄겠다.
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0 ) {
		perror("creat");
		exit(1);
	}

}
