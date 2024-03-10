#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

typedef struct Row
{
    int countrow;    //y 좌표
    struct Text* first;  // 라인에 첫번째 단어 주소
    struct Row* prev;
    struct Row* next;
}Row;



typedef struct Text
{
    int text_x;   // x좌표
    char data;
    struct Text* prev;
    struct Text* next;
}Text;


Row* G_row = NULL;
Text* G_text = NULL;
void savetofile(Row* head, const char* filename);
void loadfromfile(Row** head);
void lnx_print(Row* head);
void lnx_addtext(Row** head, char data);
void moveCursor(int x, int y);
void home();
void end();
void freedata(Row* head);
void Enterprint();
void Rowprint();

void savetofile(Row* head, const char* filename) {
    FILE* file;
 
    if ((file = fopen(filename, "w")) == NULL) {
        perror("파일 열기 오류");
        return;
    }

    Row* currentRow = head;

    while (currentRow != NULL) {
        Text* currentText = currentRow->first;

        while (currentText != NULL) {
            fputc(currentText->data, file);
            currentText = currentText->next;
        }

        fputc('\n', file); // 행 사이에 새 줄 문자 추가
        currentRow = currentRow->next;
    }

    fclose(file);
}

void freedata(Row* head) {
    Row* currentRow = head;
    Row* nextRow;
    Text* currentText;
    Text* nextText;

    while (currentRow != NULL) {
        
        currentText = currentRow->first;
        while (currentText != NULL) {
            nextText = currentText->next;
            free(currentText);
            currentText = nextText;
        }

       
        nextRow = currentRow->next;             
        free(currentRow);                    
        currentRow = nextRow;                  
    }
}



#if defined(_WIN32) || defined(_WIN64) // Windows


#include <conio.h>
#include <Windows.h>

#define KEY_BACKSPACE 8
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_PPAGE 73
#define KEY_NPAGE 77
#define ctrl_s 19
int x = 0, y = 0;  //화면상 x,y좌펴
int a_y = 0, a_x = 0;  //실제 y좌표
int max_y = 0; // y줄가장큰좌표
char* filename;
int height, width;



