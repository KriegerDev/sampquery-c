all:
	gcc -D_SAMPQUERY_DEBUG_ tests/client.c sampquery.c -g -o bin/client