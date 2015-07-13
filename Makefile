all:
	gcc ./day1/pipe.c -o ./day1/pipe	
	gcc ./day2/threads.c -o ./day2/threads -lpthread
	gcc ./day3/threads.c -o ./day3/threads -lpthread
	gcc ./day4/bin_sem.c ./day4/sem1.c -o ./day4/sem1
	gcc ./day4/bin_sem.c ./day4/sem2.c -o ./day4/sem2
	gcc ./day4/raw_client.c -o ./day4/raw_client
	gcc ./day5/thrd_sign.c -o ./day5/thrd_sign -lpthread