void clear_screen() {
    system("cls");
    return;
}
void move(int y, int x) {
    //x, y 좌표 설정
    COORD pos = { x,y };
    //커서 이동
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void lnx_print(Row* head)
{
    int i = 0;
    x = 0;
    y = 0;
    move(y, x);
    Row* currentRow = head;
    while (currentRow != NULL)
    {
        if (y == height - 2) break;
        printf("\x1B[K");  // 현재커서부터 현재 줄 지우기
        Text* currentText = currentRow->first;
        while (currentText != NULL)
        {
            i++;
            if (i == width) break;
            printf("%c", currentText->data);

            currentText = currentText->next;
        }
        printf("\n");
        i = 0;
        currentRow = currentRow->next;

        y++;

    }


}
void Rowprint()
{
    if (G_row == NULL) // 예외처리
    {
        return;
    }
    int i = 0;
    move(y, 0);
    printf("\x1B[K");   // 현재커서부터 현재 줄 지우기
    Text* T_temp;
    T_temp = G_row->first;
    while (T_temp != NULL)
    {
        if (i == width - 1)  //콘솔창 크기가 마지막일때
        {
            move(y, width - 1);
            break;
        }
        i++;
        printf("%c", T_temp->data);
        T_temp = T_temp->next;

    }

}
void Enterprint()
{

    int i = 0, k = 0;
    int tmp_y;
    if (G_row == NULL) //예외처리
    {
        return;
    }

    tmp_y = y;
    Row* R_temp = G_row;
    Text* T_temp;
    if (y == height - 2)
    {
        tmp_y--;
    }


    while (tmp_y != 0)
    {
        tmp_y--;
        R_temp = R_temp->prev;
    }

    move(0, 0);



    while (R_temp != NULL)
    {


        printf("\x1B[K");   // 현재커서부터 현재 줄 지우기

        if (R_temp->first == NULL) //라인은있는데 문자가없다
        {
            printf("\n");
        }
        else
        {
            T_temp = R_temp->first;
            while (T_temp != NULL)
            {
                if (i == width - 1)  //콘솔창 크기가 마지막일때
                {

                    break;
                }
                i++;
                printf("%c", T_temp->data);
                T_temp = T_temp->next;
            }
            i = 0;
            printf("\n");
        }

        if (k == height - 3)
        {
            if (y == height - 2)
            {
                y = height - 3;
            }
            move(y, x);

            break;
        }

        R_temp = R_temp->next;
        k++;


    }


}
void lnx_load_enterkey(Row** head) {
    if (*head == NULL) {
        Row* R_temp = (Row*)malloc(sizeof(Row));
        *head = R_temp;
        R_temp->first = NULL;
        R_temp->prev = NULL;
        R_temp->countrow = 0;  // 행의 카운트를 0부터 시작하도록 설정
        a_y++;
        Row* New_line = (Row*)malloc(sizeof(Row));
        R_temp->next = New_line;
        New_line->next = NULL;
        New_line->prev = R_temp;
        New_line->countrow = y;
        New_line->first = NULL;

        G_row = New_line;
        return;
    }



    if (G_row == NULL)
    {
        G_row = *head;
    }

    Row* New_line = (Row*)malloc(sizeof(Row));
    New_line->first = NULL;
    New_line->next = NULL;
    New_line->prev = NULL;


    G_row->next = New_line;
    New_line->prev = G_row;
    a_y++;
    New_line->countrow = a_y;


    G_row = New_line;
    a_x = 0;
    return;




}



void lnx_load_addtext(Row** head, char data) {


    Text* T_temp = (Text*)malloc(sizeof(Text));
    T_temp->data = data;
    T_temp->prev = NULL;
    T_temp->next = NULL;
    T_temp->text_x = a_x;

    if (*head == NULL) {
        *head = (Row*)malloc(sizeof(Row));
        (*head)->first = T_temp;
        (*head)->countrow = a_y;
        (*head)->next = NULL;
        (*head)->prev = NULL;
        if (x == width - 1)
        {
            a_x++;
        }
        else
        {
            x++;
            a_x++;
        }
        G_text = T_temp;
        G_row = *head;
        return;
    }

    if (G_row->first == NULL)
    {
        G_row->first = T_temp;
        a_x++;
        G_text = T_temp;
        return;

    }
    G_text->next = T_temp;
    T_temp->prev = G_text;
    a_x++;
    G_text = T_temp;
    return;






}
void bar()
{
    if (max_y < a_y)
    {
        max_y = a_y;
    }
    move(height - 2, 0);
    printf("\033[7m%*s\033[0m", width, "");
    move(height - 2, 0);
    
    printf("\033[7m[%s] - %d lines\033[0m", filename, a_y + 1);

    move(height - 2, width - 16);
    printf("\033[7mft %d/%d\033[0m", a_y + 1, max_y + 1);
    move(height - 1, 0);
    printf("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");


    move(y, x);



}

void home()
{
    if (G_row == NULL) return;

    if (G_text == NULL) return; //커서처음이라면

    G_text = NULL;
    x = 0;
    a_x = 0;
    move(y, x);

}

void end()
{
    if (G_row == NULL) return;

    if (G_row->first == NULL) return;

    Text* T_temp;

    T_temp = G_row->first;

    while (T_temp->next != NULL)    //끝까지 돌리기 
    {

        T_temp = T_temp->next;
    }

    G_text = T_temp;
    a_x = T_temp->text_x + 1;
    if (a_x > width - 1)
    {
        x = width - 1;
    }
    else
    {
        x = a_x;
    }

    move(y, x);
}
void pagedown()
{
    if (G_row == NULL) return;
    int temp_y = 0;
    int i;
    Row* R_temp;
    Text* T_temp;
    R_temp = G_row;
    temp_y = y;

    while (temp_y != height - 2)  //마지막 문장라인가기
    {
        if (R_temp->next == NULL) //내릴 문장이없음
        {
            return;
        }
        R_temp = R_temp->next;
        temp_y++;
    }

    G_row = R_temp; //화면에 첫라인에문장이됌
    for (i = 0; i < height - 1; i++)
    {
        printf("\x1B[K");    // 현재커서부터 현재 줄 지우기
        printf("\n");

    }
    temp_y = y;
    y = 0;
    Enterprint();  //새창띄우기
    y = temp_y;
    temp_y = 0;
    while (temp_y != y) //y라인까지가기
    {
        if (R_temp->next == NULL) //내렬갈 문장이없다
        {

            break;
        }
        R_temp = R_temp->next;
        temp_y++;
    }


    if (y == temp_y)   //x좌표찾기
    {
        if (R_temp->first == NULL)
        {
            x = 0;
            a_x = 0;
            G_text = NULL;
            G_row = R_temp;
            return;

        }

        T_temp = R_temp->first;
        while (T_temp->next != NULL)
        {
            if (T_temp->text_x == x - 1)
            {
                G_text = T_temp;
                a_x = G_text->text_x + 1;
                x = a_x;
                y = temp_y;
                G_row = R_temp;
                a_y = G_row->countrow;
                return;


            }


            T_temp = T_temp->next;
        }

        G_text = T_temp;
        a_x = G_text->text_x + 1;
        x = a_x;
        y = temp_y;
        G_row = R_temp;
        a_y = G_row->countrow;


        return;

    }
    else
    {
        while (R_temp->next != NULL)
        {
            if (R_temp->first == NULL)
            {
                x = 0;
                a_x = 0;
                G_text = NULL;
                G_row = R_temp;
                y = temp_y;
                a_y = G_row->countrow;
                return;

            }

            T_temp = R_temp->first;
            while (T_temp->next != NULL)
            {
                if (T_temp->text_x == x - 1)
                {
                    G_text = T_temp;
                    a_x = G_text->text_x + 1;
                    x = a_x;
                    y = temp_y;
                    G_row = R_temp;

                    a_y = G_row->countrow;


                    return;


                }


                T_temp = T_temp->next;
            }

            G_text = T_temp;
            a_x = G_text->text_x + 1;
            x = a_x;
            y = temp_y;
            G_row = R_temp;

            a_y = G_row->countrow;

            return;
        }
        R_temp = R_temp->next;
    }










}
void pageup()  //화면에 가장상단라인을 pageup을 누르면 그 라인을 제일 밑으로 나둬야함
{
    if (G_row == NULL) return;
    int temp_y = 0;
    int i;
    Row* R_temp;
    Row* R_temp2;
    Text* T_temp;
    R_temp = G_row;
    temp_y = y;

    if (a_y < height - 1) return; //올릴문장없음
    while (temp_y != 0)  //첫라인 문장가기
    {
        if (R_temp->prev == NULL) //올릴 문장이없음
        {
            return;
        }
        R_temp = R_temp->prev;
        temp_y--;
    }

    G_row = R_temp; //화면에 가장 상단라인

    while (temp_y != height - 2)
    {
        if (R_temp->prev == NULL)
        {

            break;
        }
        R_temp = R_temp->prev;
        temp_y++;
    }

    if (temp_y == height - 2)
    {

        G_row = R_temp; //화면에 첫라인에문장이됌
        for (i = 0; i < height - 1; i++)
        {
            printf("\x1B[K");    // 현재커서부터 현재 줄 지우기
            printf("\n");
        }
        temp_y = y;
        y = 0;
        Enterprint();  //새창띄우기
        y = temp_y;
        temp_y = 0;

        temp_y++;
    }
    G_row = R_temp;

    if (R_temp->first == NULL)
    {
        x = 0;
        while (temp_y != y)  //원래 커서로이동
        {
            R_temp = R_temp->next;
            a_x = 0;
            G_text = NULL;
            G_row = R_temp;
            return;

        }

        T_temp = R_temp->first;
        while (T_temp->next != NULL)
        {
            if (T_temp->text_x == x - 1)
            {
                G_text = T_temp;
                a_x = G_text->text_x + 1;
                x = a_x;
                y = temp_y;
                G_row = R_temp;
                a_y = G_row->countrow;
                return;


            }


            T_temp = T_temp->next;
        }

        G_text = T_temp;
        a_x = G_text->text_x + 1;
        x = a_x;
        y = temp_y;
        G_row = R_temp;
        a_y = G_row->countrow;

        return;



    }
    else    //올릴문장이 한페이지가안됀다
    {

        while (R_temp->prev != NULL)
        {
            R_temp = R_temp->prev;
        }
        move(0, 0);
        for (i = 0; i < height - 1; i++)
        {
            printf("\x1B[K"); ;  // 현재커서부터 현재 줄 지우기
            printf("\n");
        }
        R_temp2 = G_row;
        G_row = R_temp;
        temp_y = y;
        y = 0;
        Enterprint();  //새창띄우기
        y = temp_y;
        temp_y = 0;
        G_row = R_temp2;

        if (R_temp->first == NULL)
        {
            x = 0;
            a_x = 0;
            G_text = NULL;
            G_row = R_temp;
            return;

        }

        T_temp = R_temp->first;
        while (T_temp->next != NULL)
        {
            if (T_temp->text_x == x - 1)
            {
                G_text = T_temp;
                a_x = G_text->text_x + 1;
                x = a_x;
                y = temp_y;
                G_row = R_temp;
                a_y = G_row->countrow;
                return;


            }


            T_temp = T_temp->next;
        }

        G_text = T_temp;
        a_x = G_text->text_x + 1;
        x = a_x;
        y = temp_y;
        G_row = R_temp;
        a_y = G_row->countrow;

        return;



    }


}

void lnx_addtext(Row** head, char data) {


    Text* T_temp = (Text*)malloc(sizeof(Text));
    T_temp->data = data;
    T_temp->prev = NULL;
    T_temp->next = NULL;
    T_temp->text_x = a_x;

    if (*head == NULL) {
        *head = (Row*)malloc(sizeof(Row));
        (*head)->first = T_temp;
        (*head)->countrow = a_y;
        (*head)->next = NULL;
        (*head)->prev = NULL;
        x++;
        a_x++;

        G_text = T_temp;
        G_row = *head;
        return;
    }


    Row* R_temp;
    if (G_row == NULL)
    {
        R_temp = *head;
    }
    else
    {
        R_temp = G_row;
    }

    if (R_temp->first == NULL)  //문장이 아예없을때 문자 넣는경우
    {
        R_temp->first = T_temp;
        x++;
        a_x++;
        G_text = T_temp;
        return;
    }

    if (G_text == NULL)  //문장에 처음커서가있을때
    {
        Text* temp;
        T_temp->next = G_row->first;
        G_row->first->prev = T_temp;
        G_row->first = T_temp;
        temp = T_temp->next;
        while (temp != NULL)    //뒤에문장좌표가 +1;
        {
            temp->text_x = temp->text_x + 1;
            temp = temp->next;
        }
        G_text = T_temp;
        x = 1;
        a_x = 1;

    }
    else if (G_text->next == NULL) //문장 마지막에서 문자 삽입하는경우
    {
        G_text->next = T_temp;
        T_temp->prev = G_text;
        a_x++;
        x++;
        G_text = G_text->next;
    }
    else         //중간삽입
    {
        Text* temp;
        T_temp->next = G_text->next;
        G_text->next->prev = T_temp;
        G_text->next = T_temp;
        T_temp->prev = G_text;

        temp = T_temp;
        while (temp != NULL)    //뒤에문장좌표가 +1;
        {
            temp->text_x = temp->text_x + 1;
            temp = temp->next;
        }
        a_x++;
        x++;
        G_text = T_temp;
    }

}
void lnx_enterkey(Row** head) {
    if (*head == NULL) {
        Row* R_temp = (Row*)malloc(sizeof(Row));
        *head = R_temp;
        R_temp->first = NULL;
        R_temp->prev = NULL;
        R_temp->countrow = a_y;  // 행의 카운트를 0부터 시작하도록 설정
        y++;
        a_y++;
        Row* New_line = (Row*)malloc(sizeof(Row));
        R_temp->next = New_line;
        New_line->next = NULL;
        New_line->prev = R_temp;
        New_line->countrow = a_y;
        New_line->first = NULL;

        G_text = NULL;
        G_row = New_line;
        return;
    }



    Row* New_line = (Row*)malloc(sizeof(Row));
    New_line->first = NULL;
    New_line->next = NULL;
    New_line->prev = NULL;
    Row* R_temp;

    if (G_row == *head)  //첫번째에 텍스트입력후 예외처리
    {
        R_temp = *head;
        a_y++;
        y++;
        New_line->countrow = a_y;

    }
    else
    {
        a_y++;
        y++;
        R_temp = G_row;
        New_line->countrow = a_y;
    }




    if (G_text == NULL)      //문장 첫번째 커서에서 엔터를 치는 경우
    {
        New_line->first = R_temp->first;
        R_temp->first = NULL;

        if (R_temp->next == NULL) //엔터를 부른 문장이 텍스트전체의 마지막이다
        {
            R_temp->next = New_line;
            New_line->prev = R_temp;


            G_text = NULL;
            G_row = New_line;

        }
        else         //엔터를 부른 문장뒤에 다른문장이있다
        {
            R_temp->next->prev = New_line;
            New_line->next = R_temp->next;
            R_temp->next = New_line;
            New_line->prev = R_temp;

            R_temp = New_line->next;

            while (R_temp != NULL)  // 뒤에 문장에 다 좌표 1씩 추가
            {
                R_temp->countrow = R_temp->countrow + 1;
                R_temp = R_temp->next;
            }



        }
        x = 0;
        a_x = 0;
        G_text = NULL;
        G_row = New_line;
    }
    else if (G_text->next == NULL) // 문장 마지막에서 엔터치는경우
    {

        if (R_temp->next == NULL) //엔터를 부른 문장이 텍스트전체의 마지막이다
        {
            R_temp->next = New_line;
            New_line->prev = R_temp;

            x = 0;
            a_x = 0;
            G_row = New_line;
            G_text = NULL; //첫번째커서를 의미;
            return;
        }
        else         //엔터를 부른 문장뒤에 다른문장이있다
        {
            R_temp->next->prev = New_line;
            New_line->next = R_temp->next;
            R_temp->next = New_line;
            New_line->prev = R_temp;

            R_temp = New_line->next;

            while (R_temp != NULL)  // 뒤에 문장에 다 좌표 1씩 추가
            {
                R_temp->countrow = R_temp->countrow + 1;
                R_temp = R_temp->next;
            }

            x = 0;
            a_x = 0;
            G_text = NULL; //첫번째커서를 의미;
            G_row = New_line;

        }


    }
    else   //문장 중간에서 엔터치는경우
    {
        if (G_text->text_x != 0)  //첫번째 제외 삽입이면
        {

            Text* T_temp;
            int i = 0;
            New_line->first = G_text->next;

            G_text->next->prev = NULL;
            G_text->next = NULL;

            T_temp = New_line->first;

            while (T_temp != NULL)
            {

                T_temp->text_x = i;
                i++;
                T_temp = T_temp->next;
            }

            G_text = NULL;

            if (R_temp->next == NULL) //엔터를 부른 문장이 텍스트전체의 마지막이다
            {
                R_temp->next = New_line;
                New_line->prev = R_temp;

                G_row = New_line;

            }
            else         //엔터를 부른 문장뒤에 다른문장이있다
            {
                R_temp->next->prev = New_line;
                New_line->next = R_temp->next;
                R_temp->next = New_line;
                New_line->prev = R_temp;

                R_temp = New_line->next;

                while (R_temp != NULL)  // 뒤에 문장에 다 좌표 1씩 추가
                {
                    R_temp->countrow = R_temp->countrow + 1;
                    R_temp = R_temp->next;
                }


            }
            x = 0;
            a_x = 0;
            G_row = New_line;


        }

    }

}

void lnx_delete()
{
    int i = 0;
    Text* T_temp;
    Row* R_temp;
    Row* R_temp2;
    if (G_row == NULL) //  지울문장없다
    {

        return;
    }
    else //지울 문장이있다
    {

        if (G_text == NULL)  //커서가 처음일때  화면새로바꿔줘야됌
        {
            if (G_row->prev == NULL) //올라갈 문장없으면 리턴
            {
                return;
            }

            R_temp = G_row->prev;

            if (R_temp->first == NULL) // 올라갈문장에 아무데이터도없다면
            {
                if (G_row->first != NULL) //밑문장에도 데이터가있다면
                {
                    printf("\x1B[K");    // 현재커서부터 현재 줄 지우기
                    R_temp->first = G_row->first;

                }
                G_text = NULL;
                R_temp->next = NULL;


            }
            else  // 올라갈 문장에 데이터가있다면
            {

                if (G_row->first != NULL) //밑에 문장에 문자가있다면
                {
                    if (R_temp->first == NULL)  //위에문장에 문자가없다면
                    {
                        R_temp->first = G_row->first;
                        G_text = NULL; //좌표설정
                        x = 0;
                        a_x = 0;

                    }
                    else  //위에문장에 문자가있다면
                    {

                        T_temp = R_temp->first;// 위문장연결
                        while (T_temp->next != NULL)  //끝까지가기
                        {
                            i++;
                            T_temp = T_temp->next;
                        }
                        T_temp->next = G_row->first; // 윗문장을 밑문장을 연결
                        G_row->first->prev = T_temp;
                        G_text = T_temp; // 좌표설정
                        a_x = G_text->text_x + 1;
                        if (a_x > width - 1)
                        {
                            x = width - 1;
                        }
                        else
                        {
                            x = a_x;
                        }

                        T_temp = G_text->next;
                        while (T_temp->next != NULL)  //좌표맞춰주기
                        {
                            T_temp->text_x = i;
                            i++;
                            T_temp = T_temp->next;
                        }


                    }
                }
                else    //밑에 문장에 문장이없다면
                {

                    if (R_temp->first == NULL)  //위에문장에 문자가없다면
                    {
                        R_temp->first = G_row->first;
                        G_text = NULL; //좌표설정
                        x = 0;
                        a_x = 0;

                    }
                    else  //위에문장에 문자가있다면
                    {


                        T_temp = R_temp->first;// 위문장연결
                        while (T_temp->next != NULL)  //끝까지가기
                        {

                            T_temp = T_temp->next;
                        }

                        G_text = T_temp; // 좌표설정
                        a_x = G_text->text_x + 1;
                        if (a_x > width - 1)
                        {
                            x = width - 1;
                        }
                        else
                        {
                            x = a_x;
                        }



                    }
                }


            }

            if (G_row->next != NULL)
            {
                R_temp->next = G_row->next;
                G_row->next->prev = R_temp;
                free(G_row);
                G_row = R_temp;
                R_temp = R_temp->next;
                while (R_temp->next != NULL) //뒷라인 자표 +1씩
                {
                    R_temp->countrow++;
                    R_temp = R_temp->next;
                }
            }
            else
            {
                R_temp2 = G_row;
                free(R_temp2);
                G_row = R_temp;
                G_row->next = NULL;
            }

            a_y--;               //좌표한칸옮기기

            if (y == 0)
            {
                y = 0;
            }
            else
            {
                y--;
            }


            for (i = 0; i < height - 2; i++)
            {
                printf("\x1B[K");    // 현재커서부터 현재 줄 지우기
                printf("\n");
            }
            max_y--;
            Enterprint();





        }
        else  //커서가 문장중간일때
        {
            if (G_text->prev == NULL) //커서가1번째 뒤 예외처리
            {
                if (G_text->next == NULL) //그문장이 단어가1개이다
                {
                    T_temp = G_text;
                    free(T_temp);
                    G_text = NULL;
                    G_row->first = NULL;
                    x--;
                    a_x--;

                    return;
                }
                else  //뒤에 이어줄 문장이있다
                {
                    T_temp = G_text;
                    T_temp = T_temp->next;
                    G_row->first = T_temp;
                    free(G_text);
                    G_text = NULL;
                    T_temp->prev = NULL;
                    while (T_temp->next != NULL)  // ㅡ다음문장 -1씩
                    {
                        T_temp->text_x--;
                        T_temp = T_temp->next;
                    }
                    x--;
                    a_x--;
                    return;

                }
            }
            else      //커서가 중간이거나 마지막일때
            {
                if (G_text->next == NULL)  //마지막
                {
                    T_temp = G_text;
                    G_text = G_text->prev;
                    G_text->next = NULL;
                    free(T_temp);
                    x--;
                    a_x--;
                    return;
                }
                else  //중간
                {
                    T_temp = G_text;
                    T_temp = T_temp->prev;
                    T_temp->next = G_text->next;
                    G_text->next->prev = T_temp;
                    free(G_text);

                    G_text = T_temp;
                    T_temp = T_temp->next;

                    while (T_temp->next != NULL)  // ㅡ다음문장 -1씩
                    {
                        T_temp->text_x--;
                        T_temp = T_temp->next;
                    }
                    x--;
                    a_x--;

                }
            }
        }
    }


}

void lnx_getspecialkey(Row** head, int ch) {


    Text* T_temp;
    if (ch == KEY_DOWN) {
        if (G_row->next == NULL) // 넘어갈 문장이없다
        {
            return;
        }
        else
        {
            G_row = G_row->next;

            if (y == height - 3)  //콘솔창 끝에서 내려서 프린트 다시해야됌
            {
                if (G_row->first == NULL)  //밑문장에 리스트만있고 아무데이터도없는경우
                {
                    G_text = NULL;
                    Enterprint();
                    a_x = 0;
                    x = a_x;
                    a_y++;
                    return;
                }
                else if (G_text == NULL)
                {
                    Enterprint();
                    a_y++;
                    return;
                }
                else //밑문장에데이터가있으면
                {
                    T_temp = G_row->first;

                    while (T_temp->next != NULL)
                    {
                        if (G_text->text_x == T_temp->text_x)   //내려갈 문장이 더길면
                        {

                            G_text = T_temp;
                            a_x = G_text->text_x + 1;
                            x = a_x;
                            Enterprint();
                            a_y++;

                            return;
                        }
                        T_temp = T_temp->next;
                    }

                    G_text = T_temp;
                    a_x = G_text->text_x + 1;
                    x = a_x;
                    Enterprint();
                    a_y++;

                }
            }
            else
            {
                if (G_row->first == NULL)  //밑문장에 리스트만있고 아무데이터도없는경우
                {
                    G_text = NULL;
                    a_x = 0;
                    x = a_x;
                    y++;
                    a_y++;
                    return;
                }
                else if (G_text == NULL)
                {
                    y++;
                    a_y++;
                    return;
                }
                else //밑문장에데이터가있으면
                {
                    T_temp = G_row->first;

                    while (T_temp->next != NULL)
                    {
                        if (G_text->text_x == T_temp->text_x)   //내려갈 문장이 더길면
                        {

                            G_text = T_temp;
                            a_x = G_text->text_x + 1;
                            x = a_x;
                            y++;
                            a_y++;

                            return;
                        }
                        T_temp = T_temp->next;
                    }

                    G_text = T_temp;
                    a_x = G_text->text_x + 1;
                    x = a_x;
                    y++;
                    a_y++;

                }

            }
        }
    }
    else if (ch == KEY_UP)
    {
        if (y == 0)  //콘솔창 맨위에서 올릴때
        {
            if (G_row->prev == NULL)
            {
                return;
            }
            else       //윗 문장에 리스트가있는경우
            {
                G_row = G_row->prev;
                Text* T_temp;

                if (G_row->first == NULL)  //윗 문장에 문자가한개도없는경우
                {
                    G_text = NULL;
                    x = 0;
                    a_x = 0;
                    a_y--;
                    Enterprint();
                    return;
                }


                T_temp = G_row->first;
                if (G_text == NULL) //커서가 처음일떄
                {
                    a_y--;
                    Enterprint();
                    return;
                }
                while (T_temp->next != NULL)
                {
                    if (T_temp->text_x == G_text->text_x) // 위라인문장이 밑보다 같거나 길면
                    {
                        G_text = T_temp;
                        a_y--;
                        a_x = G_text->text_x + 1;
                        x = a_x;
                        Enterprint();
                        return;
                    }
                    T_temp = T_temp->next;

                }

                G_text = T_temp;  //위라인이 밑보다짧으면 위라인 제일마지막으로감
                Enterprint();
                a_y--;
                a_x = G_text->text_x + 1;
                x = a_x;


            }
        }
        else      //첫문장 제외해서 남은 문장에서 올릴떄
        {
            if (G_row->prev != NULL) //올라갈라인이있음
            {

                Text* T_temp;
                if (G_text == NULL) //밑문장에아무것도 없을때
                {
                    G_row = G_row->prev;
                    y--;
                    a_y--;
                    return;
                }

                G_row = G_row->prev;
                if (G_row->first == NULL)  //윗 문장에 문자가한개도없는경우
                {
                    G_text = NULL;
                    x = 0;
                    a_x = 0;
                    y--;
                    a_y--;
                    return;
                }
                else       //윗 문장에 문자가있는경우
                {


                    T_temp = G_row->first;
                    while (T_temp->next != NULL)
                    {
                        if (T_temp->text_x == G_text->text_x) // 위라인문장이 밑보다 같거나 길면
                        {
                            G_text = T_temp;
                            a_x = G_text->text_x + 1;
                            x = a_x;
                            y--;
                            a_y--;

                            return;
                        }
                        T_temp = T_temp->next;

                    }

                    G_text = T_temp;  //위라인이 밑보다짧으면 위라인 제일마지막으로감
                    y--;
                    a_y--;
                    a_x = G_text->text_x + 1;
                    x = a_x;


                }

            }
            else
            {
                return;
            }
        }

    }
    else if (ch == KEY_LEFT)
    {
        if (G_text == NULL) // 커서가 처음이면
        {
            if (G_row->prev != NULL)  //올라갈 문장이있다면
            {

                G_row = G_row->prev;
                if (G_row->first != NULL) //올라갈 문장에 문자열이있다면
                {
                    G_text = G_row->first;
                    while (G_text->next != NULL)
                    {
                        G_text = G_text->next;
                    }
                    a_x = G_text->text_x + 1;
                    if (x > width - 1)
                    {
                        x = width - 1;
                    }
                    else
                    {
                        x = a_x;
                    }

                    if (y == 0) // 첫번쨰에서 왼방향키눌러서 화면다시찍는 경우
                    {
                        Enterprint();
                        y = 0;
                        a_y--;
                        if (x > width - 1)
                        {
                            x = width - 1;
                        }
                        else
                        {
                            x = a_x;
                        }
                        return;

                    }
                    y--;
                    a_y--;

                }
                else    //올라갈 문장에 문자가 아무것도없다면
                {
                    G_row = G_row->prev;
                    G_text = NULL;
                    if (y == 0) // 첫번쨰에서 왼방향키눌러서 화면다시찍는 경우
                    {
                        Enterprint();
                        y = 0;
                        a_y--;
                        x = 0;
                        a_x = 0;
                        return;

                    }

                    y--;
                    a_y--;
                    x = 0;
                    a_x = 0;
                    return;
                }
            }
            else  //커서가 처음인데 올라갈 문장이없다면
            {
                return;
            }

        }
        else  //문장사이에 커서가있다면
        {

            if (x == 1)
            {
                G_text = NULL;
                x = 0;
                a_x = 0;
                return;
            }
            G_text = G_text->prev;
            a_x--;
            if (x > width - 1)
            {
                x = width - 1;
            }
            else
            {
                x--;
            }

        }


    }
    else if (ch == KEY_RIGHT)
    {


        if (G_text == NULL) //커서가 처음일때
        {
            if (G_row->first != NULL)  //현재커서의 문장이있을경우
            {
                G_text = G_row->first;

                x++;
                a_x++;
            }
            else if (G_row->next != NULL)  //넘어갈 문장이있고 현재커서에 문장에 아무문자열이없을때
            {
                if (y == height - 3) //마지막행에서 오른쪽으로 넘어가면 화면을 새로찍어야됌
                {
                    G_row = G_row->next;
                    Enterprint();
                    a_x = 0;
                    x = 0;
                    return;
                }
                G_row = G_row->next;
                G_text = NULL;
                y++;
                a_y++;
                x = 0;
                a_x = 0;

            }
            else  //넘어갈 문장이없다면
            {
                return;
            }
        }
        else if (G_text->next == NULL) //문장끝에서 넘어갈때
        {
            if (G_row->next != NULL)  //넘어갈 문장이있다면
            {
                if (y == height - 3) //마지막행에서 오른쪽으로 넘어가면 화면을 새로찍어야됌
                {
                    G_row = G_row->next;
                    G_text = NULL;
                    Enterprint();
                    a_x = 0;
                    x = 0;
                    a_y++;
                    return;
                }
                G_row = G_row->next;
                G_text = NULL; //첫커서로
                y++;
                a_y++;
                x = 0;
                a_x = 0;

            }
            else  //넘어갈 문장이없다면
            {
                return;
            }
        }
        else //문장중간에서 누를경우
        {
            G_text = G_text->next;
            a_x++;
            if (x != width - 1)  //나중에 구현예정
            {
                x++;
            }
            else
            {
                return;
            }

        }

    }
}






void loadfromfile(Row** head) {
    FILE* file;
    clear_screen();
    move(0, 0);

    if ((file = fopen(filename, "r")) == NULL) {
        perror("File not found");
        printf("\n");

        exit(0);
        return;
    }

    const size_t BUFFER_SIZE = 4096; 
    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (buffer == NULL) {

        perror("메모리 할당 오류");
        printf("\n");
        fclose(file);
        exit(0);
        return;
    }

    size_t bytesRead;
    size_t bufferIndex = 0; 

    while ((bytesRead = fread(buffer + bufferIndex, 1, BUFFER_SIZE - bufferIndex, file)) > 0) {
        size_t totalBytesRead = bufferIndex + bytesRead;

        for (size_t i = 0; i < totalBytesRead; ++i) {
            char c = buffer[i];
            if (c == '\n') {
                lnx_load_enterkey(head);
                if (max_y < a_y) max_y = a_y;
            }
            else {
                lnx_load_addtext(head, c);

            }
        }

        bufferIndex = totalBytesRead % BUFFER_SIZE; // 현재버퍼에서 처리안됀 데이터들 앞으로 이동 
        if (bufferIndex > 0) {
            memmove(buffer, buffer + totalBytesRead - bufferIndex, bufferIndex);
        }
    }

    free(buffer); 
    G_row = *head;
    G_text = NULL;
    a_x = 0;
    x = 0;
    y = 0;
    a_y = 0;
    fclose(file);
    return;
}
void getmaxyx(int* columns, int* rows) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;


    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == INVALID_HANDLE_VALUE)
    {
       
        return;
    }


    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {

        return;
    }

    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
