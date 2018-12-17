#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <time.h>


int i;
struct football//структура команды
{
    int number;//порядковый номер, для рандома
    char name[25];//название
    int score;//количество очков в последнем туре
    int sum_score;//количество очков за все проведенные туры
};
/*
 struct tour
 {//структура матча
 struct football team_1;//команда 1
 struct football team_2;//команда 2
 };*/

struct Score
{
    int team1;
    int team2;
    int pr;
};

struct Score** tabl;

//int rand();
void tur(struct football[], int);//функция подбора соперников на матч
void output(struct football[], int);//вывод итогово турнирной таблицы
void sort_sum_score(struct football[], int);//сортировка по количеству очков за туры
void output_tur(struct Score**, struct football[], int);//вывод результатов тура
void input_score(struct football*, struct football*, struct Score*);//ввод очков за матч
void output_file(struct Score**, struct football* , int );

int main()
{
    int count_tur = 0;
    int a, j = 0, num;
    struct football* turnir = (struct football*) malloc(1 * sizeof(struct football));//массив команд
    FILE *file;
    
    
    
    file = fopen("/Users/art.fek/Desktop/kursach_final/kursach_final/text.txt", "r");
    if (file != NULL)
    {
        fscanf(file, "%i", &num);
        turnir = (struct football*) realloc(turnir, num * sizeof(struct football));
        while(!feof(file))//ввод названий
        {
            fscanf(file, "%s", &turnir[j].name);
            turnir[j].sum_score = 0;
            //fflush(file);
            j++;
        }
    }
    fclose(file);
    if (j % 2 || j == 0)
    {
        printf("Kolichestvo komand nechetnoe!!!");
        system("pause");
        return 1;
    }
    
    tabl = (struct Score**)malloc(num * sizeof(struct Score*));
    for (i = 0; i < num; i++) {
        tabl[i] = (struct Score*)malloc(num * sizeof(struct Score));
    }
    for (i = 0; i < num; i++)
        for (j = 0; j < num; j++)
        {
            tabl[i][j].team1 = 0;
            tabl[i][j].team2 = 0;
            tabl[i][j].pr = 0;
        }
    
    /*printf("\tVvedite %i komand\n\n", num);
     for (i = 0; i<num; i++)//ввод названий
     {
     printf("komanda %i: ", i + 1);
     scanf("%s", &turnir[i].name);
     turnir[i].number = i + 1;
     turnir[i].sum_score = 0;
     }*/
    
    do{
        system("cls");
        printf("\n Viberite deistvie:\n\n\n");
        printf("1. Provedenie tura\n\n");
        printf("2. Vivod sceta komand\n\n");
        printf("3. Vivod resul'tata igr\n\n");
        printf("4. Exit\n\n\n");
        scanf("%i", &a);
        
        switch (a)
        {
            case 1:
                if (count_tur < num - 1)
                {
                    printf("Tur!\n\n");
                    tur(turnir, num);
                    count_tur++;
                }
                else
                    printf("Vse turi provedeni!\n");
                break;
            case 2:
                printf("Vivod komand\n\n");
                output(turnir, num);
                break;
            case 3:
                output_tur(tabl, turnir, num);
                break;
            case 4:
                if (count_tur < num - 1)
                {
                    printf("Ne vse turi provedeni! potverdite vihod(1/0): ");
                    scanf("%i", &a);
                    if (a)
                        a = 4;
                    else
                        a = 1;
                }
                break;
            default:
                printf("\n\nERROR!!! Please enter number 1-4...\n\n");
        }
        
    } while (a != 4);
    getchar();
    free(turnir);
    for (i = 0; i < num; i++) {
        free(tabl[i]);
    }
    free(tabl);
    return 0;
}

