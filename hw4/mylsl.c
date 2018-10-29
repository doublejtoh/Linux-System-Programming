#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#define MAX_PATH 256
#define MAX_LENGTH 256


void printFileInfo(struct stat statbuf, char * d_name) {
    char file_kind; // directory 이냐 regular file이냐를 알려주는 char
    if (S_ISDIR(statbuf.st_mode)) {
        file_kind = 'd';
    } else {
    	file_kind = '-';
    }
     
    char * modeval = malloc(sizeof(char) * 9 + 1); // 미리 placehold.
    mode_t perm = statbuf.st_mode;
     
    modeval[0] = (perm & S_IRUSR) ? 'r': '-';
    modeval[1] = (perm & S_IWUSR) ? 'w': '-';
    modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[2] = (perm & S_ISUID) ? 'S' : modeval[2]; // UID인지 체크
	modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
	modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[6] = (perm & S_IROTH) ? 'r' : '-';
    modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[9] = '\0';
     
    struct passwd *pwd;
    struct group *g;
     
    pwd = getpwuid(statbuf.st_uid); // user name알아내기 위해서
    g = getgrgid(statbuf.st_gid); // group name 알아내기 위해서
   
       
    off_t file_size = statbuf.st_size; // 파일 사이즈
    nlink_t file_n_link = statbuf.st_nlink; //har link된 갯수
	time_t t = statbuf.st_mtime; //마지막으로 변경된 시각
	struct tm lt;
	localtime_r(&t, &lt); // local time으로 받아옴.
	char timebuf[80];
	strftime(timebuf, sizeof(timebuf), "%c", &lt); // local time을 string으로 바꿔줌.
	
	printf("%c%s  %d  %s  %s  %d  %s  %s\n", file_kind, modeval, file_n_link, pwd->pw_name, g->gr_name, file_size, timebuf, d_name); 
	
 }
 
 main() {
     DIR * dp;
     struct dirent * dep;
     struct stat statbuf;
     
     char fullpath[MAX_PATH];
     
     if ((dp = opendir(".")) == NULL){
         perror("opendir");
         exit(1);
     }
     
     while (dep = readdir(dp)) {
         if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
             continue;
         strcpy(fullpath, ".");
         strcat(fullpath, "/");
         strcat(fullpath, dep->d_name);
         if (lstat(fullpath, &statbuf) < 0 ) {
             perror("lstat");
             exit(1);
         }
    	 printFileInfo(statbuf, dep->d_name);  
     }
}