void mod(Row** head) {
    int i = 0, k = 0;
    char filename1[100];
    filename1[0] = '\0';
    int columns, rows;
    clear_screen();


    getmaxyx(&columns, &rows);  // 현재 터미널 창 크기 가져오기
    for (i = 0; i < height - 2; i++) {     //초기설정
        move(i, 0);
        printf("~");


    }
    move(height / 2, (width - 20) / 2);
    printf("Visual Text editor -- version 0. 0. 1");


    bar();

    if (strcmp(filename, "No name") != 0)
    {
        loadfromfile(head);
        G_row = *head;
        G_text = NULL;
        lnx_print(*head);
        x = 0;
        y = 0;
        a_y = 0;
        a_x = 0;
        bar();

        k = 1;
    }


    while (1) {




        int c = _getch();
        for (; k < 1; k++)
        {
            move(0, 0);
            for (i = 0; i < height - 2; i++)
            {

                printf("\x1B[K");   // 현재커서부터 현재 줄 지우기
                printf("\n");
            }

            move(y, x);



        }
        if (c == 13) {  //enter키
            lnx_enterkey(head);

            //mvprintw(height - 2, 0, "[%s] - %d lines", filename, a_y + 1);

            Enterprint();


        }
        else if (c == KEY_BACKSPACE)
        {
            lnx_delete();
            Rowprint();


        }
        else if (c == 0xE0) {
            c = _getch();

            if (c == KEY_UP || c == KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT)
            {
                lnx_getspecialkey(head, c);
            }
            else if (c == KEY_PPAGE)
            {
                pageup();

            }
            else if (c == KEY_NPAGE)
            {
                pagedown();


            }
            else if (c == 71) {  // 71은 'Home' 키의 ASCII 코드
                home();
            }
            if (c == 79) {  // 79는 'End' 키의 ASCII 코드
                end();
            }
        }
        else if ('A' <= c && c <= 'z') {

            lnx_addtext(head, c);
            Rowprint();


        }
        else if (GetAsyncKeyState('S') & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000)  //저장
        {
            if (strcmp(filename, "No name") == 0)   //새로운 파일일경우
            {
                move(height - 1, 0);
                printf("\x1B[K");    // 현재커서부터 현재 줄 지우기
                printf("Please enter the name you want to save:");

                while (1)
                {
                    c = _getch();
                    if (c == 13) {
                        break;  // 엔터 키를 누르면 종료
                    }
                    else if (c == KEY_BACKSPACE) {
                        // 백스페이스 키를 누르면 파일 이름에서 마지막 문자 삭제
                        if (strlen(filename1) > 0) {
                            filename1[strlen(filename1) - 1] = '\0';
                        }
                    }
                    else if (c >= 32 && c <= 126) {
                        printf("%c", c);

                        if (strlen(filename1) < 100 - 1) {
                            strncat(filename1, (char*)&c, 1);
                        }
                    }



                }
                savetofile(*head, filename1);
                move(0, 0);
                clear_screen();
                freedata(*head);
                exit(0);
            }
            else
            {
                savetofile(*head, filename);
                move(0, 0);
                clear_screen();
                freedata(*head);
                exit(0);
            }
        }
        else if (GetAsyncKeyState('Q') & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) //나가기 
        {
            int count = 0;
            int start_time = time(NULL);
            Sleep(200);
           
            // 1.5초 동안 두 번째 'Ctrl+Q' 입력을 기다림
            while (time(NULL) - start_time < 0.3) {
                
                if (GetAsyncKeyState('Q') & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    count++;
                }
            }

            if (count > 0)
            {
                move(0, 0);
                clear_screen();
                freedata(*head);
                exit(0);
            }
            

            if (strcmp(filename, "No name") == 0)   //새로운 파일일경우
            {
                move(height - 1, 0);
                printf("\x1B[K");    // 현재커서부터 현재 줄 지우기
                move(height - 1, 0);
                printf("Please enter the name you want to save:");

                while (1)
                {
                    c = _getch();
                    // 입력이 있을 경우 처리
                    if (c == 13) {
                        break;  // 엔터 키를 누르면 종료
                    }
                    else if (c == KEY_BACKSPACE) {
                        // 백스페이스 키를 누르면 파일 이름에서 마지막 문자 삭제
                        if (strlen(filename) > 0) {
                            filename1[strlen(filename1) - 1] = '\0';
                        }
                    }
                    else if (c >= 32 && c <= 126) {
                        printf("%c", c);

                        if (strlen(filename1) < 100 - 1) {
                            strncat(filename1, (char*)&c, 1);
                        }
                    }


                }
                savetofile(*head, filename1);
                move(0, 0);
                clear_screen();
                freedata(*head);
                exit(0);
            }
            else
            {
                savetofile(*head, filename);
                move(0, 0);
                clear_screen();
                freedata(*head);
                exit(0);
            }




        }


        bar();
        move(y, x);

    }

    printf("\n");

}

