#include <stdio.h>
#include <unistd.h>
#include "record.h"

void print_record(Record * rd) {
	puts(rd->stud);
	puts(rd->num);
	puts(rd->dept);
}

main(int argc, char * argv[]) {
	FILE * fp;
	Record rec;
	
	if (argc != 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);	
	}

	if ((fp = fopen(argv[1], "r+b")) == NULL) { // reading과 writing을 동시에 하도록 하겠다. 단, 이미 해당파일이 존재한다면 truncate하지 않는다. 파일의 처음 위치에서부터 시작한다.
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");
	printf( "%lu\n", sizeof(rec)); // 45. (Record struct의 전체 크기)
	while(fread(&rec, sizeof(rec), 1, fp)) {
		print_record(&rec);
	}

	rewind(fp); // 해당 파일의 indicator를 맨처음으로 변경시켜줌.
	getchar();

	printf("-------Shuffled Record List (3, 6, 2, 4,1 ,5) -----\n");
	fseek(fp, sizeof(rec)*2L, SEEK_SET); // 2L: long int 2. 2개의 record를 건너 뛰겠다.
	fread(&rec, sizeof(rec), 1, fp); // 레코드 한개만 읽어오겠다.
	print_record(&rec);
	
	fseek(fp, sizeof(rec)*5L, SEEK_SET); // 파일의 처음에서부터 5개의 record 사이즈 만큼 file indicator를 변경시켜라.
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec,sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	getchar();

	printf("-----Updated Record List----\n");

	fseek(fp, sizeof(rec)*2L, SEEK_SET); // 2개의 record만큼 파일의 시작부터 indicator의 위치를 변경시키겠다.
	fread(&rec, sizeof(rec), 1, fp); // 1개의 record size만큼 1개의 record를 읽어오겠다.
	strcpy(rec.dept, "Movie"); // rec.dept를 "movie"로 변경시킨다.
	fseek(fp, sizeof(rec)*2L, SEEK_SET); // 다시 file indicator위치를 원상복귀 시키고,
	fwrite(&rec, sizeof(rec), 1, fp); // rec를 쓴다.

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	rewind(fp); // 파일의 indicator 위치를 맨 처음으로 둔다.
	while (fread(&rec, sizeof(rec), 1, fp)) {
		print_record(&rec);
	}

	fclose(fp);
}
