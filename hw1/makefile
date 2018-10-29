app.out: main.o foo.o bar.o # app.out 이라는 target 실행파일을 만들기 위해서는 main.o, foo.o, bar.o가 필요하다.
	gcc -o app.out main.o foo.o bar.o # app.out을 만들 때는 main.o, foo.o, bar.o를 컴파일하여 만든다.
main.o: foo.h bar.h main.c # main.o를 만들기 위해서는 foo.h, bar.h 헤더 파일들과 main.c가 필요하다.
	gcc -c -o main.o main.c # main.c를 이용해 main.o를 만들겠다.
foo.o: foo.h foo.c # foo.o를 만들기 위해서는 foo.h와 foo.c가 필요하다.
	gcc -c -o foo.o foo.c # foo.c를 이용해 foo.o를 만들겠다.
bar.o: bar.h bar.c # bar.o를 만들기 위해서는 bar.h와 bar.c가 필요하다.
	gcc -c -o bar.o bar.c # bar.c를 이용해 bar.c를 만들겠다.