#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)  // Linux 및 macOS 공통 코드
                                                                               
#include <ncurses.h>
#include <termios.h>
#include <unistd.h>
#define ctrl_s 19
#define ctrl_q 17
int x = 0, y = 0;  //화면상 x,y좌펴
int a_y = 0, a_x = 0;  //실제 y좌표
int max_y = 0; // y줄가장큰좌표
char* filename;
int height, width;

void clear_screen() {
    clear();
    return;
}
void lnx_print(Row* head)
{
    int i = 0;
    x = 0;
    y = 0;
    move(y, x);
    Row* currentRow = head;
    while (currentRow != NULL)
    {
        if (y == height - 2) break;
        clrtoeol();  // 현재커서부터 현재 줄 지우기
        Text* currentText = currentRow->first;
        while (currentText != NULL)
        {
            i++;
            if (i == width) break;
            printw("%c", currentText->data);

            currentText = currentText->next;
        }
        printw("\n");
        i = 0;
        currentRow = currentRow->next;

        y++;

    }

    refresh();
}
void Rowprint()
{
    if (G_row == NULL) // 예외처리
    {
        return;
    }
    int i = 0;
    move(y, 0);
    clrtoeol();  // 현재커서부터 현재 줄 지우기
    Text* T_temp;
    T_temp = G_row->first;
    while (T_temp != NULL)
    {
        if (i == width - 1)  //콘솔창 크기가 마지막일때 
        {
            move(y, width - 1);
            break;
        }
        i++;
        printw("%c", T_temp->data);
        T_temp = T_temp->next;

    }

}
void Enterprint()
{

    int i = 0, k = 0;
    int tmp_y;
    if (G_row == NULL) //예외처리
    {
        return;
    }

    tmp_y = y;
    Row* R_temp = G_row;
    Text* T_temp;
    if (y == height - 2)
    {
        tmp_y--;
    }


    while (tmp_y != 0)
    {
        tmp_y--;
        R_temp = R_temp->prev;
    }

    move(0, 0);
    refresh();


    while (R_temp != NULL)
    {


        clrtoeol();  // 현재커서부터 현재 줄 지우기

        if (R_temp->first == NULL) //라인은있는데 문자가없다
        {
            printw("\n");
        }
        else
        {
            T_temp = R_temp->first;
            while (T_temp != NULL)
            {
                if (i == width - 1)  //콘솔창 크기가 마지막일때 
                {

                    break;
                }
                i++;
                printw("%c", T_temp->data);
                T_temp = T_temp->next;
            }
            i = 0;
            printw("\n");
        }

        if (k == height - 3)
        {
            if (y == height - 2)
            {
                y = height - 3;
            }
            move(y, x);
            refresh();
            break;
        }

        R_temp = R_temp->next;
        k++;


    }

    refresh();


}
void lnx_load_enterkey(Row** head) {
    if (*head == NULL) {
        Row* R_temp = (Row*)malloc(sizeof(Row));
        *head = R_temp;
        R_temp->first = NULL;
        R_temp->prev = NULL;
        R_temp->countrow = 0;  // 행의 카운트를 0부터 시작하도록 설정
        a_y++;
        Row* New_line = (Row*)malloc(sizeof(Row));
        R_temp->next = New_line;
        New_line->next = NULL;
        New_line->prev = R_temp;
        New_line->countrow = y;
        New_line->first = NULL;

        G_row = New_line;
        return;
    }



    if (G_row == NULL)
    {
        G_row = *head;
    }

    Row* New_line = (Row*)malloc(sizeof(Row));
    New_line->first = NULL;
    New_line->next = NULL;
    New_line->prev = NULL;


    G_row->next = New_line;
    New_line->prev = G_row;
    a_y++;
    New_line->countrow = a_y;


    G_row = New_line;
    a_x = 0;
    return;




}



