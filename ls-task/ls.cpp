#include "ls.h"



int get_info(char *path, int mode, vector<string> strings, int num_size)
{
    string path_copy;

    int error;
    struct stat sb;
    long int mod_time;

    for (int i=0;i<num_size;i++)
    {
        //l mode
        if (mode & 4)
        {
            path_copy = string(path)+"/"+strings[i];

            if (lstat(path_copy.c_str(), &sb)==-1)
            {
                perror("lstat");
                exit(EXIT_FAILURE);
            }


            //Mode
            error = get_mode((intmax_t) sb.st_mode);
            if (error != 0)
            {
                perror("get_mode");
                exit(EXIT_FAILURE);
            }

            //Link count
            printf(" %ld", (uintmax_t) sb.st_nlink);

            //User Group name
            error = get_name((uintmax_t)sb.st_uid);
            if (error != 0)
            {
                perror("get_name");
                exit(EXIT_FAILURE);
            }
            error = get_name((uintmax_t)sb.st_gid);
            if (error != 0)
            {
                perror("get_name");
                exit(EXIT_FAILURE);
            }
            printf(" ");

            //Size
            //h mode
            if (mode & 2)
            {
                error = to_right_size((intmax_t) sb.st_size);
                if (error != 0)
                {
                    perror("to_right_size");
                    exit(EXIT_FAILURE);
                }
            }
            else
                printf("%ld", (intmax_t) sb.st_size);

            //Time
            mod_time = sb.st_mtime;
            error = get_time(mod_time);
            if (error != 0)
            {
                perror("get_time");
                exit(EXIT_FAILURE);
            }

            printf(" %s\n", strings[i].c_str());
        }
        else
        {
            printf("%s", strings[i].c_str());
            if (i!=num_size-1)
                printf("  ");
            else
                printf("\n");
        }

    }
    strings.clear();

    return 0;
}

int check_info(int mode, char *path)
{
    string file_name;
    DIR *d;
    struct dirent *de;

    vector<string> strings;
    int num_size = 0;
    int error;
    d = opendir(path);
    if (d == NULL) {
        fprintf(stderr, "Couldn't open directory\n");
        exit(1);
    }

    for (de = readdir(d); de != NULL; de = readdir(d)) {
            if (de->d_name[0] =='.')
                continue;

            strings.push_back(string(de->d_name));
            num_size++;

    }
    (void)closedir(d);
    sort_strings(&strings, num_size);
    //R mode
    if (mode & 1)
    {
        back_sorting(&strings);
    }

    //Get info
    error = get_info(path, mode, strings, num_size);
    if (error != 0)
    {
        perror("get_info");
        exit(EXIT_FAILURE);
    }


    return 0;
}



int main(int argc, char **argv)
{
    string file_name;
    int modes=0;
    int opt;
    char *dir = ".";
    extern int opterr;
    opterr = 0;

    if (argc>1)
    {
        while ((opt = getopt(argc, argv, "?lhr")) != -1)
        {
            switch (opt) {
            case 'l':
                modes = modes | 4;
                break;
            case 'h':
                modes = modes | 2;
                break;
            case 'r':
                modes = modes | 1;
                break;
            default:
                printf("Got invalid option: %c\n", optopt);
                break;

            }
        }
    }

    if (optind < argc)
    {
        dir = argv[optind];
    }

    if (check_info(modes, dir) <0)
    {
        exit(EXIT_FAILURE);
    }


    return 0;
}


