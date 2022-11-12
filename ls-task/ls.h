#ifndef LS_H
#define LS_H
#include <iostream>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "ctype.h"
#include "dirent.h"
#include "pwd.h"
#include "math.h"
#include "string.h"
#include <map>
using namespace std;

map<int, string> names;

int to_right_size(unsigned long int size)
{
    static const int K_size = 1024;
    static const int M_size = 1024*1024;
    static const unsigned long int G_size = M_size*K_size;
    static const unsigned long int T_size = G_size*K_size;
    static const unsigned int K_ten_size=  10*K_size;
    static const unsigned int M_ten_size = 10*M_size;
    static const unsigned long int G_ten_size = 10*G_size;
    static const unsigned long int T_ten_size = 10*T_size;
    if (size < K_size)
    {
        printf("%ld", size);
    }
    else if (size>=K_size && size < K_ten_size)
    {
        int first_number = size/K_size;
        float n = size % K_size;
        n = n/K_size;
        n *= 10;
        int second_number = (int)n;
        printf("%d,%dK", first_number, second_number);
    }
    else if (size >= K_ten_size && size < M_size)
    {
        float number = (float)size/K_size;
        int right_number = ceil(number);
        printf("%dK", right_number);

    }
    else if (size >= M_size && size < M_ten_size)
    {
        int first_number = size/M_size;
        float n = (float)(size % M_size);
        n = n/M_size;
        n *= 10;
        int second_number = (int)n;
        printf("%d,%dM", first_number, second_number);

    }
    else if (size >= M_ten_size && size < G_size)
    {
        float number = (float)size/M_size;
        int right_number = ceil(number);
        printf("%dM", right_number);

    }
    else if (size >= G_size && size < G_ten_size)
    {
        int first_number = size/G_size;
        float n = size % G_size;
        n = n/G_size;
        n *= 10;
        int second_number = (int)n;
        printf("%d,%dG", first_number, second_number);

    }
    else if (size >= G_ten_size && size < T_size)
    {
        float number = (float)size/G_size;
        int right_number = ceil(number);
        printf("%dG", right_number);

    }
    else if (size >= T_size && size < T_ten_size)
    {
        int first_number = size/T_size;
        float n = size % T_size;
        n = n/M_size;
        n *= 10;
        int second_number = (int)n;
        printf("%d,%dT", first_number, second_number);

    }
    else if (size >= T_ten_size)
    {
        float number = (float)size/T_size;
        int right_number = ceil(number);
        printf("%dT", right_number);

    }
    return 0;

}

int compare_strings(string str1, string str2)
{
    int i=0;

    while (1)
    {
        if (str1[i] == '\0')
            return 0;
        if (str2[i] == '\0')
            return 1;
        if (tolower(str1[i]) < tolower(str2[i]))
            return 0;
        if (tolower(str1[i]) > tolower(str2[i]))
            return 1;
        i++;
    }
}

void sort_strings(vector<string> *strings, int count_strings)
{
    int swap;
    string tmp;
    for (int i =count_strings-1; i>=0;i--)
    {
        swap = 1;
        for (int j=0;j<i;j++)
        {

            if (compare_strings(strings->at(j), strings->at(j+1)) == 1)
            {
                tmp = strings->at(j);
                strings->at(j) = strings->at(j+1);
                strings->at(j+1) = tmp;
                swap = 0;

            }
        }
        if (swap == 1)
            break;
    }
}

void back_sorting(vector<string> *strings)
{
    reverse(strings->begin(), strings->end());
}

int get_name(unsigned long int id)
{
    struct passwd *ps;
    if (names.find(id) != names.end())
    {
        printf(" %s", names[id].c_str());
        return 0;
    }

    if (!(ps = getpwuid(id)))
    {
        printf(" shadow");
        return 0;
    }
    if (strcmp(ps->pw_passwd, "x"))
    {
        printf(" shadow");
    }
    else
    {
        names[id] = string(ps->pw_name);
        printf(" %s", ps->pw_name);
    }

    return 0;
}

int get_time(long int mod_time)
{
    char *months[12] = {"янв", "фев", "мар", "апр", "мая", "июн", "июл", "авг", "сен", "окт", "ноя", "дек"};
    struct tm *m_time;
    m_time = localtime(&mod_time);
    printf(" %s %d %d:", months[m_time->tm_mon], m_time->tm_mday, m_time->tm_hour);
    if ((int)m_time->tm_min/10 == 0)
        printf("0");
    printf("%d", m_time->tm_min);
    return 0;
}

int get_mode(unsigned long int mode)
{
    static const int modes[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    switch (mode & S_IFMT) {
        case S_IFBLK:  printf("b");            break;
        case S_IFCHR:  printf("c");        break;
        case S_IFDIR:  printf("d");               break;
        case S_IFIFO:  printf("p");               break;
        case S_IFLNK:  printf("l");                 break;
        case S_IFREG:  printf("-");            break;
        case S_IFSOCK: printf("s");                  break;
        default:       printf("?");                break;
    }
    for (int i=0;i<9;i++)
    {
        if (mode & modes[i])
            switch (i%3)
            {
                case (0):
                    printf("r");
                    break;
                case (1):
                    printf("w");
                    break;
                case (2):
                    printf("x");
                    break;
                default:
                    break;
            }
        else
            printf("-");
    }

    return 0;
}

#endif // LS_H