void lnx_load_addtext(Row** head, char data) {


    Text* T_temp = (Text*)malloc(sizeof(Text));
    T_temp->data = data;
    T_temp->prev = NULL;
    T_temp->next = NULL;
    T_temp->text_x = a_x;

    if (*head == NULL) {
        *head = (Row*)malloc(sizeof(Row));
        (*head)->first = T_temp;
        (*head)->countrow = a_y;
        (*head)->next = NULL;
        (*head)->prev = NULL;
        if (x == width - 1)
        {
            a_x++;
        }
        else
        {
            x++;
            a_x++;
        }
        G_text = T_temp;
        G_row = *head;
        return;
    }

    if (G_row->first == NULL)
    {
        G_row->first = T_temp;
        a_x++;
        G_text = T_temp;
        return;

    }
    G_text->next = T_temp;
    T_temp->prev = G_text;
    a_x++;
    G_text = T_temp;
    return;






}
void bar()
{
    if (max_y < a_y)
    {
        max_y = a_y;
    }

    mvprintw(height - 1, 0, "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
    attron(A_REVERSE);
    mvhline(height - 2, 0, ' ', width);

    mvprintw(height - 2, 0, "[%s] - %d lines", filename, a_y + 1);
    mvprintw(height - 2, width - 16, "ft %d/%d", a_y + 1, max_y + 1);
    attroff(A_REVERSE);
    move(y, x);


    refresh();
}
void pagedown()
{
    if (G_row == NULL) return;
    int temp_y = 0;
    int i;
    Row* R_temp;
    Text* T_temp;
    R_temp = G_row;
    temp_y = y;

    while (temp_y != height - 2)  //마지막 문장라인가기
    {
        if (R_temp->next == NULL) //내릴 문장이없음 
        {
            return;
        }
        R_temp = R_temp->next;
        temp_y++;
    }

    G_row = R_temp; //화면에 첫라인에문장이됌
    for (i = 0; i < height - 1; i++)
    {
        clrtoeol();  // 현재커서부터 현재 줄 지우기
        printw("\n");

    }
    temp_y = y;
    y = 0;
    Enterprint();  //새창띄우기
    y = temp_y;
    temp_y = 0;
    while (temp_y != y) //y라인까지가기
    {
        if (R_temp->next == NULL) //내렬갈 문장이없다
        {

            break;
        }
        R_temp = R_temp->next;
        temp_y++;
    }


    if (y == temp_y)   //x좌표찾기
    {
        if (R_temp->first == NULL)
        {
            x = 0;
            a_x = 0;
            G_text = NULL;
            G_row = R_temp;
            return;

        }

        T_temp = R_temp->first;
        while (T_temp->next != NULL)
        {
            if (T_temp->text_x == x - 1)
            {
                G_text = T_temp;
                a_x = G_text->text_x + 1;
                x = a_x;
                y = temp_y;
                G_row = R_temp;
                a_y = G_row->countrow;
                return;


            }


            T_temp = T_temp->next;
        }

        G_text = T_temp;
        a_x = G_text->text_x + 1;
        x = a_x;
        y = temp_y;
        G_row = R_temp;
        a_y = G_row->countrow;


        return;

    }
    else
    {
        while (R_temp->next != NULL)
        {
            if (R_temp->first == NULL)
            {
                x = 0;
                a_x = 0;
                G_text = NULL;
                G_row = R_temp;
                y = temp_y;
                a_y = G_row->countrow;
                return;

            }

            T_temp = R_temp->first;
            while (T_temp->next != NULL)
            {
                if (T_temp->text_x == x - 1)
                {
                    G_text = T_temp;
                    a_x = G_text->text_x + 1;
                    x = a_x;
                    y = temp_y;
                    G_row = R_temp;

                    a_y = G_row->countrow;


                    return;


                }


                T_temp = T_temp->next;
            }

            G_text = T_temp;
            a_x = G_text->text_x + 1;
            x = a_x;
            y = temp_y;
            G_row = R_temp;

            a_y = G_row->countrow;

            return;
        }
        R_temp = R_temp->next;
    }


}
void home()
{
    if (G_row == NULL) return;

    if (G_text == NULL) return; //커서처음이라면

    G_text = NULL;
    x = 0;
    a_x = 0;
    move(y, x);

}

void end()
{
    if (G_row == NULL) return;

    if (G_row->first == NULL) return;

    Text* T_temp;

    T_temp = G_row->first;

    while (T_temp->next != NULL)    //끝까지 돌리기 
    {

        T_temp = T_temp->next;
    }

    G_text = T_temp;
    a_x = T_temp->text_x + 1;
    if (a_x > width - 1)
    {
        x = width - 1;
    }
    else
    {
        x = a_x;
    }

    move(y, x);
}
void pageup()  //화면에 가장상단라인을 pageup을 누르면 그 라인을 제일 밑으로 나둬야함
{
    if (G_row == NULL) return;
    int temp_y = 0;
    int i;
    Row* R_temp;
    Row* R_temp2;
    Text* T_temp;
    R_temp = G_row;
    temp_y = y;

    if (a_y < height - 1) return; //올릴문장없음
    while (temp_y != 0)  //첫라인 문장가기
    {
        if (R_temp->prev == NULL) //올릴 문장이없음 
        {
            return;
        }
        R_temp = R_temp->prev;
        temp_y--;
    }

    G_row = R_temp; //화면에 가장 상단라인

    while (temp_y != height - 2)
    {
        if (R_temp->prev == NULL)
        {

            break;
        }
        R_temp = R_temp->prev;
        temp_y++;
    }

    if (temp_y == height - 2)
    {

        G_row = R_temp; //화면에 첫라인에문장이됌
        for (i = 0; i < height - 1; i++)
        {
            clrtoeol();  // 현재커서부터 현재 줄 지우기
            printw("\n");
        }
        temp_y = y;
        y = 0;
        Enterprint();  //새창띄우기
        y = temp_y;
        temp_y = 0;

        while (temp_y != y)  //원래 커서로이동
        {
            R_temp = R_temp->next;
            temp_y++;
        }
        G_row = R_temp;

        if (R_temp->first == NULL)
        {
            x = 0;
            a_x = 0;
            G_text = NULL;
            G_row = R_temp;
            return;

        }

        T_temp = R_temp->first;
        while (T_temp->next != NULL)
        {
            if (T_temp->text_x == x - 1)
            {
                G_text = T_temp;
                a_x = G_text->text_x + 1;
                x = a_x;
                y = temp_y;
                G_row = R_temp;
                a_y = G_row->countrow;
                return;


            }


            T_temp = T_temp->next;
        }

        G_text = T_temp;
        a_x = G_text->text_x + 1;
        x = a_x;
        y = temp_y;
        G_row = R_temp;
        a_y = G_row->countrow;

        return;



    }
    else    //올릴문장이 한페이지가안됀다
    {

        while (R_temp->prev != NULL)
        {
            R_temp = R_temp->prev;
        }
        move(0, 0);
        for (i = 0; i < height - 1; i++)
        {
            clrtoeol();  // 현재커서부터 현재 줄 지우기
            printw("\n");
        }
        R_temp2 = G_row;
        G_row = R_temp;
        temp_y = y;
        y = 0;
        Enterprint();  //새창띄우기
        y = temp_y;
        temp_y = 0;
        G_row = R_temp2;

        if (R_temp->first == NULL)
        {
            x = 0;
            a_x = 0;
            G_text = NULL;
            G_row = R_temp;
            return;

        }

        T_temp = R_temp->first;
        while (T_temp->next != NULL)
        {
            if (T_temp->text_x == x - 1)
            {
                G_text = T_temp;
                a_x = G_text->text_x + 1;
                x = a_x;
                y = temp_y;
                G_row = R_temp;
                a_y = G_row->countrow;
                return;


            }


            T_temp = T_temp->next;
        }

        G_text = T_temp;
        a_x = G_text->text_x + 1;
        x = a_x;
        y = temp_y;
        G_row = R_temp;
        a_y = G_row->countrow;

        return;



    }


}

void lnx_addtext(Row** head, char data) {


    Text* T_temp = (Text*)malloc(sizeof(Text));
    T_temp->data = data;
    T_temp->prev = NULL;
    T_temp->next = NULL;
    T_temp->text_x = a_x;

    if (*head == NULL) {
        *head = (Row*)malloc(sizeof(Row));
        (*head)->first = T_temp;
        (*head)->countrow = a_y;
        (*head)->next = NULL;
        (*head)->prev = NULL;
        x++;
        a_x++;
        refresh();
        G_text = T_temp;
        G_row = *head;
        return;
    }


    Row* R_temp;
    if (G_row == NULL)
    {
        R_temp = *head;
    }
    else
    {
        R_temp = G_row;
    }

    if (R_temp->first == NULL)  //문장이 아예없을때 문자 넣는경우
    {
        R_temp->first = T_temp;
        x++;
        a_x++;
        G_text = T_temp;
        return;
    }

    if (G_text == NULL)  //문장에 처음커서가있을때
    {
        Text* temp;
        T_temp->next = G_row->first;
        G_row->first->prev = T_temp;
        G_row->first = T_temp;
        temp = T_temp->next;
        while (temp != NULL)    //뒤에문장좌표가 +1;
        {
            temp->text_x = temp->text_x + 1;
            temp = temp->next;
        }
        G_text = T_temp;
        x = 1;
        a_x = 1;

    }
    else if (G_text->next == NULL) //문장 마지막에서 문자 삽입하는경우
    {
        G_text->next = T_temp;
        T_temp->prev = G_text;
        a_x++;
        x++;
        G_text = G_text->next;
    }
    else         //중간삽입
    {
        Text* temp;
        T_temp->next = G_text->next;
        G_text->next->prev = T_temp;
        G_text->next = T_temp;
        T_temp->prev = G_text;

        temp = T_temp;
        while (temp != NULL)    //뒤에문장좌표가 +1;
        {
            temp->text_x = temp->text_x + 1;
            temp = temp->next;
        }
        a_x++;
        x++;
        G_text = T_temp;
    }

}
void lnx_enterkey(Row** head) {
    if (*head == NULL) {
        Row* R_temp = (Row*)malloc(sizeof(Row));
        *head = R_temp;
        R_temp->first = NULL;
        R_temp->prev = NULL;
        R_temp->countrow = a_y;  // 행의 카운트를 0부터 시작하도록 설정
        y++;
        a_y++;
        Row* New_line = (Row*)malloc(sizeof(Row));
        R_temp->next = New_line;
        New_line->next = NULL;
        New_line->prev = R_temp;
        New_line->countrow = a_y;
        New_line->first = NULL;

        G_text = NULL;
        G_row = New_line;
        return;
    }



    Row* New_line = (Row*)malloc(sizeof(Row));
    New_line->first = NULL;
    New_line->next = NULL;
    New_line->prev = NULL;
    Row* R_temp;

    if (G_row == *head)  //첫번째에 텍스트입력후 예외처리
    {
        R_temp = *head;
        a_y++;
        y++;
        New_line->countrow = a_y;

    }
    else
    {
        a_y++;
        y++;
        R_temp = G_row;
        New_line->countrow = a_y;
    }




    if (G_text == NULL)      //문장 첫번째 커서에서 엔터를 치는 경우
    {
        New_line->first = R_temp->first;
        R_temp->first = NULL;

        if (R_temp->next == NULL) //엔터를 부른 문장이 텍스트전체의 마지막이다 
        {
            R_temp->next = New_line;
            New_line->prev = R_temp;


            G_text = NULL;
            G_row = New_line;

        }
        else         //엔터를 부른 문장뒤에 다른문장이있다
        {
            R_temp->next->prev = New_line;
            New_line->next = R_temp->next;
            R_temp->next = New_line;
            New_line->prev = R_temp;

            R_temp = New_line->next;

            while (R_temp != NULL)  // 뒤에 문장에 다 좌표 1씩 추가
            {
                R_temp->countrow = R_temp->countrow + 1;
                R_temp = R_temp->next;
            }



        }
        x = 0;
        a_x = 0;
        G_text = NULL;
        G_row = New_line;
    }
    else if (G_text->next == NULL) // 문장 마지막에서 엔터치는경우
    {

        if (R_temp->next == NULL) //엔터를 부른 문장이 텍스트전체의 마지막이다 
        {
            R_temp->next = New_line;
            New_line->prev = R_temp;

            x = 0;
            a_x = 0;
            G_row = New_line;
            G_text = NULL; //첫번째커서를 의미;
            return;
        }
        else         //엔터를 부른 문장뒤에 다른문장이있다
        {
            R_temp->next->prev = New_line;
            New_line->next = R_temp->next;
            R_temp->next = New_line;
            New_line->prev = R_temp;

            R_temp = New_line->next;

            while (R_temp != NULL)  // 뒤에 문장에 다 좌표 1씩 추가
            {
                R_temp->countrow = R_temp->countrow + 1;
                R_temp = R_temp->next;
            }

            x = 0;
            a_x = 0;
            G_text = NULL; //첫번째커서를 의미;
            G_row = New_line;

        }


    }
    else   //문장 중간에서 엔터치는경우
    {
        if (G_text->text_x != 0)  //첫번째 제외 삽입이면
        {

            Text* T_temp;
            int i = 0;
            New_line->first = G_text->next;

            G_text->next->prev = NULL;
            G_text->next = NULL;

            T_temp = New_line->first;

            while (T_temp != NULL)
            {

                T_temp->text_x = i;
                i++;
                T_temp = T_temp->next;
            }

            G_text = NULL;

            if (R_temp->next == NULL) //엔터를 부른 문장이 텍스트전체의 마지막이다 
            {
                R_temp->next = New_line;
                New_line->prev = R_temp;

                G_row = New_line;

            }
            else         //엔터를 부른 문장뒤에 다른문장이있다
            {
                R_temp->next->prev = New_line;
                New_line->next = R_temp->next;
                R_temp->next = New_line;
                New_line->prev = R_temp;

                R_temp = New_line->next;

                while (R_temp != NULL)  // 뒤에 문장에 다 좌표 1씩 추가
                {
                    R_temp->countrow = R_temp->countrow + 1;
                    R_temp = R_temp->next;
                }


            }
            x = 0;
            a_x = 0;
            G_row = New_line;


        }

    }

}

void lnx_delete()
{
    int i = 0;
    Text* T_temp;
    Row* R_temp;
    Row* R_temp2;
    if (G_row == NULL) //  지울문장없다 
    {

        return;
    }
    else //지울 문장이있다
    {

        if (G_text == NULL)  //커서가 처음일때  화면새로바꿔줘야됌
        {
            if (G_row->prev == NULL) //올라갈 문장없으면 리턴
            {
                return;
            }

            R_temp = G_row->prev;

            if (R_temp->first == NULL) // 올라갈문장에 아무데이터도없다면
            {
                if (G_row->first != NULL) //밑문장에도 데이터가있다면
                {
                    clrtoeol();  // 현재커서부터 현재 줄 지우기
                    R_temp->first = G_row->first;

                }
                G_text = NULL;
                R_temp->next = NULL;


            }
            else  // 올라갈 문장에 데이터가있다면
            {

                if (G_row->first != NULL) //밑에 문장에 문자가있다면
                {
                    if (R_temp->first == NULL)  //위에문장에 문자가없다면
                    {
                        R_temp->first = G_row->first;
                        G_text = NULL; //좌표설정
                        x = 0;
                        a_x = 0;

                    }
                    else  //위에문장에 문자가있다면
                    {

                        T_temp = R_temp->first;// 위문장연결
                        while (T_temp->next != NULL)  //끝까지가기
                        {
                            i++;
                            T_temp = T_temp->next;
                        }
                        T_temp->next = G_row->first; // 윗문장을 밑문장을 연결
                        G_row->first->prev = T_temp;
                        G_text = T_temp; // 좌표설정
                        a_x = G_text->text_x + 1;
                        if (a_x > width - 1)
                        {
                            x = width - 1;
                        }
                        else
                        {
                            x = a_x;
                        }

                        T_temp = G_text->next;
                        while (T_temp->next != NULL)  //좌표맞춰주기
                        {
                            T_temp->text_x = i;
                            i++;
                            T_temp = T_temp->next;
                        }


                    }
                }
                else    //밑에 문장에 문장이없다면
                {

                    if (R_temp->first == NULL)  //위에문장에 문자가없다면
                    {
                        R_temp->first = G_row->first;
                        G_text = NULL; //좌표설정
                        x = 0;
                        a_x = 0;

                    }
                    else  //위에문장에 문자가있다면
                    {


                        T_temp = R_temp->first;// 위문장연결
                        while (T_temp->next != NULL)  //끝까지가기
                        {

                            T_temp = T_temp->next;
                        }

                        G_text = T_temp; // 좌표설정
                        a_x = G_text->text_x + 1;
                        if (a_x > width - 1)
                        {
                            x = width - 1;
                        }
                        else
                        {
                            x = a_x;
                        }



                    }
                }


            }

            if (G_row->next != NULL)
            {
                R_temp->next = G_row->next;
                G_row->next->prev = R_temp;
                free(G_row);
                G_row = R_temp;
                R_temp = R_temp->next;
                while (R_temp->next != NULL) //뒷라인 자표 +1씩
                {
                    R_temp->countrow++;
                    R_temp = R_temp->next;
                }
            }
            else
            {
                R_temp2 = G_row;
                free(R_temp2);
                G_row = R_temp;
                G_row->next = NULL;
            }

            a_y--;               //좌표한칸옮기기

            if (y == 0)
            {
                y = 0;
            }
            else
            {
                y--;
            }


            for (i = 0; i < height - 2; i++)
            {
                clrtoeol();  // 현재커서부터 현재 줄 지우기
                printw("\n");
            }
            Enterprint();
            max_y--;




        }
        else  //커서가 문장중간일때
        {
            if (G_text->prev == NULL) //커서가1번째 뒤 예외처리
            {
                if (G_text->next == NULL) //그문장이 단어가1개이다
                {
                    T_temp = G_text;
                    free(T_temp);
                    G_text = NULL;
                    G_row->first = NULL;
                    x--;
                    a_x--;

                    return;
                }
                else  //뒤에 이어줄 문장이있다
                {
                    T_temp = G_text;
                    T_temp = T_temp->next;
                    G_row->first = T_temp;
                    free(G_text);
                    G_text = NULL;
                    T_temp->prev = NULL;
                    while (T_temp->next != NULL)  // ㅡ다음문장 -1씩
                    {
                        T_temp->text_x--;
                        T_temp = T_temp->next;
                    }
                    x--;
                    a_x--;
                    return;

                }
            }
            else      //커서가 중간이거나 마지막일때
            {
                if (G_text->next == NULL)  //마지막
                {
                    T_temp = G_text;
                    G_text = G_text->prev;
                    G_text->next = NULL;
                    free(T_temp);
                    x--;
                    a_x--;
                    return;
                }
                else  //중간
                {
                    T_temp = G_text;
                    T_temp = T_temp->prev;
                    T_temp->next = G_text->next;
                    G_text->next->prev = T_temp;
                    free(G_text);

                    G_text = T_temp;
                    T_temp = T_temp->next;

                    while (T_temp->next != NULL)  // ㅡ다음문장 -1씩
                    {
                        T_temp->text_x--;
                        T_temp = T_temp->next;
                    }
                    x--;
                    a_x--;

                }
            }
        }
    }


}

void lnx_getspecialkey(Row** head, int ch) {


    Text* T_temp;
    if (ch == KEY_DOWN) {
        if (G_row->next == NULL) // 넘어갈 문장이없다
        {
            return;
        }
        else
        {
            G_row = G_row->next;

            if (y == height - 3)  //콘솔창 끝에서 내려서 프린트 다시해야됌
            {
                if (G_row->first == NULL)  //밑문장에 리스트만있고 아무데이터도없는경우
                {
                    G_text = NULL;
                    Enterprint();
                    a_x = 0;
                    x = a_x;
                    a_y++;
                    return;
                }
                else if (G_text == NULL)
                {
                    Enterprint();
                    a_y++;
                    return;
                }
                else //밑문장에데이터가있으면
                {
                    T_temp = G_row->first;

                    while (T_temp->next != NULL)
                    {
                        if (G_text->text_x == T_temp->text_x)   //내려갈 문장이 더길면
                        {

                            G_text = T_temp;
                            a_x = G_text->text_x + 1;
                            x = a_x;
                            Enterprint();
                            a_y++;

                            return;
                        }
                        T_temp = T_temp->next;
                    }

                    G_text = T_temp;
                    a_x = G_text->text_x + 1;
                    x = a_x;
                    Enterprint();
                    a_y++;

                }
            }
            else
            {
                if (G_row->first == NULL)  //밑문장에 리스트만있고 아무데이터도없는경우
                {
                    G_text = NULL;
                    a_x = 0;
                    x = a_x;
                    y++;
                    a_y++;
                    return;
                }
                else if (G_text == NULL)
                {
                    y++;
                    a_y++;
                    return;
                }
                else //밑문장에데이터가있으면
                {
                    T_temp = G_row->first;

                    while (T_temp->next != NULL)
                    {
                        if (G_text->text_x == T_temp->text_x)   //내려갈 문장이 더길면
                        {

                            G_text = T_temp;
                            a_x = G_text->text_x + 1;
                            x = a_x;
                            y++;
                            a_y++;

                            return;
                        }
                        T_temp = T_temp->next;
                    }

                    G_text = T_temp;
                    a_x = G_text->text_x + 1;
                    x = a_x;
                    y++;
                    a_y++;

                }

            }
        }
    }
    else if (ch == KEY_UP)
    {
        if (y == 0)  //콘솔창 맨위에서 올릴때 
        {
            if (G_row->prev == NULL)
            {
                return;
            }
            else       //윗 문장에 리스트가있는경우
            {
                G_row = G_row->prev;
                Text* T_temp;

                if (G_row->first == NULL)  //윗 문장에 문자가한개도없는경우
                {
                    G_text = NULL;
                    x = 0;
                    a_x = 0;
                    a_y--;
                    Enterprint();
                    return;
                }


                T_temp = G_row->first;
                if (G_text == NULL) //커서가 처음일떄
                {
                    a_y--;
                    Enterprint();
                    return;
                }
                while (T_temp->next != NULL)
                {
                    if (T_temp->text_x == G_text->text_x) // 위라인문장이 밑보다 같거나 길면
                    {
                        G_text = T_temp;
                        a_y--;
                        a_x = G_text->text_x + 1;
                        x = a_x;
                        Enterprint();
                        return;
                    }
                    T_temp = T_temp->next;

                }

                G_text = T_temp;  //위라인이 밑보다짧으면 위라인 제일마지막으로감
                Enterprint();
                a_y--;
                a_x = G_text->text_x + 1;
                x = a_x;


            }
        }
        else      //첫문장 제외해서 남은 문장에서 올릴떄
        {
            if (G_row->prev != NULL) //올라갈라인이있음
            {

                Text* T_temp;
                if (G_text == NULL) //밑문장에아무것도 없을때
                {
                    G_row = G_row->prev;
                    y--;
                    a_y--;
                    return;
                }

                G_row = G_row->prev;
                if (G_row->first == NULL)  //윗 문장에 문자가한개도없는경우
                {
                    G_text = NULL;
                    x = 0;
                    a_x = 0;
                    y--;
                    a_y--;
                    return;
                }
                else       //윗 문장에 문자가있는경우
                {


                    T_temp = G_row->first;
                    while (T_temp->next != NULL)
                    {
                        if (T_temp->text_x == G_text->text_x) // 위라인문장이 밑보다 같거나 길면
                        {
                            G_text = T_temp;
                            a_x = G_text->text_x + 1;
                            x = a_x;
                            y--;
                            a_y--;

                            return;
                        }
                        T_temp = T_temp->next;

                    }

                    G_text = T_temp;  //위라인이 밑보다짧으면 위라인 제일마지막으로감
                    y--;
                    a_y--;
                    a_x = G_text->text_x + 1;
                    x = a_x;


                }

            }
            else
            {
                return;
            }
        }

    }
    else if (ch == KEY_LEFT)
    {
        if (G_text == NULL) // 커서가 처음이면
        {
            if (G_row->prev != NULL)  //올라갈 문장이있다면
            {

                G_row = G_row->prev;
                if (G_row->first != NULL) //올라갈 문장에 문자열이있다면
                {
                    G_text = G_row->first;
                    while (G_text->next != NULL)
                    {
                        G_text = G_text->next;
                    }
                    a_x = G_text->text_x + 1;
                    if (x > width - 1)
                    {
                        x = width - 1;
                    }
                    else
                    {
                        x = a_x;
                    }

                    if (y == 0) // 첫번쨰에서 왼방향키눌러서 화면다시찍는 경우 
                    {
                        Enterprint();
                        y = 0;
                        a_y--;
                        if (x > width - 1)
                        {
                            x = width - 1;
                        }
                        else
                        {
                            x = a_x;
                        }
                        return;

                    }
                    y--;
                    a_y--;

                }
                else    //올라갈 문장에 문자가 아무것도없다면
                {
                    G_row = G_row->prev;
                    G_text = NULL;
                    if (y == 0) // 첫번쨰에서 왼방향키눌러서 화면다시찍는 경우 
                    {
                        Enterprint();
                        y = 0;
                        a_y--;
                        x = 0;
                        a_x = 0;
                        return;

                    }

                    y--;
                    a_y--;
                    x = 0;
                    a_x = 0;
                    return;
                }
            }
            else  //커서가 처음인데 올라갈 문장이없다면 
            {
                return;
            }

        }
        else  //문장사이에 커서가있다면
        {

            if (x == 1)
            {
                G_text = NULL;
                x = 0;
                a_x = 0;
                return;
            }
            G_text = G_text->prev;
            a_x--;
            if (x > width - 1)
            {
                x = width - 1;
            }
            else
            {
                x--;
            }

        }


    }
    else if (ch == KEY_RIGHT)
    {


        if (G_text == NULL) //커서가 처음일때
        {
            if (G_row->first != NULL)  //현재커서의 문장이있을경우
            {
                G_text = G_row->first;

                x++;
                a_x++;
            }
            else if (G_row->next != NULL)  //넘어갈 문장이있고 현재커서에 문장에 아무문자열이없을때
            {
                if (y == height - 3) //마지막행에서 오른쪽으로 넘어가면 화면을 새로찍어야됌
                {
                    G_row = G_row->next;
                    Enterprint();
                    a_x = 0;
                    x = 0;
                    return;
                }
                G_row = G_row->next;
                G_text = NULL;
                y++;
                a_y++;
                x = 0;
                a_x = 0;

            }
            else  //넘어갈 문장이없다면
            {
                return;
            }
        }
        else if (G_text->next == NULL) //문장끝에서 넘어갈때
        {
            if (G_row->next != NULL)  //넘어갈 문장이있다면
            {
                if (y == height - 3) //마지막행에서 오른쪽으로 넘어가면 화면을 새로찍어야됌
                {
                    G_row = G_row->next;
                    G_text = NULL;
                    Enterprint();
                    a_x = 0;
                    x = 0;
                    a_y++;
                    return;
                }
                G_row = G_row->next;
                G_text = NULL; //첫커서로
                y++;
                a_y++;
                x = 0;
                a_x = 0;

            }
            else  //넘어갈 문장이없다면
            {
                return;
            }
        }
        else //문장중간에서 누를경우
        {
            G_text = G_text->next;
            a_x++;
            if (x != width - 1)  //나중에 구현예정
            {
                x++;
            }
            else
            {
                return;
            }

        }

    }
}






void loadfromfile(Row** head) {
    FILE* file;
    clear();
    move(0, 0);
    refresh();
    if ((file = fopen(filename, "r")) == NULL) {
        perror("File not found");
        refresh();
        endwin(); // curses 모드 종료
        printf("File not found");
        printf("\n");

        exit(0);
        return;
    }

    const size_t BUFFER_SIZE = 4096; // 필요에 따라 버퍼 크기 조절
    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (buffer == NULL) {

        perror("메모리 할당 오류");
        endwin(); // curses 모드 종료
        printf("\n");
        fclose(file);
        exit(0);
        return;
    }

    size_t bytesRead;
    size_t bufferIndex = 0; // 현재 버퍼에 저장된 데이터의 인덱스

    while ((bytesRead = fread(buffer + bufferIndex, 1, BUFFER_SIZE - bufferIndex, file)) > 0) {
        size_t totalBytesRead = bufferIndex + bytesRead;

        for (size_t i = 0; i < totalBytesRead; ++i) {
            char c = buffer[i];
            if (c == '\n') {
                lnx_load_enterkey(head);
                if (max_y < a_y) max_y = a_y;
            }
            else {
                lnx_load_addtext(head, c);

            }
        }

        // 현재 버퍼에 처리되지 않은 나머지 데이터를 앞으로 이동
        bufferIndex = totalBytesRead % BUFFER_SIZE;
        if (bufferIndex > 0) {
            memmove(buffer, buffer + totalBytesRead - bufferIndex, bufferIndex);
        }
    }

    free(buffer); // 할당된 메모리 해제
    G_row = *head;
    G_text = NULL;
    a_x = 0;
    x = 0;
    y = 0;
    a_y = 0;
    refresh();
    fclose(file);
    return;
}
void mod(Row** head) {
    int i = 0, k = 0;
    char filename1[100];


    initscr(); // 라이브러리 초기화
    raw(); // 라인 버퍼링 비활성화
    noecho();

    scrollok(stdscr, FALSE); // 스크롤 가능하게 설정
    keypad(stdscr, TRUE); // 특수 키 활성화

    getmaxyx(stdscr, height, width);  // 현재 터미널 창 크기 가져오기
    for (i = 0; i < height - 3; i++) {     //초기설정
        mvprintw(i, 0, "~");
    }
    mvprintw(height / 2, (width - 20) / 2, "Visual Text editor -- version 0. 0. 1");

    bar();

    if (strcmp(filename, "No name") != 0)
    {
        loadfromfile(head);
        G_row = *head;
        G_text = NULL;
        lnx_print(*head);
        x = 0;
        y = 0;
        a_y = 0;
        a_x = 0;
        bar();
        refresh();
        k = 1;
    }


    while (1) {




        int c = getch();
        for (; k < 1; k++)
        {
            move(0, 0);
            for (i = 0; i < height - 2; i++)
            {

                clrtoeol();  // 현재커서부터 현재 줄 지우기
                printw("\n");
            }

            move(y, x);
            refresh();


        }
        if (c == '\n') {
            lnx_enterkey(head);
            attron(A_REVERSE);
            mvprintw(height - 2, 0, "[%s] - %d lines", filename, a_y + 1);
            attroff(A_REVERSE);
            Enterprint();


        }
        else if (c == KEY_UP || c == KEY_DOWN || c == KEY_LEFT || c == KEY_RIGHT)
        {
            lnx_getspecialkey(head, c);

        }
        else if (c == KEY_HOME) {
            home();
        }
        else if (c == KEY_END)
        {
            end();
        }
        else if (c == KEY_BACKSPACE || c== 127)
        {
            lnx_delete();
            Rowprint();

        }
        
        else if (c == KEY_PPAGE)
        {
            pageup();

        }
        else if (c == KEY_NPAGE)
        {
            pagedown();

        }
        else if ('A' <= c && c <= 'z') {

            lnx_addtext(head, c);
            Rowprint();


        }
        else if (c == ctrl_s)  //저장
        {
            if (strcmp(filename, "No name") == 0)   //새로운 파일일경우
            {
                move(height - 1, 0);
                clrtoeol();  // 현재커서부터 현재 줄 지우기
                mvprintw(height - 1, 0, "Please enter the name you want to save:");

                refresh();
                while (1)
                {
                    c = getch();
                    if (c != ERR) {
                        // 입력이 있을 경우 처리
                        if (c == '\n') {
                            break;  // 엔터 키를 누르면 종료
                        }
                        else if (c == KEY_BACKSPACE || c == 127) {
                            // 백스페이스 키를 누르면 파일 이름에서 마지막 문자 삭제
                            if (strlen(filename) > 0) {
                                filename1[strlen(filename1) - 1] = '\0';
                            }
                        }
                        else if (c >= 32 && c <= 126) {
                            printw("%c", c);
                            refresh();
                            if (strlen(filename1) < 100 - 1) {
                                strncat(filename1, (char*)&c, 1);
                            }
                        }
                    }
                }
                savetofile(*head, filename1);
                endwin(); // curses 모드 종료
                freedata(*head);
                exit(0);
            }
            else
            {
                savetofile(*head, filename);
                endwin(); // curses 모드 종료
                freedata(*head);
                exit(0);
            }
        }
        else if (c == ctrl_q) //나가기
        {
            timeout(800);
            int start_time = time(NULL);

            // 1.5초 동안 두 번째 'Ctrl+Q' 입력을 기다림
            while (time(NULL) - start_time < 0.8) {
                c = getch();
                if (c == ctrl_q) {
                    // 두 번째 'Ctrl+Q' 입력 감지
                    endwin(); // curses 모드 종료
                    freedata(*head);
                    exit(0);
                }

            }

            if (strcmp(filename, "No name") == 0)   //새로운 파일일경우
            {
                move(height - 1, 0);
                clrtoeol();  // 현재커서부터 현재 줄 지우기
                mvprintw(height - 1, 0, "Please enter the name you want to save:");

                refresh();
                while (1)
                {
                    c = getch();
                    if (c != ERR) {
                        // 입력이 있을 경우 처리
                        if (c == '\n') {
                            break;  // 엔터 키를 누르면 종료
                        }
                        else if (c == KEY_BACKSPACE || c == 127) {
                            // 백스페이스 키를 누르면 파일 이름에서 마지막 문자 삭제
                            if (strlen(filename) > 0) {
                                filename1[strlen(filename1) - 1] = '\0';
                            }
                        }
                        else if (c >= 32 && c <= 126) {
                            printw("%c", c);
                            refresh();
                            if (strlen(filename1) < 100 - 1) {
                                strncat(filename1, (char*)&c, 1);
                            }
                        }
                    }
                }
                savetofile(*head, filename1);
                endwin(); // curses 모드 종료
                freedata(*head);
                exit(0);
            }
            else
            {
                savetofile(*head, filename);
                endwin(); // curses 모드 종료
                freedata(*head);
                exit(0);
            }




        }



        bar();
        move(y, x);
        refresh();
    }

    endwin(); // curses 모드 종료
    printf("\n");

}

#else
#error Unsupported platform



#endif







int main(int argc, char* argv[]) {



    Row* head = NULL;

    if (argc == 2)
    {



        filename = argv[1];




    }
    else
    {
        filename = "No name";
    }

    mod(&head);



    return 0;
}


