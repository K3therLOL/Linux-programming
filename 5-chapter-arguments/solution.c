#include <stdio.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    struct option long_options[] = { 
        {"query", required_argument, NULL, 'q'},
        {"longinformationrequest", no_argument, NULL, 'i'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    extern int opterr, optind;
    opterr = 0;

    int opchar;
    char sign = '+';
    int option_index = 0;
    while ((opchar = getopt_long(argc, argv, "q:iv", long_options, &option_index)) != -1) {
        if (opchar == '?') {
            sign = '-';
            break;
        }
    }
    if (optind != 1 && optind < argc) {
        sign = '-';
    }

    printf("%c", sign);
}
