#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include "../tools/generate.h"
#include "../lib/util.h"


// ./generate -l 6 -a -cl 1000 -t 4 -tl 10000 -ths 8

/*
Longitud: 6
Iteraciones: 5000
Longitud de tabla: 40000 
Numero de tablas: 4
*/


void print_usage() {
	printf("Usage: generate_rbt [--version]\n");
	printf("                    [--keylen key_length]\n");
	printf("                    [--min --may --num --special]\n");
	printf("                    [--chainlen chain_length]\n");
	printf("                    [--tablelen table_length]\n");
	printf("                    [--tables tables]\n");
	printf("                    [--threads threads]\n");
}

void print_version() {
	printf("rainbow-tables_1.0\n");
}



int main (argc, argv)
int argc;
char *argv[];
{
	char *ptr;

	int _vflag = 0;
	int _hflag = 0;
	int _kflag = 0;
	int _cflag = 0;
	int _lflag = 0;
	int _tflag = 0;	
	int _Tflag = 0;
	unsigned int _kvalue = 0;
	unsigned int _cvalue = 0;
	unsigned int _lvalue = 0;
	unsigned int _tvalue = 0;
	unsigned int _Tvalue = 0;
	unsigned int _charset[4] = {0, 0, 0, 0};	// {minuscules, mayuscules, numeric, special characters}


	int opt = 0;
	/* getopt_long stores the option index here. */
	int option_index = 0;

	static struct option long_options[] =
		{
			{"version",   no_argument,       0, 'v'},
			{"help",      no_argument,       0, 'h'},
			{"min",       no_argument,       0, 'a'},
			{"may",       no_argument,       0, 'A'},
			{"num",       no_argument,       0, '0'},
			{"special",   no_argument,       0, 's'},
			{"keylen",    required_argument, 0, 'k'},
			{"chainlen",  required_argument, 0, 'c'},
			{"tablelen",  required_argument, 0, 'l'},
			{"tables",    required_argument, 0, 't'},
			{"threads",   required_argument, 0, 'T'},
			{0, 0, 0, 0}
		};

		while((opt = getopt_long(argc, argv, ":vhaA0sk:c:l:t:T:", long_options, &option_index)) != -1){
     
		switch(opt){
 
			case 'v':
				_vflag = 1;
				break;

			case 'h':
				_hflag = 1;
				break;
     
			case 'a':
				_charset[MIN] = 1;
				break;

			case 'A':
				_charset[MAY] = 1;
				break;

			case '0':
				_charset[NUM] = 1;
				break;
     
			case 's':
				_charset[SPE] = 1;
				break;
     
			case 'k':
				_kflag = 1;
				_kvalue = strtoul(optarg, &ptr, 10);
				break;

			case 'c':
				_cflag = 1;
				_cvalue = strtoul(optarg, &ptr, 10);
				break;

			case 'l':
				_lflag = 1;
				_lvalue = strtoul(optarg, &ptr, 10);
				break;

			case 't':
				_tflag = 1;
				_tvalue = strtoul(optarg, &ptr, 10);
				break;

			case 'T':
				_Tflag = 1;
				_Tvalue = strtoul(optarg, &ptr, 10);
				break;

			case ':':   /* missing option argument */
				fprintf(stderr, "%s: option `-%c' requires an argument\n", argv[0], optopt);
				break;

			case '?':

			default:    /* invalid option */
				fprintf(stderr, "%s: option `-%c' is invalid: ignored\n", argv[0], optopt);
				print_usage();
				return 1;		
		}
	}

	if(_vflag){
		print_version();
		return 0;
	}

	if(_hflag){
		print_usage();
		return 0;
	}

	if(! (_charset[MIN] || _charset[MAY] || _charset[NUM] || _charset[SPE])){
		printf("Unless one of --min --may --num --spe is required\n");
		print_usage();
		return 1;
	}		

	if(! _tflag){
		printf("--tables is required\n");
		print_usage();
		return 1;
	}

	if(! _kflag){
		printf("--keylen is required\n");
		print_usage();
		return 1;
	}

	if(! _cflag){
		printf("--chainlen is required\n");
		print_usage();	
		return 1;
	}


	if(! _lflag){
		printf("--tablelen is required\n");
		print_usage();
		return 1;
	}


	if(! _Tflag){
		_Tvalue = 8;
	}

	/*
	printf("keyleng: %u, charset: %u, %u, %u, %u, chainlen: %u, tablelen: %u, tables: %u, threads: %u\n", 
													_kvalue, _charset[0], _charset[1], _charset[2], _charset[3],
													_cvalue, _lvalue, _tvalue, _Tvalue);
	*/


	init_rbt(_kvalue, &_charset, _cvalue, _lvalue, _tvalue, _Tvalue);
}
