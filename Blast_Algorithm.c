#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lineNo; //for finding the DNA position

char *line(FILE *fd) //for getting the line from file descriptor
{
    char *l1 = (char *)calloc(75, sizeof(char));
    char element = fgetc(fd);
    int i = 0;
    while (element != '\n')
    {
        l1[i] = element;
        i++;
        element = fgetc(fd);
    }
    return l1;
}

char *filter(char *line) //it filters the dna line, returns just letters without space, \n and numbers
{
    char *l = (char *)calloc(60, sizeof(char));
    char element;
    int counter = 0;
    for (int i = 0; i < 75; i++)
    {

        if (line[i] < 48 || line[i] > 57)
        {

            if (line[i] != ' ')
            {
                l[counter] = line[i];
                counter++;
            }
        }
    }
    return l;
}

void indexFinder(char *l) //finds the number on the lines and gives this value to global variable(for example, 56481)
{
    char *index = (char *)calloc(5, sizeof(char));
    int count = 0;

    for (int i = 0; i < 12; i++) //the part till the letters
    {
        if (l[i] > 47 && l[i] < 58) //for taking the numbers I use ascii 
        {
            strncat(index,&l[i],1); 
            if (l[i + 1] == ' ') //when I reach the space between number and letters, I stop
            {
                break;
            }
        }
    }
    lineNo = atoi(index);
}

int head_finder(FILE *fd) //finds the starting of the DNA data
{
    char *l = (char *)calloc(75, sizeof(char));
    while (1)
    {

        l = line(fd);
        for (int i = 0; i < 70; i++)
        {
            if (strstr(l, "ORIGIN"))
            {
                free(l);
                return 1;
            }
        }
    }
}

struct minmax
{
    int score;
    int position;
    int index;
};

struct minmax maxHeight(char *str,int index) //finds the score and local position and returns struct
{
    int x = 0, y = 0;
    int position;
    int nodeN = 0; //negative node
    int nodeP = 0; //positive node
    int score = 0;
    for (int i = 0; i < 22; i++)
    {
        x++;
        if (str[i] == '1')
        {
            y++;
        }
        else
        {
            y--;
        }
        if (nodeN > y) //when I reach the negative end point 
        {
            nodeN = y;
            position = x;
            nodeP = -22;
        }
        else //when it is above the negative end point
        {
            if (nodeP < y)
            {

                nodeP = y;
                if (nodeP - nodeN > score)
                {
                    score = nodeP - nodeN;
                }
            }
        }
    }
    struct minmax a;
    a.score = score;
    a.position = position;
    a.index=index;
    return a;
}

struct minmax check(char *line, char *search) //it simply creates 1 and 0s by checking dnas' matching and uses maxHeight function using this data
{
    char *temp = (char *)calloc(60, sizeof(char));
    char *r = (char *)calloc(22, sizeof(char));
    temp = filter(line);
    struct minmax result;
    result.score=0;
    int index;
    char letter;
    for (int i = 0; i < 39; i++)
    {
        index = 0;
        for (int j = i; j < i + 22; j++)
        {
            letter = search[index];
            if (temp[j] == letter)
            {
                r[index] = '1';
            }
            else
            {
                r[index] = '0';
            }
            index++;
        }
        struct minmax a;
        
        a=maxHeight(r,i);
        if(a.score>result.score){
            result=a;
        }
    }
    return result;
}

int main()
{
    FILE *fd = fopen("DNACervisiaeSaccharomyces.txt", "r");
    head_finder(fd);
    char s[] = "acacgctacaggcctataactt";
    struct minmax a;
    char *l = (char *)calloc(75, sizeof(char));
    char *temp= (char *)calloc(60, sizeof(char));
    int counter = 0, max = 0, pos,index;
    while (counter < 3836)
    {
        l = line(fd);
        a = check(l, s);
        if (a.score > max) //checks max score
        {
            indexFinder(l);
            max = a.score;
            pos = a.position;
            index=a.index;
        }
        counter++;
    }
    printf("DNA POSITION: %d  \nLOCAL POSITION: %d \nSCORE: %d",  lineNo+index-1,pos,max);
    free(l);
    free(temp);
    fclose(fd);
    return 0;
}