void tur(struct football turnir[], int num)
{
    int count = 0;
    int pr = 0;
    int j, n, m, r = 0;
    int *antirepet = (int*)malloc((num - 2)*sizeof(int));//массив сохраняющий игравшие в туре команды для избежания повторов
    for (i = 0; i < num-2; i++)
        antirepet[i] = -1;
    do{
        srand(time(NULL));
        pr = 0;
        r = (rand() % (num*num));//рандом возвращает порядковый номер ячейки матрицы, затем ищем ее координаты
        n = (r / num);//строка
        m = (r % num);//столбец
        //printf("\n n = %i m = %i\n\n", n, m);
        if (tabl[n][m].pr != 1)//если данных еще нет
        {
            //printf("\n n = %i m = %i\n\n", n, m);
            for (i = 0; i < num-2; i++)
                if (antirepet[i] == n || antirepet[i] == m || (n == m))//если уже играли в туре
                {
                    //printf("\n n = %i m = %i chuinya delov\n\n", n, m);
                    pr = 1;
                    break;
                }
            if (!pr)//если еще не играли, то запоняем данные по их играм
            {
                antirepet[count] = n;
                count++;
                antirepet[count] = m;
                count++;
                input_score(&turnir[n], &turnir[m], &tabl[n][m]);
                tabl[n][m].pr = 1;
                input_score(&turnir[m], &turnir[n], &tabl[m][n]);
                tabl[m][n].pr = 1;
            }
            //printf("\n\ncounter = %i\n\n", count);
        }
        //system("pause");
    } while (count < num - 2);
    pr = 0;
    n = 0;
    m = 0;
    for (int i = 0; i < num; i++)
    {
        if (n == antirepet[i])
            n++;
        else pr += 1;
        if (m == antirepet[i] || n == m)
            m++;
        else pr += 1;
        if (pr == 2)
            break;
    }
    if (pr == 2 && tabl[n][m].pr != 1)
    {
        input_score(&turnir[n], &turnir[m], &tabl[n][m]);
        tabl[n][m].pr = 1;
        input_score(&turnir[m], &turnir[n], &tabl[m][n]);
        tabl[m][n].pr = 1;
    }
    else
    {
        pr = 1;
        for (i = 0; i < num; i++)
        {
            for (j = 0; j < num; j++)
                if (tabl[i][j].pr != 1 && i != j)
                {
                    input_score(&turnir[i], &turnir[j], &tabl[i][j]);
                    tabl[i][j].pr = 1;
                    input_score(&turnir[j], &turnir[i], &tabl[j][i]);
                    tabl[j][i].pr = 1;
                    break;
                    pr = 1;
                }
            if (pr)
                break;
        }
    }
    free (antirepet);
}

void input_score(struct football* team1, struct football* team2, struct Score* sc)
{
    printf("Igra \n");
    printf("Komanda %s: ", team1->name);
    scanf("%i", &team1->score);
    sc->team1 = team1->score;
    printf("Komanda %s: ", team2->name);
    scanf("%i", &team2->score);
    sc->team2 = team2->score;
    if (team1->score > team2->score)
    {
        team1->sum_score += 3;
        team2->sum_score += 0;
    }
    else if (team1->score == team2->score)
    {
        team1->sum_score += 1;
        team2->sum_score += 1;
    }
    else
    {
        team1->sum_score += 0;
        team2->sum_score += 3;
    }
}


void output_file(struct Score** sc, struct football *team, int num)
{
    int j;
    FILE *file;
    
    file = fopen("/Users/art.fek/Desktop/kursach_final/kursach_final/text.txt", "w");
    if (file != NULL)
    {
        for (i = 0; i < num; i++)
            fprintf(file,"\t%s ", team[i].name);
        fprintf(file, "\n");
        for (i = 0; i < num; i++)
        {
            fprintf(file, "\n%s", team[i].name);
            for (j = 0; j < num; j++)
                fprintf(file, "\t%i : %i", sc[i][j].team1, sc[i][j].team2);
            fprintf(file, "\n");
        }
        fprintf(file, "\nSumma");
        for (i = 0; i < num; i++)
            fprintf(file, "\t%i ", team[i].sum_score);
    }
    fclose(file);
}

void output_tur(struct Score** sc, struct football team[], int num)
{
    int j;
    //printf("\n Itogi turnira %i\n\n", n);//вывод итогов матчей
    for (i = 0; i < num; i++)
        printf("\t%s ", team[i].name);
    printf("\n");
    for (i = 0; i < num; i++)
    {
        printf("\n%s", team[i].name);
        for (j = 0; j < num; j++)
            printf("\t%i : %i", sc[i][j].team1, sc[i][j].team2);
        printf("\n");
    }
    printf("\nSumma");
    for (i = 0; i < num; i++)
        printf("\t%i ", team[i].sum_score);
    
    output_file(sc, team, num);
}

void output(struct football team[], int num)//вывод команд в соответсвии с набранными очками
{
    sort_sum_score(team, num);//сортировка по количеству набранных очков
    system("cls");
    printf("\tItogovay talblica\n\n");
    for (i = 0; i<num; i++)
    {
        printf("%i - %s\n", team[i].sum_score, team[i].name);
    }
}

void sort_sum_score(struct football team[], int num)//сортировка по убыванию методом "шейкера"
{
    struct football R;
    int leftMark = 1;
    int rightMark = num - 1;
    while (leftMark <= rightMark)
    {
        for (i = rightMark; i >= leftMark; i--)
            if (team[i - 1].sum_score < team[i].sum_score)
            {
                R = team[i];
                team[i] = team[i - 1];
                team[i - 1] = R;
            }
        leftMark++;
        for (i = leftMark; i <= rightMark; i++)
            if (team[i - 1].sum_score <
                team[i].sum_score)
            {
                R = team[i];
                team[i] = team[i - 1];
                team[i - 1] = R;
            }
        rightMark--;
    }
}

